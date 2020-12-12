#ifndef STEPPER_NTI_
#define STEPPER_NTI_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define MAX_HALF_STEP_TIME 448
#define DEFAULT_SPR 200

class Stepper
{
	private:
		int pin_dir, pin_step, pin_m1, pin_m2, pin_m3,
			divide_koeff = 1,
			mm_per_revolution = DEFAULT_SPR,
			half_step_time = MAX_HALF_STEP_TIME;
		float mm_per_step;
		bool move_side = true, linear = false;
		
	public:
		Stepper(int pin_dir, int pin_step, int pin_m1, int pin_m2, int pin_m3);
		void initLinear(int mm_per_rev);
		void initLinear(int n_teeth, int tooth_width);
		void setSPR(int new_spr);
		void moveStep(long steps);
		void moveAngle(long deg);
		int setDivision(int div);
};

#endif
