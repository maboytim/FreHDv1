
#define PROTEUS_SIMULATOR	0

#if PROTEUS_SIMULATOR == 1
#define EEPROM_CRC_ADDR		0xFE
#else
#define EEPROM_CRC_ADDR		0x3FE
#endif

#define FLASH_CRC_ADDR		0x1FFFE

#define BOOTLOADER_MAJOR	0x1
#define BOOTLOADER_MINOR	0x2

#define BOOTLOADER_END		0xFFF
#define APP_STARTUP		0x1000
//#define APP_LO_INT		0x1006
//#define APP_HI_INT		0x100C
//#define APP_HI_INT2		0x1100
