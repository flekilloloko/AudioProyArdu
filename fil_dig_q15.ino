#include "fil_dig_q15.h"

q15_t muestreo[1024];
q15_t salida[1024];
unsigned long tiempo;

void setup()
{
  Serial.begin(9600); 

}



void llenar_muestras(q15_t *arreglo){
  for(int i=0 ; i<1024; i++){
    arreglo[i] = ((cos(PI*2*1000*i*((double)1/50000)) + cos(PI*2*7000*i*((double)1/50000) ))/4)*(1<<15) ; 
  }
}
void loop() {
  llenar_muestras(muestreo);

  fil_dig_q15Type *fil_1 = fil_dig_q15_create(); 
  
  Serial.print("_");
  for(int i = 0; i<1024; i++) {
    tiempo = micros();
    fil_dig_q15_writeInput (fil_1, muestreo[i]);
    salida[i]=fil_dig_q15_readOutput(fil_1);
    tiempo = micros () - tiempo ;
    Serial.print((double)salida[i]/(double)(1<<13));
    Serial.print("Tiempo = ");
    Serial.print(tiempo);
    Serial.print(",");
      
    //Serial.flush();
  }

}
