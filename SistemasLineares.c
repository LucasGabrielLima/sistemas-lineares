#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "utils.h"
#include "SistemasLineares.h"

//O vetor pivos é preenchido com os índices correspondentes àquela linha na matriz original.
int *pivos;

/*!
  \brief Essa função calcula a norma L2 do resíduo de um sistema linear 

  \param SL Ponteiro para o sistema linear
  \param x Solução do sistema linear
*/
double normaL2Residuo(SistLinear_t *SL, real_t *x)
{
  real_t *r = (real_t *) malloc(SL->n * sizeof(real_t));
  real_t ax; //A * X da linha corrente
  real_t somaR2 = 0; // Somatório dos quadrados dos resíduos de cada linha
  int n = SL->n;

  for(int i = 0; i < n; i++){
    ax = 0;
    for(int j = 0; j < n; j++){
      ax += SL->A[i * n + j] * x[pivos[j]];
    }
    r[i] = SL->b[i] - ax;
    printf("b - a: %10.10lg %10.10lg\n", SL->b[i], ax);
  }

  puts("Vetor de resíduos:");
  prnVetor(r, n);

  for(int i = 0; i < n; i++){
    somaR2 += r[i] * r[i];
  }

  return sqrt(somaR2);
}

void copiaSistema(SistLinear_t *dest, SistLinear_t *src){
    int n = src->n;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            dest->A[i * n + j] = src->A[i * n + j];
        }

        dest->b[i] = src->b[i];
    }

    src->n = n;
}


/*!
  \brief Método da Eliminação de Gauss

  \param SL Ponteiro para o sistema linear
  \param x ponteiro para o vetor solução
  \param pivotamento flag para indicar se o pivotamento parcial deve ser feito (!=0)

  \return código de erro. 0 em caso de sucesso.

  Manter SL original, para comparação posterior
*/
int eliminacaoGauss (SistLinear_t *SL, real_t *x, int pivotamento)
{
  int iPivo, k, j;
  int n = SL->n;
  real_t m;

  
  if(pivotamento){
    pivos = (int *) malloc(n * sizeof(int));
    for(int i = 0; i < n; i++){
      pivos[i] = i;
    }
  }


  for(int i = 0; i < n; i++){
    if(pivotamento){
      iPivo = encontraMax(SL, i);

      printf("i: %d pivo: %d\n",i, iPivo);
      if(i != iPivo){
        trocaLinha(SL, i, iPivo);
        prnSistLinear(SL);
      }

    }
    
    for(k = i+1; k < n; k++){
      if( SL->A[i * n + i] == 0){
        fprintf(stderr, "Divisão por zero na eliminacao de Gauss\n");
        return -1;
      }
      m = SL->A[k * n + i] / SL->A[i * n + i];
      SL->A[k * n + i] = 0.0;
      SL->b[k] -= SL->b[i] * m;

      for(j = i+1; j < n; j++){
        SL->A[k * n + j] -= SL->A[i * n + j] * m;
      }
    }

    retroSubst(SL, x);
  }

}

int retroSubst(SistLinear_t *SL, real_t *x){
  real_t soma;
  int n = SL->n;
  int j, i = n - 1;

  if( SL->A[i * n + i] == 0){
    fprintf(stderr, "Divisão por zero na retrosubstituição\n");
    return -1;
  }

  x[i] = SL->b[i] / SL->A[i * n + i];

  for(i = n-2; i >= 0; i--){
    soma = SL->b[i];

    for(j = i+1; j < n; j++){
      soma -= SL->A[i * n + j] * x[j];
    }

  if( SL->A[i * n + i] == 0){
    fprintf(stderr, "Divisão por zero na retrosubstituição\n");
    return -1;
  }

    x[i] = soma / SL->A[i * n + i];
  }

  return 1;
}

