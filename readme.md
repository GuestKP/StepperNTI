# General information

The 5.1.4 task of profile NPT of NTI Contest 2020.  
Library made by team "GPU с функцией NNN"  
Task division in team and some notes can be viewed at the end of readme.

# Documentation

class **Stepper**
- `*constructor*(*int* pin_dir, *int* pin_step, *int* pin_m1, *int* pin_m2, *int* pin_m3)`
- `initLinear(*int* mm_per_rev)`  
`initLinear(*int* n_teeth, *int* tooth_width)`
- `setSPR(*int* new_spr)`
- `moveStep(*long* steps)`
- `moveAngle(*long* deg)`
- `setDivision(*int* div)`

____

Stepik IDs of members:
- Pavel - 64992613
- Kirill - 63537290

Task division:
1. By-step, linear motion - Pavel
2. Angle motion - Pavel
3. Step division - Pavel
4. Linear motion in absolute and relative coordinate system - Kirill
5. Set current position of motor - Kirill
6. Set acceleration - Kirill
7. Examples - Pavel
8. Documentation - both

Library is being tested with motor 42BYGHW609 and driver DRV8825.
