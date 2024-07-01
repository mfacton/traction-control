#include "eeprom.h"
#include "stm32f1xx.h"

extern I2C_HandleTypeDef EEPROM_HANDLE;

void Eeprom_Write(uint16_t address, uint8_t *buf, uint16_t len) {
	HAL_I2C_Mem_Write(&EEPROM_HANDLE, EEPROM_ADDRESS, address, I2C_MEMADD_SIZE_16BIT, buf, len, HAL_MAX_DELAY);
}

void Eeprom_ReadNext(uint8_t *buf, uint8_t len) {
	HAL_I2C_Master_Receive(&EEPROM_HANDLE, EEPROM_ADDRESS, buf, len, HAL_MAX_DELAY);
}

void Eeprom_Read(uint16_t address, uint8_t *buf, uint16_t len) {
	HAL_I2C_Mem_Read(&EEPROM_HANDLE, EEPROM_ADDRESS, address, I2C_MEMADD_SIZE_16BIT, buf, len, HAL_MAX_DELAY);
}
