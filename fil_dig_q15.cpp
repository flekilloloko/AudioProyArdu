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

#include "fil_dig_q15.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

/*q15_t fil_dig_q15_coefficients[6] =
{
  // Scaled for a 16x16:64 Direct form 1 IIR filter with:
  // Feedback shift = 14
  // Output shift = 14
  // Input  has a maximum value of 1, scaled by 2^15
  // Output has a maximum value of 1.741724639009106, scaled by 2^14

  //2^14 = 16.384
  11453, 0, -22907, 11453, 20488, -8941  //b0 Q14(0,699059936589549), 0, b1 Q14 (-1,3981198731791), b2 Q14(0,699059936589549), a1 Q14(-1,25051643084874), a2 Q14( 0,545723315509458)  //5234, 0, -10468, 5234, 18727, -6763// b0 Q13(0.639), 0, b1 Q13(-1.28), b2 Q13(0.639), a1 Q14(1.14), a2 Q14(-0.413)

};
HPF:        H(s) = s^2 / (s^2 + s/Q + 1)

            b0 =  (1 + cos(w0))/2
            b1 = -(1 + cos(w0))
            b2 =  (1 + cos(w0))/2
            a0 =   1 + alpha
            a1 =  -2*cos(w0)
            a2 =   1 - alpha

             alpha = sin(w0)/(2*Q)   
 w0 = 2*pi*f0/Fs
*/
void llenar_muestras(q15_t *arreglo){
  for(int i=0 ; i<1024; i++){
    arreglo[i] = ((cos(PI*2*1000*i*((double)1/50000)) + cos(PI*2*7000*i*((double)1/50000) ))/4)*(1<<15) ; 
  }
}

void fil_dig_q15_llenarCoef( fil_dig_q15Type *pThis, float frecCorte )
{
  float a0, w0, alpha;
  w0 = 2 * PI * frecCorte / frecMuestreo;
  alpha = sin(w0)/(2*Q);
  a0 = alpha + 1;

  pThis->fil_dig_q15_coefficients[0] = ((1 + cos(w0))/(2*a0)) * 16384;    //b0
  pThis->fil_dig_q15_coefficients[1] = 0;                                 //a0
  pThis->fil_dig_q15_coefficients[2] = ((1 + cos(w0))/(-a0)) * 16384;     //b1
  pThis->fil_dig_q15_coefficients[3] = ((1 + cos(w0))/(2*a0)) * 16384;    //b2
  pThis->fil_dig_q15_coefficients[4] = ((2 * cos(w0))/a0) * 16384;        //a1
  pThis->fil_dig_q15_coefficients[5] = ((a0-2)/a0) * 16384;               //a2
}

fil_dig_q15Type *fil_dig_q15_create( void )
{
  fil_dig_q15Type *result = (fil_dig_q15Type *)malloc( sizeof( fil_dig_q15Type ) );	// Allocate memory for the object
  fil_dig_q15_init( result );											// Initialize it
  return result;																// Return the result
}

void fil_dig_q15_destroy( fil_dig_q15Type *pObject )
{
  free( pObject );
}

void fil_dig_q15_init( fil_dig_q15Type * pThis )
{
  arm_biquad_cascade_df1_init_q15(	&pThis->instance, fil_dig_q15_numStages, pThis->fil_dig_q15_coefficients, pThis->state, fil_dig_q15_postShift );
  fil_dig_q15_reset( pThis );

}

void fil_dig_q15_reset( fil_dig_q15Type * pThis )
{
  memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
  pThis->output = 0;									// Reset output

}

int fil_dig_q15_filterBlock( fil_dig_q15Type * pThis, short * pInput, short * pOutput, unsigned int count )
{
  arm_biquad_cascade_df1_q15( &pThis->instance, pInput, pOutput, count );
  return count;

}

