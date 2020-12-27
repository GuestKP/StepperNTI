/* --------------- MOVE EXAMPLE ---------------
  This example shows how to create stepper, how to
  move using moveSteps... and difference between
  absolute and relative coordinate system.

  It also shows difference of -Rel and -Abs versions
  of moveAngle and moveLinear functions
*/

#include "StepperNTI.h"

#define PIN_DIR 2
#define PIN_STEP 3
#define PIN_MS1 4
#define PIN_MS2 5
#define PIN_MS3 6

Stepper st(PIN_DIR, PIN_STEP, PIN_MS1, PIN_MS2, PIN_MS3);

void setup()
{
  Serial.begin(9600);
  Serial.println("Current position 0");
  delay(1000);

  // in relative coordinate system, number of units (steps) is constant,
  // but position from origin may be different
  st.moveStepsRel(50);
  Serial.println("Current position 50 (moved on 50)");
  delay(1000);
  st.moveStepsRel(-50);
  Serial.println("Current position 0 (moved on -50)");
  delay(1000);
  
  // in absolute coordinate system, number of units (steps) is different,
  // but position from origin is constant
  st.moveStepsAbs(50);
  Serial.println("Current position 50 (moved on 50)");
  delay(1000);
  st.moveStepsAbs(-50);
  Serial.println("Current position -50 (moved on -100)");
  delay(1000);
}

void loop()
{
  
}
