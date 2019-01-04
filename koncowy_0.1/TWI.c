#include "TWI.h"

void ERROR(uint8_t flag)
{
  while(1)
  {
    /*
    Serial.print("BLAD");
    Serial.print(flag);
    Serial.print("\n");
    Serial.println(TWIGetStatus(),HEX);*/
  }
}

void TWIInit(void)
{
  //set SCL to 400kHz
  TWSR = 0x01;
  TWBR = 0xFF;
  //enable TWI
 TWCR = (1<<TWEN);
}
void TWIStart(void)
{
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
  while ((TWCR & (1<<TWINT)) == 0);

}
//send stop signal
void TWIStop(void)
{
  TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void TWIWrite(uint8_t u8data)
{
  TWDR = u8data;
  TWCR = (1<<TWINT)|(1<<TWEN);
  while (!(TWCR &(1<<TWINT)));  
}

uint8_t TWIReadACK(void)
{
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
  while ((TWCR & (1<<TWINT)) == 0);
  return TWDR;
}
//read byte with NACK
uint8_t TWIReadNACK(void)
{
  TWCR = (1<<TWINT)|(1<<TWEN);
  while ((TWCR & (1<<TWINT)) == 0);
  return TWDR;
}

uint8_t TWIGetStatus(void)
{
  uint8_t status;
  //mask status
  status = TWSR & 0xF8;
  return status;
}
