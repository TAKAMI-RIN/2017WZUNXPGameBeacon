#ifndef __OLED_H__
#define __OLED_H__

#include "gpio.h"
#include "common.h"
/*GPIOŒª≤Ÿ∂®“Â*/
#define OLED_SCL PEout(5) 
#define OLED_SDA PEout(2)
#define OLED_RST PEout(3) 
#define OLED_DC PEout(6)
#define OLED_CS PEout(4) 

 extern uint8_t lanzhou96x64[768];
 void OLED_Init(void);
 void OLED_CLS(void);
 void OLED_P6x8Str(uint8_t x,uint8_t y,uint8_t ch[]);
 void OLED_P8x16Str(uint8_t x,uint8_t y,uint8_t ch[]);
 void OLED_P14x16Str(uint8_t x,uint8_t y,uint8_t ch[]);
 void OLED_HEXACSII(uint16_t hex,uint8_t* Print);
 void OLED_Print_Num(uint8_t x, uint8_t y, uint16_t num);
 void OLED_Print_Str(uint8_t x, uint8_t y, uint8_t ch[]);
 void OLED_PutPixel(uint8_t x,uint8_t y);
 void OLED_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t gif);
 void OLED_Set_Pos(uint8_t x, uint8_t y);
 void OLED_WrDat(uint8_t data);
 void Draw_LibLogo(void);
 void Draw_Landzo(void);
 void Draw_BMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t bmp[]);
 void OLED_Fill(uint8_t dat);
 void Dly_ms(uint16_t ms);
 void OLED_Wr6Dat(uint8_t data);
void OledPrint(uint16_t ADdataCount,uint16_t LineCount,uint16_t PredatorFAV, uint8_t *dataAD ) ;


#endif
