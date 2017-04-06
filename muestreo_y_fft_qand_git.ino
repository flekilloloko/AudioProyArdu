#define ARM_MATH_CM3
#include <arm_math.h>
//#include <arm_const_structs.h>

const int pinLed = 13;
const int pinAnalogico = A0;

#define TEST_LENGTH_SAMPLES 2048

static q31_t testOutput[TEST_LENGTH_SAMPLES/2];
static q31_t salida[TEST_LENGTH_SAMPLES/2];


/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t fftSize = 1024;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;
int8_t  posShift = 1;


q31_t muestreo[TEST_LENGTH_SAMPLES] = {0};
q31_t sFiltrada[TEST_LENGTH_SAMPLES/2] = {0};
q31_t sNoFiltrada[TEST_LENGTH_SAMPLES/2] = {0};
q31_t estados[4];
q31_t coef[6];

//int input = A0;
//int val;
//#define BUFFER_SIZE  1000
//uint16_t buf[BUFFER_SIZE];



void setup()
{
  Serial.begin(9600); 
  //adc_setup ();
}

double ventaneo(int n){
  double resultado;
  resultado = 0.54 - 0.46 * cos(2*PI*n/(fftSize-1));   //Hamming window
  return resultado;
}

void llenar_muestras(q31_t *arreglo){
  for(int i=0 ; i<fftSize*2 ; i+=2){
    arreglo[i] = (q31_t) ((cos(PI*2*1000*i*0.00002) + cos(PI*2*7000*i*0.00002 )) * ventaneo(i/2) * (1<<29)); //50 KHz de muestreo
    arreglo[i+1] = 0;
  }
}

void llenar_coef(q31_t *arreglo){
  arreglo[0] = (0.644044808751193 / (1<<1)) * (1<<31);  // b0
  arreglo[1] = 0;
  arreglo[2] = (-1.28808961750239 / (1<<1)) * (1<<31);  // b1
  arreglo[3] = (0.644044808751193 / (1<<1)) * (1<<31);  // b2
  arreglo[4] = (-1.08616999241551 / (1<<1)) * (1<<31);  // a1
  arreglo[5] = (0.490009242589262 / (1<<1)) * (1<<31);  // a2
}

void loop() {
  //unsigned long reg;
  
  arm_biquad_casd_df1_inst_q31 F;
  
  //reg = REG_ADC_MR;
  
  //rellenar_muestras(muestreo);
  //corrimiento = 29;
  llenar_muestras(muestreo);
  llenar_coef(coef);
  
  arm_status status;
  status = ARM_MATH_SUCCESS; 
  arm_rfft_instance_q31 S;
  //tiempo = millis();
  arm_rfft_init_q31(&S, fftSize, ifftFlag, doBitReverse);
  

  arm_biquad_cascade_df1_init_q31(&F, 1, coef, estados, posShift);
  
  //tiempo -= millis();
  
  /*for(int i = 1; i<1024; i++) {
      Serial.print(int(salida[i]/(1<<16)));
      Serial.print(",");
      
      Serial.flush();
      
  }*/
 
  // static uint8_t pwmCnt=10;
  //analogWrite(pwm,pwmCnt);
  //pwmCnt+=30;
  // configure Peripheral DMA
  /*PDC_ADC->PERIPH_RPR = (uint32_t) buf; // address of buffer
  PDC_ADC->PERIPH_RCR = BUFFER_SIZE; 
  PDC_ADC->PERIPH_PTCR = PERIPH_PTCR_RXTEN; // enable receive*/
  //digitalWrite(led,HIGH);
                       //while((adc_get_status(ADC) & ADC_ISR_ENDRX) == 0)
                          //{};
  //tiempo = millis() - tiempo;
  for(int i=0 ; i<fftSize ; i++)
    sFiltrada[i] = (q31_t) ((cos(1000*i*0.00002) + cos(7000*i*0.00002)) * ventaneo(i) * (1<<28)); //50 KHz de muestreo
    
  
  //arm_biquad_cascade_df1_fast_q31(&F, sNoFiltrada, sFiltrada, fftSize);
  for(int i=0 ; i<fftSize*2 ; i+=2)
    muestreo[i] = sFiltrada[(int)(i/2)]; //50 KHz de muestreo

 arm_rfft_q31(&S, muestreo, testOutput); //testInput_f32_10khz
 arm_cmplx_mag_q31(testOutput, salida, fftSize);  
   Serial.print("_");
  for(int i = 1; i<1024; i++) {
      Serial.print(int(salida[i]/(1<<13)));
      Serial.print(",");
      
      Serial.flush();
      
  }
  //Serial.print("la conv tomó: ");Serial.print(tiempo);
  //Serial.print("   El registro MSR es: ");Serial.println(reg, HEX);
  /*Serial.print("Andas!!_");
  delay(500);*/
  delay(2000);
}
  
/*
void adc_setup(){
  // Setup all registers
  pmc_enable_periph_clk (ID_ADC);
  adc_init (ADC, SystemCoreClock, ADC_FREQ_MAX, 2);//adc_init (ADC, , ADC_FREQ_MAX, ADC_STARTUP_FAST);
  adc_disable_interrupt (ADC, 0xFFFFFFFF);
  adc_set_resolution (ADC, ADC_10_BITS);
  adc_configure_power_save (ADC, ADC_MR_SLEEP_NORMAL, ADC_MR_FWUP_OFF);
  adc_configure_timing (ADC, 0, ADC_SETTLING_TIME_3, 1);
  adc_set_bias_current (ADC, 1);
  adc_disable_tag (ADC);
  adc_disable_ts (ADC);
  adc_stop_sequencer (ADC);
  adc_disable_channel_differential_input (ADC, ADC_CHANNEL_7);
  adc_disable_all_channel (ADC);
  adc_enable_channel (ADC, ADC_CHANNEL_7);
  adc_configure_trigger (ADC, ADC_TRIG_SW, 1);
  //Serial1.begin(115200);

  REG_ADC_MR = (REG_ADC_MR & 0xFFF0FFFF) | 0x00020000;
  adc_start( ADC );
  //tiempo = millis();
}

void rellenar_muestras(q31_t *muestre){
  
  for(int i=0 ; i<fftSize*2 ; i+=2){
    muestre[i] = (q31_t) (cos(PI*2*2000*i*0.00002) * (1<<corrimiento)); //50 KHz de muestreo
    muestre[i+1] = 0;
  }
  //return muestre;
}*/

