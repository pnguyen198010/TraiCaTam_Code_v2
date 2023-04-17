/* ==================================================
** Libraries
**
** =============================================== */


#include "servo_service.h"

#include <Servo.h>

#include "log_service.h"
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
static const uint32_t TIME_PER_POS    = 15;
static const uint32_t TIME_180_POS    = 180 * TIME_PER_POS;


static Log_t LOG;
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
    LOG.inf("[servo] start init");

    servo.attach(SERVO_PIN);
    servo.write(0);

    pos = servo.read();
    delay(pos*TIME_PER_POS);
    pos = 0;

    LOG.inf("[servo] end init");
}


void Servo_upd_pos()
{
    static uint32_t intv_wait_sweep = millis();
    static uint32_t intv_180_pos    = millis();

    if(millis() - intv_wait_sweep >= TIME_WAIT_SWEEP
    && count_sweep == 0) 
    {
        LOG.raw("\n");
        LOG.inf("[servo] sweep start");

        Turbidity_disable();
        count_sweep  = 2*SERVO_NUM_SWEEP;
        intv_180_pos = millis();
    }

    if(count_sweep != 0)
    {
        uint32_t dur = millis() - intv_180_pos;

        if(dur > TIME_180_POS
        && (pos >= 180 || pos <= 0))
        {
            count_sweep -= 1;
            intv_180_pos = millis();
            dur          = 0;

            LOG.raw("\n");
            LOG.inf("[servo] count_sweep: %d", count_sweep);
        }

        if(count_sweep != 0)
        {
            pos = map(dur, 0, TIME_180_POS, 0, 180);
            pos = count_sweep%2 == 0 ? pos : 180 - pos;
            pos = constrain(pos, 0, 180);
            servo.write(pos);

            Serial.print("I (");
            Serial.print(millis());
            Serial.print(") dur/time: ");
            Serial.print(dur);
            Serial.print("/");
            Serial.print(TIME_180_POS);
            Serial.print(" | pos: ");
            Serial.print(pos);
            Serial.print("\r");
        }

        if(count_sweep == 0)
        {
            LOG.inf("[servo] sweep end");

            Turbidity_enable();
            intv_wait_sweep = millis();
        }
    }
}