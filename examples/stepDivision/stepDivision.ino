/* --------------- MOVE EXAMPLE ---------------
  This example shows how to use setDivision.
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
  // setDivision affects amount of vibrations and sound of motor.
  // Also, the larger division is set, the less torque motor have.
  for(int i=1;i<=16;i*=2)
  {
    st.setDivision(i);
    Serial.println("Set step division 1/"+String(i));
    
    // Step division DOESN'T affect move functions;
    // there is always steps, not microsteps.
    
    // If you give float number with moveSteps..., it will approximate
    // steps to integer number of microsteps.
    
    st.moveStepsRel(200.4);
    delay(1000);
  }
}

void loop() {}
