#ifndef _TIMER_H
#define _TIMER_H

void Timer_Bdelay_Milli (unsigned long Millis);

/***** Servo motor ********/
void DecServoSpeedDelay();
void IncServoSpeedDelay();
void SetServoDirection(U32 direction);
void StartServoMotor();
void StopServoMotor();

/***** Step motor ********/
void StartStepMotor();
void StopStepMotor();
void StepMotorMove();
void DecStepSpeedDelay();
void IncStepSpeedDelay();
void SetStepsDirection(U32 direction);

#endif
