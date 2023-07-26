
/* OTAA para Helium */
uint8_t devEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* ABP para not used */
uint8_t nwkSKey[16];
uint8_t appSKey[16] ;
uint32_t devAddr;


/*LoraWan channelsmask, default channels 0-7*/
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t loraWanClass = LORAWAN_CLASS;

/*wake up every 450 sec(7.5 min) and take measurement */
uint32_t appTxDutyCycle = (7.5 * 60 * 1000);

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;
  
/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = true;


/* Application port */
uint8_t appPort = 2; // default scenario (Regular scenario Data Paket )
/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment.
*/
uint8_t confirmedNbTrials = 3;