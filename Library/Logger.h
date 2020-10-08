#ifndef __LOGGER_H
#define __LOGGER_H

#ifdef __cplusplus
 extern "C" {
#endif
   
#include <stdint.h>

/*
Format characters in the format string for binary log messages
  a   : int16_t[32]
  b   : int8_t
  B   : uint8_t
  h   : int16_t
  H   : uint16_t
  i   : int32_t
  I   : uint32_t
  f   : float
  d   : double
  n   : char[4]
  N   : char[16]
  Z   : char[64]
  c   : int16_t * 100
  C   : uint16_t * 100
  e   : int32_t * 100
  E   : uint32_t * 100
  L   : int32_t latitude/longitude
  M   : uint8_t flight mode
  q   : int64_t
  Q   : uint64_t
 */
#define PACKED __attribute__((packed, aligned(1)))
#define ARRAY_SIZE(_arr) (sizeof(_arr) / sizeof(_arr[0]))

/*
 unfortunately these need to be macros because of a limitation of
 named member structure initialisation in g++
*/
#define LOG_PACKET_HEADER	       uint8_t head1, head2, msgid;
#define LOG_PACKET_HEADER_INIT(id) HEAD_BYTE1, HEAD_BYTE2, id
#define LOG_PACKET_HEADER_LEN 3 // bytes required for LOG_PACKET_HEADER

// once the logging code is all converted we will remove these from
// this header
#define HEAD_BYTE1  0xA3    // Decimal 163
#define HEAD_BYTE2  0x95    // Decimal 149

// structure used to define logging format
struct LogStructure {
  uint8_t msg_type;
  uint8_t msg_len;
  const char *name;
  const char *format;
  const char *labels;
};

/*
  log structures common to all vehicle types
 */
struct PACKED log_Format {
  LOG_PACKET_HEADER
  uint8_t type;
  uint8_t length;
  char name[4];
  char format[16];
  char labels[64];
};

struct PACKED log_TEST {
  LOG_PACKET_HEADER
  uint64_t time_us;
  uint16_t value;
};

struct PACKED log_PWM {
  LOG_PACKET_HEADER
  uint64_t time_us;
  int16_t  pwm_1;
  int16_t  pwm_2;
  int16_t  pwm_3;
  int16_t  pwm_4;
};

enum LogMessages{
	LOG_TEST_MSG = 0,
	LOG_PWM_MSG,
	LOG_ECU_MSG,

	LOG_FORMAT_MSG = 128, // this must remain #128

	_LOG_LAST_MSG_
};

struct PACKED log_ECU {
	LOG_PACKET_HEADER
	uint64_t time_us;
	float   voltage_set;
	float   voltage;
	float   current_set;
	float   current_used;
	float   current_charge;
	float   temperature;
//	uint16_t pwm_max_set;
//	uint16_t pwm_min_set;
//	uint16_t pwm_out_throttle;
//	uint16_t pwm_in_mode;
//	uint16_t pwm_in_throttle;
//	uint16_t servo_direction;
//	uint16_t motor_speed;
};
typedef struct ECU_Info {
	float   voltage_set;
	float   voltage;
	float   current_set;
	float   current_used;
	float   current_charge;
	float   temperature;
	uint16_t pwm_max_set;
	uint16_t pwm_min_set;
	uint16_t pwm_out_throttle;
	uint16_t pwm_in_mode;
	uint16_t pwm_in_throttle;
	uint16_t servo_direction;
	uint16_t motor_speed;
}ECU_Info;


void     Log_Init(void);
void     Write_Test(uint64_t time_us, uint16_t value);
void Write_PWM(int16_t pwm_1, int16_t pwm_2, int16_t pwm_3, int16_t pwm_4);
void Write_ECU(uint64_t time_us, const ECU_Info *info);

#ifdef __cplusplus
}
#endif
   
#endif /*__LOGGER_H*/
