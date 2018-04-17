#include "common.h"
#include "motorpid.h"
#include "lptmr.h"                         //lptmr head
#include "pit.h"                           //lptmr needed pit head
#include "motor.h"
#include "ftm.h"
#include "gpio.h"
#include "uart.h"
//#include "led.h"
      
			
static void PIT0_CallBack(void);                       //lptmr needed
     
     int16_t target ;                      //lptmr workpoint  左： -60：电机反转最快 +60：电机正转最快
     int16_t target1 ;                     //lptmr workpoint  右： -60：电机反转最快 +60：电机正转最快
     int16_t g;                            //lptmr workpoint     中间变量
     int16_t g1 ;                          //lptmr workpoint     中间变量
     int16_t Kp = 12;                      //lptmr workpoint     比例系数
     int16_t Ki = 6;                       //lptmr workpoint     积分系数
     int16_t Kd = 1;                       //lptmr workpoint     微分系数
     int16_t e_0=0;                        //lptmr workpoint     一次误差量（左）
     int16_t e_1=0;                        //lptmr workpoint     二次误差量（左）
     int16_t e_2=0;                        //lptmr workpoint     三次误差量（左）
     int16_t out, count, kr ;              //lptmr workpoint     中间量
     int16_t ep, ei, ed;                   //lptmr workpoint     中间量
     int16_t feedback;                     //lptmr workpoint     编码器的脉冲计数返回值（左）
     int16_t feedback1;                    //lptmr workpoint     编码器的脉冲计数返回值（右）
     int16_t e_01=0;                       //lptmr workpoint     一次误差量（右）
     int16_t e_11=0;                       //lptmr workpoint     二次误差量（右）
     int16_t e_21=0;                       //lptmr workpoint     三次误差量（右）
     int16_t out1, count1, kr1 ;           //lptmr workpoint     中间量
     int16_t ep1, ei1, ed1;                //lptmr workpoint     中间量
     int16_t amg=5;                        //lptmr workpoint
		 
		 void motorpid_Init()
		 {
	//		 led_Init();
		  target = 0;                                       //lptmr workpoint  向target赋值即可设置左后轮转速
	    target1 = 0;                                      //lptmr workpoint  向target1赋值即可设置右后轮转速
		LPTMR_PC_QuickInit(LPTMR_ALT1_PA19);	              //lptmr workpoint
	  PIT_QuickInit(HW_PIT_CH0, 2000);                    //lptmr needed pit setup point
    PIT_CallbackInstall(HW_PIT_CH0, PIT0_CallBack);     //lptmr needed
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF,ENABLE);    //lptmr needed
		GPIO_QuickInit(HW_GPIOC, 12, kGPIO_Mode_IFT);      //lptmr workpoint  右边编码器正反转标志位
	  GPIO_QuickInit(HW_GPIOC, 11, kGPIO_Mode_IFT);      //lptmr workpoint  左边编码器正反转标志位
		if(target<=80 &&target>=7 ){  
		  g = target *7+53;                                    //lptmr workpoint
	  }
		else if(target>=-80 && target<=-7){
			g = target *7-53;  
		}
	  else{
			g=0;
		}
		if(target1<=80 &&target1>=7 ){  
  		g1 = target1 *7+53;                                    //lptmr workpoint
	  }
		else if(target1>=-80 && target1<=-7){
			g1 = target1 *7-53;  
		}
		else{
		  g1=0;
		}
    }
		 
		 void set_target(int16_t a,int16_t b)
		 {
		  // led2_on();        //set_target函数调试标记
			 target = a;
			  target1 = b;
			  //led3_on();         //set_target函数调试标记
			 if(target<=80 && target>=7 ){  
		      g = target *7+53; 				 //lptmr workpoint
	      }
		    else if(target>=-80 && target<=-7){
			    g = target *7-53;  
		    }
	      else{
			    g=0;
		    }
			  //led0_on();                    //set_target函数调试标记
		    if(target1<=80 && target1>=7 ){  
  		    g1 = target1 *7+53;                                    //lptmr workpoint
	      }
		    else if(target1>=-80 && target1<=-7){
			    g1 = target1 *7-53;  
		    }
		    else{
		      g1=0;
		    }
				//led1_on();                       //set_target函数调试标记
		 }
		 
		 void circle_right()
		 {
		   	  FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH0  , 545);  
	        set_target(30,15);
		 }
		 
		 void circle_left()
		 {
		   	  FTM_PWM_ChangeDuty(HW_FTM1, HW_FTM_CH0  , 695);  
	        set_target(12,18);
		 }
		 
	 static void PIT0_CallBack(void){                               //lptmr workpoint
       if(amg==5){                                              //lptmr workpoint
				  count =  LPTMR_PC_ReadCounter();                      //lptmr workpoint
          LPTMR_PC_QuickInit(LPTMR_ALT2_PC05);	                //lptmr workpoint
		      if(GPIO_ReadBit(HW_GPIOC, 11)==1){                    //lptmr workpoint
				    feedback = count;                                   //lptmr workpoint
				  }                                                     //lptmr workpoint
				  else{                                                 //lptmr workpoint
				    feedback = 0-count;                                 //lptmr workpoint
				  }                                                     //lptmr workpoint
					e_0 = target - feedback ;                             //lptmr workpoint
          ep = e_0  - e_1;                                      //lptmr workpoint
          ei = e_0;                                             //lptmr workpoint
          ed = e_0 - 2*e_1 + e_2;                               //lptmr workpoint
          out = Kp*ep + Ki*ei + Kd*ed ;                         //lptmr workpoint
          kr = (out+g)*10 ;                              		    //lptmr workpoint
				  amg++;                                                //lptmr workpoint
		      if(kr>=10000){   					                          	//lptmr workpoint
             FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0  , 8000);   //lptmr workpoint
             FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1  , 0);			//lptmr workpoint
					}	                                                    //lptmr workpoint
           else if(kr<0 && target>=7){
						 FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0  , 0);      //lptmr workpoint
						 FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1  , 0);  //lptmr workpoint
					}
					else if(kr<0){                                        //lptmr workpoint
						 FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0  , 0);      //lptmr workpoint
						 FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1  , 0-kr);   //lptmr workpoint
					}                                                     //lptmr workpoint
					else{			                                            //lptmr workpoint
	           FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0  , kr);     //lptmr workpoint
             FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1  , 0);      //lptmr workpoint
					}                                           					//lptmr workpoint
	        e_2 = e_1;                                            //lptmr workpoint
					e_1 =e_0;                                             //lptmr workpoint
				  LPTMR_ClearCounter();                                 //lptmr workpoint
		    }			                                                  //lptmr workpoint
		    else{                                                   //lptmr workpoint
		      count1 =  LPTMR_PC_ReadCounter();                     //lptmr workpoint
          LPTMR_PC_QuickInit(LPTMR_ALT1_PA19);	                //lptmr workpoint
		      if(GPIO_ReadBit(HW_GPIOC, 12)==0){                    //lptmr workpoint
				    feedback1 = count1;                                 //lptmr workpoint
				  }                                                     //lptmr workpoint
				  else{                                                 //lptmr workpoint
				    feedback1 = 0-count1;                               //lptmr workpoint
				  }                                                     //lptmr workpoint
          e_01 = target1 - feedback1 ;                          //lptmr workpoint
          ep1 = e_01  - e_11;                                   //lptmr workpoint
          ei1 = e_01;                                           //lptmr workpoint
          ed1 = e_01 - 2*e_11 + e_21;                           //lptmr workpoint
          out1 = Kp*ep1 + Ki*ei1 + Kd*ed1 ;                     //lptmr workpoint
          kr1= (out1+g1)*10 ;                            	    	//lptmr workpoint
          amg--;		                                            //lptmr workpoint
				  if(kr1>=10000){                                       //lptmr workpoint
             FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2  , 8000);   //lptmr workpoint
			       FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3  , 0);      //lptmr workpoint
					}	                                                    //lptmr workpoint
          else if(kr1<0 && target1>=7){
						 FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2  , 0);      //lptmr workpoint
						 FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3  , 0);  //lptmr workpoint
					}
					else if(kr1<0){                                       //lptmr workpoint
						 FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2  , 0);      //lptmr workpoint
						 FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3  , 0-kr1);  //lptmr workpoint
					}                                                     //lptmr workpoint
					else{			                                            //lptmr workpoint
		         FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2  , kr1);    //lptmr workpoint
             FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3  , 0);      //lptmr workpoint
					}                                                     //lptmr workpoint
          e_21 = e_11;                                          //lptmr workpoint
					e_11 =e_01;	                                          //lptmr workpoint
					LPTMR_ClearCounter();                                 //lptmr workpoint
			 }                                                        //lptmr workpoint
      }                                                         //lptmr workpoint
void bluetooth_back(){
        printf("%d %d %d %d\r\n", feedback, feedback1, kr,kr1);
}	
			//20170407  reverse pid and g value fixed  LBH