/*
 * SYSPARAM.h
 *
 *  Created on: 20 oct. 2023
 *      Author: Paul
 */

#ifndef SYSPARAM_H_
#define SYSPARAM_H_

/*Defines*/
#define MS_TO_NS 1000000UL
#define SYS_CLK_HZ 100000000UL
#define SYS_PERIOD_NS 10
#define ENABLE 1
#define DISABLE 0
#define UINT32MAX 4294967295
#define BIT12_MAX_VALUE_INT 4095UL
#define BIT12_MAX_VALUE_FLOAT 4095.0
#define UINT16_TO_FLOAT_SLOPE  0.0004884
#define UINT16_TO_FLOAT_OFFSET -1.0
#define FLOAT_TO_UINT16_SLOPE 2047.5
#define FLOAT_TO_UINT16_OFFSET 2047.5

#define DEBUG

//#define SOFTWARE_FILTERS

#define DEBUG_PINS 3
#define DEBUG_PIN1 7
#define DEBUG_PIN2 8
#define DEBUG_PIN3 9

#endif /* SYSPARAM_H_ */
