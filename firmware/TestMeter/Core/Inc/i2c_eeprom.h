#ifndef INC_I2C_EEPROM_H_
#define INC_I2C_EEPROM_H_

#include <inttypes.h>
//32 pages, 8 bytes each

#define I2C_EEPROM_HANDLE hi2c1
#define I2C_EEPROM_ADDRESS 0xA0
#define I2C_EEPROM_TIMEOUT 100

#define PAGE_SIZE 64     // in Bytes
#define PAGE_NUM  512    // number of pages

//will roll over onto same page
void eeprom_write(uint16_t address, uint8_t *buf, uint16_t len);

//current address looped through entire memory
void eeprom_address_read(uint8_t *buf, uint8_t len);

void eeprom_random_read(uint16_t address, uint8_t *buf, uint16_t len);

#endif
