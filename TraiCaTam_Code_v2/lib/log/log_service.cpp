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


// static int myPrintf(const char *format, ...);


/* ==================================================
** Static function definition
**
** =============================================== */


// static int myPrintf(const char *format, ...)
// {
// #ifdef LOG_PORT

//     char loc_buf[64];
//     char * temp = loc_buf;

//     va_list arg;
//     va_list copy;

//     va_start(arg, format);
//     va_copy(copy, arg);

//     int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
//     va_end(copy);

//     if(len < 0) 
//     {
//         va_end(arg);
//         return 0;
//     };

//     if(len >= sizeof(loc_buf))
//     {
//         temp = (char*) malloc(len+1);
//         if(temp == NULL) {
//             va_end(arg);
//             return 0;
//         }
//         len = vsnprintf(temp, len+1, format, arg);
//     }

//     va_end(arg);
//     len = LOG_PORT.write((uint8_t*)temp, len);
//     if(temp != loc_buf){
//         free(temp);
//     }
//     return len;

// #endif
// }


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
        LOG_PORT.print(F("\n"));
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
        LOG_PORT.print(F("\n"));
        flg_upd = false;
    }

#endif
}


void Log_t::inf(const char *format, ...)
{
#ifdef LOG_PORT

    if(flg_disable) {return;}

    if(flg_upd) {
        LOG_PORT.print(F("\n"));
        flg_upd = false;
    }

    LOG_PORT.print(F("I ("));
    LOG_PORT.print(millis());
    LOG_PORT.print(F(") "));

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

    LOG_PORT.print(F("\n"));

#endif
}


void Log_t::err(const char *format, ...)
{
#ifdef LOG_PORT

    if(flg_disable) {return;}

    if(flg_upd) {
        LOG_PORT.print(F("\n"));
        flg_upd = false;
    }

    LOG_PORT.print(F("E ("));
    LOG_PORT.print(millis());
    LOG_PORT.print(F(") "));

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

    LOG_PORT.print(F("\n"));

#endif
}


void Log_t::upd(const char *format, ...)
{
#ifdef LOG_PORT

    if(flg_disable) {return;}

    if(flg_upd == false) {
        flg_upd = true;
    }

    LOG_PORT.print(F("U ("));
    LOG_PORT.print(millis());
    LOG_PORT.print(F(") "));

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

    LOG_PORT.print(F("\r"));

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

    uint64_t intv = millis();

    LOG_PORT.begin(LOG_BAUD_RATE);

    while(millis() - intv < LOG_TIME_INIT){
        if(Serial){
            break;
        }
    }
    LOG_PORT.print(F("\n\n"));

#endif
}

