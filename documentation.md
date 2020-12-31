# Documentation

### class Stepper
```C++
constructor(int pin_dir, int pin_step, int pin_ms1, int pin_ms2, int pin_ms3);
```
Constructor of class.  
_Params:_  
`int pin_dir` arduino pin, connected to DIR pin of shield.
It is recommended to use external pull-up/pull-down resistor on this pin to avoid rattle during arduino reset.  
`int pin_step` arduino pin, connected to STEP pin of shield  
`int pin_ms1` arduino pin, connected to MS1 pin of shield  
`int pin_ms2` arduino pin, connected to MS2 pin of shield  
`int pin_ms3` arduino pin, connected to MS3 pin of shield  
All pins digital, PWM is not used.
```C++
bool setParams(int new_spr, int n_teeth, float tooth_width);
bool setParams(int new_spr, float new_mpr);
```
It is **highly recommended** to use this function before using anything else.  
Set parameters used in moveAngle..., moveLinear... and setSpeed functions.
If you're using motor with gear and toothed belt, use 1st function;
if you're using motor with coil/other round thing, use 2nd function.  
`int new_spr` steps per revolution, depends on stepper motor model, default `200`  
`float new_mpr` millimeters per revolution (coil radius[mm] * 2 * pi)  
`int n_teeth` number of teeth on gear  
`float tooth_width` width of one tooth on belt  
**return** `true` if all numbers `> 0`, else `false`
```C++
bool setDivision(int div);
```
Set microstep division.  
`int div` number of microsteps in step, for A4988 it must be `1, 2, 4, 8 or 16`. Division coefficient = 1/div.  
**return** `true` if `div` is correct, else `false`
```C++
bool setSpeed(int new_spd);
```
**setParams must** be called **before** this function's call (else, function won't work).  
Set rotation speed.  
`int new_spd` speed in rotations per minute (tested - `new_spd <= 100`)  
**return** `true` if `new_spd > 0`, else `false`
```C++
bool setAcceleration(int new_acc);
```
Set acceleration. Used in all types of moves at beginning (to accelerate from 0 to full-speed)
and ending (to decelerate from full-speed to 0).  
If length of acceleration+deceleration is more than length of move, acceleration and deceleration will be truncated
to match half-length of move (so half-length+half-length = length of move).  
`int new_acc` acceleration in steps per sqare millisecond, typical `from 1000 to 40000`.
If `0`, motor starts and stops at full-speed.  
**return** `true` if `new_acc > 0`, else `false`
```C++
void rewritePosition(float mm);
```
Set current position as given position, change _origin_. No move is applied.  
`float mm` new position in millimeters.
```C++
void moveStepsRel(float steps);
```  
Move given number of steps from current position.  
`float steps` number of steps. If `steps >= 0`, moves in the first direction, else - in the second direction.
Note: motor connection to the shield affect directions - are they counter- or clockwise.
```C++
void moveStepsAbs(float steps);
```  
Move given number of steps from _origin_ (in absolute coordinate system).  
`float steps` distance from _origin_ in steps.
```C++
void moveAngleRel(float angle);
```
**setParams must** be called **before** this function's call (else, function won't work).  
Move on given angle from current position.  
`float angle` angle in degrees. If `angle >= 0`, moves in the first direction, else - in the second.
```C++
void moveAngleAbs(float angle, int type = AT_SHORTEST);
```
**setParams must** be called **before** this function's call (else, function won't work).  
Move on given angle in absolute coordinate system.  
`float angle` angle from _origin_ in degrees. `0 <= angle < 360`  
`int type` type of move, _optional_, must be one of these values (default `AT_SHORTEST`):  
- `AT_SIMPLE` moves in both directions, delta angle can be ` > 180`  
- `AT_FIRST_SIDE` moves only in first direction  
- `AT_SECOND_SIDE` moves only in second direction  
- `AT_SHORTEST` moves in both directions, `0 <= delta angle <= 180`
```C++
void moveLinearRel(float mm);
```
**setParams must** be called **before** this function's call (else, function won't work).  
Move on given distance from current position.  
`float mm` distance in millimeters. If `mm >= 0`, moves in the first direction, else - in the second.

```C++
void moveLinearAbs(float mm);
```
**setParams must** be called **before** this function's call (else, function won't work).  
Move on given distance from _origin_ (in absolute coordinate system).  
`float mm` distance from _origin_ in millimeters.
