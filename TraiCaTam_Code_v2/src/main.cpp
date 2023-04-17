/* ==================================================
** Library
**
** =============================================== */


#include <Arduino.h>

#include "log_service.h"
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
  Lora_init();
}


void loop() 
{
  Lora_receive_structComplex();
}