#ifndef _TIMER_H
#define _TIMER_H

void Timer_Bdelay_Milli (unsigned long Millis);
void Timer_Bdelay_Sec (unsigned long Sec);
void SetPWMValues(U32 direction, U32 speed_timer_delay);
void StartServoMotor();
void StopServoMotor();

#endif
