#include "i2c_eeprom.h"
#include "stm32f1xx.h"

extern I2C_HandleTypeDef I2C_EEPROM_HANDLE;

void eeprom_write(uint16_t address, uint8_t *buf, uint16_t len) {
	HAL_I2C_Mem_Write(&I2C_EEPROM_HANDLE, I2C_EEPROM_ADDRESS, address, I2C_MEMADD_SIZE_16BIT, buf, len, I2C_EEPROM_TIMEOUT);
}

void eeprom_address_read(uint8_t *buf, uint8_t len) {
	HAL_I2C_Master_Receive(&I2C_EEPROM_HANDLE, I2C_EEPROM_ADDRESS, buf, len, I2C_EEPROM_TIMEOUT);
}

void eeprom_random_read(uint16_t address, uint8_t *buf, uint16_t len) {
	HAL_I2C_Mem_Read(&I2C_EEPROM_HANDLE, I2C_EEPROM_ADDRESS, address, I2C_MEMADD_SIZE_16BIT, buf, len, I2C_EEPROM_TIMEOUT);
}
