#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "CMOS.h" 
#include "TimeControl.h"
#include "ftm.h"
#include "motor.h"
#include "OLED.h"
#include "mode.h"
#include "motorpid.h"
#include "ultrasonic.h"


#define Ctrl_H 5



uint16_t flag_L=25, flag_R=25;

//     int   select={0,1}
    
		
		
		 
		 int8_t image_count;                   //ͳ��ͼ��Ĳ�����
		 int a,error;
		 int x,y,z;
		 int select[]={0,1,1};


void AdjustAnalogImage(void);
void AdjustDigitImage(void);
void find_light(int setfig);
void circle_direct(int setfig);
void RUN();

int main(void)
{
	  
   
    DelayInit();
	  TimeCrtlInit();   //��ʱ����ʼ�� 
	  UART_QuickInit(UART0_RX_PA15_TX_PA14,115200);  //���ڳ�ʼ��
	  CMOS_Init();
	  motor_init();
	  direct_init();
	  mode_init();
	  motorpid_Init();
	  OLED_Init();
    sonic_init();
	  set_target(20,20);
	 
		while(1)
    {
// if( getmode_State()==0)
// {
   AdjustAnalogImage();
//	 FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH0  , 963);
//	    printf("%d %d %d %d\r\n", feedback, feedback1, kr,kr1);
//   circle_right();
//	 bluetooth_back();
//	 set_target(-50,-50);
//	   straight();
//	 left();
	//    right();
// }
//  if( getmode_State()==1)
//	{
////	  if(TimeFlag.Arrival_100ms==1)
////			 {
////			    TimeFlag.Arrival_100ms=0;   //ʱ��Ƭ��ÿ100ms��1
//				   RUN();
//				 	 OLED_CLS();  //OLED����
//		      OLED_P6x8Str(15,2,"flag_L");
//			  	OLED_Print_Num(60,2,flag_L);
//			   	//OLED_P14x16Str(60,4,"flag_R");
//			    OLED_P6x8Str(15,4,"flag_R");
//			  	OLED_Print_Num(60,4,flag_R);
////		      a=getresult();
//		        	if(flag_L!=0 && flag_R!=0 )
//						{
//							x=0;
//							if(y==1)
//							{
//							  y=0;
//								z++;
//							}
//							find_light(select[z]);
//						}
//		        else
//						  circle_direct(select[z]);
//						if(x==20)   //��������
//						{
//						  y=1;      //����ͷȷʵû�п����ı�־λ
//						}
//						if(x>100)   //��ֹx���
//							x=100;					
//	}
    }		
}
		


void AdjustAnalogImage()
{
    int i = 0;
	  int j = 0;	
    if(CMOS_Flag.Image == FinishSampling)
    {
		        //�رճ��жϺ����ж�
	      GPIO_ITDMAConfig(HW_GPIOC,17,kGPIO_IT_FallingEdge,DISABLE);
	      GPIO_ITDMAConfig(HW_GPIOC,16,kGPIO_IT_RisingEdge,DISABLE);	
//			  FindLine(); 
			  
        CMOS_Flag.Image = FinishProcess;						 
						 //�������жϺ����ж�
			 
	       if(TimeFlag.Arrival_1000ms == 1)
         {
				     TimeFlag.Arrival_1000ms = 0;
						 for(i=CMOS_H-1;i>=0;i--)
						 {
						     for(j=0;j<CMOS_W;j++)
								 {
  							     if(ImageBuf[i][j]==0xff)ImageBuf[i][j]--;  //ģ��
								     UART_WriteByte(HW_UART0,ImageBuf[i][j]);
						     }						
             }	
						 UART_WriteByte(HW_UART0, 0xff);				
				 }		

        GPIO_ITDMAConfig(HW_GPIOC,17,kGPIO_IT_FallingEdge,ENABLE);
	      GPIO_ITDMAConfig(HW_GPIOC,16,kGPIO_IT_RisingEdge,ENABLE);				 
    }
}

