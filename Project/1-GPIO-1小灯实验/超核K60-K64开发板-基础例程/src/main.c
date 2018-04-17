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
    
		
		
		 
		 int8_t image_count;                   //统计图像的采样数
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
	  TimeCrtlInit();   //定时器初始化 
	  UART_QuickInit(UART0_RX_PA15_TX_PA14,115200);  //串口初始化
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
////			    TimeFlag.Arrival_100ms=0;   //时间片：每100ms置1
//				   RUN();
//				 	 OLED_CLS();  //OLED清屏
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
//						if(x==20)   //参数待调
//						{
//						  y=1;      //摄像头确实没有看到的标志位
//						}
//						if(x>100)   //防止x溢出
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
		        //关闭场中断和行中断
	      GPIO_ITDMAConfig(HW_GPIOC,17,kGPIO_IT_FallingEdge,DISABLE);
	      GPIO_ITDMAConfig(HW_GPIOC,16,kGPIO_IT_RisingEdge,DISABLE);	
//			  FindLine(); 
			  
        CMOS_Flag.Image = FinishProcess;						 
						 //开启场中断和行中断
			 
	       if(TimeFlag.Arrival_1000ms == 1)
         {
				     TimeFlag.Arrival_1000ms = 0;
						 for(i=CMOS_H-1;i>=0;i--)
						 {
						     for(j=0;j<CMOS_W;j++)
								 {
  							     if(ImageBuf[i][j]==0xff)ImageBuf[i][j]--;  //模拟
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
		        //关闭场中断和行中断
	      GPIO_ITDMAConfig(HW_GPIOC,17,kGPIO_IT_FallingEdge,DISABLE);
	      GPIO_ITDMAConfig(HW_GPIOC,16,kGPIO_IT_RisingEdge,DISABLE);	
//	 		  FindLine(); 
        CMOS_Flag.Image = FinishProcess;						 
						//开启场中断和行中断
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
  							    if(ImageBuf[i][j]==0xff)ImageBuf[i][j]--;  //模拟
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
		        //关闭场中断和行中断
	      GPIO_ITDMAConfig(HW_GPIOC,17,kGPIO_IT_FallingEdge,DISABLE);
	      GPIO_ITDMAConfig(HW_GPIOC,16,kGPIO_IT_RisingEdge,DISABLE);	
			
			    Binaryzation(31,100);  //二值化  第一个数字是要二值化的行数，第二个数字是二值化的阈值
			


			  for(i=0 ; i<CMOS_W ; i++)    //从左到右列扫描
						{ 
							 duty2=0;
						    for(j=CMOS_H; j>=0 ;j--)   //从下到上行扫描
								{
  							  if(BinaryImage[j][i]==1 )
									{
									  if(duty1==0)   //duty1是判断是否是第一个光点的标志位
                  	{
										flag_L=i;      //光点最左边坐标
									  duty1=1;
										}										
										duty2=1;       //判断一列坐标有没有光点的标志位
									}
						    }
								if(duty2==0 && duty1==1)  //光点最右边的坐标
								{
								  flag_R=i;
									duty1=0;
								}
									 
						}		


						
			 
	
        CMOS_Flag.Image = FinishProcess;
						
						   //开启场中断和行中断
			  GPIO_ITDMAConfig(HW_GPIOC,17,kGPIO_IT_FallingEdge,ENABLE);
	      GPIO_ITDMAConfig(HW_GPIOC,16,kGPIO_IT_RisingEdge,ENABLE);
			

		}
		
   			
}

void find_light(int setfig)
 {
	 					
						 image_count=0;
								 if (setfig==0)
								{
								 error = (flag_R+flag_L)/2 - 30;      //往右转 待调参数
								 steering_pid(error);	
								} 
								 else if (setfig==1)
								 {
									 error = (flag_R+flag_L)/2 - 90;      //往左转  待调参数
									 steering_pid(error);	
								 } 		
						
						else //if(flag_L==0 || flag_R==0)       //此处更改信标丢失后的动作
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
                   x++;     //连续没有采集到的标志位										 
							 }
 
 }
 
 

     

