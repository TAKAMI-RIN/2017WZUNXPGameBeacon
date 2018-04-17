#include "ultrasonic.h"
#include "gpio.h"
#include "pit.h"
#include "uart.h"

int num;
int16_t Status;

 
void sonic_init()
{
  
	  GPIO_QuickInit(HW_GPIOE, 24, kGPIO_Mode_OPP);   /*TRIGGER */
	  GPIO_QuickInit(HW_GPIOE, 26, kGPIO_Mode_IPD);   /*ECHO */
	
	  PIT_QuickInit(HW_PIT_CH3,10);  //1us��ʱ
	
	  PIT_ITDMAConfig(HW_PIT_CH3, kPIT_IT_TOF, ENABLE); //����ʱ���жϣ�ʹ���ж�
	
	  
	
	  /* ����GPIO�ⲿ�����жϻص����� */
     GPIO_CallbackInstall(HW_GPIOE, GPIO_ISR);
	   GPIO_ITDMAConfig(HW_GPIOE,26,kGPIO_IT_RisingFallingEdge,true); //�����ⲿ�ж�
	
	   PIT_CallbackInstall(HW_PIT_CH3, PIT_ISR); 
     PIT_ITDMAConfig(HW_PIT_CH3, kPIT_IT_TOF, false);
}

void start()
{
   GPIO_SetBit(HW_GPIOE, 24);
	 DelayUs(20); 
	 GPIO_ResetBit(HW_GPIOE, 24);
}

 int result()
 {
   return num*17;
 }
 
 void GPIO_ISR()
{
    GPIO_ITDMAConfig(HW_GPIOE, 26, kGPIO_IT_RisingFallingEdge, false);
	        Status = GPIO_ReadBit( HW_GPIOE,26);
   
			if(Status==1)
			{
				num=0;
				PIT_ResetCounter(HW_PIT_CH3);
				PIT_ITDMAConfig(HW_PIT_CH3, kPIT_IT_TOF, true);
			  
			} 
			else  if(Status==0)
			{
			 PIT_ITDMAConfig(HW_PIT_CH3, kPIT_IT_TOF, false);
			}

    GPIO_ITDMAConfig(HW_GPIOE, 26, kGPIO_IT_RisingFallingEdge, true);
}

 void PIT_ISR(void)
{
    num++;
}

int getresult()
{
   start();
	 DelayMs(30); 
	return result();
}




