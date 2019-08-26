#ifndef __SISLINEAR_H__
#define __SISLINEAR_H__

// Parâmetros para teste de convergência
#define MAXIT 100
#define EPS 1.0e-4

#define COEF_MAX 32.0

typedef float real_t;

typedef struct {
  real_t *A; // coeficientes
  real_t *b; // termos independentes
  unsigned int n; // tamanho do SL
} SistLinear_t;

typedef enum {
    comSolucao = 0,
    eqNula,
    eqProporcional,
    eqCombLinear,
    hilbert,
    diagDominante,
} tipoSistLinear_t;

// Alocaçao e desalocação de memória
SistLinear_t* alocaSistLinear (unsigned int n);
void liberaSistLinear (SistLinear_t *SL);
void inicializaSistLinear (SistLinear_t *SL, tipoSistLinear_t tipo, real_t coef_max);

// Leitura e impressão de sistemas lineares
SistLinear_t *lerSistLinear ();
void prnSistLinear (SistLinear_t *SL);
void prnVetor (real_t *vet, unsigned int n);

// Calcula a normaL2 do resíduo
double normaL2Residuo(SistLinear_t *SL, real_t *x);

// Método da Eliminação de Gauss
int eliminacaoGauss (SistLinear_t *SL, real_t *x, int pivotamento);

// Método de Gauss-Jacobi
int gaussJacobi (SistLinear_t *SL, real_t *x, real_t erro);

// Método de Gauss-Seidel
int gaussSeidel (SistLinear_t *SL, real_t *x, real_t erro);

#endif // __SISLINEAR_H__

