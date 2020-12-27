/* --------------- ANGLE EXAMPLE ---------------
  This example shows how to use moveAngle.
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

  // Move on given angle - works similar as moveStepsRel
  st.moveAngleRel(90);
  delay(50);
  st.moveAngleAbs(20);
  delay(2000);

  st.setSpeed(40);
  //Let's move from 20 to 340 with diferent types of move

  // AT_SIMPLE - moves in both directions,
  // CAN'T go 'through' null-degree
  st.moveAngleAbs(340, AT_SIMPLE);
  delay(500);
  st.moveAngleAbs(20, AT_SIMPLE);
  delay(1000);
  
  // AT_FIRST_SIDE - moves only in first direction
  st.moveAngleAbs(340, AT_FIRST_SIDE);
  delay(500);
  st.moveAngleAbs(20, AT_FIRST_SIDE);
  delay(1000);
  
  // AT_SECOND_SIDE - moves only in second direction
  st.moveAngleAbs(340, AT_SECOND_SIDE);
  delay(500);
  st.moveAngleAbs(20, AT_SECOND_SIDE);
  delay(1000);

  // AT_SHORTEST - default type,
  // as AT_SIMPLE, but can go through null-degree
  st.moveAngleAbs(340);
  delay(500);
  st.moveAngleAbs(20);
  delay(1000);
}

void loop() {}
