#include "StepperNTI.h"
#include <cmath>

#define MM_TO_ST(mm) mm / mm_per_turnover * float(steps_per_turnover)

//																													construct
Stepper::Stepper(int pin_dir, int pin_step, int pin_m1, int pin_m2, int pin_m3)
{
    // save pins
    Stepper::pin_dir = pin_dir;
    Stepper::pin_step = pin_step;
    Stepper::pin_m1 = pin_m1;
    Stepper::pin_m2 = pin_m2;
    Stepper::pin_m3 = pin_m3;
    // configure pins
    pinMode(pin_dir, OUTPUT);
    pinMode(pin_step, OUTPUT);
    pinMode(pin_m1, OUTPUT);
    pinMode(pin_m2, OUTPUT);
    pinMode(pin_m3, OUTPUT);
}


//																													set
bool setParams(int new_spr, float mpr)
{
    if ((new_spr <= 0) || (new_mpr <= 0))
    	return false;
    
    steps_per_turnover = new_spr;
    mm_per_turnover = new_mpr;
    return true;
}

bool setParams(int new_spr, int n_teeth, float tooth_width)
{
	return setParams(new_spr, n_teeth*tooth_width);
}


bool setSpeed(int new_spd)
{
	if((0 > new_spd)/* || (new_spd > 50000)*/)
		return false;
		
	speed = new_spd;
	return true;
}


bool setAcceleration(int new_acc)
{
	if((0 > new_acc)/* || (new_acc > 50000)*/)
		return false;
		
	acceleration = new_acc;
	return true;
}


bool Stepper::setDivision(int div)
{
    if (div != 1 && div != 2 && div != 4 && div != 8 && div != 16 && div != 32)
        return false;

    divide_koeff = div;

    // power levels on pins M1, M2, M3 is binary representation
    // of 'n', where 2^n - number of microsteps in step, so:
    // find 'n'
    n = 0;
    for(; div>1; div/=2) n += 1;
    
    // write bits of 'n'
    digitalWrite(pin_m1, n & 0x1);
    digitalWrite(pin_m2, n & 0x2);
    digitalWrite(pin_m3, n & 0x4);

    return true;
}


//																													rewrite
void Stepper::rewritePosition(float mm)
{
    if (mm_per_turnover < 0)
        return;
    
    linear_pos = MM_TO_ST(mm) * 32;
}


//																													steps
void Stepper::moveStepRel(float steps)
{
    // "length" with current divide_koeff
    int msteps_k = abs(steps) * divide_koeff;
    
	// target speed with current divide_koeff
    int cur_hst = half_step_time / divide_koeff;

    // set move direction
    digitalWrite(pin_dir, ((steps > 0) == move_side));

    // acceleration

    // 'main' move
    for (int i = 0; i < int(msteps_k); i++)
    {
        digitalWrite(pin_step, 1);
        delayMicroseconds(cur_hst);
        digitalWrite(pin_step, 0);
        delayMicroseconds(cur_hst);
    }

    // deceleration
	
	// save position
    pos_linear += msteps_k * 32 / divide_koeff;
    
    // pos_angle = (pos_angle + steps * 32) % (steps_per_turnover * 32);
}

void Stepper::moveStepAbs(float steps)
{
	moveStepRel(steps - pos_linear/32.)
}


//																													angle
void Stepper::moveAngleRel(float angle)
{
	moveStepRel(steps_per_turnover * angle / 360.)
}

void Stepper::moveAngleAbs(float angle, int type = AT_SHORTEST)
{
	angle -= pos_angle;
	switch(type)
	{
		/*case AT_SIMPLE:
			// angle -= pos_angle;
			break;*/
		case AT_FIRST_SIDE:
			angle += (angle < 0 ? 360 : 0);
			break;
		case AT_SECOND_SIDE:
			angle -= (angle > 0 ? 360 : 0);
			break;
		case AT_SHORTEST:
			angle + (angle < -180 ? 360 : 0) - (angle > 180 ? 360 : 0);
			break;
	}
	moveAngleRel(angle);
}

//																													Linear
void Stepper::moveLinearRel(float mm)
{
    if (mm_per_turnover < 0)
        return;

    moveStepsRel(MM_TO_ST(mm));
}

void Stepper::moveLinearAbs(float mm)
{
    if (mm_per_turnover < 0)
        return;
    
    moveStepsAbs(MM_TO_ST(mm));
}


