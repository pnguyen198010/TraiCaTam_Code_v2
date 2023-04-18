/* ==================================================
** Library
**
** =============================================== */


#include <Arduino.h>
#include <SoftwareSerial.h>

#include "log_service.h"
#include "bell.h"
#include "lora_service.h"


/* ==================================================
** static variable
**
** =============================================== */


//


/* ==================================================
** main
**
** =============================================== */


void setup() 
{
  Log_init();
  Bell_init();
  Lora_init();
}


void loop() 
{
  Lora_receive_message();
}