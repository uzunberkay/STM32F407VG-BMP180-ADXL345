/*
 * BMP180.h
 *
 *  Created on: Nov 3, 2023
 *      Author: berky
 */

#ifndef INC_BMP180_H_
#define INC_BMP180_H_


#include "stm32f4xx_hal.h"
#include "math.h"
 /* 			 */
extern I2C_HandleTypeDef hi2c1;
#define BMP180_I2C &hi2c1

/* BMP180 Address   */
#define BMP180_ADDRESS 0xEE


#define atmPress 101325

/* Function */
void BMP180_Init(void);

void BMP180_Start (void);

float BMP180_GetTemp (void);

float BMP180_GetPress (int oss);

float BMP180_GetAlt (int oss);





#endif /* INC_BMP180_H_ */
