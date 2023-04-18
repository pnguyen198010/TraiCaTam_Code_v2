/* ==================================================
** Library
**
** =============================================== */


#include <Arduino.h>
#include <SoftwareSerial.h>

#include "log_service.h"
#include "bell.h"


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
}


void loop() 
{
  
}