int encontraMax(SistLinear_t *SL, int i){
  int n = SL->n;
  real_t maior = fabs(SL->A[i*n + i]);
  int maiorI = i;
  int j;

  for(j = i; j < SL->n; j++){
    if(fabs(SL->A[j*n+i]) > maior){
      maior = fabs(SL->A[j*n+i]);
      maiorI = j;
    }
  }

  return maiorI;
}

//Troca linha i pela linha j
int trocaLinha(SistLinear_t *SL, int i, int j){
  int k = 0;
  int n = SL->n;
  int bTemp, iTemp;
  real_t *aTemp = (real_t *) malloc(SL->n * sizeof(real_t));
  
  iTemp = pivos[i];
  pivos[i] = pivos[j];
  pivos[j] = iTemp;

  for(k = 0; k < SL->n; k++){
    aTemp[k] = SL->A[i * n + k];
    SL->A[i * n + k] = SL->A[j * n + k];
    SL->A[j * n + k] = aTemp[k];
  }

  bTemp = SL->b[i];
  SL->b[i] = SL->b[j];
  SL->b[j] = bTemp;
}

/*!
  \brief Método de Gauss-Jacobi

  \param SL Ponteiro para o sistema linear
  \param x ponteiro para o vetor solução
  \param erro menor erro aproximado para encerrar as iterações

  \return código de erro. Um nr positivo indica sucesso e o nr
          de iterações realizadas. Um nr. negativo indica um erro.
*/
int gaussJacobi (SistLinear_t *SL, real_t *x, real_t erro)
{
  int n = SL->n, k;
  real_t norma = 1 + erro;
  real_t soma;
  real_t xk[n]; // x na iteração k

  for(k = 0; norma > erro; k++){
    for(int i = 0; i < n; i++){
      soma = 0.0;


      for(int j = 0; j < n; j++){ 
        if(j != i){
          soma += SL->A[i*n+j] * x[j];
        }
      }

      xk[i] = (SL->b[i] - soma) / SL->A[i * n + i];
    }

    norma = normaL2Residuo(SL, x);
    for (int i = 0; i < n; i++){
      x[i] = xk[i];
    }
    
  }

  return k;

}

/*!
  \brief Método de Gauss-Seidel

  \param SL Ponteiro para o sistema linear
  \param x ponteiro para o vetor solução
  \param erro menor erro aproximado para encerrar as iterações

  \return código de erro. Um nr positivo indica sucesso e o nr
          de iterações realizadas. Um nr. negativo indica um erro.
  */
int gaussSeidel (SistLinear_t *SL, real_t *x, real_t erro)
{
  int n = SL->n, k;
  real_t norma = 1 + erro;
  real_t soma;
  real_t xk[n]; // x na iteração k

  for(k = 0; norma > erro; k++){
    for(int i = 0; i < n; i++){
      soma = 0.0;

      for(int j = 0; j < i; j++){ soma += SL->A[i*n+j] * xk[j] ;}

      for(int j = i+1; j < n; j++){ soma += SL->A[i*n+j] * x[j] ;}

      xk[i] = (SL->b[i] - soma) / SL->A[i * n + i];
    }

    norma = normaL2Residuo(SL, x);
    for (int i = 0; i < n; i++){
      x[i] = xk[i];
    }
    
  }

  return k;
}


// Alocaçao de memória
SistLinear_t* alocaSistLinear (unsigned int tam)
{
  SistLinear_t *SL = (SistLinear_t *) malloc(sizeof(SistLinear_t));
  if ( SL ) {
    SL->A = (real_t *) malloc(tam * tam * sizeof(real_t));
    SL->b = (real_t *) malloc(tam * sizeof(real_t));

    if (!(SL->A) || !(SL->b))
      liberaSistLinear(SL);
  }
  
  SL->n = tam;

  return (SL);
}

// Liberacao de memória
void liberaSistLinear (SistLinear_t *SL)
{
  free(SL->A);
  free(SL->b);
  free(SL);
}

