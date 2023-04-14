#ifndef turbidity_h
#define turbidity_h


/* ==================================================
** Libraries
**
** =============================================== */


#include <Arduino.h>


/* ==================================================
** Macro definition
**
** =============================================== */


typedef void (*cb_onTurbid_t) (uint32_t dur);
typedef void (*cb_onClear_t)  (uint32_t dur);
typedef void (*cb_onChange_t) ();


/* ==================================================
** Type definition
**
** =============================================== */


//


/* ==================================================
** Class declaration
**
** =============================================== */


class turbidity_t;


/* ==================================================
** Class definition
**
** =============================================== */



class turbidity_t
{
    public: turbidity_t(uint8_t pin, int8_t vcc=-1);

    public: void onTurbid (cb_onTurbid_t cb);
    public: void onClear  (cb_onClear_t  cb);
    public: void onChange (cb_onChange_t cb);

    private: uint8_t  get_stateCurr();
    private: uint32_t get_durTurbid();
    private: uint32_t get_durClear();

    public: bool is_enable();
    public: bool is_turbid();
    public: bool is_clear();

    public: void read();
    public: void enable();
    public: void disable();


    private: int8_t   vcc;
    private: uint8_t  pin;

    private: uint8_t  state_prev;

    private: uint32_t  time_debounce_prev;
    private: uint32_t time_turbid_prev;
    private: uint32_t time_clear_prev;

    private: uint8_t  flg_disable;

    private: cb_onTurbid_t cb_onTurbid;
    private: cb_onClear_t  cb_onClear;
    private: cb_onChange_t cb_onChange;

};


/* ==================================================
** Extern function declaration
**
** =============================================== */


void Turbidity_init();
void Turbidity_read();


/* ==================================================
** Extern variables
**
** =============================================== */


extern const uint8_t STATE_TURBID;
extern const uint8_t STATE_CLEAR;


#endif