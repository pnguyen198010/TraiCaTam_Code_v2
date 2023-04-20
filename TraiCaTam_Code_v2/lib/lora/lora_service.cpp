/* ==================================================
** Libraries
**
** =============================================== */


#include "lora_service.h"
#include "lora_address.h"

#include <LoRa_E32.h>

#include "log_service.h"


/* ==================================================
** Macro definition
**
** =============================================== */


#define LORA_AUX        4
#define LORA_RX         16
#define LORA_TX         17
#define LORA_M1         18
#define LORA_M0         19

#define LORA_BAUD_RATE  UART_BPS_RATE_9600


#define THIS_IS_LORA_OF_GATEWAY
// #define THIS_IS_LORA_OF_SENSOR1
// #define THIS_IS_LORA_OF_SENSOR2
// #define THIS_IS_LORA_OF_SENSOR3
// #define THIS_IS_LORA_OF_BELL1_LARGE
// #define THIS_IS_LORA_OF_BELL1_SMALL
// #define THIS_IS_LORA_OF_BELL2_LARGE
// #define THIS_IS_LORA_OF_BELL2_SMALL

#ifdef THIS_IS_LORA_OF_GATEWAY
#define LORA_ADDL   ADDL_GATEWAY
#define LORA_ADDH	ADDH_GATEWAY
#define LORA_CHAN	CHAN_GATEWAY
#endif

#ifdef THIS_IS_LORA_OF_SENSOR1
#define LORA_ADDL   ADDL_SENSOR1
#define LORA_ADDH	ADDH_SENSOR1
#define LORA_CHAN	CHAN_SENSOR1
#endif

#ifdef THIS_IS_LORA_OF_SENSOR2
#define LORA_ADDL   ADDL_SENSOR2
#define LORA_ADDH	ADDH_SENSOR2
#define LORA_CHAN	CHAN_SENSOR2
#endif

#ifdef THIS_IS_LORA_OF_SENSOR3
#define LORA_ADDL   ADDL_SENSOR3
#define LORA_ADDH	ADDH_SENSOR3
#define LORA_CHAN	CHAN_SENSOR3
#endif

#ifdef THIS_IS_LORA_OF_BELL1_LARGE
#define LORA_ADDL   ADDL_BELL1_LARGE
#define LORA_ADDH	ADDH_BELL1_LARGE
#define LORA_CHAN	CHAN_BELL1_LARGE
#endif

#ifdef THIS_IS_LORA_OF_BELL1_SMALL
#define LORA_ADDL   ADDL_BELL1_SMALL
#define LORA_ADDH	ADDH_BELL1_SMALL
#define LORA_CHAN	CHAN_BELL1_SMALL
#endif

#ifdef THIS_IS_LORA_OF_BELL2_LARGE
#define LORA_ADDL   ADDL_BELL2_LARGE
#define LORA_ADDH	ADDH_BELL2_LARGE
#define LORA_CHAN	CHAN_BELL2_LARGE
#endif

#ifdef THIS_IS_LORA_OF_BELL2_SMALL
#define LORA_ADDL   ADDL_BELL2_SMALL
#define LORA_ADDH	ADDH_BELL2_SMALL
#define LORA_CHAN	CHAN_BELL2_SMALL
#endif


// Messages of sensor 1: state
#define MESSAGE_SENSOR1_CLEAR	0x00
#define MESSAGE_SENSOR1_TURBID	0x01

// Messages of sensor 2: state
#define MESSAGE_SENSOR2_CLEAR	0x10
#define MESSAGE_SENSOR2_TURBID	0x11

// Messages of sensor 3: state
#define MESSAGE_SENSOR3_CLEAR	0x20
#define MESSAGE_SENSOR3_TURBID	0x21

// Messages of gateway: which bell will alert
#define MESSAGE_TURN_OFF_BELL1	0x30
#define MESSAGE_TURN_ON_BELL1	0x31

#define MESSAGE_TURN_OFF_BELL2	0x40
#define MESSAGE_TURN_ON_BELL2	0x41

// Messages of bell 1: heart beat
#define MESSAGE_BELL1_LARGE		0x50
#define MESSAGE_BELL1_SMALL		0x51

// Messages of bell 2: heart beat
#define MESSAGE_BELL2_LARGE		0x52
#define MESSAGE_BELL2_SMALL		0x53


#define LORA_NUM_CLIENT			7
#define LORA_NUM_SENSOR			3
#define LORA_NUM_BELL_LARGE		2
#define LORA_NUM_BELL_SMALL		2


/* ==================================================
** Type definition
**
** =============================================== */


typedef enum
{
	IND_SENSOR1 = 0,
	IND_SENSOR2,
	IND_SENSOR3,
	IND_BELL1_LARGE,
	IND_BELL1_SMALL,
	IND_BELL2_LARGE,
	IND_BELL2_SMALL,
	
} ind_client_t;


