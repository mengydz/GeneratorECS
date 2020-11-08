/*
 * inner_flash.c
 *
 *  Created on: 2020年10月17日
 *      Author: 21954
 */

/**************************************************************************
  * @brief     : 实现对内部flash的读写操作，带互斥锁
  * @author    : 陈梦洋(mengyang.chen@leadchina.cn)
  * @copyright : LeadChina
  * @version   : V1.0
  * @note      : 注意事项
  * @history   : 历史记录
***************************************************************************/
#include "flash_internal.h"

struct device_flash_sector {
    uint32_t start;
    uint32_t size;
    uint16_t st_sector;
};

#define NELEMENTS(x) (sizeof(x) / sizeof(*(x)))

uint8_t ParameterDataRAMMap[1024];  //普通RAM

static struct device_flash_sector flash_sectors[] = {//STM32F767IGT6共12个扇区为1M Flash
    [0] =  {
        .start     = 0x08000000,
        .size      = 16 * 1024,
        .st_sector = FLASH_SECTOR_0,		//start addr :0k
    },
    [1] =  {
        .start     = 0x08004000,
        .size      = 16 * 1024,
        .st_sector = FLASH_SECTOR_1,		//start addr :16k
    },
    [2] =  {
        .start     = 0x08008000,
        .size      = 16 * 1024,
        .st_sector = FLASH_SECTOR_2,		//start addr :32k
    },
    [3] =  {
        .start     = 0x0800C000,
        .size      = 16 * 1024,
        .st_sector = FLASH_SECTOR_3,		//start addr :48k
    },
    [4] =  {
        .start     = 0x08010000,
        .size      = 64 * 1024,
        .st_sector = FLASH_SECTOR_4,		//start addr :64k
    },
    [5] =  {
        .start     = 0x08020000,
        .size      = 128 * 1024,
        .st_sector = FLASH_SECTOR_5,		//start addr :128k
    },
    [6] =  {
        .start     = 0x08040000,
        .size      = 128 * 1024,
        .st_sector = FLASH_SECTOR_6,		//start addr :256k
    },
    [7] =  {
        .start     = 0x08060000,
        .size      = 128 * 1024,
        .st_sector = FLASH_SECTOR_7,		//start addr :384k
    },
    [8] =  {
        .start     = 0x08080000,
        .size      = 128 * 1024,
        .st_sector = FLASH_SECTOR_8,		//start addr :512k
    },
    [9] =  {
        .start     = 0x080A0000,
        .size      = 128 * 1024,
        .st_sector = FLASH_SECTOR_9,		//start addr :640k
    },
    [10] = {
        .start     = 0x080C0000,
        .size      = 128 * 1024,
        .st_sector = FLASH_SECTOR_10,		//start addr :768k
    },
    [11] = {
        .start     = 0x080E0000,
        .size      = 128 * 1024,
        .st_sector = FLASH_SECTOR_11,		//start addr :896k
    },
};
/*******************************************************************************
* 函数名 : InternalFlashGetSectorInfo(uint32_t address, uint8_t *sector_number, uint32_t *sector_start, uint32_t *sector_size)
* 描  述 : 获取当前地址在哪个sector，以及sector起始地址及sector的大小
* 输  入 : address：当前地址，sector_number：当前sector号，sector_start：sector起始地址，sector_size：sector的大小
* 返回值 : 无
* 说  明 : 无
*******************************************************************************/
static bool InternalFlashGetSectorInfo(uint32_t address, uint8_t *sector_number, uint32_t *sector_start, uint32_t *sector_size)
{
    for (uint8_t i = 0; i < NELEMENTS(flash_sectors); i++) {
        struct device_flash_sector *sector = &flash_sectors[i];
        if ((address >= sector->start) &&
            (address < (sector->start + sector->size))) {
            /* address lies within this sector */
            *sector_number = sector->st_sector;
            *sector_start  = sector->start;
            *sector_size   = sector->size;
            return true;
        }
    }

    return false;
}

