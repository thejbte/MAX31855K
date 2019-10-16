#include "Max31855.h"

/**/

/*
*@brief Function for initialize driver
*@param param1 : Data struct containing all data of driver
*@param param2 : pointer to function SPI_ExchangeBytes of shif register SPI peripheral
*@param param3 : pointer to function SPI chip selector or slave selector
*
*@return void
*/
void MAX31855_Init(Max31855_Data_t *obj, SPI_ExchangeByte_t SPI_TransferByte, SPI_CS_SS_t SPI_cs_ss){
    obj->SPI_RW_Byte = SPI_TransferByte;
    obj->SPI_CS_SS = SPI_cs_ss;
}


/*
*@brief Function static for Read data of spi
*@param param1 : Data struct containing all data of driver
*@return void
*/
void MAX31855_ReadData(Max31855_Data_t *obj){
  uint8_t DataW = 0xff;  /*Device is only read*/
  uint8_t spival[4];
  int8_t i;

  obj->TempDataRegFull =0x0000;
  obj->SPI_CS_SS(MAX31855_FALSE); /*CS=0 */
  
  for(i = MAX_NUM_BYTES_EXCHANGE - 1  ; i >= 0 ; i--)
    spival[i] = obj->SPI_RW_Byte(DataW); /*Return byte of shif register SPI*/
  
  obj->SPI_CS_SS(MAX31855_TRUE);  /*CS=1*/
  
  for(i = 0 ; i < MAX_NUM_BYTES_EXCHANGE ; i++)
    obj->TempDataRegFull |= spival[i] << (i*8) ;  
}

/*
*@brief This Function for reads data  temperature external - Thermocouple temperature
*@param param1 : Data struct containing all data of driver
*@return Thermocouple temperature in °C  0.25°C to 1024°C
*/
int16_t MAX31855_ReadExtTemp(Max31855_Data_t *obj){
  int16_t value;
  
  MAX31855_ReadData(obj);
  
    /* Bits D[31:18] are the signed 14-bit thermocouple temperature value*/
  if (obj->TempDataRegFull & ((uint32_t)1 << 31)) { /* Sign extend negative numbers*/
      value = ((obj->TempDataRegFull >> 18) & 0x3FFF); 
      value = ( (~ (value ) & 0x3fff ) + 0x01 ) / (-4);     /*Complemento a 2, niego y sumo 1*/ 
  } else {
      value = (obj->TempDataRegFull >> 18)  & 0x3fff; /* Shift off all but the temperature data*/
      value /= 4 ;
    }
  
  return (value); /*Data>>2*/
}

/*
*@brief This function reads the cold junction temperature
*@param param1 : Data struct containing all data of driver
*@return reference juntion temperature in °C 0.0625°C to 64°C
*/
int16_t MAX31855_ReadIntTemp(Max31855_Data_t *obj){
  int16_t value;
  
  MAX31855_ReadData(obj);
  
  /* Bits D[31:18] are the signed 14-bit thermocouple temperature value*/
  if (obj->TempDataRegFull & ((uint32_t)1 << 15)) { /* Sign extend negative numbers*/
      value = ((obj->TempDataRegFull >> 4) & 0xFFF); 
      value = ( (~ (value ) & 0xfff ) + 0x01 ) / (-16);     /*Complemento a 2   - 2^-4*/ 
  } else {
      value = (obj->TempDataRegFull >> 4) & 0xfff; /* Shift off all but the temperature data*/
      value /= 16 ;
    }
  
  return (value); /*Data>>2*/
}