struct message_t
{
	uint8_t package[1];
};


/* ==================================================
** Extern variables
**
** =============================================== */


//


/* ==================================================
** Static variables
**
** =============================================== */


static const uint32_t TIME_HEARTBEAT = 2UL*60UL*1000UL;

static uint32_t intv_heartbeat[LORA_NUM_CLIENT];

static Log_t LOG;
static LoRa_E32 e32ttl100(&Serial2, LORA_BAUD_RATE);


/* ==================================================
** Static function declaration
**
** =============================================== */


static void printParameters(struct Configuration configuration);
static void printModuleInformation(struct ModuleInformation moduleInformation);

static void handle_hearbeat(uint8_t package);
static void handle_package(uint8_t package);

static void control_bell1(bool on);
static void control_bell2(bool on);


/* ==================================================
** Static function definition
**
** =============================================== */


void printParameters(struct Configuration configuration) 
{
	Serial.println("----------------------------------------");

	Serial.print(F("HEAD BIN: "));  Serial.print(configuration.HEAD, BIN);Serial.print(" ");Serial.print(configuration.HEAD, DEC);Serial.print(" ");Serial.println(configuration.HEAD, HEX);
	Serial.println(F(" "));
	Serial.print(F("AddH BIN: "));  Serial.println(configuration.ADDH, BIN);
	Serial.print(F("AddL BIN: "));  Serial.println(configuration.ADDL, BIN);
	Serial.print(F("Chan BIN: "));  Serial.print(configuration.CHAN, DEC); Serial.print(" -> "); Serial.println(configuration.getChannelDescription());
	Serial.println(F(" "));
	Serial.print(F("SpeedParityBit BIN    : "));  Serial.print(configuration.SPED.uartParity, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTParityDescription());
	Serial.print(F("SpeedUARTDataRate BIN : "));  Serial.print(configuration.SPED.uartBaudRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getUARTBaudRate());
	Serial.print(F("SpeedAirDataRate BIN  : "));  Serial.print(configuration.SPED.airDataRate, BIN);Serial.print(" -> "); Serial.println(configuration.SPED.getAirDataRate());

	Serial.print(F("OptionTrans BIN       : "));  Serial.print(configuration.OPTION.fixedTransmission, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getFixedTransmissionDescription());
	Serial.print(F("OptionPullup BIN      : "));  Serial.print(configuration.OPTION.ioDriveMode, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getIODroveModeDescription());
	Serial.print(F("OptionWakeup BIN      : "));  Serial.print(configuration.OPTION.wirelessWakeupTime, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getWirelessWakeUPTimeDescription());
	Serial.print(F("OptionFEC BIN         : "));  Serial.print(configuration.OPTION.fec, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getFECDescription());
	Serial.print(F("OptionPower BIN       : "));  Serial.print(configuration.OPTION.transmissionPower, BIN);Serial.print(" -> "); Serial.println(configuration.OPTION.getTransmissionPowerDescription());

	Serial.println("----------------------------------------");

}


void printModuleInformation(struct ModuleInformation moduleInformation) 
{
	Serial.println("----------------------------------------");
	Serial.print(F("HEAD BIN: "));  Serial.print(moduleInformation.HEAD, BIN);Serial.print(" ");Serial.print(moduleInformation.HEAD, DEC);Serial.print(" ");Serial.println(moduleInformation.HEAD, HEX);

	Serial.print(F("Freq.: "));  Serial.println(moduleInformation.frequency, HEX);
	Serial.print(F("Version  : "));  Serial.println(moduleInformation.version, HEX);
	Serial.print(F("Features : "));  Serial.println(moduleInformation.features, HEX);
	Serial.println("----------------------------------------");

}


static void printConfig()
{

	ResponseStructContainer c;
	c = e32ttl100.getConfiguration();

	// It's important get configuration pointer before all other operation
	Configuration configuration = *(Configuration*) c.data;
	Serial.println(c.status.getResponseDescription());
	Serial.println(c.status.code);

	printParameters(configuration);

	ResponseStructContainer cMi;
	cMi = e32ttl100.getModuleInformation();
	// It's important get information pointer before all other operation
	ModuleInformation mi = *(ModuleInformation*)cMi.data;

	Serial.println(cMi.status.getResponseDescription());
	Serial.println(cMi.status.code);

	printModuleInformation(mi);

	c.close();
	cMi.close();

}


