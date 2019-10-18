/*!
 * *******************************************************************************
 * @file Max31855.h
 * @author julian bustamante
 * @version 1.0.2
 * @date Oct 17, 2019
 * @brief Library to read temperature from a MAX31855K type K thermocouple
 * 
 *********************************************************************************/

#ifndef _MAX31855_H_
#define _MAX31855_H_

#include <stdint.h>

#define MAX31855_TRUE 1u
#define MAX31855_FALSE 0u
#define MAX_NUM_BYTES_EXCHANGE 4   /*Register 32 bits*/
#define MAX31855_VERSION     "1.0.2"
#define MAX31855_CAPTION     "MAX31855 " MAX31855_VERSION
/* #define MAX31855_DATE_COMPILER       __DATE__ "-" __TIME__ */


/*@brief : pointer to function for read 8 bits of shif register Spi*/
typedef uint8_t (*SPI_ExchangeByte_t)(uint8_t);

/*@brief : pointer to function chip select or slave select Spi*/
typedef void (*SPI_CS_SS_t)(uint8_t);   

/*@brief struct containing all data*/
typedef struct {
    union{
        uint32_t TempDataRegFull;       /*!< Register 4 bytes Spi*/
        struct {
           unsigned Fault_OC        :1; /*!< This bit is a 1 when the thermocouple is open(no connections). Default value is 0.*/
           unsigned Fault_SCG       :1; /*!< This bit is a 1 when the thermocouple is short-circuited to GND. Default value is 0.*/
           unsigned Fault_SCV       :1; /*!< This bit is a 1 when the thermocouple is short-circuited to VCC. Default value is 0.*/
           unsigned Reserved_1      :1; /*!< This bit always reads 0.*/
           unsigned TempInt_Raw     :8;/*!< 12 These bits contain the signed 12-bit value of the reference junction temperature*/
           unsigned Fault           :1; /*!< This bit reads at 1 when any of the SCV, SCG, or OC faults are active. Default values 0*/
           unsigned Reserved_2      :1; /*!< This bit always reads 0.*/
           unsigned TempExt_Raw     :8;/*!< 14These bits contain the signed 14-bit thermocouple temperature value*/
        };
        
    };
    SPI_ExchangeByte_t SPI_RW_Byte; /*!< Function for read 8 bits of shif register Spi*/
    SPI_CS_SS_t SPI_CS_SS;          /*!< Function chip selector or slave selector spi*/
}Max31855_Data_t;


/*
*@brief Function for initialize driver
*@param param1 : Data struct containing all data of driver
*@param param2 : pointer to function SPI_ExchangeBytes of shif register SPI peripheral
*@param param3 : pointer to function SPI chip selector or slave selector
*
*@return void
*/

void MAX31855_Init(Max31855_Data_t *obj, SPI_ExchangeByte_t SPI_TransferByte, SPI_CS_SS_t SPI_cs_ss);
/*
*@brief Function static for Read data of spi
*@param param1 : Data struct containing all data of driver
*@return void
*/
void MAX31855_ReadData(Max31855_Data_t *obj);

/*
*@brief Function for Read data  temperature external - Thermocouple temperature
*@param param1 : Data struct containing all data of driver
*@return Thermocouple temperature in °C  0.25°C to 1024°C
*/
int16_t MAX31855_ReadExtTemp(Max31855_Data_t *obj);


/*
*@brief Function for Read data  temperature internal- reference juntion temperature 
*@param param1 : Data struct containing all data of driver
*@return reference juntion temperature in °C 0.0625°C to 64°C
*/
int16_t MAX31855_ReadIntTemp(Max31855_Data_t *obj);


/*
@see https://datasheets.maximintegrated.com/en/ds/MAX31855.pdf
The practical temperature ranges vary with the thermocouple type.
TEMPERATURE                                 TEMPERATURE
(°C)                                           (°C)
DIGITAL OUTPUT                            DIGITAL OUTPUT
(D[31:18])                                       (D[15:4])
+1600.00    0110 0100 0000 00       +127.0000    0111 1111 0000
+1000.00    0011 1110 1000 00       +100.5625    0110 0100 1001
+100.75     0000 0110 0100 11       +25.0000     0001 1001 0000
+25.00      0000 0001 1001 00        0.0000      0000 0000 0000
0.00        0000 0000 0000 00       -0.0625      1111 1111 1111
-0.25       1111 1111 1111 11       -1.0000      1111 1111 0000
-1.00       1111 1111 1111 00       -20.0000     1110 1100 0000
-250.00     1111 0000 0110 00       -55.0000     1100 1001 0000










-55.0000 1100 1001 0000

*/

#endif