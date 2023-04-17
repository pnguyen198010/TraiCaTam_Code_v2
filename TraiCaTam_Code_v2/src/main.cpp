/* ==================================================
** Library
**
** =============================================== */


#include <Arduino.h>
#include <SoftwareSerial.h>

#include "log_service.h"
#include "turbidity.h"
#include "servo_service.h"
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
  Turbidity_init();
  Servo_init();
  Lora_init();
}


void loop() 
{
  Turbidity_read();
}