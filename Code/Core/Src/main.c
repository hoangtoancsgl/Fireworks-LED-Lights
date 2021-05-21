/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FlASH_PAGE.h"
#include "FlASH_PAGE.c"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);

uint32_t data = 0x12ABCDEF;
uint32_t Rx_Data;
unsigned char Buff[8];
unsigned char LED[7700]; 
unsigned char sb16[]={1,3,5,8,10,20,30,40,50,60,70,80,90,100,110,254};
unsigned char sb8[]={1,3,5,8,10,50,80,254};
unsigned char sb4[]={10,30,100,254};
unsigned char sb2[]={50,254};
unsigned int num_led = 512;
unsigned int num_col = 41;
unsigned int num_tia = 12;
unsigned char speed_col = 21;
unsigned char speed_tia = 50;
unsigned int Data[4] = {1, 2, 3, 4};
unsigned int RxData[5];

void LED_init()
{
	for(int led=0;led<512;led++)
	{
		int index=15*led;
		for(int i=0;i<3;i++)
		{
			LED[i*5+index]=0x84;
			LED[1+i*5+index]=0x21;
			LED[2+i*5+index]=0x08;
			LED[3+i*5+index]=0x42;
			LED[4+i*5+index]=0x10;
		}
	}
}

void setledR(int led,unsigned char data)
{
	led=15*led;
	for(int i=0;i<5;i++)LED[led+i]=0; //clear data cu
	
	if(data&0x80){LED[led]=0xF0;}
	else {LED[led]=0x80;}
	if(data&0x40){LED[led]|=0x07;LED[led+1]=0x80;}
	else {LED[led]|=0x04;}
	if(data&0x20){LED[led+1]|=0x3C;}
	else {LED[led+1]|=0x20;}
	if(data&0x10){LED[led+1]|=0x01;LED[led+2]=0xE0;}
	else {LED[led+1]|=0x01;}
	if(data&0x08){LED[led+2]|=0x0F;}
	else {LED[led+2]|=0x08;}
	if(data&0x04){LED[led+3]=0x78;}
	else {LED[led+3]=0x40;}
	if(data&0x02){LED[led+3]|=0x03;LED[led+4]=0xC0;}
	else {LED[led+3]|=0x02;}
	if(data&0x01){LED[led+4]|=0x1E;}
	else {LED[led+4]|=0x10;}
}
void setledG(int led,unsigned char data)
{
	led=15*led+5;
	for(int i=0;i<5;i++)LED[led+i]=0; //clear data cu
	
	if(data&0x80){LED[led]=0xF0;}
	else {LED[led]=0x80;}
	if(data&0x40){LED[led]|=0x07;LED[led+1]=0x80;}
	else {LED[led]|=0x04;}
	if(data&0x20){LED[led+1]|=0x3C;}
	else {LED[led+1]|=0x20;}
	if(data&0x10){LED[led+1]|=0x01;LED[led+2]=0xE0;}
	else {LED[led+1]|=0x01;}
	if(data&0x08){LED[led+2]|=0x0F;}
	else {LED[led+2]|=0x08;}
	if(data&0x04){LED[led+3]=0x78;}
	else {LED[led+3]=0x40;}
	if(data&0x02){LED[led+3]|=0x03;LED[led+4]=0xC0;}
	else {LED[led+3]|=0x02;}
	if(data&0x01){LED[led+4]|=0x1E;}
	else {LED[led+4]|=0x10;}
}
void setledB(int led,unsigned char data)
{
	led=15*led+10;
	for(int i=0;i<5;i++)LED[led+i]=0; //clear data cu
	
	if(data&0x80){LED[led]=0xF0;}
	else {LED[led]=0x80;}
	if(data&0x40){LED[led]|=0x07;LED[led+1]=0x80;}
	else {LED[led]|=0x04;}
	if(data&0x20){LED[led+1]|=0x3C;}
	else {LED[led+1]|=0x20;}
	if(data&0x10){LED[led+1]|=0x01;LED[led+2]=0xE0;}
	else {LED[led+1]|=0x01;}
	if(data&0x08){LED[led+2]|=0x0F;}
	else {LED[led+2]|=0x08;}
	if(data&0x04){LED[led+3]=0x78;}
	else {LED[led+3]=0x40;}
	if(data&0x02){LED[led+3]|=0x03;LED[led+4]=0xC0;}
	else {LED[led+3]|=0x02;}
	if(data&0x01){LED[led+4]|=0x1E;}
	else {LED[led+4]|=0x10;}
}
void SetLED(unsigned int led, unsigned char R, unsigned char G, unsigned char B)
{
		setledR(led, R);
		setledG(led, G);
		setledB(led, B);	
}
void NhapNhay(unsigned char color1, unsigned char color2, unsigned int stt1, unsigned int stt2 )
{
	for(int i=0;i<5;i++)
	{
		LED_init();
		switch (color1)
		{
			case 1://RED
				SetLED(stt2,255,0,0);
				break;
			case 2://GREEN
				SetLED(stt2,0,255,0);
				break;
			case 3://BLUE
				SetLED(stt2,0,0,255);
				break;
			case 4://RED GREEN
				SetLED(stt2,255,255,0);
				break;
			case 5://GREEN BLUE
				SetLED(stt2,0,255,255);
				break;
			case 6://RED BLUE
				SetLED(stt2,255,0,255);
				break;
			case 7://RED GREEN BLUE 
				SetLED(stt2,255,255,255);
				break;
			default: 
				break;
		}
		HAL_Delay(50);
		SetLED(stt2,0,0,0);
		switch (color2)
		{
			case 1://RED
				SetLED(stt1,255,0,0);
				HAL_Delay(100);
				break;
			case 2://GREEN
				SetLED(stt1,0,255,0);
				break;
			case 3://BLUE
				SetLED(stt1,0,0,255);
				break;
			case 4://RED GREEN
				SetLED(stt1,255,255,0);
				break;
			case 5://GREEN BLUE
				SetLED(stt1,0,255,255);
				break;
			case 6://RED BLUE
				SetLED(stt1,255,0,255);
				break;
			case 7://RED GREEN BLUE 
				SetLED(stt1,255,255,255);
				break;
			default: 
				break;
		}
		HAL_Delay(50);
		SetLED(stt1,0,0,0);
	}
}
void SaoBang16(unsigned char color, unsigned char colorH, int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+16;i++)
		{	
			 LED_init();//clear LED
			 int dem=15;
			 for(int s=start;s<16+start;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop-start){dem--;continue;}
					switch ((s!=start) ? color : colorH)
					{
						case 1://RED
							SetLED(i-s+start,sb16[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb16[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb16[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb16[dem+1],sb16[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb16[dem+1],sb16[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb16[dem+1],0,sb16[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb16[dem+1],sb16[dem+2],sb16[dem--]);
							break;
						default: 
							break;
					}		 			 
			 }
			 HAL_Delay(speed);
		}
}

void SaoBang16DoiNoiTiep(unsigned char color1, unsigned char color2, unsigned char colorH1,unsigned char colorH2,unsigned int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+35;i++)
		{	
			 LED_init();//clear LED
			 int dem=15;
			 if(i>start+16)
			 {
				 int k = i-19;
				 for(int s=start;s<16+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s==start) ? colorH2 : color2)
						{
							case 1://RED
								SetLED(k-s+start,sb16[dem--],0,0);
								break;
							case 2://GREEN
								SetLED(k-s+start,0,sb16[dem--],0);
								break;
							case 3://BLUE
								SetLED(k-s+start,0,0,sb16[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb16[dem+1],sb16[dem--],0);
								break;
							case 5://GREEN BLUE
								SetLED(k-s+start,0,sb16[dem+1],sb16[dem--]);
								break;
							case 6://RED BLUE
								SetLED(k-s+start,sb16[dem+1],0,sb16[dem--]);
								break;
							case 7://RED GREEN BLUE 
								SetLED(k-s+start,sb16[dem+1],sb16[dem+2],sb16[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
			 dem=15;
			 for(int s=start;s<16+start;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop-start){dem--;continue;}
					switch ((s==start) ? colorH1 : color1)
					{
						case 1://RED
							SetLED(i-s+start,sb16[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb16[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb16[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb16[dem+1],sb16[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb16[dem+1],sb16[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb16[dem+1],0,sb16[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb16[dem+1],sb16[dem+2],sb16[dem--]);
							break;
						default: 
							break;
					}		 			 
			 }
			 HAL_Delay(speed);
		}
}

void SaoBang8(unsigned char color, unsigned char colorH, int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+8;i++)
		{	
			 LED_init();//clear LED
			 int dem=7;
			 for(int s=start;s<start+8;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop-start){dem--;continue;}
					switch ((s==start) ? colorH : color)
					{
						case 1://RED
							SetLED(i-s+start,sb8[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb8[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb8[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb8[dem+1],sb8[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb8[dem+1],sb8[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb8[dem+1],0,sb8[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb8[dem],sb8[dem],sb8[dem--]);
							break;
						default: 
							break;
					}		 			 
			 }
			 HAL_Delay(speed);
		}
}
void SaoBang4(unsigned char color, unsigned char colorH, int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+4;i++)
		{	
			 LED_init();//clear LED
			 int dem=3;
			 for(int s=start;s<start+4;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop-start){dem--;continue;}
					switch ((s==start) ? colorH : color)
					{
						case 1://RED
							SetLED(i-s+start,sb4[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb4[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb4[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb4[dem+1],sb4[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb4[dem+1],sb4[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb4[dem+1],0,sb4[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb4[dem],sb8[dem],sb4[dem--]);
							break;
						default: 
							break;
					}		 			 
			 }
			 HAL_Delay(speed);
		}
}
void SaoBang2(unsigned char color, unsigned char colorH, int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+2;i++)
		{	
			 LED_init();//clear LED
			 int dem=1;
			 for(int s=start;s<start+2;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop-start){dem--;continue;}
					switch ((s==start) ? colorH : color)
					{
						case 1://RED
							SetLED(i-s+start,sb2[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb2[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb2[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb2[dem+1],sb2[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb2[dem+1],sb2[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb2[dem+1],0,sb2[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb2[dem],sb8[dem],sb2[dem--]);
							break;
						default: 
							break;
					}		 			 
			 }
			 HAL_Delay(speed);
		}
    LED_init();
    for(int i=0;i<5;i++)
    {
        SetLED(stop-1 ,255,0,0);
        HAL_Delay(50);
        SetLED(stop-1 ,0,0,0); 
        HAL_Delay(50);  
    }
}
void SaoBang8DoiSongSong(unsigned char color1, unsigned char color2, unsigned char colorH1,unsigned char colorH2,  unsigned int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+8;i++)
		{
			 LED_init();//clear LED
			 int dem=7;
			 for(int s=start;s<8+start;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop-start){dem--;continue;}
					switch ((s!=start) ? color1 : colorH1)
					{
						case 1://RED
							SetLED(i-s+start,sb8[dem],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb8[dem],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb8[dem]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb8[dem+1],sb8[dem],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb8[dem+1],sb8[dem]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb8[dem+1],0,sb8[dem]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb8[dem],sb8[dem],sb8[dem]);
							break;
						default: 
							break;
					}		 
					switch ((s!=start) ? color2 : colorH2)
					{
						case 1://RED
							SetLED(i-s+8+start,sb8[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+8+start,0,sb8[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+8+start,0,0,sb8[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+8+start,sb4[dem+1],sb8[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+8+start,0,sb8[dem+1],sb8[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+8+start,sb8[dem+1],0,sb8[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+8+start,sb8[dem+1],sb8[dem+2],sb8[dem--]);
							break;
						default: 
							break;	
					}		
			 }
			 HAL_Delay(speed);
		}
}
void SaoBang4DoiSongSong(unsigned char color1, unsigned char color2, unsigned char colorH1,unsigned char colorH2,  unsigned int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+8;i++)
		{
			 LED_init();//clear LED
			 int dem=7;
			 for(int s=start;s<8+start;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop-start){dem--;continue;}
					switch ((s!=start) ? color1 : colorH1)
					{
						case 1://RED
							SetLED(i-s+start,sb4[dem],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb4[dem],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb4[dem]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb4[dem+1],sb4[dem],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb4[dem+1],sb4[dem]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb4[dem+1],0,sb4[dem]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb4[dem],sb4[dem],sb4[dem]);
							break;
						default: 
							break;
					}		 
					switch ((s!=start) ? color2 : colorH2)
					{
						case 1://RED
							SetLED(i-s+8+start,sb4[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+8+start,0,sb4[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+8+start,0,0,sb4[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+8+start,sb4[dem+1],sb4[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+8+start,0,sb4[dem+1],sb4[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+8+start,sb4[dem+1],0,sb4[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+8+start,sb4[dem+1],sb4[dem+2],sb4[dem--]);
							break;
						default: 
							break;	
					}		
			 }
			 HAL_Delay(speed);
		}
}

void SaoBang8BaSongSong(unsigned char color1, unsigned char color2, unsigned char color3,  unsigned int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+8;i++)
		{
			 LED_init();//clear LED
			 int dem=7;
			 for(int s=start;s<8+start;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop-start){dem--;continue;}
					switch (color1)
					{
						case 1://RED
							SetLED(i-s+start,sb8[dem],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb8[dem],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb8[dem]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb8[dem],sb8[dem],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb8[dem],sb8[dem]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb8[dem],0,sb8[dem]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb8[dem],sb8[dem],sb8[dem]);
							break;
						default: 
							break;
					}		 
					switch (color2)
					{
						case 1://RED
							SetLED(i-s+5+start,sb8[dem],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+5+start,0,sb8[dem],0);
							break;
						case 3://BLUE
							SetLED(i-s+5+start,0,0,sb8[dem]);
							break;
						case 4://RED GREEN
							SetLED(i-s+5+start,sb4[dem],sb8[dem],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+5+start,0,sb8[dem],sb8[dem]);
							break;
						case 6://RED BLUE
							SetLED(i-s+5+start,sb8[dem],0,sb8[dem]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+5+start,sb8[dem],sb8[dem],sb8[dem]);
							break;
						default: 
							break;	
					}		
					switch (color3)
					{
						case 1://RED
							SetLED(i-s+10+start,sb8[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+10+start,0,sb8[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+10+start,0,0,sb8[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+10+start,sb4[dem],sb8[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+10+start,0,sb8[dem],sb8[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+10+start,sb8[dem],0,sb8[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+10+start,sb8[dem+1],sb8[dem+2],sb8[dem--]);
							break;
						default: 
							break;	
					}		
			 }
			 HAL_Delay(speed);
		}
}
void SaoBang8DoiNoiTiep(unsigned char color1, unsigned char color2,unsigned char colorH1,unsigned char colorH2, unsigned int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+18;i++)
		{	
			 LED_init();//clear LED
			 int dem=7;
			 if(i>start+8)
			 {
				 int k = i-10;
				 for(int s=start;s<8+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? color2 : colorH2)
						{
							case 7://RED
								SetLED(k-s+start,sb8[dem--],0,0);
								break;
							case 6://GREEN
								SetLED(k-s+start,0,sb8[dem--],0);
								break;
							case 5://BLUE
								SetLED(k-s+start,0,0,sb8[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb8[dem+1],sb8[dem--],0);
								break;
							case 3://GREEN BLUE
								SetLED(k-s+start,0,sb8[dem+1],sb8[dem--]);
								break;
							case 2://RED BLUE
								SetLED(k-s+start,sb8[dem+1],0,sb8[dem--]);
								break;
							case 1://RED GREEN BLUE 
								SetLED(k-s+start,sb8[dem+1],sb8[dem+2],sb8[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
			 dem=7;
			 for(int s=start;s<7+start;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop){dem--;continue;}
					switch ((s!=start) ? color1 : colorH1)
					{
						case 1://RED
							SetLED(i-s+start,sb8[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb8[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb8[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb16[dem+1],sb8[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb8[dem+1],sb8[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb8[dem+1],0,sb8[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb8[dem+1],sb8[dem+2],sb8[dem--]);
							break;
						default: 
							break;
					}		 			 
			 }
			 HAL_Delay(speed);
		}
}
void SaoBang8BaNoiTiep(unsigned char color1, unsigned char color2,unsigned char colorH1,unsigned char colorH2, unsigned int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+26;i++)
		{	
			 LED_init();//clear LED
			 int dem=7;
			 if(i>start+8)
			 {
				 int k = i-10;
				 for(int s=start;s<8+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? color2 : colorH2)
						{
							case 1://RED
								SetLED(k-s+start,sb8[dem--],0,0);
								break;
							case 2://GREEN
								SetLED(k-s+start,0,sb8[dem--],0);
								break;
							case 3://BLUE
								SetLED(k-s+start,0,0,sb8[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb8[dem+1],sb8[dem--],0);
								break;
							case 5://GREEN BLUE
								SetLED(k-s+start,0,sb8[dem+1],sb8[dem--]);
								break;
							case 6://RED BLUE
								SetLED(k-s+start,sb8[dem+1],0,sb8[dem--]);
								break;
							case 7://RED GREEN BLUE 
								SetLED(k-s+start,sb8[dem+1],sb8[dem+2],sb8[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
       dem=7;
       if(i>start+16)
			 {
				 int k = i-18;
				 for(int s=start;s<8+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? color1 : colorH2)
						{
							case 7://RED
								SetLED(k-s+start,sb8[dem--],0,0);
								break;
							case 6://GREEN
								SetLED(k-s+start,0,sb8[dem--],0);
								break;
							case 5://BLUE
								SetLED(k-s+start,0,0,sb8[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb8[dem+1],sb8[dem--],0);
								break;
							case 3://GREEN BLUE
								SetLED(k-s+start,0,sb8[dem+1],sb8[dem--]);
								break;
							case 2://RED BLUE
								SetLED(k-s+start,sb8[dem+1],0,sb8[dem--]);
								break;
							case 1://RED GREEN BLUE 
								SetLED(k-s+start,sb8[dem+1],sb8[dem+2],sb8[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
			 dem=7;
			 for(int s=start;s<7+start;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop){dem--;continue;}
					switch ((s!=start) ? color1 : colorH1)
					{
						case 1://RED
							SetLED(i-s+start,sb8[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb8[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb8[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb16[dem+1],sb8[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb8[dem+1],sb8[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb8[dem+1],0,sb8[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb8[dem+1],sb8[dem+2],sb8[dem--]);
							break;
						default: 
							break;
					}		 			 
			 }
			 HAL_Delay(speed);
		}
}
void SaoBang4BaNoiTiep(unsigned char color1, unsigned char color2,unsigned char colorH1,unsigned char colorH2, unsigned int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+20;i++)
		{	
			 LED_init();//clear LED
			 int dem=3;
			 if(i>start+4)
			 {
				 int k = i-6;
				 for(int s=start;s<4+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? color2 : colorH2)
						{
							case 1://RED
								SetLED(k-s+start,sb4[dem--],0,0);
								break;
							case 2://GREEN
								SetLED(k-s+start,0,sb4[dem--],0);
								break;
							case 3://BLUE
								SetLED(k-s+start,0,0,sb4[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb4[dem+1],sb4[dem--],0);
								break;
							case 5://GREEN BLUE
								SetLED(k-s+start,0,sb4[dem+1],sb4[dem--]);
								break;
							case 6://RED BLUE
								SetLED(k-s+start,sb4[dem+1],0,sb4[dem--]);
								break;
							case 7://RED GREEN BLUE 
								SetLED(k-s+start,sb4[dem+1],sb4[dem+2],sb4[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
       dem=3;
       if(i>start+14)
			 {
				 int k = i-16;
				 for(int s=start;s<4+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? color1 : colorH2)
						{
							case 7://RED
								SetLED(k-s+start,sb4[dem--],0,0);
								break;
							case 6://GREEN
								SetLED(k-s+start,0,sb4[dem--],0);
								break;
							case 5://BLUE
								SetLED(k-s+start,0,0,sb4[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb4[dem+1],sb4[dem--],0);
								break;
							case 3://GREEN BLUE
								SetLED(k-s+start,0,sb4[dem+1],sb4[dem--]);
								break;
							case 2://RED BLUE
								SetLED(k-s+start,sb4[dem+1],0,sb4[dem--]);
								break;
							case 1://RED GREEN BLUE 
								SetLED(k-s+start,sb4[dem+1],sb4[dem+2],sb4[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
			 dem=3;
			 for(int s=start;s<3+start;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop){dem--;continue;}
					switch ((s!=start) ? color1 : colorH1)
					{
						case 1://RED
							SetLED(i-s+start,sb4[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb4[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb4[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb4[dem+1],sb4[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb4[dem+1],sb4[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb4[dem+1],0,sb4[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb4[dem+1],sb4[dem+2],sb4[dem--]);
							break;
						default: 
							break;
					}		 			 
			 }
			 HAL_Delay(speed);
		}
}
void SaoBang2DoiNoiTiep(unsigned char color1, unsigned char color2,unsigned char colorH1,unsigned char colorH2, unsigned int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+35;i++)
		{	
			 LED_init();//clear LED
			 int dem=1;
			 if(i>start+10)
			 {
				 int k = i-12;
				 for(int s=start;s<2+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? color2 : colorH1)
						{
							case 1://RED
								SetLED(k-s+start,sb2[dem--],0,0);
								break;
							case 2://GREEN
								SetLED(k-s+start,0,sb2[dem--],0);
								break;
							case 3://BLUE
								SetLED(k-s+start,0,0,sb2[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb2[dem+1],sb2[dem--],0);
								break;
							case 5://GREEN BLUE
								SetLED(k-s+start,0,sb2[dem+1],sb2[dem--]);
								break;
							case 6://RED BLUE
								SetLED(k-s+start,sb2[dem+1],0,sb2[dem--]);
								break;
							case 7://RED GREEN BLUE 
								SetLED(k-s+start,sb2[dem+1],sb2[dem+2],sb2[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
       dem=1;
			 if(i>start+15)
			 {
				 int k = i-17;
				 for(int s=start;s<2+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? color2 : colorH2)
						{
							case 1://RED
								SetLED(k-s+start,sb2[dem--],0,0);
								break;
							case 2://GREEN
								SetLED(k-s+start,0,sb2[dem--],0);
								break;
							case 3://BLUE
								SetLED(k-s+start,0,0,sb2[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb2[dem+1],sb2[dem--],0);
								break;
							case 5://GREEN BLUE
								SetLED(k-s+start,0,sb2[dem+1],sb2[dem--]);
								break;
							case 6://RED BLUE
								SetLED(k-s+start,sb2[dem+1],0,sb2[dem--]);
								break;
							case 7://RED GREEN BLUE 
								SetLED(k-s+start,sb2[dem+1],sb2[dem+2],sb2[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
       dem=1;
			 if(i>start+20)
			 {
				 int k = i-22;
				 for(int s=start;s<2+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? colorH2 : color1)
						{
							case 1://RED
								SetLED(k-s+start,sb2[dem--],0,0);
								break;
							case 2://GREEN
								SetLED(k-s+start,0,sb2[dem--],0);
								break;
							case 3://BLUE
								SetLED(k-s+start,0,0,sb2[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb2[dem+1],sb2[dem--],0);
								break;
							case 5://GREEN BLUE
								SetLED(k-s+start,0,sb2[dem+1],sb2[dem--]);
								break;
							case 6://RED BLUE
								SetLED(k-s+start,sb2[dem+1],0,sb2[dem--]);
								break;
							case 7://RED GREEN BLUE 
								SetLED(k-s+start,sb2[dem+1],sb2[dem+2],sb2[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
       dem=1;
			 if(i>start+30)
			 {
				 int k = i-32;
				 for(int s=start;s<2+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? colorH1 : colorH2)
						{
							case 1://RED
								SetLED(k-s+start,sb2[dem--],0,0);
								break;
							case 2://GREEN
								SetLED(k-s+start,0,sb2[dem--],0);
								break;
							case 3://BLUE
								SetLED(k-s+start,0,0,sb2[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb2[dem+1],sb2[dem--],0);
								break;
							case 5://GREEN BLUE
								SetLED(k-s+start,0,sb2[dem+1],sb2[dem--]);
								break;
							case 6://RED BLUE
								SetLED(k-s+start,sb2[dem+1],0,sb2[dem--]);
								break;
							case 7://RED GREEN BLUE 
								SetLED(k-s+start,sb2[dem+1],sb2[dem+2],sb2[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
       dem=1;
			 for(int s=start;s<2+start;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop){dem--;continue;}
					switch ((s!=start) ? color1 : colorH1)
					{
						case 1://RED
							SetLED(i-s+start,sb2[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb2[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb2[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb2[dem+1],sb2[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb4[dem+1],sb2[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb2[dem+1],0,sb2[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb2[dem+1],sb2[dem+2],sb2[dem--]);
							break;
						default: 
							break;
					}		 			 
			 }
			 HAL_Delay(speed);
		}
}
void SaoBang2NamNoiTiep(unsigned char color1, unsigned char color2,unsigned char colorH1,unsigned char colorH2, unsigned int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+20;i++)
		{	
			 LED_init();//clear LED
			 int dem=1;
			 if(i>start+3)
			 {
				 int k = i-4;
				 for(int s=start;s<2+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? color2 : colorH1)
						{
							case 1://RED
								SetLED(k-s+start,sb2[dem--],0,0);
								break;
							case 2://GREEN
								SetLED(k-s+start,0,sb2[dem--],0);
								break;
							case 3://BLUE
								SetLED(k-s+start,0,0,sb2[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb2[dem+1],sb2[dem--],0);
								break;
							case 5://GREEN BLUE
								SetLED(k-s+start,0,sb2[dem+1],sb2[dem--]);
								break;
							case 6://RED BLUE
								SetLED(k-s+start,sb2[dem+1],0,sb2[dem--]);
								break;
							case 7://RED GREEN BLUE 
								SetLED(k-s+start,sb2[dem+1],sb2[dem+2],sb2[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
       dem=1;
			 if(i>start+6)
			 {
				 int k = i-8;
				 for(int s=start;s<2+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? color2 : colorH2)
						{
							case 1://RED
								SetLED(k-s+start,sb2[dem--],0,0);
								break;
							case 2://GREEN
								SetLED(k-s+start,0,sb2[dem--],0);
								break;
							case 3://BLUE
								SetLED(k-s+start,0,0,sb2[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb2[dem+1],sb2[dem--],0);
								break;
							case 5://GREEN BLUE
								SetLED(k-s+start,0,sb2[dem+1],sb2[dem--]);
								break;
							case 6://RED BLUE
								SetLED(k-s+start,sb2[dem+1],0,sb2[dem--]);
								break;
							case 7://RED GREEN BLUE 
								SetLED(k-s+start,sb2[dem+1],sb2[dem+2],sb2[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
       dem=1;
			 if(i>start+8)
			 {
				 int k = i-12;
				 for(int s=start;s<2+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? colorH2 : color1)
						{
							case 1://RED
								SetLED(k-s+start,sb2[dem--],0,0);
								break;
							case 2://GREEN
								SetLED(k-s+start,0,sb2[dem--],0);
								break;
							case 3://BLUE
								SetLED(k-s+start,0,0,sb2[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb2[dem+1],sb2[dem--],0);
								break;
							case 5://GREEN BLUE
								SetLED(k-s+start,0,sb2[dem+1],sb2[dem--]);
								break;
							case 6://RED BLUE
								SetLED(k-s+start,sb2[dem+1],0,sb2[dem--]);
								break;
							case 7://RED GREEN BLUE 
								SetLED(k-s+start,sb2[dem+1],sb2[dem+2],sb2[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
       dem=1;
			 if(i>start+10)
			 {
				 int k = i-15;
				 for(int s=start;s<2+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? colorH1 : colorH2)
						{
							case 1://RED
								SetLED(k-s+start,sb2[dem--],0,0);
								break;
							case 2://GREEN
								SetLED(k-s+start,0,sb2[dem--],0);
								break;
							case 3://BLUE
								SetLED(k-s+start,0,0,sb2[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb2[dem+1],sb2[dem--],0);
								break;
							case 5://GREEN BLUE
								SetLED(k-s+start,0,sb2[dem+1],sb2[dem--]);
								break;
							case 6://RED BLUE
								SetLED(k-s+start,sb2[dem+1],0,sb2[dem--]);
								break;
							case 7://RED GREEN BLUE 
								SetLED(k-s+start,sb2[dem+1],sb2[dem+2],sb2[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
       dem=1;
       if(i>start+12)
			 {
				 int k = i-19;
				 for(int s=start;s<2+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? color2 : colorH1)
						{
							case 7://RED
								SetLED(k-s+start,sb2[dem--],0,0);
								break;
							case 6://GREEN
								SetLED(k-s+start,0,sb2[dem--],0);
								break;
							case 5://BLUE
								SetLED(k-s+start,0,0,sb2[dem--]);
								break;
							case 4://RED GREEN
								SetLED(k-s+start,sb2[dem+1],sb2[dem--],0);
								break;
							case 3://GREEN BLUE
								SetLED(k-s+start,0,sb2[dem+1],sb2[dem--]);
								break;
							case 2://RED BLUE
								SetLED(k-s+start,sb2[dem+1],0,sb2[dem--]);
								break;
							case 1://RED GREEN BLUE 
								SetLED(k-s+start,sb2[dem+1],sb2[dem+2],sb2[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
			 dem=1;
			 for(int s=start;s<2+start;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop){dem--;continue;}
					switch ((s!=start) ? color1 : colorH1)
					{
						case 1://RED
							SetLED(i-s+start,sb2[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb2[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb2[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb2[dem+1],sb2[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb4[dem+1],sb2[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb2[dem+1],0,sb2[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb2[dem+1],sb2[dem+2],sb2[dem--]);
							break;
						default: 
							break;
					}		 			 
			 }
			 HAL_Delay(speed);
		}
}
void SaoBang4DoiNoiTiep(unsigned char color1, unsigned char color2,unsigned char colorH1,unsigned char colorH2, unsigned int speed, unsigned int start , unsigned int stop)
{
	for(int i=start;i<stop+10;i++)
		{	
			 LED_init();//clear LED
			 int dem=3;
			 if(i>start+4)
			 {
				 int k = i-6;
				 for(int s=start;s<4+start;s++)
				 {
						if(k-s<0)break;
						if(k-s >=stop-start){dem--;continue;}
						switch ((s!=start) ? color1 : color2)
						{
							case 4://RED
								SetLED(k-s+start,sb4[dem--],0,0);
								break;
							case 7://GREEN
								SetLED(k-s+start,0,sb4[dem--],0);
								break;
							case 6://BLUE
								SetLED(k-s+start,0,0,sb4[dem--]);
								break;
							case 5://RED GREEN
								SetLED(k-s+start,sb4[dem+1],sb4[dem--],0);
								break;
							case 1://GREEN BLUE
								SetLED(k-s+start,0,sb4[dem+1],sb4[dem--]);
								break;
							case 2://RED BLUE
								SetLED(k-s+start,sb4[dem+1],0,sb4[dem--]);
								break;
							case 3://RED GREEN BLUE 
								SetLED(k-s+start,sb4[dem+1],sb4[dem+2],sb4[dem--]);
								break;
							default: 
								break;
						}		 			 
				 }
			 }
			 dem=3;
			 for(int s=start;s<3+start;s++)
			 {
					if(i-s<0)break;
					if(i-s >=stop){dem--;continue;}
					switch ((s!=start) ? color1 : colorH2)
					{
						case 1://RED
							SetLED(i-s+start,sb4[dem--],0,0);
							break;
						case 2://GREEN
							SetLED(i-s+start,0,sb4[dem--],0);
							break;
						case 3://BLUE
							SetLED(i-s+start,0,0,sb4[dem--]);
							break;
						case 4://RED GREEN
							SetLED(i-s+start,sb4[dem+1],sb4[dem--],0);
							break;
						case 5://GREEN BLUE
							SetLED(i-s+start,0,sb4[dem+1],sb4[dem--]);
							break;
						case 6://RED BLUE
							SetLED(i-s+start,sb4[dem+1],0,sb4[dem--]);
							break;
						case 7://RED GREEN BLUE 
							SetLED(i-s+start,sb4[dem+1],sb4[dem+2],sb4[dem--]);
							break;
						default: 
							break;
					}		 			 
			 }
			 HAL_Delay(speed);
		}
}
void MotBongChay(unsigned char color, unsigned int speed, unsigned int start ,unsigned int stop)
{
  for(int i=start;i<stop;i++)
  {	
     LED_init();
	   switch(color)
     {
        case 1://RED
          SetLED(i,255,0,0);
          break;
        case 2://GREEN
          SetLED(i,0,255,0);
          break;
        case 3://BLUE
          SetLED(i,0,0,255);
          break;
        case 4://RED GREEN
          SetLED(i,255,255,0);
          break;
        case 5://GREEN BLUE
          SetLED(i,0,255,255);
          break;
        case 6://RED BLUE
          SetLED(i,255,0,255);
          break;
        case 7://RED GREEN BLUE 
          SetLED(i,255,255,255);
          break;
        default: 
          break;
     }	         
     HAL_Delay(speed);
  }
  NhapNhay(2,3,stop-1,stop-2);
      
}
void MotBongChayNoiTiep(unsigned char color, unsigned int speed, unsigned int start ,unsigned int stop)
{
  for(int i=start;i<stop+62;i++)
  {	
     LED_init();
     if(i>start +10)
     {
       int k = i-11;
       switch((color+6)%7)
       {
          case 1://RED
            SetLED(k,255,0,0);
            break;
          case 2://GREEN
            SetLED(k,0,255,0);
            break;
          case 3://BLUE
            SetLED(k,0,0,255);
            break;
          case 4://RED GREEN
            SetLED(k,255,255,0);
            break;
          case 5://GREEN BLUE
            SetLED(k,0,255,255);
            break;
          case 6://RED BLUE
            SetLED(k,255,0,255);
            break;
          case 7://RED GREEN BLUE 
            SetLED(k,255,255,255);
            break;
          default: 
            break;
       }   
     }
     if(i>start +20)
     {
       int k = i-21;
       switch((color+5)%7)
       {
          case 1://RED
            SetLED(k,255,0,0);
            break;
          case 2://GREEN
            SetLED(k,0,255,0);
            break;
          case 3://BLUE
            SetLED(k,0,0,255);
            break;
          case 4://RED GREEN
            SetLED(k,255,255,0);
            break;
          case 5://GREEN BLUE
            SetLED(k,0,255,255);
            break;
          case 6://RED BLUE
            SetLED(k,255,0,255);
            break;
          case 7://RED GREEN BLUE 
            SetLED(k,255,255,255);
            break;
          default: 
            break;
       }   
     }
     if(i>start +30)
     {
       int k = i-31;
       switch((color+4)%7)
       {
          case 1://RED
            SetLED(k,255,0,0);
            break;
          case 2://GREEN
            SetLED(k,0,255,0);
            break;
          case 3://BLUE
            SetLED(k,0,0,255);
            break;
          case 4://RED GREEN
            SetLED(k,255,255,0);
            break;
          case 5://GREEN BLUE
            SetLED(k,0,255,255);
            break;
          case 6://RED BLUE
            SetLED(k,255,0,255);
            break;
          case 7://RED GREEN BLUE 
            SetLED(k,255,255,255);
            break;
          default: 
            break;
       }   
     }
     if(i>start +40)
     {
       int k = i-41;
       switch((color+3)%7)
       {
          case 1://RED
            SetLED(k,255,0,0);
            break;
          case 2://GREEN
            SetLED(k,0,255,0);
            break;
          case 3://BLUE
            SetLED(k,0,0,255);
            break;
          case 4://RED GREEN
            SetLED(k,255,255,0);
            break;
          case 5://GREEN BLUE
            SetLED(k,0,255,255);
            break;
          case 6://RED BLUE
            SetLED(k,255,0,255);
            break;
          case 7://RED GREEN BLUE 
            SetLED(k,255,255,255);
            break;
          default: 
            break;
       }   
     }
     if(i>start +50)
     {
       int k = i-51;
       switch((color+2)%7)
       {
          case 1://RED
            SetLED(k,255,0,0);
            break;
          case 2://GREEN
            SetLED(k,0,255,0);
            break;
          case 3://BLUE
            SetLED(k,0,0,255);
            break;
          case 4://RED GREEN
            SetLED(k,255,255,0);
            break;
          case 5://GREEN BLUE
            SetLED(k,0,255,255);
            break;
          case 6://RED BLUE
            SetLED(k,255,0,255);
            break;
          case 7://RED GREEN BLUE 
            SetLED(k,255,255,255);
            break;
          default: 
            break;
       }   
     }
     if(i>start +60)
     {
       int k = i-61;
       switch((color+1)%7)
       {
          case 1://RED
            SetLED(k,255,0,0);
            break;
          case 2://GREEN
            SetLED(k,0,255,0);
            break;
          case 3://BLUE
            SetLED(k,0,0,255);
            break;
          case 4://RED GREEN
            SetLED(k,255,255,0);
            break;
          case 5://GREEN BLUE
            SetLED(k,0,255,255);
            break;
          case 6://RED BLUE
            SetLED(k,255,0,255);
            break;
          case 7://RED GREEN BLUE 
            SetLED(k,255,255,255);
            break;
          default: 
            break;
       }   
     }
     switch(color)
     {
        case 1://RED
          SetLED(i,255,0,0);
          break;
        case 2://GREEN
          SetLED(i,0,255,0);
          break;
        case 3://BLUE
          SetLED(i,0,0,255);
          break;
        case 4://RED GREEN
          SetLED(i,255,255,0);
          break;
        case 5://GREEN BLUE
          SetLED(i,0,255,255);
          break;
        case 6://RED BLUE
          SetLED(i,255,0,255);
          break;
        case 7://RED GREEN BLUE 
          SetLED(i,255,255,255);
          break;
        default: 
          break;
     }	      
     HAL_Delay(speed);
  }
      
}


void UART_SendCMD(unsigned char CMD, unsigned int dat)
{
		Buff[3] = CMD;
		Buff[5] = (unsigned char)(dat>>8);
		Buff[6] = (unsigned char)(dat);
		HAL_UART_Transmit(&huart1, (uint8_t*)Buff,sizeof(Buff),100);
}

void GhiFlash(unsigned num_col, unsigned char num_tia, unsigned speed_col, unsigned char speed_tia)
{
	uint32_t temp;
	temp = num_col;
	temp = (temp<<8)|num_tia;
	temp = (temp<<8)|speed_col;
	temp = (temp<<8)|speed_tia;	
	Flash_Write_Data(0x0801F810, &temp);	
}
void DocFlash()
{
		uint32_t temp;
		Flash_Read_Data(0x0801F810, &temp);
		num_col = (unsigned char)(temp>>24);
		num_tia = (unsigned char)(temp>>16);
		speed_col = (unsigned char)(temp>>8);
		speed_tia = (unsigned char)(temp);
}
void Ghi()
{
    Data[0] = num_col;
    Data[1] = num_tia;
    Data[2] = speed_col;
    Data[3] = speed_tia;
    Flash_Write_Data(0x801FC00, Data);
}
void Doc()
{
    Flash_Read_Data(0x801FC00, RxData);
    num_col = (unsigned int)(RxData[0]);
    num_tia = (unsigned int)(RxData[1]);
    speed_col = (unsigned int)(RxData[2]);
    speed_tia = (unsigned int)(RxData[3]);
}
void HienThiCot()
{
	for(int i=0;i<num_led;i++)
	{
			if(i<num_col) 
				SetLED(i,255,0,0);
			else
				SetLED(i,0,0,0);			
	}
}
void HienThiCotTia()
{
	for(int i=0;i<num_led;i++)
	{
			if(i<num_col)
				SetLED(i,255,0,0);
			else if(i<num_col+num_tia)
				SetLED(i,0,0,255);	
			else 
				SetLED(i,0,0,0);	
	}
}
void HienThiSpeedCol()
{
		SaoBang8(4,7,speed_col,0,num_col);
}
void SetUpCol()
{
	HienThiCot();
	while(1)
	{
			if(HAL_GPIO_ReadPin(GPIOA, ADD_Pin)==0)
			{
					HAL_Delay(20);
					if(HAL_GPIO_ReadPin(GPIOA, ADD_Pin)==0)
					{
							num_col++;
							HienThiCot();
							while(HAL_GPIO_ReadPin(GPIOA, ADD_Pin)==0);		
					}	
			}
			if(HAL_GPIO_ReadPin(GPIOA, SUB_Pin)==0)
			{
					HAL_Delay(20);
					if(HAL_GPIO_ReadPin(GPIOA, SUB_Pin)==0)
					{
							if(num_col==1) ; 
							else 
							num_col--;
							HienThiCot();
							while(HAL_GPIO_ReadPin(GPIOA, SUB_Pin)==0);		
					}	
			}
			if(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0)
			{
					HAL_Delay(20);
					if(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0)
					{
						while(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0);		
						break;
					}
			}
	}
}
void SetUpTia()
{
	HienThiCotTia();
	while(1)
	{
			if(HAL_GPIO_ReadPin(GPIOA, ADD_Pin)==0)
			{
					HAL_Delay(20);
					if(HAL_GPIO_ReadPin(GPIOA, ADD_Pin)==0)
					{
							num_tia++;
							HienThiCotTia();
							while(HAL_GPIO_ReadPin(GPIOA, ADD_Pin)==0);		
					}	
			}
			if(HAL_GPIO_ReadPin(GPIOA, SUB_Pin)==0)
			{
					HAL_Delay(20);
					if(HAL_GPIO_ReadPin(GPIOA, SUB_Pin)==0)
					{
							if(num_tia==1) ; 
							else 
							num_tia--;
							HienThiCotTia();
							while(HAL_GPIO_ReadPin(GPIOA, SUB_Pin)==0);		
					}	
			}
			if(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0)
			{
					HAL_Delay(20);
					if(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0)
					{
						//GhiFlash(num_col, num_tia, 0x17, 0x18);
						break;
					}
			}
	}
}
void SetSpeedCol()
{
	while(1)
	{
			SaoBang8(1,1,speed_col,0,num_col);
			if(HAL_GPIO_ReadPin(GPIOA, ADD_Pin)==0)
			{
					HAL_Delay(20);
					if(HAL_GPIO_ReadPin(GPIOA, ADD_Pin)==0)
					{
              if(speed_col==5) ; 
							else 
							speed_col-=2;
							while(HAL_GPIO_ReadPin(GPIOA, ADD_Pin)==0);		
					}	
			}
			if(HAL_GPIO_ReadPin(GPIOA, SUB_Pin)==0)
			{
					HAL_Delay(20);
					if(HAL_GPIO_ReadPin(GPIOA, SUB_Pin)==0)
					{
							if(speed_col==100) ; 
							else 
							speed_col+=2;
							while(HAL_GPIO_ReadPin(GPIOA, SUB_Pin)==0);		
					}	
			}
			if(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0)
			{
					HAL_Delay(20);
					if(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0)
					{
						while(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0);		
						//GhiFlash(num_col, num_tia, speed_col, 0x18);
						break;
					}
			}
	}
}
void SetSpeedTia()
{
	while(1)
	{
			SaoBang8(2,2,speed_tia,num_col,num_col+num_tia);
			if(HAL_GPIO_ReadPin(GPIOA, ADD_Pin)==0)
			{
					HAL_Delay(20);
					if(HAL_GPIO_ReadPin(GPIOA, ADD_Pin)==0)
					{
							if(speed_tia==5) ; 
							else 
							speed_tia-=2;
							while(HAL_GPIO_ReadPin(GPIOA, ADD_Pin)==0);		
					}	
			}
			if(HAL_GPIO_ReadPin(GPIOA, SUB_Pin)==0)
			{
					HAL_Delay(20);
					if(HAL_GPIO_ReadPin(GPIOA, SUB_Pin)==0)
					{
              if(speed_tia==200) ; 
							else 
							speed_tia+=2;
							while(HAL_GPIO_ReadPin(GPIOA, SUB_Pin)==0);		
					}	
			}
			if(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0)
			{
					HAL_Delay(20);
					if(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0)
					{
						while(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0);		
						//GhiFlash(num_col, num_tia, speed_col, speed_tia);
            Ghi();
						break;
					}
			}
	}
}
void SetUp()
{
	LED_init();
	SetUpCol();
	SetUpTia();	
	SetSpeedCol();
	SetSpeedTia();
}
void CheckSetUp()
{
		if(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0)
		{
			HAL_Delay(20);
			if(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0)
			{
					while(HAL_GPIO_ReadPin(GPIOA, SET_Pin)==0);
					SetUp();
			}
		}
}
void HieuUng1()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16(1,2,speed_col,0,num_col);
	
		UART_SendCMD(0x03,0x03);	
		SaoBang8(3,4, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);	
		SaoBang8(2,1, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);	
		NhapNhay(1,5,num_col+4, num_col+11);
  	
		CheckSetUp();
}
void HieuUng2()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16(3,4,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang16(4,5, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+14);
  
		//NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng3()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16(5,6,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang8DoiNoiTiep(5,6,2,3, speed_tia,num_col,num_col+num_tia);	
		//NhapNhay(2,3,num_col+7, num_col+15);
    UART_SendCMD(0x03,0x04);
		NhapNhay(6,4,num_col+8, num_col+14);
		NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng4()
{
		UART_SendCMD(0x03,0x01);
    SaoBang8(1,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang16(2,3, speed_tia,num_col,num_col+num_tia);	
		//NhapNhay(2,3,num_col+7, num_col+15);
    UART_SendCMD(0x03,0x04);
		NhapNhay(6,4,num_col+10, num_col+18);
		NhapNhay(4,1,num_col+9, num_col+15);
		CheckSetUp();
}
void HieuUng5()
{
		UART_SendCMD(0x03,0x01);
    SaoBang8(1,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x02);	
		SaoBang8DoiSongSong(4,5,5,4, speed_tia,num_col,num_col+num_tia);	
		//SaoBang8DoiSongSong(4,5,5,4, speed_tia,num_col,num_col+num_tia);	
		//NhapNhay(2,3,num_col+7, num_col+15);
		//NhapNhay(6,4,num_col+8, num_col+14);
    UART_SendCMD(0x03,0x04);
		NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng6()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16(2,4,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		//SaoBang8DoiSongSong(2,3,4,5, speed_tia,num_col,num_col+num_tia);	
		SaoBang8(2,5, speed_tia,num_col,num_col+num_tia);
    UART_SendCMD(0x03,0x04);	
		NhapNhay(2,3,num_col+7, num_col+15);
		//NhapNhay(6,4,num_col+8, num_col+14);
		NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng7()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16(1,2,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		//SaoBang8BaSongSong(2,3,4, speed_tia,num_col,num_col+num_tia);	
		SaoBang8BaSongSong(5,6,7, speed_tia,num_col,num_col+num_tia);	
		//NhapNhay(2,3,num_col+7, num_col+15);
    UART_SendCMD(0x03,0x04);
		NhapNhay(6,4,num_col+8, num_col+14);
		//NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng8()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16(6,7,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang8DoiNoiTiep(2,3,4,5, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		//SaoBang16(4,5, speed_tia,num_col,num_col+num_tia);	
		NhapNhay(2,3,num_col+7, num_col+15);
		//NhapNhay(6,4,num_col+8, num_col+14);
		NhapNhay(1,5,num_col+9, num_col+15);
		CheckSetUp();
}
void HieuUng9()
{
		UART_SendCMD(0x03,0x01);
    SaoBang8DoiNoiTiep(1,5,3,4,speed_col,0,num_col);

		UART_SendCMD(0x03,0x02);	
		SaoBang8DoiSongSong(2,1,4,3, speed_tia,num_col,num_col+num_tia);	
		UART_SendCMD(0x03,0x03);	
		SaoBang8DoiSongSong(6,4,2,5, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		//NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+14);
		//NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng10()
{
		UART_SendCMD(0x03,0x01);
    SaoBang8(3,2,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		//SaoBang16DoiNoiTiep(5,2,4,5, speed_tia,num_col,num_col+num_tia);	
		SaoBang8(2,1, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		//NhapNhay(2,3,num_col+7, num_col+15);
		//NhapNhay(6,4,num_col+8, num_col+14);
		NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng11()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16DoiNoiTiep(1,7,3,2,speed_col,0,num_col);

		UART_SendCMD(0x03,0x02);	
		SaoBang8(4,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang8(1,2, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		//NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+20);
		//NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}

void HieuUng12()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16DoiNoiTiep(2,3,4,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(1,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8BaSongSong(1,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		//NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+20);
		//NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng13()
{
		UART_SendCMD(0x03,0x01);
    SaoBang8DoiNoiTiep(1,2,3,4,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(2,5,6, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8BaSongSong(4,1,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		//NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+20);
		//NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng14()
{
		UART_SendCMD(0x03,0x01);
    SaoBang8(4,1,speed_col,0,num_col);

		UART_SendCMD(0x03,0x04);	
		SaoBang8DoiSongSong(1,2,3,5, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8BaSongSong(1,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		//NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+20);
		//NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng15()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16(1,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x04);	
		SaoBang8BaSongSong(1,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8DoiNoiTiep(2,5,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		//NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,3,num_col+10, num_col+20);
		NhapNhay(2,5,num_col+8, num_col+15);
		CheckSetUp();
}
void HieuUng16()
{
		UART_SendCMD(0x03,0x01);
    SaoBang8DoiNoiTiep(1,5,1,3,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang8DoiSongSong(1,2,5,4, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8(2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		//NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+20);
		//NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng17()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16DoiNoiTiep(2,3,4,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(1,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);	
		SaoBang8(2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);	
		SaoBang8DoiNoiTiep(5,2,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		//NhapNhay(2,3,num_col+7, num_col+15);
		//NhapNhay(6,4,num_col+8, num_col+20);
		NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng18()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16DoiNoiTiep(2,3,4,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(1,2,3, speed_tia-5,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(3,2,1, speed_tia-2,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(4,5,6, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8(1,2, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		//NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+20);
		//NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng19()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16(1,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x02);	
		SaoBang16DoiNoiTiep(3,1,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);	
		SaoBang16(2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		//NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+20);
		//NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng20()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16DoiNoiTiep(2,3,4,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(1,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8BaSongSong(5,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);	
		SaoBang8BaSongSong(3,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		//NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+20);
		//NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng21()
{
		UART_SendCMD(0x03,0x01);
    SaoBang8DoiNoiTiep(1,2,3,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(1,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8BaSongSong(3,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
    SaoBang2(2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);	
		NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+20);
		NhapNhay(1,5,num_col+15, num_col+13);
		CheckSetUp();
}
void HieuUng22()
{
		UART_SendCMD(0x03,0x01);
    SaoBang8(2,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang8DoiSongSong(1,2,3,5, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8BaSongSong(5,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(3,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
    SaoBang2(3,5,speed_tia+30, num_col, num_col+num_tia);
    UART_SendCMD(0x03,0x04);
		//NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+20);
		NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng23()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16DoiNoiTiep(2,3,4,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(1,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8BaSongSong(5,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);	
		SaoBang8(2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang8BaSongSong(5,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang8BaSongSong(1,2,6, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang2(1,4,speed_tia+30, num_col, num_col+num_tia);
    UART_SendCMD(0x03,0x04);
		NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+20);
		NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng24()
{
		UART_SendCMD(0x03,0x01);
    SaoBang8DoiNoiTiep(2,1,2,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);
    SaoBang2(1,4,speed_tia+10, num_col, num_col+num_tia);
    UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(5,4,1, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8(2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang8BaSongSong(5,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang8DoiNoiTiep(4,1,2,6, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang16(1,4,speed_tia, num_col, num_col+num_tia);
    UART_SendCMD(0x03,0x04);
		NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+13);
		NhapNhay(1,5,num_col+9, num_col+16);
		CheckSetUp();
}
void HieuUng25()
{
		UART_SendCMD(0x03,0x01);
    SaoBang2(2,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);
    SaoBang8DoiSongSong(1,4,3,5,speed_tia, num_col, num_col+num_tia);
    UART_SendCMD(0x03,0x04);	
		SaoBang8BaSongSong(5,4,1, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8DoiNoiTiep(2,3,5,1, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang8BaSongSong(5,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang16(2,6, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang16(1,4,speed_tia, num_col, num_col+num_tia);
    UART_SendCMD(0x03,0x04);
		NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+13);
		//NhapNhay(1,5,num_col+9, num_col+16);
		CheckSetUp();
}
void HieuUng26()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16(6,1,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);
    SaoBang8DoiNoiTiep(1,4,3,5,speed_tia, num_col, num_col+num_tia);
    UART_SendCMD(0x03,0x04);	
		SaoBang8BaSongSong(5,4,1, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8DoiNoiTiep(2,3,5,1, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang8BaSongSong(5,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang8BaSongSong(6,5,1, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang8(2,6, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang16(1,4,speed_tia, num_col, num_col+num_tia);
    UART_SendCMD(0x03,0x03);
    SaoBang2(1,4,speed_tia, num_col, num_col+num_tia);
    UART_SendCMD(0x03,0x04);
		NhapNhay(5,4,num_col+15, num_col+7);
		NhapNhay(1,2,num_col+3, num_col+14);
		NhapNhay(1,5,num_col+9, num_col+16);
		CheckSetUp();
}
void HieuUng27()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16DoiNoiTiep(6,1,2,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);
    SaoBang16(1,5,speed_tia, num_col, num_col+num_tia);
    UART_SendCMD(0x03,0x04);	
		SaoBang8BaSongSong(2,5,1, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8DoiSongSong(2,3,5,1, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
    SaoBang8BaSongSong(5,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);
    SaoBang8BaSongSong(6,5,1, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang2(2,6, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang8(1,4,speed_tia, num_col, num_col+num_tia);
    UART_SendCMD(0x03,0x02);
    SaoBang16(1,4,speed_tia, num_col, num_col+num_tia);
    UART_SendCMD(0x03,0x04);
		NhapNhay(5,2,num_col+15, num_col+3);
		NhapNhay(1,2,num_col+6, num_col+12);
		NhapNhay(1,5,num_col+9, num_col+19);
		CheckSetUp();
}
void HieuUng28()
{
		UART_SendCMD(0x03,0x01);
    SaoBang8(4,1,speed_col,0,num_col);

		UART_SendCMD(0x03,0x02);	
		SaoBang8DoiSongSong(1,2,3,5, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(5,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);	
		SaoBang8BaSongSong(3,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);
    SaoBang2(3,5,speed_tia+10, num_col, num_col+num_tia);
    UART_SendCMD(0x03,0x04);
		NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+12, num_col+20);
		NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng29()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16DoiNoiTiep(2,3,4,5,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(1,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang8(5,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);	
		SaoBang2(3,2, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		NhapNhay(2,3,num_col+7, num_col+15);
		NhapNhay(6,4,num_col+8, num_col+20);
		//NhapNhay(1,5,num_col+9, num_col+13);
		CheckSetUp();
}
void HieuUng30()
{
		UART_SendCMD(0x03,0x01);
    SaoBang16(2,3,speed_col,0,num_col);

		UART_SendCMD(0x03,0x03);	
		SaoBang8(2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);	
		SaoBang8BaSongSong(5,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x03);	
		SaoBang8BaSongSong(3,2,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x02);	
		SaoBang16(1,3, speed_tia,num_col,num_col+num_tia);	
    UART_SendCMD(0x03,0x04);
		NhapNhay(2,1,num_col+7, num_col+15);
		NhapNhay(5,4,num_col+8, num_col+20);
		NhapNhay(1,2,num_col+9, num_col+13);
		CheckSetUp();
}
int main(void)
{
 
  HAL_Init();

  SystemClock_Config();

  
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
	HAL_SPI_Transmit_DMA(&hspi1,(uint8_t*)LED,7700);
	Buff[0] = 0x7E; //start
	Buff[1] = 0xFF; //version
	Buff[2] = 0x06; //num_data
	Buff[4] = 0x00; //feedback	
	Buff[7] = 0xEF;

  //Ghi();
  //Doc();
  HAL_Delay(2000);  
  UART_SendCMD(0x06,30);
  while(1)
  {  
//     SaoBang2DoiNoiTiep(1,2,3,4, speed_tia,num_col,num_col+num_tia);	
//     MotBongChay(1,40,num_col,num_col+num_tia );
//     MotBongChayNoiTiep(2,40,num_col,num_col+num_tia );
    HieuUng1();
    HAL_Delay(2000);
		HieuUng2();
    HAL_Delay(2000);
		HieuUng3();
    HAL_Delay(3000);
		HieuUng4();
    HAL_Delay(2000);
		HieuUng5();
    HAL_Delay(1500);
		HieuUng6();
    HAL_Delay(1600);
		HieuUng7();
    HAL_Delay(1700);
		HieuUng8();
    HAL_Delay(1800);
		HieuUng9();
    HAL_Delay(1900);
		HieuUng10();
    HAL_Delay(2000);
    HieuUng11();
    HAL_Delay(1000);
    HieuUng12();
    HAL_Delay(3000);
    HieuUng13();
    HAL_Delay(500);
		HieuUng14();
    HAL_Delay(1000);
		HieuUng15();
    HAL_Delay(500);
		HieuUng16();
    HAL_Delay(1000);
		HieuUng17();
    HAL_Delay(1000);
		HieuUng18();
    HAL_Delay(1000);
		HieuUng19();
    HAL_Delay(1000);
		HieuUng21();
    HAL_Delay(1000);
    HieuUng22();
    HAL_Delay(1000);
    HieuUng23();
    HAL_Delay(1000);
    HieuUng24();
    HAL_Delay(500);
    HieuUng25();
    HAL_Delay(500);
    HieuUng26();
    HieuUng27();
    HieuUng28();
    HieuUng29();
    HieuUng20();
    HieuUng30();
	}
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PA0 PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
