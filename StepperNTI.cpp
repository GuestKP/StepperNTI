#include "StepperNTI.h"


//																													construct
Stepper::Stepper(int pin_dir, int pin_step, int pin_ms1, int pin_ms2, int pin_ms3)
{
    // save pins
    Stepper::pin_dir = pin_dir;
    Stepper::pin_step = pin_step;
    Stepper::pin_m1 = pin_ms1;
    Stepper::pin_m2 = pin_ms2;
    Stepper::pin_m3 = pin_ms3;
    // configure pins
    pinMode(pin_dir, OUTPUT);
    pinMode(pin_step, OUTPUT);
    pinMode(pin_ms1, OUTPUT);
    pinMode(pin_ms2, OUTPUT);
    pinMode(pin_ms3, OUTPUT);
}


bool Stepper::setParams(int new_spr, float new_mpr)
{
	//check if params isn't correct
    if ((new_spr <= 0) || (new_mpr <= 0))
    	return false;
    
    // save
    steps_per_revolution = new_spr;
    mm_per_revolution = new_mpr;
    return true;
}

bool Stepper::setParams(int new_spr, int n_teeth, float tooth_width)
{
	return setParams(new_spr, n_teeth*tooth_width);
}


bool Stepper::setSpeed(int new_spd)
{
	//check if param isn't correct
	if((0 >= new_spd)/* || (new_spd > 50000)*/)
		return false;
		
	speed = new_spd*60.*float(steps_per_revolution)/1000.;
	return true;
}


bool Stepper::setAcceleration(int new_acc)
{
	//check if param isn't correct
	if((0 > new_acc)/* || (new_acc > 50000)*/)
		return false;
		
	acceleration = new_acc;
	return true;
}


bool Stepper::setDivision(int div)
{
	//check if param isn't correct
    if (div != 1 && div != 2 && div != 4 && div != 8 && div != 16 /*&& div != 32*/)
        return false;
	
	// save coefficient
    divide_koeff = div;

    // power levels on pins M1, M2, M3 is binary representation
    // of 'n', where 2^n - number of microsteps in step, so:
    // find 'n'
    int n = 0;
    for(; div>1; div/=2) n += 1;
    
    // write bits of 'n'
    digitalWrite(pin_m1, n & 0x4);
    digitalWrite(pin_m2, n & 0x2);
    digitalWrite(pin_m3, n & 0x1);

    return true;
}


void Stepper::rewritePosition(float mm)
{
	// if Linear motion params aren't initialised
    if (mm_per_revolution < 0)
        return;
    
    // convert MM to microsteps(16)
    pos_linear = MM_TO_ST(mm) * 16;
}


void Stepper::step(int t)
{
	// make step
    digitalWrite(pin_step, 1);
    delayMicroseconds(t);
    digitalWrite(pin_step, 0);
    delayMicroseconds(t);
}

void Stepper::accelerate(int n16, int nadd, int hst, bool acc)
{
	// set maximum division for acceleration
	int tmpdiv = divide_koeff;
	setDivision(16);
	
	//if decelerate and have any steps in full-speed
	if(!acc)
	{
		for(int i = 0; i < nadd; i++)
			step(hst);
	}
	
	// accelerate/decelerate
	for (int i=0; i<n16; i++)
	{
		float k=acc?(float (i+1)):(float(n16-i));
		int x = 1000000./32./sqrt(float(acceleration)*k);
		step(x);
	}
	
	//if accelerate and have any steps in full-speed
	if(acc)
	{
		for(int i = 0; i < nadd; i++)
			step(hst);
	}
	// reset division
	setDivision(tmpdiv);
}


void Stepper::moveStepsRel(float steps)
{
    // "length" with current divide_koeff
    int msteps_k = steps * divide_koeff,
    // time for half-microstep with current divide_koeff
		cur_hst = 1000000/(2.*float(divide_koeff*speed)),
		tmp_k = 16/divide_koeff, n16, nk;
	
	// save position
    pos_linear += msteps_k * 16 / divide_koeff;
    // save angle position
    pos_angle = pos_angle + msteps_k * 360. / (divide_koeff * float(steps_per_revolution));
    // check that 0 <= angle < 360
    for(;pos_angle>=360;pos_angle-=360);for(;pos_angle<0;pos_angle+=360);
    // get length
    msteps_k = abs(msteps_k);

    // set move direction
    digitalWrite(pin_dir, steps >= 0);
    
    // acceleration
    if(acceleration)
    {
    	// get number of microsteps(16)
    	n16 = float(speed)*float(speed)/float(acceleration);
    	// get number of microsteps(divide_koeff) that contain
    	// n16 microsteps(16)
    	nk = ceil(n16/float(tmp_k));
    	
    	// if length of acceleration and deceleration
    	// less than move length (in microsteps(divide_koeff))
    	if(2*nk >= msteps_k)
    	{
    		// half-move accelerate
    		n16 = msteps_k/2 * tmp_k;
    		
			accelerate(n16, 0, 0, true);
    		
    		// half-move + remainder decelerate
    		n16 += msteps_k%2 * tmp_k;
    		nk = 0;
    		msteps_k = 0;
		}
		else
		{
			// just accelerate
			accelerate(n16, nk*tmp_k-n16, 0, true);
			
			msteps_k -= 2*nk;
		}
	}

    // 'main' move
    for (int i = 0; i < int(msteps_k); i++)
		step(cur_hst);

    // deceleration
    if(acceleration)
		accelerate(n16, nk*tmp_k-n16, cur_hst, false);
}

void Stepper::moveStepsAbs(float steps)
{
	// convert absolute steps in relative steps
	moveStepsRel(steps - pos_linear/16.);
}


void Stepper::moveAngleRel(float angle)
{
	// convert relative angle in relative steps
	moveStepsRel(float(steps_per_revolution) * angle / 360.);
}

void Stepper::moveAngleAbs(float angle, int type = AT_SHORTEST)
{
	// convert absolute angle in relative angle
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
			angle += (angle < -180 ? 360 : 0) - (angle > 180 ? 360 : 0);
			break;
	}
	moveAngleRel(angle);
}

void Stepper::moveLinearRel(float mm)
{
	// if Linear motion params aren't initialised
    if (mm_per_revolution < 0)
        return;

    moveStepsRel(MM_TO_ST(mm));
}

void Stepper::moveLinearAbs(float mm)
{
	// if Linear motion params aren't initialised
    if (mm_per_revolution < 0)
        return;
    
    moveStepsAbs(MM_TO_ST(mm));
}


