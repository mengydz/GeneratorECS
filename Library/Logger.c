#include <string.h>
#include "Logger.h"
#include "fatfs.h"
#include "math.h"
#include <stdio.h>
#if defined(USE_RTTHREAD)
#include <entry.h>
#endif

#define LOG_FILE_NAME "log.bin"

FATFS fs;
FATFS *pfs;
FIL fil;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
FRESULT fres;
UINT br, bw;
uint16_t filemax=1,filemin =1;

const struct LogStructure log_structure[] = {
  {
    LOG_FORMAT_MSG, sizeof(struct log_Format),
    "FMT", "BBnNZ",       "Type,Length,Name,Format,Columns"
  },
  {
    LOG_TEST_MSG, sizeof(struct log_TEST),
    "TEST", "QH",         "TimeUS,value"
  },
  { LOG_ECU_MSG, sizeof(struct log_ECU), 
    "ECU", "QffffffHHHHHHH",   "TimeUS,VS,V,CS,C,CC,temp,ma,mi,ou,im,it,sd,ms"
  }, 
  { LOG_PWM_MSG, sizeof(struct log_PWM), 
    "PWM", "Qhhhh",   "TimeUS,m1,m2,m3,m4"
  },
};

uint8_t Write_Format(const struct LogStructure *s);
	
uint8_t WriteBlock(const void *pBuffer, uint16_t size)
{
  /* Check freeSpace space */
  if(f_getfree("", &fre_clust, &pfs) != FR_OK)
    return 1;

  totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
  freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);

  /* free space is less than 1kb */
  if(freeSpace < 1)
    return 2;

  /* Writing*/
  f_write (&fil, pBuffer, size, &bw);
  f_sync(&fil);

  return 0;
}

void Fill_Format(const struct LogStructure *s, struct log_Format *pkt)
{
  memset(pkt, 0, sizeof(*pkt));
  pkt->head1 = HEAD_BYTE1;
  pkt->head2 = HEAD_BYTE2;
  pkt->msgid = LOG_FORMAT_MSG;
  pkt->type = s->msg_type;
  pkt->length = s->msg_len;
  strncpy(pkt->name, s->name, sizeof(pkt->name));
  strncpy(pkt->format, s->format, sizeof(pkt->format));
  strncpy(pkt->labels, s->labels, sizeof(pkt->labels));
}

uint16_t transfilename(TCHAR *name)
{
    uint8_t filename[14];
    uint8_t num =0;
    uint16_t fileindex =0;
    for(int i = 0; i<14; i++)//数组定长14
    {
        if(*(name+i) > 46)
        {
          filename[i]= *(name+i);
        }
        else
        {
            num = i-1;
            break;
        }
    }
    for(int i = num; i>=0; i--)
    {
        fileindex+=(filename[i]-48)*powf(10,num-i);
    }
    return fileindex;
}

FRESULT scan_files(char *path)
{
    FRESULT res;
    DIR dir;
    static FILINFO fileinfo;
    uint16_t fileindex  = 0;
    res = f_opendir(&dir,path);
    if(res == FR_OK)
    {
        while(1)
        {
            res = f_readdir(&dir,&fileinfo);
            if(res !=FR_OK || fileinfo.fname[0] ==0)//读取失败，或者读取结束
                break;
            if(fileinfo.fattrib & AM_ARC)//是文件
            {
               fileindex = transfilename(fileinfo.fname);    //转为十进制
               if(fileindex >= filemax)
               {
                   filemax = fileindex;
               }
               else if(fileindex <= filemin)
               {
                   filemin = fileindex;
               }
            }
        }
    }
    else
    {
        return res;
    }
    f_closedir(&dir);
    return res;
}

void Log_Init(void)
{
  uint8_t i;
 
  if(f_mount(&fs, "", 0) != FR_OK)
    return;

  if(scan_files("") != FR_OK)//查找排序
    return;
  TCHAR log_name_str[14];
  filemax++;
  sprintf((char*)log_name_str,"%04d.bin",filemax);

  f_open(&fil, log_name_str, FA_CREATE_ALWAYS | FA_WRITE); 
  
  for(i = 0; i < ARRAY_SIZE(log_structure); i++) 
  {
	Write_Format(&log_structure[i]);
	HAL_Delay(100);
  }
}

/*
  write a structure format to the log
 */
uint8_t Write_Format(const struct LogStructure *s)
{
  struct log_Format pkt;

  Fill_Format(s, &pkt);

  return WriteBlock(&pkt, sizeof(pkt));
}

void Write_Test(uint64_t time_us, uint16_t value)
{
  struct log_TEST pkt = {
    LOG_PACKET_HEADER_INIT(LOG_TEST_MSG),
    time_us,
    value,
  };
  WriteBlock(&pkt, sizeof(pkt));
}

void Write_ECU(uint64_t time_us, const ECU_Info *info)
{
  struct log_ECU pkt = {
	LOG_PACKET_HEADER_INIT(LOG_ECU_MSG),
	time_us,
	info->voltage_set,
	info->voltage,
	info->current_set,
	info->current_used,
	info->current_charge,
	info->temperature,
	info->pwm_max_set,
	info->pwm_min_set,
	info->pwm_out_throttle,
	info->pwm_in_mode,
	info->pwm_in_throttle,
	info->servo_direction,
	info->motor_speed,
  };
  WriteBlock(&pkt, sizeof(pkt));
}

void Write_PWM(int16_t pwm_1, int16_t pwm_2, int16_t pwm_3, int16_t pwm_4)
{
  struct log_PWM pkt = {
    LOG_PACKET_HEADER_INIT(LOG_PWM_MSG),
    HAL_GetTick(),
    pwm_1,
    pwm_2,
    pwm_3,
    pwm_4
  };
  WriteBlock(&pkt, sizeof(pkt));
}
