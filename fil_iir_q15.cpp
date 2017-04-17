/******************************* SOURCE LICENSE *********************************
Copyright (c) 2015 MicroModeler.

A non-exclusive, nontransferable, perpetual, royalty-free license is granted to the Licensee to 
use the following Information for academic, non-profit, or government-sponsored research purposes.
Use of the following Information under this License is restricted to NON-COMMERCIAL PURPOSES ONLY.
Commercial use of the following Information requires a separately executed written license agreement.

This Information is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

******************************* END OF LICENSE *********************************/

// A commercial license for MicroModeler DSP can be obtained at http://www.micromodeler.com/launch.jsp

#include "fil_iir_q15.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

q15_t fil_iir_q15_coeficientes[12];
int fil_iir_q15_grado;
int fil_iir_q15_posCorr;


fil_iir_q15Type *fil_iir_q15_create( string* tipo, float f0, short grado )
{
	fil_iir_q15Type *result = (fil_iir_q15Type *)malloc( sizeof( fil_iir_q15Type ) );	// Allocate memory for the object
	fil_iir_q15_grado = grado;
	
	
	
	fil_iir_q15_inic( result );											// inicialize it
	return result;																// Return the result
}

void fil_iir_q15_destruir( fil_iir_q15Type *puntObj )
{
	free( puntObj );
}

 void fil_iir_q15_inic( fil_iir_q15Type * puntEste )
{
	arm_biquad_cascade_df1_inic_q15(	&puntEste->instancia, fil_iir_q15_grado, fil_iir_q15_coeficientes, puntEste->estados, fil_iir_q15_postShift );
	fil_iir_q15_reiniciar( puntEste );

}

 void fil_iir_q15_reiniciar( fil_iir_q15Type * puntEste )
{
	memset( &puntEste->estados, 0, sizeof( puntEste->estados ) ); // reiniciar estados to 0
	puntEste->salida = 0;									// reiniciar salida

}

 int fil_iir_q15_filterBlock( fil_iir_q15Type * puntEste, short * puntEntrada, short * puntSalida, unsigned int count )
{
	arm_biquad_cascade_df1_q15( &puntEste->instancia, puntEntrada, puntSalida, count );
	return count;

}