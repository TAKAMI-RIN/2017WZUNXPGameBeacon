#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H
#include "common.h"


void sonic_init();
void start();
int result();
void GPIO_ISR();
void PIT_ISR(void);
int getresult();


#endif