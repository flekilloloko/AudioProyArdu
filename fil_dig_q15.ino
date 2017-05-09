#include "fil_dig_q15.h"

q15_t muestreo[1024];
q15_t salida[1024];
unsigned long tiempo;
fil_dig_q15Type *fil_1;

void setup()
{
  Serial.begin(9600); 
  fil_1 = fil_dig_q15_create(); 
}


void loop() {
  llenar_muestras(muestreo);

  fil_dig_q15_llenarCoef(fil_1, 5000, 'A');
  
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
