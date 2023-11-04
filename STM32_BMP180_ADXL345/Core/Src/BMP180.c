/*
 * BMP180.c
 *
 *  Created on: Nov 3, 2023
 *      Author: berky
 */




#include "BMP180.h"

/*     KALİBRASYON DEGERLERİ */
short AC1=0;
short AC2=0;
short AC3=0;
unsigned short AC4=0;
unsigned short AC5=0;
unsigned short AC6=0;
short B1=0;
short B2=0;
short MB=0;
short MC=0;
short MD=0;
/* *********************************** */

long UT = 0;
short oss = 0;
long UP = 0;
long X1 = 0;
long X2 = 0;
long X3 = 0;
long B3 = 0;
long B5 = 0;
unsigned long B4 = 0;
long B6 = 0;
unsigned long B7 = 0;

long Press = 0;
float Temp = 0;

float yukseklik=0;





void read_calliberation_data (void)
{
	uint8_t Callib_Data[22] = {0};
	uint16_t Callib_Start = 0xAA;
	HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS, Callib_Start, 1, Callib_Data,22, HAL_MAX_DELAY);

	AC1 = ((Callib_Data[0] << 8) | Callib_Data[1]);
	AC2 = ((Callib_Data[2] << 8) | Callib_Data[3]);
	AC3 = ((Callib_Data[4] << 8) | Callib_Data[5]);
	AC4 = ((Callib_Data[6] << 8) | Callib_Data[7]);
	AC5 = ((Callib_Data[8] << 8) | Callib_Data[9]);
	AC6 = ((Callib_Data[10] << 8) | Callib_Data[11]);
	B1 = ((Callib_Data[12] << 8) | Callib_Data[13]);
	B2 = ((Callib_Data[14] << 8) | Callib_Data[15]);
	MB = ((Callib_Data[16] << 8) | Callib_Data[17]);
	MC = ((Callib_Data[18] << 8) | Callib_Data[19]);
	MD = ((Callib_Data[20] << 8) | Callib_Data[21]);
	if ((AC1 == 0x0000 || AC1 == 0xFFFF) ||
	    (AC2 == 0x0000 || AC2 == 0xFFFF) ||
	    (AC3 == 0x0000 || AC3 == 0xFFFF) ||
	    (AC4 == 0x0000 || AC4 == 0xFFFF) ||
	    (AC5 == 0x0000 || AC5 == 0xFFFF) ||
	    (AC6 == 0x0000 || AC6 == 0xFFFF) ||
	    (B1 == 0x0000 || B1 == 0xFFFF) ||
	    (B2 == 0x0000 || B2 == 0xFFFF) ||
	    (MB == 0x0000 || MB == 0xFFFF) ||
	    (MC == 0x0000 || MC == 0xFFFF) ||
	    (MD == 0x0000 || MD == 0xFFFF))
	{
	    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	    while(1);												// Kalibrasyonun gerceklesmedigini gosterir
	}


}
uint16_t Get_UTemp (void)
{
	uint8_t datatowrite = 0x2E;
	uint8_t Temp_RAW[2] = {0};
	HAL_I2C_Mem_Write(BMP180_I2C, BMP180_ADDRESS, 0xF4, 1, &datatowrite, 1, 1000);
	HAL_Delay (5);  // wait 4.5 ms
	HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS, 0xF6, 1, Temp_RAW, 2, 1000);
	return ((Temp_RAW[0]<<8) + Temp_RAW[1]);
}

float BMP180_GetTemp (void)
{
	UT = Get_UTemp();
	X1 = ((UT-AC6) * (AC5/(pow(2,15))));
	X2 = ((MC*(pow(2,11))) / (X1+MD));
	B5 = X1+X2;
	Temp = (B5+8)/(pow(2,4));

	return Temp/10;
}
uint32_t Get_UPress (int oss)
{
	uint8_t datatowrite = 0x34+(oss<<6);
	uint8_t Press_RAW[3] = {0};
	HAL_I2C_Mem_Write(BMP180_I2C, BMP180_ADDRESS, 0xF4, 1, &datatowrite, 1, 1000);
	switch (oss)		 //  Çözünürlük modları "oversampling_setting"
	{
		case (0):
			HAL_Delay (5);
			break;
		case (1):
			HAL_Delay (8);
			break;
		case (2):
			HAL_Delay (14);
			break;
		case (3):
			HAL_Delay (26);
			break;
	}
	HAL_I2C_Mem_Read(BMP180_I2C, BMP180_ADDRESS, 0xF6, 1, Press_RAW, 3, 1000);
	return (((Press_RAW[0]<<16)+(Press_RAW[1]<<8)+Press_RAW[2]) >> (8-oss));
}

float BMP180_GetPress (int oss)
{
	UP = Get_UPress(oss);
	X1 = ((UT-AC6) * (AC5/(pow(2,15))));
	X2 = ((MC*(pow(2,11))) / (X1+MD));
	B5 = X1+X2;
	B6 = B5-4000;
	X1 = (B2 * (B6*B6/(pow(2,12))))/(pow(2,11));
	X2 = AC2*B6/(pow(2,11));
	X3 = X1+X2;
	B3 = (((AC1*4+X3)<<oss)+2)/4;
	X1 = AC3*B6/pow(2,13);
	X2 = (B1 * (B6*B6/(pow(2,12))))/(pow(2,16));
	X3 = ((X1+X2)+2)/pow(2,2);
	B4 = AC4*(unsigned long)(X3+32768)/(pow(2,15));
	B7 = ((unsigned long)UP-B3)*(50000>>oss);
	if (B7<0x80000000) Press = (B7*2)/B4;
	else Press = (B7/B4)*2;
	X1 = (Press/(pow(2,8)))*(Press/(pow(2,8)));
	X1 = (X1*3038)/(pow(2,16));
	X2 = (-7357*Press)/(pow(2,16));
	Press = Press + (X1+X2+3791)/(pow(2,4));

	return Press;
}


float BMP180_GetAlt (int oss)
{
	BMP180_GetPress (oss);
	return 44330*(1-(pow((Press/(float)atmPress), 0.19029495718)));
}

void BMP180_Start (void)
{
	read_calliberation_data();
}
