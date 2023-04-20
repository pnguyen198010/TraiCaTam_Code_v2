/* ==================================================
** Libraries
**
** =============================================== */


#include "led.h"


/* ==================================================
** Macro definition
**
** =============================================== */


#define LED_PIN_RED                 22
#define LED_PIN_GREEN               23


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


//


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


void Led_init()
{
    pinMode(LED_PIN_GREEN, OUTPUT);
    pinMode(LED_PIN_RED, OUTPUT);

    digitalWrite(LED_PIN_GREEN, HIGH);
    digitalWrite(LED_PIN_RED, LOW);
}


void Led_turn_off()
{
    digitalWrite(LED_PIN_GREEN, LOW);
    digitalWrite(LED_PIN_RED,   LOW);
}


void Led_turn_green()
{
    digitalWrite(LED_PIN_GREEN, HIGH);
    digitalWrite(LED_PIN_RED,   LOW);
}


void Led_turn_red()
{
    digitalWrite(LED_PIN_GREEN, LOW);
    digitalWrite(LED_PIN_RED,   HIGH);
}
