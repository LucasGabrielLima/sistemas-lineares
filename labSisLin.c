#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"
#include "SistemasLineares.h"

int main ()
{
    // inicializa gerador de nr aleatoreos
    srand(20192);




  	SistLinear_t *SL;

  	SL = lerSistLinear();
  	// prnSistLinear (SL);

  	eliminacaoGauss(SL, NULL, 1);

}

