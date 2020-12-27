/* --------------- SPEED ACCELERATION EXAMPLE ---------------
  This example shows how to use setSpeed and setAcceleration.
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

  //Two moves, first is 2 times longer than second.
  Serial.println("Speed 50");
  st.setSpeed(50);
  st.moveStepsRel(200);
  delay(1000);
  Serial.println("Speed 100");
  st.setSpeed(100);
  st.moveStepsRel(200);
  delay(1000);

  // After acceleration set, all moves will have
  // acceleration/deceleration phase.
  Serial.println("Acceleration");
  st.setAcceleration(500);
  st.moveStepsRel(600);
  delay(500);
  st.moveStepsRel(100);
  delay(1000);

  // If acceleration = 0, no acceleration is applied.
  Serial.println("No acceleration");
  st.setAcceleration(0);
  st.moveStepsRel(600);
}

void loop() {}
