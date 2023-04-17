/* ==================================================
** Libraries
**
** =============================================== */


#include "servo_service.h"

#include <Servo.h>

#include "turbidity.h"


/* ==================================================
** Macro definition
**
** =============================================== */


#define SERVO_PIN           9

#define SERVO_NUM_SWEEP     2


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


// static const float TIME_SWEEP = (float) SERVO_NUM_SWEEP * 360 * TIME_PER_POS / 1000;
static const uint32_t TIME_WAIT_SWEEP = 1 * 10 * 1000;
static const uint32_t TIME_PER_POS    = 1 * 10 * 1000;
static const uint32_t TIME_180_POS    = 180 * TIME_PER_POS;


static Servo servo;


static int16_t pos = 0;
static uint8_t count_sweep = 0;



/* ==================================================
** Static function declaration
**
** =============================================== */


//


/* ==================================================
** Static function definition
**
** =============================================== */


//


/* ==================================================
** Extern function definition
**
** =============================================== */


void Servo_init()
{
    servo.attach(SERVO_PIN);

    pos = servo.read();
    servo.write(pos);
    delay(pos*TIME_PER_POS);
}


void Servo_upd_pos()
{
    static uint32_t intv_wait_sweep = millis();
    static uint32_t intv_180_pos    = millis();

    if(millis() - intv_wait_sweep >= TIME_WAIT_SWEEP
    && count_sweep == 0) 
    {
        Turbidity_disable();
        count_sweep  = 2*SERVO_NUM_SWEEP;
        intv_180_pos = millis();
    }

    if(count_sweep != 0)
    {
        uint32_t dur = millis() - intv_180_pos;

        if(dur > TIME_180_POS)
        {
            count_sweep -= 1;
            intv_180_pos = millis();
            dur          = 0;
        }

        if(count_sweep != 0)
        {
            pos = map(pos, dur, TIME_180_POS, 0, 180);
            pos = count_sweep%2 == 1 ? pos : 180 - pos;
            servo.write(pos);
        }
    }

    if(count_sweep == 0)
    {
        Turbidity_enable();
        intv_wait_sweep = millis();
    }
}