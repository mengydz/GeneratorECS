/*
 * inner_flash.h
 *
 *  Created on: 2020年10月17日
 *      Author: 21954
 */

#ifndef INNER_FLASH_H_
#define INNER_FLASH_H_

/**************************************************************************
  * @brief     : 内部flash头文件，
  * @author    : 陈梦洋(mengyang.chen@leadchina.cn)
  * @copyright : LeadChina
  * @version   : V1.0
  * @note      : 注意事项
  * @history   : 历史记录
***************************************************************************/
#ifndef __FLASH_INTERNAL_H
#define __FLASH_INTERNAL_H
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

extern uint8_t ParameterDataRAMMap[1024];

typedef struct{
	int32_t (*start_transaction)(void);
	int32_t (*end_transaction)(void);
	int32_t (*erase_chip)(void);
	int32_t (*erase_sector)(uint32_t addr);
	int32_t (*write_data)(uint32_t addr, uint8_t *data, uint16_t len);
	int32_t (*rewrite_data)(uint32_t addr, uint8_t *data, uint16_t len);
	int32_t (*read_data)(uint32_t addr, uint8_t *data, uint16_t len);
}FlashDriver;

typedef struct{
	uint32_t checksum;
	uint32_t flashMagic;
	uint32_t pwm_in_mode;
	uint32_t pwm_in_throttle;
}__attribute__((packed)) GeneratorSystemParaSaveDataStruct;

#define	FlashInterDataAddrBase					(uint32_t)0x08004000
#define FlashInterUserDataAddrBase				FlashInterDataAddrBase
#define GeneratorSystemParaMemSize				sizeof(GeneratorSystemParaSaveDataStruct)

#define InternFlashAddrMap(x)	(x - FlashInterDataAddrBase)
#define GetFlashMapAddr(x)	    ((uint32_t)(ParameterDataRAMMap) + InternFlashAddrMap(x))

/*******************************************************************************
* 函数名 : FlashDriver * GetFlashInternalDriver(void)
* 作  者 :
* 描  述 : 获取内部flash函数指针
* 输  入 : 无
* 输  出 : 无
* 返回值 : 无
* 说  明 :
*******************************************************************************/
FlashDriver * GetFlashInternalDriver(void);
bool SystemParameterRAMMapInit(void);
void ClearSystemParameter(void);
bool CheckSystemParameterSaveData(void);
void EncoderLinearCorrectSaveData(uint32_t _pwm_in_mode,uint32_t _pwm_in_throttle);

#endif




#endif /* INNER_FLASH_H_ */
