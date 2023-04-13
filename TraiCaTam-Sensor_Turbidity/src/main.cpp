/* ==================================================
** Library
**
** =============================================== */


#include <Arduino.h>
#include <SoftwareSerial.h>

#include "log_service.h"


/* ==================================================
** static variable
**
** =============================================== */


Log_t LOG;

bool flg_log = true;

const uint32_t TIME_TOGGLE = 5000;


/* ==================================================
** main
**
** =============================================== */


void setup() 
{
  Log_init();

  LOG.raw("[Log] inited");
  LOG.fmt("I(%lu) [Log] this is fmt function");
  LOG.inf("This is info function");
  LOG.inf("This is err function");
}


void loop() 
{
  static uint32_t intv = millis();

  if(millis() - intv < TIME_TOGGLE){
    LOG.upd("This is log upd");
  }

  if(flg_log){
    LOG.inf("Disable log service");
    LOG.disable();
  }

  else{
    LOG.enable();
    LOG.inf("Enable log service");
  }

  flg_log = !flg_log;
  intv = millis();
}