void AdjustDigitImage()
{
    int i = 0;
	  int j = 0;	
    if(CMOS_Flag.Image == FinishSampling)
    {
		        //�رճ��жϺ����ж�
	      GPIO_ITDMAConfig(HW_GPIOC,17,kGPIO_IT_FallingEdge,DISABLE);
	      GPIO_ITDMAConfig(HW_GPIOC,16,kGPIO_IT_RisingEdge,DISABLE);	
//	 		  FindLine(); 
        CMOS_Flag.Image = FinishProcess;						 
						//�������жϺ����ж�
			  GPIO_ITDMAConfig(HW_GPIOC,17,kGPIO_IT_FallingEdge,ENABLE);
	      GPIO_ITDMAConfig(HW_GPIOC,16,kGPIO_IT_RisingEdge,ENABLE);
//        KeystoneCorrection(); 
	      if(TimeFlag.Arrival_1000ms == 1)
        {
				    TimeFlag.Arrival_1000ms = 0;
						for(i=CMOS_H-1;i>=0;i--)
						{
						    for(j=0;j<CMOS_W;j++)
								{
  							    if(ImageBuf[i][j]==0xff)ImageBuf[i][j]--;  //ģ��
											UART_WriteByte(HW_UART0,ImageBuf[i][j]);
						    }						
            }	
						UART_WriteByte(HW_UART0, 0xff);				
				}				        			
    }
}


void RUN(void)
{
	uint16_t a=0	 ;
	int i,j;
	int duty1=0,duty2=0;
	int error;
	
	
    if(CMOS_Flag.Image == FinishSampling)
    {
		        //�رճ��жϺ����ж�
	      GPIO_ITDMAConfig(HW_GPIOC,17,kGPIO_IT_FallingEdge,DISABLE);
	      GPIO_ITDMAConfig(HW_GPIOC,16,kGPIO_IT_RisingEdge,DISABLE);	
			
			    Binaryzation(31,100);  //��ֵ��  ��һ��������Ҫ��ֵ�����������ڶ��������Ƕ�ֵ������ֵ
			


			  for(i=0 ; i<CMOS_W ; i++)    //��������ɨ��
						{ 
							 duty2=0;
						    for(j=CMOS_H; j>=0 ;j--)   //���µ�����ɨ��
								{
  							  if(BinaryImage[j][i]==1 )
									{
									  if(duty1==0)   //duty1���ж��Ƿ��ǵ�һ�����ı�־λ
                  	{
										flag_L=i;      //������������
									  duty1=1;
										}										
										duty2=1;       //�ж�һ��������û�й��ı�־λ
									}
						    }
								if(duty2==0 && duty1==1)  //������ұߵ�����
								{
								  flag_R=i;
									duty1=0;
								}
									 
						}		


						
			 
	
        CMOS_Flag.Image = FinishProcess;
						
						   //�������жϺ����ж�
			  GPIO_ITDMAConfig(HW_GPIOC,17,kGPIO_IT_FallingEdge,ENABLE);
	      GPIO_ITDMAConfig(HW_GPIOC,16,kGPIO_IT_RisingEdge,ENABLE);
			

		}
		
   			
}

void find_light(int setfig)
 {
	 					
						 image_count=0;
								 if (setfig==0)
								{
								 error = (flag_R+flag_L)/2 - 30;      //����ת ��������
								 steering_pid(error);	
								} 
								 else if (setfig==1)
								 {
									 error = (flag_R+flag_L)/2 - 90;      //����ת  ��������
									 steering_pid(error);	
								 } 		
						
						else //if(flag_L==0 || flag_R==0)       //�˴������ű궪ʧ��Ķ���
						{
							
						}
 }
 
 void circle_direct(int setfig)
 {
                 image_count++;
					        if(image_count==10)
							 {    
								    image_count=0;
											if (setfig==0)
										{
										 circle_right();
										} 
										 else if (setfig==1)
										 {
											 circle_left();
										 }
                   x++;     //����û�вɼ����ı�־λ										 
							 }
 
 }
 
 

     

