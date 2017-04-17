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

// Begin header file, fil_iir_q15.h

#ifndef FIL_IIR_Q15_H_ // Include guards
#define FIL_IIR_Q15_H_

#define ARM_MATH_CM3	// Use ARM Cortex M3
//#define __FPU_PRESENT 1		// Does this device have a floating point unit?
#include <arm_math.h>	// Include CMSIS header

// Link with library: libarm_cortexM3_mathL.a (or equivalent)
// Add CMSIS/Lib/GCC to the library search path
// Add CMSIS/Include to the include search path
extern q15_t fil_iir_q15_coeficientes[12];
extern int fil_iir_q15_grado;
extern int fil_iir_q15_posCorr;

typedef struct
{
	arm_biquad_casd_df1_inst_q15 instancia;
	q15_t estados[8];
	q15_t salida;
} fil_iir_q15Type;


fil_iir_q15Type *fil_iir_q15_create( void );
void fil_iir_q15_destruir( fil_iir_q15Type *puntObj );
 void fil_iir_q15_inic( fil_iir_q15Type * puntEste );
 void fil_iir_q15_reiniciar( fil_iir_q15Type * puntEste );
#define fil_iir_q15_writeInput( puntEste, input )  \
	arm_biquad_cascade_df1_q15( &puntEste->instancia, &input, &puntEste->salida, 1 );

#define fil_iir_q15_readsalida( puntEste )  \
	puntEste->salida


 int fil_iir_q15_filterBlock( fil_iir_q15Type * puntEste, short * puntEntrada, short * puntSalida, unsigned int count );
#define fil_iir_q15_salidaToFloat( salida )  \
	(( (1.0f/8192) * (salida) ))    //2 ^ 13

#define fil_iir_q15_inputFromFloat( input )  \
	((short)(32768f * (input)))		//2 ^ 15

#endif // fil_iir_q15_H_