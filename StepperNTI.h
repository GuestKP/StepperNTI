#ifndef STEPPER_NTI_
#define STEPPER_NTI_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include "math.h"

#define DEFAULT_SPEED 1200
#define DEFAULT_SPR 200
#define AT_SIMPLE 0
#define AT_FIRST_SIDE 1
#define AT_SECOND_SIDE 2
#define AT_SHORTEST 3

class Stepper
{
private:
    int pin_dir, pin_step, pin_m1, pin_m2, pin_m3,
            steps_per_revolution = DEFAULT_SPR,
            speed = DEFAULT_SPEED,
            acceleration = 0,
            mult_koeff = 32, divide_koeff = 1;
    // angle and linear position units is STEPS*32, due to loss of float numbers
    long pos_linear = 0;
    float mm_per_revolution = -1, pos_angle = 0;
    
    void step(int t);
    void accelerate(int n32, int nadd, int hst, bool acc);

public:
    Stepper(int pin_dir, int pin_step, int pin_m1, int pin_m2, int pin_m3);
    
    bool setParams(int new_spr, float mpr);
    bool setParams(int new_spr, int n_teeth, float tooth_width);
    bool setDivision(int div);
    bool setSpeed(int new_spd);
    bool setAcceleration(int new_acc);
    
    void rewritePosition(float mm);
    
    void moveStepsRel(float steps);
    void moveStepsAbs(float steps);
    
    void moveAngleRel(float angle);
    void moveAngleAbs(float angle, int type = AT_SHORTEST);
    
    void moveLinearRel(float mm);
    void moveLinearAbs(float mm);
};

#endif

