#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include <inttypes.h>
//32 pages, 8 bytes each

#define EEPROM_HANDLE hi2c1
#define EEPROM_ADDRESS 0xA0

#define EEPROM_PAGE_SIZE 64     // in Bytes
#define EEPROM_PAGE_NUM  512    // number of pages

//will roll over onto same page
void Eeprom_Write(uint16_t address, uint8_t *buf, uint16_t len);

//current address looped through entire memory
void Eeprom_ReadNext(uint8_t *buf, uint8_t len);

void Eeprom_Read(uint16_t address, uint8_t *buf, uint16_t len);

#endif