/*******************************************************************************
* 函数名 : FlashInternalStartTransaction(uint32_t flash_id)
* 描  述 : 获取互斥信号量，直行UNLOCK函数
* 输  入 : ID号
* 返回值 : 返回0则成功，
* 说  明 : 无
*******************************************************************************/
static int32_t FlashInternalStartTransaction(void)
{
    HAL_FLASH_Unlock();
	return 0;
}

/*******************************************************************************
* 函数名 : FlashInternalEndTransaction(uint32_t flash_id)
* 描  述 : 释放信号量，并直行LOCK函数
* 输  入 : ID号
* 返回值 : 返回0则成功，
* 说  明 : 无
*******************************************************************************/
static int32_t FlashInternalEndTransaction(void)
{
    HAL_FLASH_Lock();
	return 0;
}

/*******************************************************************************
* 函数名 : FlashInternalEraseSector(uint32_t flash_id)
* 描  述 : 擦除函数
* 输  入 : ID号
* 返回值 : 无
* 说  明 : 无
*******************************************************************************/
static int32_t FlashInternalEraseSector(uint32_t addr)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    uint32_t SectorError=0;

    uint8_t sector_number;
    uint32_t sector_start;
    uint32_t sector_size;
    if (!InternalFlashGetSectorInfo(addr,
								   &sector_number,
								   &sector_start,
								   &sector_size)) {
        /* We're asking for an invalid flash address */
        return -2;
    }

	FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
	FlashEraseInit.Sector=sector_number;   //要擦除的扇区
	FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
	FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
	if(HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError)!=HAL_OK)
	{
		return -1;;//发生错误了
	}
	//SCB_CleanInvalidateDCache();//清除无效的D-Cache
	return 0;
}
/*******************************************************************************
* 函数名 : FlashInternalWriteData(uint32_t flash_id)
* 描  述 : 内部flash写函数
* 输  入 : flash_id：ID号，addr：地址，data：数据，len：长度
* 返回值 : 无
* 说  明 : 无
*******************************************************************************/
static int32_t FlashInternalWriteData(uint32_t addr, uint8_t *data, uint16_t len)
{
    uint8_t sector_number;
    uint32_t sector_start;
    uint32_t sector_size;

    /* Ensure that the base address is in a valid sector */
    if (!InternalFlashGetSectorInfo(addr,
								   &sector_number,
								   &sector_start,
								   &sector_size)) {
        /* We're asking for an invalid flash address */
        return -2;
    }

    /* Ensure that the entire write occurs within the same sector */
    if ((uintptr_t)addr + len > sector_start + sector_size) {
        /* Write crosses the end of the sector */
        return -3;
    }
    for (uint16_t i = 0; i < len / 4; i++) {
        uint32_t data_word = *(uint32_t *)(data + i * 4);
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,addr + i * 4, data_word) != HAL_OK)
        {
            return -4;
        }
    }

    /* Write the data */
    for (uint16_t i = len - len % 4; i < len; i++) {
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,addr + i, data[i]) != HAL_OK)
        {
            return -5;
        }
    }
	return 0;
}

/*******************************************************************************
* 函数名 : FlashInternalReadData(uint32_t flash_id,uint32_t addr,uint8_t *data, uint16_t len)
* 描  述 : 内部flash都函数
* 输  入 : flash_id：ID号，addr：地址，data：数据，len：长度
* 返回值 : 无
* 说  明 : 无
*******************************************************************************/
static int32_t FlashInternalReadData(uint32_t addr,uint8_t *data, uint16_t len)
{
    uint8_t sector_number;
    uint32_t sector_start;
    uint32_t sector_size;

    /* Ensure that the base address is in a valid sector */
    if (!InternalFlashGetSectorInfo(addr,
								   &sector_number,
								   &sector_start,
								   &sector_size)) {
        /* We're asking for an invalid flash address */
        return -2;
    }

    /* Ensure that the entire read occurs within the same sector */
    if ((uintptr_t)addr + len > sector_start + sector_size) {
        /* Read crosses the end of the sector */
        return -3;
    }

    /* Read the data into the buffer directly */
    memcpy(data, (void *)addr, len);

	return 0;
}

