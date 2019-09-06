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




  	SistLinear_t *SL, *Soriginal, *Seg;
  	real_t *x;

  	SL = lerSistLinear();
  	x = (real_t *) malloc(SL->n * sizeof(real_t));

    Soriginal = alocaSistLinear(SL->n);

    copiaSistema(Soriginal, SL); // copia sistema original
  	eliminacaoGauss(SL, x, 1);
    puts("solução eliminação de gauss");
  	prnSistLinear (SL);
  	prnVetor(x, SL->n);

    // copiaSistema(Seg, SL); //copia sistema após e.g.
		// gaussSeidel(Seg, x, EPS);
		// puts("solução eliminação de gauss");
  	// prnSistLinear (Seg);
  	// prnVetor(x, SL->n);

}

