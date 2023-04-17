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
#define SERVO_TIME_PER_POS  15


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


static Servo servo;

static int16_t pos = 0;

// static const float TIME_SWEEP = (float) SERVO_NUM_SWEEP * 360 * SERVO_TIME_PER_POS / 1000;
static const uint32_t TIME_WAIT_SWEEP = 1 * 10 * 1000;


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
    delay(pos*SERVO_TIME_PER_POS);
}


void Servo_upd_pos()
{
    static uint32_t intv = millis();

    if(millis() - intv < TIME_WAIT_SWEEP) {return;}

    Turbidity_disable();

    for(uint8_t i=0; i<SERVO_NUM_SWEEP; ++i)
    {
        Serial.print(millis());
        Serial.print(" [Servo] start ");
        Serial.println(i);

        for(pos=0; pos<=180; ++pos){
            servo.write(pos);
            delay(SERVO_TIME_PER_POS);

            // Serial.print(millis());
            // Serial.print(" \t pos: ");
            // Serial.println(pos);
        }

        for(pos=180; pos>=0; --pos){
            servo.write(pos);
            delay(SERVO_TIME_PER_POS);

            // Serial.print(millis());
            // Serial.print(" \t pos: ");
            // Serial.println(pos);
        }

        Serial.print(millis());
        Serial.print(" Servo] end ");
        Serial.println(i);
    }

    Turbidity_enable();
    intv = millis();
}