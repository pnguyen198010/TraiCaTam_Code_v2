#ifndef lora_service_h
#define lora_service_h


/* ==================================================
** Libraries
**
** =============================================== */


#include <Arduino.h>
#include <LoRa_E32.h>
#include <SoftwareSerial.h>


/* ==================================================
** Macro definition
**
** =============================================== */


//


/* ==================================================
** Type definition
**
** =============================================== */


//


/* ==================================================
** Extern function declaration
**
** =============================================== */


void Lora_init();

// ResponseStatus Lora_send_fixedMessage(byte ADDH, byte ADDL, byte CHAN, String message);

// void Lora_receive_fixedMessage();

void Lora_upd_turbidity();

void Lora_send_turbidityState();


/* ==================================================
** Extern variables
**
** =============================================== */


//


#endif