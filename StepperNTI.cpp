#include "StepperNTI.h"
#include <cmath>

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


void Stepper::initLinear(float new_mpr)
{
	if(new_mpr > 0)
	{
		mm_per_revolution = new_mpr;
		flag_linear = true;
	}
}

void Stepper::initLinear(int n_teeth, float tooth_width)
{
	initLinear(n_teeth*tooth_width);
}


void Stepper::setSPR(int new_spr)
{
	if(new_spr > 0) step_per_revolution = new_spr;
}


int Stepper::setDivision(int div)
{
	if((div == 1) || (div == 2) || (div == 4) || (div == 8) || (div == 16) || (div == 32))
	{
		divide_koeff = div;
		
		// power levels on pins M1, M2, M3 is binary representation
		// of 'n', where 2^n - number of microsteps in step, so:
		// find 'n'
		n = 0;
		while(div > 1)
		{
			div /= 2;
			n += 1;
		}
		// write bits of 'n'
		digitalWrite(pin_m1, n & 0x1);
		digitalWrite(pin_m2, n & 0x2);
		digitalWrite(pin_m3, n & 0x4);
		
		return 1;
	}
	else
	{
		return 0;
	}

}


void Stepper::moveSteps(float steps)
{
	int msteps_32 = (abs(steps) % (1/divide_koeff)) * 32;
	int msteps_k = abs(steps) * divide_koeff;
	
	int cur_hst = half_step_time / divide_koeff;
	
	// set move direction
	digitalWrite(pin_dir, ((steps > 0) == move_side));
	
	// there must be acceleration
	
	// 'main' move
	for(int i=0; i<int(msteps_k); i++)
	{
		digitalWrite(pin_step, 1);
		delayMicroseconds(cur_hst);
		digitalWrite(pin_step, 0);
		delayMicroseconds(cur_hst);
	}
	if(msteps_32)
	{
		int tmp_dk = divide_koeff;
		setDivision(32);
		for(int i=0; i<int(steps); i++)
		{
			digitalWrite(pin_step, 1);
			delayMicroseconds(cur_hst);
			digitalWrite(pin_step, 0);
			delayMicroseconds(cur_hst);
		}
		setDivision(tmp_dk);
	}
	
	// there must be deceleration
	
	pos_linear += steps * 32;
	pos_angle = (pos_angle + steps * 32) % (step_per_revolution * 32);
}


void Stepper::moveAngleRel(float angle)
{
	
}


void Stepper::moveAngleAbs(float angle)
{
	
}


void Stepper::moveLinearRel(float mm)
{
	
}


void Stepper::moveLinearAbs(float mm)
{
	
}



