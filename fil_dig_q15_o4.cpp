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

#include "dig_fil_q15.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

q15_t dig_fil_q15_coefficients[12] = 
{
// Scaled for a 16x16:64 Direct form 1 IIR filter with: 
// Feedback shift = 14
// Output shift = 14
// Input  has a maximum value of 1, scaled by 2^15
// Output has a maximum value of 2.201848663722923, scaled by 2^13

    3546, 0, -7092, 3546, 17180, -4852,// b0 Q12(0.866), 0, b1 Q12(-1.73), b2 Q12(0.866), a1 Q14(1.05), a2 Q14(-0.296)
    8192, 0, -16384, 8192, 21642, -10367// b0 Q14(0.500), 0, b1 Q14(-1.00), b2 Q14(0.500), a1 Q14(1.32), a2 Q14(-0.633)

};


dig_fil_q15Type *dig_fil_q15_create( void )
{
	dig_fil_q15Type *result = (dig_fil_q15Type *)malloc( sizeof( dig_fil_q15Type ) );	// Allocate memory for the object
	dig_fil_q15_init( result );											// Initialize it
	return result;																// Return the result
}

void dig_fil_q15_destroy( dig_fil_q15Type *pObject )
{
	free( pObject );
}

 void dig_fil_q15_init( dig_fil_q15Type * pThis )
{
	arm_biquad_cascade_df1_init_q15(	&pThis->instance, dig_fil_q15_numStages, dig_fil_q15_coefficients, pThis->state, dig_fil_q15_postShift );
	dig_fil_q15_reset( pThis );

}

 void dig_fil_q15_reset( dig_fil_q15Type * pThis )
{
	memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
	pThis->output = 0;									// Reset output

}

 int dig_fil_q15_filterBlock( dig_fil_q15Type * pThis, short * pInput, short * pOutput, unsigned int count )
{
	arm_biquad_cascade_df1_q15( &pThis->instance, pInput, pOutput, count );
	return count;

}