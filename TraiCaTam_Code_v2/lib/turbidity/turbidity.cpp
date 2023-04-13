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


#define TURBIDITY_VCC       4
#define TURBIDITY_PIN       2

#define TURBIDITY_TRUE      LOW


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


static Log_t LOG;


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


void Turbidity_init()
{
    pinMode(TURBIDITY_PIN, INPUT);
    pinMode(TURBIDITY_VCC, OUTPUT);

    digitalWrite(TURBIDITY_VCC, HIGH);
}

void Turbidity_disable()
{
    digitalWrite(TURBIDITY_VCC, LOW);
}


void Turbidity_enable()
{
    digitalWrite(TURBIDITY_VCC, HIGH);
}


bool Turbidity_is_true()
{
    bool flg = digitalRead(TURBIDITY_PIN)==TURBIDITY_TRUE ? true : false;

    LOG.inf("[turbidity] %s", flg ? "true" : "false");
    return flg;
}

