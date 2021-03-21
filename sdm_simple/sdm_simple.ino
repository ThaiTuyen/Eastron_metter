
//REMEMBER! uncomment #define USE_HARDWARESERIAL 
//in SDM_Config_User.h file if you want to use hardware uart

#include "SDM.h"                                                               //import SDM library

#if defined ( USE_HARDWARESERIAL )                                              //for HWSERIAL                                                                    //for AVR
SDM sdm(Serial1, 9600, NOT_A_PIN);                                              //config SDM on Serial1 (if available!)
#else                                                                           //for SWSERIAL
#include <SoftwareSerial.h>                                                     //import SoftwareSerial library                                                                     //for AVR
SoftwareSerial swSerSDM(SDM_RX_PIN, SDM_TX_PIN);                                //config SoftwareSerial
SDM sdm(swSerSDM, 9600, NOT_A_PIN);                                             //config SDM
#endif

void setup() {
  Serial.begin(9600);                                                         //initialize serial
  sdm.begin();                                                                  //initialize SDM communication
}

void loop() {
  char bufout[10];
  sprintf(bufout, "%c[1;0H", 27);
  Serial.print(bufout);

  Serial.print("Voltage:   ");
  Serial.print(sdm.readVal(SDM_PHASE_1_VOLTAGE), 2);                            //display voltage
  Serial.println("V");

  Serial.print("Current:   ");
  Serial.print(sdm.readVal(SDM_PHASE_1_CURRENT), 2);                            //display current  
  Serial.println("A");

  Serial.print("Power:     ");
  Serial.print(sdm.readVal(SDM_PHASE_1_POWER), 2);                              //display power
  Serial.println("W");

  Serial.print("Frequency: ");
  Serial.print(sdm.readVal(SDM_FREQUENCY), 2);                                  //display frequency
  Serial.println("Hz");   

  delay(1000);                                                                  //wait a while before next loop
}
