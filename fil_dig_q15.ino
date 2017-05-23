#include "fil_dig_q15.h"

q15_t muestreo[1024];
q15_t salida[1024];
unsigned long tiempo;
fil_dig_q15Type *fil_1;
short frecCorte = 5000;
int recepcion, tamano;
byte cadenaRx[20];

int F_InterrHz = 60;

void TC3_Handler(){
    int i = 0, n = 0;


    frecCorte = 0;
    for(i=0;i<20;i++) cadenaRx[i] = 0;
    
    TC_GetStatus(TC1, 0);
    if(Serial.available()>1){
      recepcion = Serial.read();      //recibo el primer byte
      while(recepcion != 'F' && Serial.available()>0 )
        recepcion = Serial.read();    //busco la primera coma
      recepcion = Serial.read();    //capturo el primer caracter de la frecuencia
      recepcion = Serial.read();
      while(recepcion >= '0' && recepcion <= '9' && Serial.available()>0){                                        // ! tener en cuenta que se reciba el numero entero
        cadenaRx[n] = recepcion;
        //frecCorte += (recepcion - 48) * pow(10,n);
        n++;
        recepcion = Serial.read();
        
      }
      for(i=0;i<n;i++)
        frecCorte += (cadenaRx[n-i-1] - 48) * pow(10,i);
      if(frecCorte>0 && recepcion == 'F'){
        Serial.print("FA"); 
        Serial.print(frecCorte);
        Serial.print("AF");    
        //Serial.print("_");  
      }
      //Serial.print(cadenaRx, BYTE);
    }
}

void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency){
    uint32_t rc;
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

void setup()
{
  Serial.begin(9600); 
  startTimer(TC1, 0, TC3_IRQn, F_InterrHz);
  fil_1 = fil_dig_q15_create(); 
}


void loop() {
  llenar_muestras(muestreo);

  fil_dig_q15_llenarCoef(fil_1, frecCorte, 'A');
  
  Serial.print("_");
  for(int i = 0; i<1024; i++) {
    //tiempo = micros();
    fil_dig_q15_writeInput (fil_1, muestreo[i]);
    salida[i]=fil_dig_q15_readOutput(fil_1);
    //tiempo = micros () - tiempo ;
    Serial.print((double)salida[i]/(double)(1<<13));
    //Serial.print("Tiempo = ");
    //Serial.print(tiempo);
    Serial.print(",");
      
    //Serial.flush();
  }

}
