/*
 * I2C.h
 *
 *  Created on: 21 oct. 2023
 *      Author: ihate
 */

#ifndef I2C_H_
#define I2C_H_

/*Defines*/
#define SCD_IE 0x20
#define XRDY_IE 0x10
#define RRDY_IE 0x08
#define ARDY_IE 0x04
#define NACK_IE 0x02
#define OWN_ADDRESS 0x30U
#define SLAVE_ADDRESS 0x27U//0x01001110
/*Structures*/
/*Function prototypes*/
void I2C_vI2CInit(Uint16 u16OwnAddress, Uint16 u16SlaveAddress, Uint16 u16ByteCount);
void I2C_vI2CWrite(Uint16 u16SlaveAddress, Uint16 u16ByteCount, Uint16 *u16Data, _Bool bSendStopCondition);
/*Variables*/



#endif /* I2C_H_ */
