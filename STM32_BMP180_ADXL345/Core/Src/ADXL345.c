 /*
 * ADXL345.c
 *
 *  Created on: Nov 1, 2023
 *      Author: berky
 */

#include "ADXL345.h"

uint8_t myDatas[6];
int16_t x,y,z;
float xG,yG,zG;

void SCAN_I2C_Address(void)
{
	for(int i=0;i<=255;i++)
	{
		if(HAL_I2C_IsDeviceReady(&hi2c1, i, 1, 10)== HAL_OK)
		{
			break;
		}
	}
}

void ADXL345_Read(uint8_t rRegister, uint8_t numberOfByte)
{
	HAL_I2C_Mem_Read(&hi2c1, ADXL345_I2C_SLAVE_ADDRESS, rRegister, 1, myDatas, numberOfByte, 100);
    /*HAL_StatusTypeDef i2cStatus = HAL_I2C_Mem_Read(&hi2c1, ADXL345_I2C_SLAVE_ADDRESS, rRegister, 1, myDatas, 1, 100);

    if (i2cStatus != HAL_OK) {
        I2C okuma işlemi hatalı ise D15 pini üzerinden bir işareti ayarla
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET); // Varsayılan olarak HIGH (1)
    }
    */
}

void ADXL345_Write(uint8_t wRegister,uint8_t value)
{
	uint8_t data[2] = {0};  // Register ve value degerlerini diziye atayıp master_transmit ile bu diziyi gonderelim.

	data[0]=wRegister;
	data[1]=value;

	HAL_I2C_Master_Transmit(&hi2c1, ADXL345_I2C_SLAVE_ADDRESS, data, 2, 100);

}

void ADXL345_Init()
{
	SCAN_I2C_Address();

	ADXL345_Read(0x00,1); // Eger okudugumuz data 0xE5 ise okuma basarili (cihaz hazir)

	ADXL345_Write(0x2D,0); // Kontrol bitlerinin tamamini sifirla.

	ADXL345_Write(0x2D,0x08); // Measure bit 1, wake up bit 0,autosleep 0,sleep 0 and wakup frequency 8hz;

	ADXL345_Write(0x31,0x01); // +- 4g range


}

void ADXL345_Start()
{

	 ADXL345_Read(0x32,6);
	 x = ((myDatas[1] << 8) | myDatas[0]);
	 HAL_Delay(10);
	 y = ((myDatas[3] << 8) | myDatas[2]);
	 HAL_Delay(10);
	 z = ((myDatas[5] << 8) | myDatas[4]);
	 xG= x*.0087;
	 yG= y*.0087;
	 zG= z*.0087;





}