void handle_hearbeat(uint8_t package)
{
	switch(package)
	{
		case MESSAGE_SENSOR1_CLEAR:
		case MESSAGE_SENSOR1_TURBID:
		intv_heartbeat[IND_SENSOR1] = millis();
		break;

		
		case MESSAGE_SENSOR2_CLEAR:
		case MESSAGE_SENSOR2_TURBID:
		intv_heartbeat[IND_SENSOR2] = millis();
		break;

		case MESSAGE_SENSOR3_CLEAR:
		case MESSAGE_SENSOR3_TURBID:
		intv_heartbeat[IND_SENSOR3] = millis();
		break;

		case MESSAGE_BELL1_LARGE:
		intv_heartbeat[IND_BELL1_LARGE] = millis();
		break;

		case MESSAGE_BELL1_SMALL:
		intv_heartbeat[IND_BELL1_SMALL] = millis();
		break;

		case MESSAGE_BELL2_LARGE:
		intv_heartbeat[IND_BELL2_LARGE] = millis();
		break;

		case MESSAGE_BELL2_SMALL:
		intv_heartbeat[IND_BELL2_SMALL] = millis();
		break;
	}

	uint32_t millis_curr = millis();
	for(uint8_t i=0; i<LORA_NUM_CLIENT; ++i)
	{
		if(millis_curr - intv_heartbeat[i] > TIME_HEARTBEAT)
		{
			//
		}
	}
}


void handle_package(uint8_t package)
{
	switch(package)
	{
		case MESSAGE_BELL1_LARGE:
		case MESSAGE_BELL1_SMALL:
		case MESSAGE_BELL2_LARGE:
		case MESSAGE_BELL2_SMALL:
		break;

		case MESSAGE_SENSOR1_TURBID:
		case MESSAGE_SENSOR2_TURBID:
		control_bell1(HIGH);
		break;

		case MESSAGE_SENSOR3_TURBID:
		control_bell2(HIGH);
		break;

		case MESSAGE_SENSOR1_CLEAR:
		case MESSAGE_SENSOR2_CLEAR:
		control_bell1(LOW);
		break;

		case MESSAGE_SENSOR3_CLEAR:
		control_bell2(LOW);
		break;
	}
}


void control_bell1(bool on)
{
	struct message_t
	{
		uint8_t package[1];
	} message;

	*(uint8_t*)(message.package) = on ? MESSAGE_TURN_ON_BELL1 : MESSAGE_TURN_OFF_BELL1;

	e32ttl100.sendBroadcastFixedMessage(CHAN_BELL1, &message, sizeof(message_t));
}


void control_bell2(bool on)
{
	struct message_t
	{
		uint8_t package[1];
	} message;

	*(uint8_t*)(message.package) = on ? MESSAGE_TURN_ON_BELL2 : MESSAGE_TURN_OFF_BELL2;

	e32ttl100.sendBroadcastFixedMessage(CHAN_BELL2, &message, sizeof(message_t));
}


/* ==================================================
** Extern function definition
**
** =============================================== */


void Lora_init()
{

	pinMode(LORA_AUX, INPUT);
	pinMode(LORA_M0, OUTPUT);
	pinMode(LORA_M1, OUTPUT);

	digitalWrite(LORA_M0, HIGH);
	digitalWrite(LORA_M1, HIGH);

	e32ttl100.begin();

	ResponseStructContainer c   = e32ttl100.getConfiguration();
	Configuration configuration = *(Configuration*) c.data;

	// Serial.println(c.status.getResponseDescription());
	// Serial.println(c.status.code);
	// printParameters(configuration);

	configuration.ADDL = LORA_ADDL;
	configuration.ADDH = LORA_ADDH;
	configuration.CHAN = LORA_CHAN;

	configuration.OPTION.fec                = FEC_1_ON;
	configuration.OPTION.fixedTransmission  = FT_FIXED_TRANSMISSION;
	configuration.OPTION.ioDriveMode        = IO_D_MODE_PUSH_PULLS_PULL_UPS;
	configuration.OPTION.transmissionPower  = POWER_17;
	configuration.OPTION.wirelessWakeupTime = WAKE_UP_1250;

	configuration.SPED.airDataRate  = AIR_DATA_RATE_010_24;
	configuration.SPED.uartBaudRate = UART_BPS_9600;
	configuration.SPED.uartParity   = MODE_00_8N1;

	e32ttl100.setConfiguration(configuration, WRITE_CFG_PWR_DWN_LOSE);

	// ResponseStatus rs = e32ttl100.setConfiguration(configuration, WRITE_CFG_PWR_DWN_LOSE);
	// Serial.println(rs.getResponseDescription());
	// Serial.println(rs.code);
	// printParameters(configuration);

	c.close();

	printConfig();

	digitalWrite(LORA_M0, LOW);
	digitalWrite(LORA_M1, LOW);
}


void Lora_receive_message()
{
	if(e32ttl100.available() < 1) {return;}

	ResponseStructContainer rsc = e32ttl100.receiveMessage(sizeof(message_t));
	message_t message = *(message_t *)rsc.data;
	uint8_t package = message.package[0];

	LOG.inf("[Lora] receive package: '%#04X'", package);
	free(rsc.data);

	handle_hearbeat(package);
	handle_package (package);
}