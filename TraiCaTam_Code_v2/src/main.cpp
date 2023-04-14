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

  LOG.raw("\n\nTest fmt function\n");
  LOG.fmt("d:  %d,  %d,  %d\n", 1, 2, 3);
  LOG.fmt("u:  %u,  %u,  %u\n", 1, 2, 3);
  LOG.fmt("lu: %lu, %lu, %lu\n", (uint64_t)1, (uint64_t)2, (uint64_t)3);
  LOG.fmt("s:  %s,  %s,  %s\n", "1", "2", "3");
  LOG.fmt("f:  %f,  %f,  %f\n", 1, 2, 3);

  LOG.raw("\n\nTest inf function\n");
  LOG.inf("d:  %d,  %d,  %d", 1, 2, 3);
  LOG.inf("u:  %u,  %u,  %u", 1, 2, 3);
  LOG.inf("lu: %lu, %lu, %lu", (uint64_t)1, (uint64_t)2, (uint64_t)3);
  LOG.inf("s:  %s,  %s,  %s", "1", "2", "3");
  LOG.inf("f:  %f,  %f,  %f", 1, 2, 3);

  LOG.raw("\n\nTest err function\n");
  LOG.err("d:  %d,  %d,  %d", 1, 2, 3);
  LOG.err("u:  %u,  %u,  %u", 1, 2, 3);
  LOG.err("lu: %lu, %lu, %lu", (uint64_t)1, (uint64_t)2, (uint64_t)3);
  LOG.err("s:  %s,  %s,  %s", "1", "2", "3");
  LOG.err("f:  %f,  %f,  %f", 1, 2, 3);

  LOG.raw("\n\nTest upd function\n");

}


void loop() 
{
  static uint32_t intv = millis();

  if(millis() - intv < TIME_TOGGLE){
    LOG.upd("%%u: %u | %%lu: %lu | %%lu: %lu | %%f: %f", (uint32_t)10000, (uint64_t)10000, millis(), 10000.00);
    return;
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