#define FLASH_SYSTEM_PARAMETER_MAGIC			0x50415241 //"PARA"
void GetParaSaveData(uint32_t FlashAddr,uint32_t ramAddr)
{
	for(uint16_t i = 0;i < sizeof(GeneratorSystemParaSaveDataStruct);i++)
	{
		*((uint8_t *)ramAddr + i) = *((uint8_t *)FlashAddr + i);
	}
}

bool CheckOrignDatProcess(GeneratorSystemParaSaveDataStruct *FlashDat)
{
	volatile uint32_t checksum = 0,*p;

	if(FlashDat ->flashMagic != FLASH_SYSTEM_PARAMETER_MAGIC)
	{
		return false;
	}
	checksum += FLASH_SYSTEM_PARAMETER_MAGIC;
	p = (uint32_t *)&FlashDat->pwm_in_mode;
	checksum += *p;
	p = (uint32_t *)&FlashDat->pwm_in_throttle;
	checksum += *p;

	if(checksum == FlashDat->checksum)
	{
		return true;
	}
	return false;
}

bool CheckSystemParameterSaveData(void)
{
	GeneratorSystemParaSaveDataStruct *FlashDat = (GeneratorSystemParaSaveDataStruct *)FlashInterUserDataAddrBase;
	if(CheckOrignDatProcess(FlashDat))
	{
		return true;
	}
	return false;
}

bool SystemParameterRAMMapInit(void)
{
	if(CheckSystemParameterSaveData() != false) //里面有数据然后读取里面数据
	{
		GetParaSaveData(FlashInterUserDataAddrBase,GetFlashMapAddr(FlashInterUserDataAddrBase));
		return true;
	}
	return false;
}

void SystemParaSave2Flash(void)
{
	uint32_t addr;

	FlashDriver *flashDriver;

	flashDriver = GetFlashInternalDriver();
//	IRQ_Disable();
	addr = FlashInterUserDataAddrBase;
	while(flashDriver->start_transaction() < 0);
	flashDriver->erase_sector(addr);
	flashDriver->write_data(addr,(uint8_t *)&ParameterDataRAMMap,sizeof(ParameterDataRAMMap));
	while(flashDriver->end_transaction() < 0);
//	IRQ_Enable();
}

void ClearSystemParameter(void)
{
	GeneratorSystemParaSaveDataStruct *saveDat =  (GeneratorSystemParaSaveDataStruct *)GetFlashMapAddr(FlashInterUserDataAddrBase);
	saveDat->checksum = 0;
	saveDat->flashMagic = 0;
    SystemParaSave2Flash();
}

void EncoderLinearCorrectSaveData(uint32_t _pwm_in_mode,uint32_t _pwm_in_throttle)
{
	GeneratorSystemParaSaveDataStruct *_system_parameter;
	_system_parameter = (GeneratorSystemParaSaveDataStruct *)GetFlashMapAddr(FlashInterUserDataAddrBase);

	_system_parameter->checksum = 0;

	_system_parameter->flashMagic = FLASH_SYSTEM_PARAMETER_MAGIC;
	_system_parameter->checksum += FLASH_SYSTEM_PARAMETER_MAGIC;
    _system_parameter->pwm_in_mode = _pwm_in_mode;
    _system_parameter->checksum += _pwm_in_mode;
    _system_parameter->pwm_in_throttle = _pwm_in_throttle;
    _system_parameter->checksum += _pwm_in_throttle;
	SystemParaSave2Flash();
}

FlashDriver flash_driver = {
	.start_transaction = FlashInternalStartTransaction,
    .end_transaction   = FlashInternalEndTransaction,
    .erase_sector 	   = FlashInternalEraseSector,
    .write_data   	   = FlashInternalWriteData,
    .read_data    	   = FlashInternalReadData,

};

/*******************************************************************************
* 函数名 : GetFlashInternalDriver(void)
* 描  述 : 获取FLASH驱动
* 输  入 : 无
* 返回值 : 内部FALSH驱动
* 说  明 : 无
*******************************************************************************/
FlashDriver * GetFlashInternalDriver(void)
{
	return &flash_driver;
}


