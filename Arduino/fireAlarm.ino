#include "LoRaWan_APP.h"
#include <CayenneLPP.h>
#include "BME.h"
#include "LORA.h"
/*
 
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green (Short) means received done;
 * RGB green(Long) means take BME Sensor Measurement;
 * RGB red (1 Sec) means fire Deteced   ;
 
 */
#define INT_GPIO USER_KEY  // emergency button, also known as the SOS button

bool wake = false;    //variable is used to indicate if the device wake up from SOS Button
int counter = 3;      //A counter used to send measurements to the hotspot every 4 measurements
bool isfire = false;  // variable used to indicate  a fire is detected or not.

unsigned int fire_Air_Quality = 15000;  //  which is the threshold for air resistance when fire is detected.


/* Prepares the payload of the frame */
static bool prepareTxFrame(uint8_t port) {

  CayenneLPP lpp(LORAWAN_APP_DATA_MAX_SIZE);  //   initialize a Cayenne Low Power Payload  object
  switch (port) {
    case 1:
      lpp.addSwitch(1, true);  //SOS signal
      isTxConfirmed = true;
      Serial.println("SoS paket");


      break;
    case 2:

      getBMEdata(); //get the temperature, humidity, pressure, and air resistance values
      lpp.reset();  // Reset the buffer

      lpp.addTemperature(1, (temp / 100.00) - 5.20);                 // -5.20 C°  grad  temp offset
      lpp.addRelativeHumidity(1, (humidity / 1000.00) + 10);         // Humidity milli-pct +10% offset
      lpp.addPower(1, map(BoardGetBatteryLevel(), 1, 254, 0, 100));  //map BatteryLevel to %
      lpp.addBarometricPressure(1, pressure / 100.00);               //Pressure Pascals
      lpp.addFrequency(1, gas);                                      //Air resistance Ω

      //Fire Detection scenario
      //if Air_resistance is less than fire Airqualty  .fire Property will be True and the node send payload to Helium Network directly.
      isfire = (gas < fire_Air_Quality);
      if (isfire) {
        turnOnRGB(0xF1C232, 1000);  //RGB led red for 1 Second

        isTxConfirmed = true;  //Sending the payload in this scenario is  confirmed
      } else {
        isTxConfirmed = false;
      }

      lpp.addSwitch(1, isfire);  //Fire signal


      break;
  }




  lpp.getBuffer(),
    appDataSize = lpp.getSize();
  memcpy(appData, lpp.getBuffer(), appDataSize);
  byte* buffer = lpp.getBuffer();  // Get the buffer
  return true;
}
void onWakeUp() {
  delay(10);
  if (digitalRead(INT_GPIO) == HIGH) {
    wake = true;  // Sos scenario 
  }
}

void setup() {

  Serial.begin(115200);
  BMEstart();  //BME Sensor start
  deviceState = DEVICE_STATE_INIT;
  
  //If the device has already joined the network before going to sleep, it can skip the join procedure to save time and power. 
  LoRaWAN.ifskipjoin();

  wake = false;

  pinMode(INT_GPIO, INPUT);
  attachInterrupt(INT_GPIO, onWakeUp, RISING);
}

void loop() {

  switch (deviceState) {
    case DEVICE_STATE_INIT:
      {

        LoRaWAN.init(loraWanClass, loraWanRegion);
        deviceState = DEVICE_STATE_JOIN;
        break;
      }
    case DEVICE_STATE_JOIN:
      {
        LoRaWAN.join();

        break;
      }
    case DEVICE_STATE_SEND:
      {
        appPort = 2;
        counter++;
        prepareTxFrame(appPort);
        if (counter == 4 || isfire) {

          if (isfire) Serial.println("Fire Detection scenario Paket");
          else Serial.println("Regular scenario Data Paket");


          LoRaWAN.send();
          counter = 0;  // reset the counter
        }
        wake = false;
        deviceState = DEVICE_STATE_CYCLE;
        break;
      }
    case DEVICE_STATE_CYCLE:
      {
        // Schedule next packet transmission
        txDutyCycleTime = appTxDutyCycle + randr(0, APP_TX_DUTYCYCLE_RND);

        LoRaWAN.cycle(txDutyCycleTime);
        deviceState = DEVICE_STATE_SLEEP;
        break;
      }
    case DEVICE_STATE_SLEEP:
      {

        if (wake) {
          appPort = 1;
          if (IsLoRaMacNetworkJoined) {
            if (prepareTxFrame(appPort)) {
              LoRaWAN.send();
            }
          }
          wake = false;
          appPort = 2;
        }
        LoRaWAN.sleep();
        break;
      }
    default:
      {
        deviceState = DEVICE_STATE_INIT;
        break;
      }
  }
}