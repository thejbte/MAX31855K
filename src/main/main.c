#include "Max31855.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "unity_internals.h"


Max31855_Data_t Max31855_Temp_Data;
uint8_t Vector[4] ={0x40,0x19,0x50,0x02};

uint8_t Wrapper_Exchange8Bits(uint8_t data){
    /*uint8_t Vector[4] ={0x40,0x19,0x50,0x02};*/  /*37 Y 25 °C */
    /*uint8_t Vector[4] ={0x00,0xc9,0x60,0xf0};*/ /*  -250  Y -55°C*/
    /*uint8_t Vector[4] ={0x00,0x7f,0x00,0x64}; */ /*1600 Y 127 °C*/

    static  int cont;
    uint8_t ret=0;
      
    if(cont == 0) ret = Vector[3];
    if(cont == 1) ret = Vector[2];
    if(cont == 2) ret = Vector[1];
    if(cont == 3) ret = Vector[0];
    if(cont++ >= 3) cont=0;

    return ret;
}

void Wrapper_Pin_cs(uint8_t value){
    /*if(value) GPIO_PinCS=1;   
     else if(!value) GPIO_PinCS=0;
    */
}

int16_t PasarDato32_bits_MAX31855_ReadExtTemp(uint32_t Data){
    memcpy(Vector,&Data,sizeof(Vector));
    return  MAX31855_ReadExtTemp(&Max31855_Temp_Data);   
}

int16_t PasarDato32_bits_MAX31855_ReadIntTemp(uint32_t Data){
    memcpy(Vector,&Data,sizeof(Vector));
    return  MAX31855_ReadIntTemp(&Max31855_Temp_Data);   
}
/*Wrapper Test assert unity   0xF060c900 = -250 y -55    0x40,0x19,0x58,0x02  02581940*/
void test_lala(void){

 TEST_ASSERT_EQUAL(25,MAX31855_ReadExtTemp(&Max31855_Temp_Data));  /*MAX31855_ReadExtTemp(&Max31855_Temp_Data));*/
}

int main(int argc, char** argv){

MAX31855_Init(&Max31855_Temp_Data,Wrapper_Exchange8Bits, Wrapper_Pin_cs);


/*printf("Temperature  %d %d",MAX31855_ReadExtTemp(&Max31855_Temp_Data),MAX31855_ReadIntTemp(&Max31855_Temp_Data));*/
printf("Temp %d %d",PasarDato32_bits_MAX31855_ReadExtTemp(0x02501940),PasarDato32_bits_MAX31855_ReadIntTemp(0x02501940));

/*UNITY_BEGIN();*/
/*RUN_TEST(test_lala);*/
/*UNITY_END();*/
return 0;

}