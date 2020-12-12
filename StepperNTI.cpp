#include "StepperNTI.h"


Stepper::Stepper(int pin_dir, int pin_step, int pin_m1, int pin_m2, int pin_m3)
{
	Stepper::pin_dir = pin_dir;
	Stepper::pin_step = pin_step;
	Stepper::pin_m1 = pin_m1;
	Stepper::pin_m2 = pin_m2;
	Stepper::pin_m3 = pin_m3;
	pinMode(pin_dir, OUTPUT);
	pinMode(pin_step, OUTPUT);
	pinMode(pin_m1, OUTPUT);
	pinMode(pin_m2, OUTPUT);
	pinMode(pin_m3, OUTPUT);
}


void Stepper::initLinear(int new_mpr)
{
	mm_per_revolution = new_mpr;
	linear = true;
}

void Stepper::initLinear(int n_teeth, int tooth_width)
{
	initLinear(n_teeth*tooth_width);
}


void Stepper::setSPR(int new_spr)
{
	step_per_revolution = new_spr;
}


void Stepper::moveStep(long steps)
{
	digitalWrite(pin_dir, ((steps > 0) == move_side));
	steps *= divide_koeff;
	int cur_hst = half_step_time / divide_koeff;
	for(int i=0; i<steps; i++)
	{
		digitalWrite(pin_step, 1);
		delayMicroseconds(cur_hst);
		digitalWrite(pin_step, 0);
		delayMicroseconds(cur_hst);
	}
}


void Stepper::moveAngle(long angle)
{
	
}


int Stepper::setDivision(int div)
{
	if((div == 1) || (div == 2) || (div == 4) || (div == 8) || (div == 16) || (div == 32))
	{
		divide_koeff = div;
		n = 0;
		while(div > 1) // get n, where 2^n == div
		{
			div /= 2;
			n += 1;
		}
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



