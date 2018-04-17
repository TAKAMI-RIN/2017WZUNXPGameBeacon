#include "TimeControl.h"

TimeFlagTypedef TimeFlag;

void TimeCrtlInit(void)
{
    /* ��ʼ��PITģ�� */
    PIT_InitTypeDef PIT_InitStruct1;  //����ṹ�����
    PIT_InitStruct1.chl = HW_PIT_CH1; /* ʹ��0�Ŷ�ʱ�� */
    PIT_InitStruct1.timeInUs = 1000; /* ��ʱ����1mS */
    PIT_Init(&PIT_InitStruct1); //pitģ���ʼ��
    /* ע��PIT �жϻص����� */
    PIT_CallbackInstall(HW_PIT_CH1, TimeCrtl_ISR); //1�Ŷ�ʱ�����жϴ���
    /* ����PIT0��ʱ���ж� */
    PIT_ITDMAConfig(HW_PIT_CH1, kPIT_IT_TOF, true);
}

void TimeCrtl_ISR(void)
{
    static uint32_t TimeSlice = 0;
		TimeSlice++;
	  
	  TimeFlag.Arrival_1ms = 1;
    if(TimeSlice % 50 == 0)
    {
		    TimeFlag.Arrival_50ms = 1;
		}

	  if(TimeSlice % 100 == 0)
    {
		    TimeFlag.Arrival_100ms = 1;
		}
		if(TimeSlice % 500 == 0)
    {
		    TimeFlag.Arrival_500ms = 1;
		}
		if(TimeSlice % 1000 == 0)
    {
        TimeFlag.Arrival_1000ms = 1;
	
		}		
		if(TimeSlice == 4000000000) 
		{
		    TimeSlice = 0;
		}   
}

