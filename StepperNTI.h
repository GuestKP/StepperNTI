#ifndef STEPPER_NTI_
#define STEPPER_NTI_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define MIN_HALF_STEP_TIME 448
#define DEFAULT_SPR 200

class Stepper
{
private:
    int pin_dir, pin_step, pin_m1, pin_m2, pin_m3,
            steps_per_turnover = DEFAULT_SPR,
            half_step_time = MIN_HALF_STEP_TIME,
            mult_koeff = 32, divide_koeff = 1,
            acceleration = 0;
    // angle and linear position units is STEPS*32, due to loss of float numbers
    long pos_linear = 0, pos_angle = 0;
    float mm_per_turnover;
    bool move_side = true, linear_initialised = false, acceleration_initialised = false;

public:
    Stepper(int pin_dir, int pin_step, int pin_m1, int pin_m2, int pin_m3);
    void initLinear(int mm_per_rev);
    void initLinear(int n_teeth, int tooth_width);
    void setSPR(int new_spr);
    bool setDivision(int div);
    void moveSteps(float steps);
    void moveAngleRel(float angle);
    void moveAngleAbs(float angle);
    void moveLinearRel(float mm);
    void moveLinearAbs(float mm);
};

#endif