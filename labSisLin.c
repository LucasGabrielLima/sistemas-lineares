#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "utils.h"
#include "SistemasLineares.h"

int main ()
{
    // inicializa gerador de nr aleatoreos
    srand(20192);




  	SistLinear_t *SL, *Soriginal;
  	real_t *x;

  	SL = lerSistLinear();
  	x = (real_t *) malloc(SL->n * sizeof(real_t));

    Soriginal = alocaSistLinear(SL->n);

    copiaSistema(Soriginal, SL);

  	eliminacaoGauss(SL, x, 1);
  	prnSistLinear (SL);
  	prnVetor(x, SL->n);
  	normaL2Residuo(Soriginal, x);

}

