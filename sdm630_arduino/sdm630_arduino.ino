#include "SDM.h"                                                               //import SDM library

const int dere_pin = 2;

#if defined ( USE_HARDWARESERIAL )                                              //for HWSERIAL                                                                       //for AVR
SDM sdm(Serial, 9600, dere_pin);                                              //config SDM on Serial1 (if available!)
#else                                                                           //for SWSERIAL
#include <SoftwareSerial.h>                                                     //import SoftwareSerial library                                                                        //for AVR
SoftwareSerial swSerSDM(SDM_RX_PIN, SDM_TX_PIN);                                //config SoftwareSerial
SDM sdm(swSerSDM, 9600, dere_pin);                                             //config SDM
#endif //#if defined ( USE_HARDWARESERIAL )

#define READSDMEVERY  1000                                                      //read sdm every 2000ms
#define NBREG   23    // SET TO the number of parameters in sdm_struct sdmarr[NBREG] and maximum 40 


typedef struct {
  float regvalarr;
  const uint16_t regarr;
  const String regtext;
} sdm_struct;

sdm_struct sdmarr[NBREG] = {
  {0.00, SDM_PHASE_1_VOLTAGE,"VoltageL1"},                                      //V
  {0.00, SDM_PHASE_2_VOLTAGE,"VoltageL2"},                                      //V
  {0.00, SDM_PHASE_3_VOLTAGE,"VoltageL3"},                                      //V
  {0.00, SDM_PHASE_1_CURRENT,"CurrentL1"},                                      //A
  {0.00, SDM_PHASE_2_CURRENT,"CurrentL2"},                                      //A
  {0.00, SDM_PHASE_3_CURRENT,"CurrentL3"},                                      //A
  {0.00, SDM_SUM_LINE_CURRENT,"CurrentSUM"},                                    //A
  {0.00, SDM_PHASE_1_POWER,"PowerL1"},                                          //W
  {0.00, SDM_PHASE_2_POWER,"PowerL2"},                                          //W
  {0.00, SDM_PHASE_3_POWER,"PowerL3"},                                          //W
  {0.00, SDM_TOTAL_SYSTEM_POWER,"PowerSUM"},                                    //W
  {0.00, SDM_TOTAL_SYSTEM_POWER_FACTOR,"PFTOTAL"},                              //PF
  {0.00, SDM_FREQUENCY,"FREQUENCY"},                                            //Hz
  {0.00, SDM_IMPORT_ACTIVE_ENERGY,"ImportEnergi"},                              //kWh
  {0.00, SDM_TOTAL_ACTIVE_ENERGY,"TotalEnergi"},                                //kWh
  {0.00, SDM_LINE_1_TO_LINE_2_VOLTS,"VoltageL1L2"},                             //V
  {0.00, SDM_LINE_2_TO_LINE_3_VOLTS,"VoltageL2L3"},                             //V
  {0.00, SDM_LINE_3_TO_LINE_1_VOLTS,"VoltageL3L1"},                             //V
  {0.00, SDM_TOTAL_SYSTEM_REACTIVE_POWER,"ReactivePowerSUM"},                   //VAr
  {0.00, SDM_TOTAL_SYSTEM_APPARENT_POWER,"ApparentPowerSUM"},                   //VA
  {0.00, SDM_L1_IMPORT_ACTIVE_ENERGY,"ImportL1"},                               //kWh
  {0.00, SDM_L2_IMPORT_ACTIVE_ENERGY,"ImportL2"},                               //kWh
  {0.00, SDM_L3_IMPORT_ACTIVE_ENERGY,"ImportL3"}                                //kWh
};

unsigned long readtime;
int ntpSyncTime = 3600;
bool read_done = false;


void setup() {
  //Serial.begin(115200);                                                         //initialize serial
  sdm.begin();                                                                  //initialize SDM communication

}

void loop() {

  if (millis() - readtime >= READSDMEVERY) {
    sdmRead();
    readtime = millis();
  }

  if(read_done)
  {
      for(int i = 0; i < NBREG; i++) 
      {
        Serial.print(sdmarr[i].regtext);
        Serial.print(" = ");
        Serial.println(sdmarr[i].regvalarr);
        }
    read_done = false;
    }
  yield();
}


//------------------------------------------------------------------------------


void sdmRead() {
  float tmpval = NAN;

  for (uint8_t i = 0; i < NBREG; i++) {
    tmpval = sdm.readVal(sdmarr[i].regarr);

    if (isnan(tmpval))
      sdmarr[i].regvalarr = 0.00;
    else
      sdmarr[i].regvalarr = tmpval;

    yield();
  }
  read_done = true;
}
