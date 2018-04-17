#ifndef __MOTORPID_H
#define __MOTORPID_H
#include "common.h"

 static void PIT0_CallBack(void);
 void motorpid_Init();
 void set_target(int16_t a,int16_t b);
 void circle_right();
 void circle_left();
 void bluetooth_back() ;

#endif

