/* ==================================================
** Libraries
**
** =============================================== */


#include "log_service.h"


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
** Extern variables
**
** =============================================== */


//


/* ==================================================
** Static variables
**
** =============================================== */


static bool flg_upd = false;


/* ==================================================
** Static function declaration
**
** =============================================== */


static void myPrintf(const char *format, ...);


/* ==================================================
** Static function definition
**
** =============================================== */


static void myPrintf(const char *format, ...)
{
#ifdef LOG_PORT

    char loc_buf[64];
    char * temp = loc_buf;

    va_list arg;
    va_list copy;

    va_start(arg, format);
    va_copy(copy, arg);

    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);

    if(len < 0) {
        va_end(arg);
        return;
    };

    if((unsigned long)len >= sizeof(loc_buf))
    {
        temp = (char*) malloc(len+1);

        if(temp == NULL) {
            va_end(arg);
            return;
        }

        len = vsnprintf(temp, len+1, format, arg);
    }

    va_end(arg);
    LOG_PORT.print(temp);

    if(temp != loc_buf) {free(temp);}

#endif
}


/* ==================================================
** Log_t: Constructors
**
** =============================================== */


Log_t::Log_t()
{
}


Log_t::Log_t(bool flg_disable)
{
    this->flg_disable = flg_disable;
}


/* ==================================================
** Log_t: Methods
**
** =============================================== */


void Log_t::raw(const char *format)
{
#ifdef LOG_PORT

    if(flg_disable) {return;}

    if(flg_upd) {
        raw("\n");
        flg_upd = false;
    }

    LOG_PORT.print(format);

#endif
}


void Log_t::fmt(const char *format, ...)
{
#ifdef LOG_PORT

    if(flg_disable) {return;}

    if(flg_upd) {
        raw("\n");
        flg_upd = false;
    }

    va_list args;
    va_start(args, format);

    myPrintf(format, args);

    va_end(args);

#endif
}


void Log_t::inf(const char *format, ...)
{
#ifdef LOG_PORT

    if(flg_disable) {return;}

    if(flg_upd) {
        LOG_PORT.print("\n");
        flg_upd = false;
    }

    va_list args;
    va_start(args, format);

    myPrintf("I (%lu) ", millis());
    myPrintf(format, args);
    LOG_PORT.print("\n");

    va_end(args);

#endif
}


void Log_t::err(const char *format, ...)
{
#ifdef LOG_PORT

    if(flg_disable) {return;}

    if(flg_upd) {
        LOG_PORT.print("\n");
        flg_upd = false;
    }

    va_list args;
    va_start(args, format);

    myPrintf("E (%lu) ", millis());
    myPrintf(format, args);
    LOG_PORT.print("\n");

    va_end(args);

#endif
}


void Log_t::upd(const char *format, ...)
{
#ifdef LOG_PORT

    if(flg_disable) {return;}

    if(flg_upd == false) {
        flg_upd = true;
    }

    va_list args;
    va_start(args, format);

    myPrintf("U (%lu) ", millis());
    myPrintf(format, args);
    LOG_PORT.print("\r");

    va_end(args);

#endif
}


void Log_t::enable()
{
    flg_disable = false;
}


void Log_t::disable()
{
    flg_disable = true;
}


/* ==================================================
** Extern function definition
**
** =============================================== */


void Log_init()
{
#ifdef LOG_PORT

    uint32_t intv = millis();

    LOG_PORT.begin(LOG_BAUD_RATE);

    while(millis() - intv < LOG_TIME_INIT){
        if(Serial){
            break;
        }
    }

    LOG_PORT.print(F("\n\n"));

#endif
}

