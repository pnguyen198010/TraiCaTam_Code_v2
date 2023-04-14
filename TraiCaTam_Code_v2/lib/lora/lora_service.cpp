/* ==================================================
** Libraries
**
** =============================================== */


#include "lora_service.h"

#include "turbidity.h"


/* ==================================================
** Macro definition
**
** =============================================== */


#define LORA_RX         5
#define LORA_TX         6
#define LORA_AUX        4
#define LORA_M0         7
#define LORA_M1         8

#define LORA_BAUD_RATE  UART_BPS_RATE_9600

#define LORA_ID_SLAVE	"T0"

#define LORA_ADDL       0x01
#define LORA_ADDH       0x00
#define LORA_CHAN       0x19

#define LORA_ADDL_GATEWAY	0x0
#define LORA_ADDH_GATEWAY	0x1
#define LORA_CHAN_GATEWAY	0x18


/* ==================================================
** Type definition
**
** =============================================== */


//


/* ==================================================
** Extern variables
**
** =============================================== */


//


/* ==================================================
** Static variables
**
** =============================================== */


// static const byte LORA_RX        = 3;
// static const byte LORA_TX        = 4;
// static const byte LORA_AUX       = 5;
// static const byte LORA_M0        = 5;
// static const byte LORA_M1        = 6;

// static const byte LORA_ADDL      = 0x0;
// static const byte LORA_ADDH      = 0x2;
// static const byte LORA_CHAN      = 0x19;

// static const uint32_t TIME_UPD_TURBIDITY = 1 * 60 * 1000;
static const uint32_t TIME_UPD_TURBIDITY = 60 * 1000;

static SoftwareSerial mySerial(LORA_RX, LORA_TX);
static LoRa_E32 e32ttl100(&mySerial, LORA_BAUD_RATE);


/* ==================================================
** Static function declaration
**
** =============================================== */


static void printParameters(struct Configuration configuration);
static 
void printModuleInformation(struct ModuleInformation moduleInformation);


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

	ResponseStatus rs = e32ttl100.setConfiguration(configuration, WRITE_CFG_PWR_DWN_LOSE);

	// Serial.println(rs.getResponseDescription());
	// Serial.println(rs.code);
	// printParameters(configuration);

	c.close();

	printConfig();

	digitalWrite(LORA_M0, LOW);
	digitalWrite(LORA_M1, LOW);
}


// ResponseStatus Lora_send_fixedMessage(byte ADDH, byte ADDL, byte CHAN, String message)
// {
//     ResponseStatus rs = e32ttl100.sendFixedMessage(ADDH, ADDL, CHAN, message);

//     // Serial.println("Send message to 00 03 04");
//     // Serial.println(rs.getResponseDescription());

//     return rs;
// }


// void Lora_receive_fixedMessage()
// {
//     if (e32ttl100.available() > 1)
//     {
//         ResponseContainer rs = e32ttl100.receiveMessage();
//         // First of all get the data
//         String message = rs.data;

//         Serial.println(rs.status.getResponseDescription());
//         Serial.println(message);
//     }
// }


void Lora_upd_turbidity()
{
	static uint32_t intv = millis();

	if(millis() - intv < TIME_UPD_TURBIDITY) {return;}

	uint8_t turbidity = Turbidity_is_true();
	String  message   = String(LORA_ID_SLAVE) + "-" + String(turbidity);

	ResponseStatus rs = e32ttl100.sendFixedMessage(LORA_ADDH_GATEWAY, LORA_ADDL_GATEWAY, LORA_CHAN_GATEWAY, message);

	Serial.print("Lora message: ");
	Serial.println(message);
	
    // Serial.println("Send message to 00 03 04");
    // Serial.println(rs.getResponseDescription());

	intv = millis();
}