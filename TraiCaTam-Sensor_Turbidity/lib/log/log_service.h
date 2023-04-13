#ifndef log_h
#define log_h


/* ==================================================
** Libraries
**
** =============================================== */


#include <Arduino.h>


/* ==================================================
** Macro definition
**
** =============================================== */


#define LOG_ENABLE

#ifdef  LOG_ENABLE
#define LOG_PORT            Serial
#define LOG_BAUD_RATE       9600
#define LOG_TIME_INIT       1000
#endif


/* ==================================================
** Type definition
**
** =============================================== */


class Log_t
{
    public: Log_t();
    public: Log_t(bool flg_disable);

    public: void raw (const char *format);
    public: void fmt (const char *format, ...);
    public: void inf (const char *format, ...);
    public: void err (const char *format, ...);
    public: void upd (const char *format, ...);

    public: void enable();
    public: void disable();

    private: bool flg_disable;
};


/* ==================================================
** Extern function declaration
**
** =============================================== */


void Log_init();


/* ==================================================
** Extern variables
**
** =============================================== */


//


#endif