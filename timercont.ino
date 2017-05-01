/*  Proyecto de muestreo a 44.1 KHz (frecuencia estándar de muestreo de audio)  *  
 *  para lograr esta frecuencia, utilizo 952 períodos de MCK/2                  *
 *  fmuestreo = 44,11765 kHz                                                    *  
 *  El tiempo de una converción ADC - DAC total es de 10uS, por lo que entra    *
 *  dentro del período de interrupción.                                         */

int led = 13;

volatile boolean ledon;
volatile unsigned long lasttime;
volatile unsigned long now, lapso;
uint32_t rc;

int FREQ_Hz = 44100;

void TC3_Handler(){
    TC_GetStatus(TC1, 0);
    
    
    
}

void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency){
  
    //Enable or disable write protect of PMC registers.
    pmc_set_writeprotect(false);
    //Enable the specified peripheral clock.
    pmc_enable_periph_clk((uint32_t)irq);  
    
    TC_Configure(tc, channel, TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC|TC_CMR_TCCLKS_TIMER_CLOCK1);  //TIMER_CLOCK1 = 2 de preescaler (MCK/2)
    rc = VARIANT_MCK/2/frequency ;  //VARIANT_MCK = 84000000
                                                 
    TC_SetRA(tc, channel, rc/2);  //50% Duty Cycle
    TC_SetRC(tc, channel, rc);    //Período
    TC_Start(tc, channel);
    
    tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;
    tc->TC_CHANNEL[channel].TC_IDR = ~TC_IER_CPCS;
    NVIC_EnableIRQ(irq);
}

void setup() {
    pinMode(led, OUTPUT);
    Serial.begin(9600);
    
    
    lasttime = 0;
    startTimer(TC1, 0, TC3_IRQn, FREQ_Hz);
    
    //analogReference
    //analogResolution
}

void loop() {
  int val;
    lasttime = micros();
    
    val = analogRead(A1);
    analogWrite(DAC0, val );
    now = micros ();
    lapso = now - lasttime;
    //lasttime = now;
    Serial.print("    Tiempo de converciones(DAC - ADC) = ");
    Serial.print(lapso);
    delay(1000);
}
