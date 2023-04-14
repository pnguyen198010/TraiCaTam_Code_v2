/* ==================================================
** Libraries
**
** =============================================== */


#include "turbidity.h"

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


static const int8_t  VCC = 2;
static const uint8_t PIN = 4;

static const uint8_t STATE_TURBID  = LOW;
static const uint8_t STATE_CLEAR   = !STATE_TURBID;
static const uint8_t STATE_DEBOUNCE = 3;

static const uint32_t TIME_DEBOUNCE = 2UL*1000UL;


static Log_t       LOG;
static turbidity_t sensor(VCC, PIN);


/* ==================================================
** Static function declaration
**
** =============================================== */


static uint8_t get_stateRaw(uint8_t pin);


/* ==================================================
** Static function definition
**
** =============================================== */


inline static uint8_t get_stateRaw(uint8_t pin)
{
    return digitalRead(pin)==STATE_TURBID ? true : false;
}


/* ==================================================
** Constructor: turbidity_t
**
** =============================================== */


turbidity_t::turbidity_t(uint8_t pin, int8_t vcc)
{
    this->vcc = vcc;

    if(vcc != -1)
    {
        pinMode(vcc, OUTPUT);
        digitalWrite(vcc, HIGH);
    }

    this->pin = pin;
    pinMode(pin, INPUT);

    state_prev = get_stateRaw(pin);
}


/* ==================================================
** onEvent: turbidity_t
**
** =============================================== */


void turbidity_t::onTurbid(cb_onTurbid_t cb)
{
    cb_onTurbid = cb;
}


void turbidity_t::onClear(cb_onClear_t cb)
{
    cb_onClear = cb;
}


void turbidity_t::onChange(cb_onChange_t cb)
{
    cb_onChange = cb;
}


/* ==================================================
** Getter: turbidity_t
**
** =============================================== */


uint8_t turbidity_t::get_stateCurr()
{
    static uint8_t state_debounce = state_prev;
           uint8_t state_curr     = get_stateRaw(pin);

    if(state_debounce != state_curr)
    {
        state_debounce     = state_curr;
        time_debounce_prev = millis();
        return STATE_DEBOUNCE;
    }

    if(millis() - time_debounce_prev < TIME_DEBOUNCE){
        return STATE_DEBOUNCE;
    }

    return state_curr;
}


uint32_t turbidity_t::get_durTurbid()
{
    if(is_turbid()){
        return millis() - time_turbid_prev;
    }

    uint32_t clear = millis() - time_clear_prev;
    return millis() - time_turbid_prev - clear;
}


uint32_t turbidity_t::get_durClear()
{
    if(is_clear()){
        return millis() - time_clear_prev;
    }

    uint32_t dur_turbid = millis() - time_turbid_prev;
    return millis() - time_clear_prev - dur_turbid;
}


/* ==================================================
** Checker: turbidity_t
**
** =============================================== */


bool turbidity_t::is_enable()
{
    return flg_disable ? false : true;
}


bool turbidity_t::is_turbid()
{
    uint8_t state_curr = get_stateCurr();
    state_curr = state_curr==STATE_DEBOUNCE ? state_prev : state_curr;

    return state_curr==STATE_TURBID ? true : false;
}


bool turbidity_t::is_clear()
{
    uint8_t state_curr = get_stateCurr();
    state_curr = state_curr==STATE_DEBOUNCE ? state_prev : state_curr;

    return state_curr == STATE_CLEAR ? true : false;
}


/* ==================================================
** Method: turbidity_t
**
** =============================================== */


void turbidity_t::read()
{
    if(is_enable() == false) {return;}

    uint8_t state_curr = get_stateCurr();
    if(state_curr == STATE_DEBOUNCE) {return;}

    // Water change from clear to turbid
    if(state_curr == STATE_TURBID
    && state_curr != state_prev)
    {
        state_prev = state_curr;
        time_turbid_prev = millis();
        if(cb_onChange) {cb_onChange();}
        if(cb_onTurbid) {cb_onTurbid(millis() - time_turbid_prev);}

        LOG.inf("[Turbidity] change to turbid");
        return;
    }

    // Water keeps turbid
    if(state_curr == STATE_TURBID
    && state_curr == state_prev)
    {
        if(cb_onTurbid) {cb_onTurbid(millis() - time_turbid_prev);}
        return;
    }

    // Water change from turbid to clear
    if(state_curr == STATE_CLEAR
    && state_curr != state_prev)
    {
        state_prev = state_curr;
        time_clear_prev = millis();
        if(cb_onChange) {cb_onChange();}
        if(cb_onClear) {cb_onClear(millis() - time_clear_prev);}

        LOG.inf("[Turbidity] change to clear");
        return;
    }

    if(state_curr == STATE_CLEAR
    && state_curr == state_prev)
    {
        if(cb_onClear) {cb_onClear(millis() - time_turbid_prev);}
        return;
    }
}


void turbidity_t::enable()
{
    flg_disable = false;
    LOG.inf("[Tubidity] sensor is enabled");

    if(vcc == -1) {return;}
    digitalWrite(vcc, HIGH);
}


void turbidity_t::disable()
{
    flg_disable = true;
    LOG.inf("[Turbidity] sensor is disabled");

    state_prev  = STATE_CLEAR;

    time_debounce_prev = millis();
    time_turbid_prev   = millis();
    time_clear_prev    = millis();

    if(vcc == -1) {return;}
    digitalWrite(vcc, LOW);
}


/* ==================================================
** Extern function definition
**
** =============================================== */


void Turbidity_init()
{
    LOG.raw("\n\n");
    LOG.inf("[Turbidity] start init");
    // sensor.onChange();
    // sensor.onTurbid();
    // sensor.onClear();
    LOG.inf("[Turbidiy] end init");
}


void Turbidity_read()
{
    sensor.read();
}