/*!
  \brief Cria coeficientes e termos independentes do SL
  *
  \param SL Ponteiro para o sistema linear
  \param tipo Tipo de sistema linear a ser criado. Pode ser: comSolucao,
  eqNula, eqProporcional, eqCombLinear, hilbert 
  \param coef_max Maior valor para coeficientes e termos independentes
*/
void inicializaSistLinear (SistLinear_t *SL, tipoSistLinear_t tipo, real_t coef_max)
{
  unsigned int tam = SL->n;
  // para gerar valores no intervalo [0,coef_max[
  real_t invRandMax = ((real_t)coef_max / (real_t)RAND_MAX);

  // inicializa vetor b
  for (unsigned int i=0; i<tam; ++i) {
    SL->b[i] = (real_t)rand() * invRandMax;
  }
    
  if (tipo == hilbert) {
    for (unsigned int i=0; i<tam; ++i) {
      for (unsigned int j=0; j<tam; ++j)  {
	      SL->A[i*tam+j] = 1.0 / (real_t)(i+j+1);
      }
    }
  }
  else { // inicializa sistema normal e depois altera
    // inicializa a matriz A
    for (unsigned int i=0; i<tam; ++i) {
      for (unsigned int j=0; j<tam; ++j)  {
	      SL->A[i*tam+j] = (real_t)rand() * invRandMax;
      }
    }
    if (tipo == eqNula) {
      // sorteia eq a ser "nula"
      unsigned int nula = rand() % tam;
      for (unsigned int j=0; j<tam; ++j) {
	      SL->A[nula*tam+j] = 0.0;
      }
      SL->b[nula] = 0.0;
    } 
    else if (tipo == eqProporcional) {
      // sorteia eq a ser "proporcional" e valor
      unsigned int propDst = rand() % tam;
      unsigned int propSrc = (propDst + 1) % tam;
      real_t mult = (real_t)rand() * invRandMax;
      for (unsigned int j=0; j<tam; ++j) {
	      SL->A[propDst*tam+j] = SL->A[propSrc*tam+j] * mult;
      }
      SL->b[propDst] = SL->b[propSrc] * mult;
    } 
    else if (tipo == eqCombLinear) {
      // sorteia eq a ser "combLinear"
      unsigned int combDst = rand() % tam;
      unsigned int combSrc1 = (combDst + 1) % tam;
      unsigned int combSrc2 = (combDst + 2) % tam;
      for (unsigned int j=0; j<tam; ++j) {
	      SL->A[combDst*tam+j] = SL->A[combSrc1*tam+j] + SL->A[combSrc2*tam+j];
      }
      SL->b[combDst] = SL->b[combSrc1] + SL->b[combSrc2];
    }
    else if (tipo == diagDominante) {
      // aumenta o expoente dos termos da diagonal principal
      for (unsigned int i=0; i<tam; ++i) {
        SL->A[i*tam+i] *= (real_t)tam;
      }
    }

  }
}


SistLinear_t *lerSistLinear ()
{
  unsigned int n;
  SistLinear_t *SL;
  
  scanf("%d",&n);

  SL = alocaSistLinear (n);
  
  for(int i=0; i < n; ++i)
    for(int j=0; j < n; ++j)
      scanf ("%lg", &SL->A[i*n+j]);

  for(int i=0; i < n; ++i)
    scanf ("%lg", &SL->b[i]);
  
  return SL;
}


void prnSistLinear (SistLinear_t *SL)
{
  int n=SL->n;

  for(int i=0; i < n; ++i) {
    printf("\n\t");
    for(int j=0; j < n; ++j)
      printf ("%10.5lg", SL->A[i*n+j]);
    printf ("   |   %.8lg", SL->b[i]);
  }
  printf("\n\n");
}

void prnVetor (real_t *v, unsigned int n)
{
  int i;

  printf ("\n");
  for(i=0; i < n; ++i)
      printf ("%10.10lg ", v[i]);
  printf ("\n\n");

}

