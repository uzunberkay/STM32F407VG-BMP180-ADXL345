/*
 * ADXL345.h
 *
 *  Created on: Nov 1, 2023
 *      Author: berky
 */

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

#include "stm32f4xx_hal.h"


extern I2C_HandleTypeDef hi2c1;

/* ADXL345 Slave Address */
#define ADXL345_I2C_SLAVE_ADDRESS 0xA6





/* ADXL Function */
void SCAN_I2C_Address(void);
void ADXL345_Read(uint8_t rRegister,uint8_t numberOfByte);
void ADXL345_Write(uint8_t wRegister,uint8_t value);
void ADXL345_Init(void);
void ADXL345_Start(void);






#endif /* INC_ADXL345_H_ */
