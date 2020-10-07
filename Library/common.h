#ifndef _COMMON_H_
#define _COMMON_H_
#include "stdint.h"

<<<<<<< Updated upstream
<<<<<<< Updated upstream

=======
=======
>>>>>>> Stashed changes
<<<<<<< HEAD
#define PI 3.1415926535898f
#define deg2rad  PI/180.0
#define rad2deg  180.0/PI

enum{
	WalkMotor1 = 0,
	WalkMotor2,
	WalkMotor3,
	WalkMotor4,
	WalkMotor5,
	WalkMotor6,
	WalkMotor7,
	WalkMotor8,
	WalkMotorNum,
};

enum{
	SteerMotor1 = 0,
	SteerMotor2,
	SteerMotor3,
	SteerMotor4,
	SteerMotor5,
	SteerMotor6,
	SteerMotor7,
	SteerMotor8,
	SteerMotorNum,
};

// ÖؓҠۅìسʏއÖؓطΪ֚һٶÖؓ
typedef struct
{
	uint8_t model_type;
    uint8_t wheel_direction[4];	//Öؓˇر״в
    float wheel_radius[4];		//Öؓքѫ޶
    float ratio;				//ݵ̙҈
    float l1;					//ÖؓאфսܺǷɋאфքߠk
    float l2;
    float l3;
    float l4;
    float encoder_resolution;	//ҠëǷؖҦÊ
}Robotparam;

//
typedef struct
{
    float speed;
    float position;
    float current;
    uint32_t status;
}Wheel;

// 
typedef struct
{
    float vx;
    float vy;
    float v_theta;
}RobotVel;

typedef struct
{
	float x;  //m
	float y;  //m
	float theta; //rad

}RobotPos;

// 
typedef struct
{
	uint8_t model_type;
    RobotVel robot_velocity;			//车体坐标系下
}VehicleModelVel_t;

typedef struct
{
	RobotPos start_pos;
	RobotPos goal_pos;
	uint8_t path_type;
	float   radius;
	uint8_t is_clock_wise;    // 0： 顺时针  1： 逆时针
}PathRouteInfo;

typedef struct
{
	float a0;
	float a1;
	float a2;
	float a3;
	float a4;
	float a5;
}PolarPolynomialCoeffs_t;

/*  @berif  Adjust angle to [-pi,pi]
*   @params[in]  angle  input angle value
*   @params[out] angle  output angle value that adjust to [-pi,pi]
*   @return  
*/

void AngleAdjust(float* angle);//rad
=======

>>>>>>> main
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes


#endif

