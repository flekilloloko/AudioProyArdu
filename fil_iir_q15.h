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

// Begin header file, dig_fil_q15.h

#ifndef DIG_FIL_Q15_H_ // Include guards
#define DIG_FIL_Q15_H_

#define ARM_MATH_CM3	// Use ARM Cortex M3
#define __FPU_PRESENT 1		// Does this device have a floating point unit?
#include <arm_math.h>	// Include CMSIS header

// Link with library: libarm_cortexM3_mathL.a (or equivalent)
// Add CMSIS/Lib/GCC to the library search path
// Add CMSIS/Include to the include search path
extern q15_t dig_fil_q15_coefficients[12];
static const int dig_fil_q15_numStages = 2;
static const int dig_fil_q15_postShift = 1;

typedef struct
{
	arm_biquad_casd_df1_inst_q15 instance;
	q15_t state[8];
	q15_t output;
} dig_fil_q15Type;


dig_fil_q15Type *dig_fil_q15_create( void );
void dig_fil_q15_destroy( dig_fil_q15Type *pObject );
 void dig_fil_q15_init( dig_fil_q15Type * pThis );
 void dig_fil_q15_reset( dig_fil_q15Type * pThis );
#define dig_fil_q15_writeInput( pThis, input )  \
	arm_biquad_cascade_df1_q15( &pThis->instance, &input, &pThis->output, 1 );

#define dig_fil_q15_readOutput( pThis )  \
	pThis->output


 int dig_fil_q15_filterBlock( dig_fil_q15Type * pThis, short * pInput, short * pOutput, unsigned int count );
#define dig_fil_q15_outputToFloat( output )  \
	(( (1.0f/8192) * (output) ))    //2 ^ 13

#define dig_fil_q15_inputFromFloat( input )  \
	((short)(32768f * (input)))		//2 ^ 15

#endif // DIG_FIL_Q15_H_