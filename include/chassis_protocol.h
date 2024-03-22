/**
 * @Author: Xia Yunkai
 * @Date:   2024-03-11 19:05:31
 * @Last Modified by:   Xia Yunkai
 * @Last Modified time: 2024-03-22 16:32:48
 */
#include <stdint.h>

#ifndef __CHASSIS_PROTOCOL_H__
#define __CHASSIS_PROTOCOL_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define WHEEL_FL 0 // 左前轮
#define WHEEL_FR 1 // 右前轮
#define WHEEL_RL 2 // 左后轮
#define WHEEL_RR 3 // 右后轮

// 两字节对齐
#pragma pack(push, 2)

    typedef struct
    {
        union
        {
            struct
            {
                int8_t typeCode[2];      // 数据标志位，V C 表示Vehicle To capl
                uint16_t counter;        // 发送计数
                uint16_t comState;       // 通信状态 101表示通信正常
                uint16_t controlType;    // 00表示不操作 01 表示横向握手，02表示纵向握手，03表示同时握手，04表示解除握手
                int32_t tar_torque;      // 目标扭矩，除以1000等于真实值
                int32_t tar_speed;       // 目标速度             除以1000等于真实值
                int32_t tar_acc;         // 目标加速度           除以1000等于真实值
                int32_t tar_brake;       // 目标刹车力度         除以1000等于真实值
                int32_t tar_gear;        // 目标档位             0,2,3,4
                int32_t tar_stop_dis;    // 目标刹停距离          mm
                int32_t emergency_req;   // 目标急刹请求          1/0
                int32_t driveoff_req;    // 目标起步请求          1/0
                int32_t standstill_req;  // 目标驻车请求          1/0
                int32_t tar_steer_angle; // 目标方向盘转角       除以1000等于真实值
                int32_t tar_steer_speed; // 目标方向盘转速       除以1000等于真实值
                int32_t tar_eps;         // 目标EPS              -7840 - 7840
                int32_t tar_fw_angle;    // 目标前轮转角         除以1000等于真实值
            } cmd;
            char raw[60];
        } data;
    } ControlCmd;

    typedef struct
    {
        union
        {
            struct
            {
                int8_t typeCode[2];          // 数据标志位，C V 表示Capl To Vehicle
                uint16_t counter;            // 发送计数
                uint16_t comState;           // 通信状态 101表示通信正常
                uint16_t chassisState;       // 00表示未握手，01表示握手成功，02表示异常
                uint16_t veh_gearpos;        // 车身当前档位，0-初始，1-N，2-R，3-D，4-P，15-Error，-1-无效
                uint16_t veh_standstill;     // 车辆当前停止状态 0为运动，1为停止
                uint16_t veh_brake_pedal;    // 车辆当前刹车踏板状态，0为空置，1为踩下
                int8_t wheel_dir[4];         // 车轮转向 0 静止，1 前进，-1 后退
                int32_t wheel_pulse[4];      // 车轮脉冲信息
                int32_t wheel_speed[4];      // 车轮轮速信息，精度0.01 km/h
                int32_t veh_lat_acc;         // 横向加速度，精度0.01 m/s^2
                int32_t veh_lon_acc;         // 纵向加速度，精度0.01 m/s^2
                int32_t veh_yawrate;         // 车身横摆角速度信号，精度0.01 deg/sec
                int32_t veh_t_angle;         // 车辆倾斜角度信号
                int32_t veh_steer_angle;     // 车身方向盘转角，精度0.0625°
                int32_t veh_steer_angle_spd; // 车身方向盘转速
                int32_t veh_torque;          // 车身当前扭矩大小,具体单位视CAN协议决定
                int32_t veh_velocity;        // 车辆当前速度，具体单位视CAN协议决定
            } status;
            // 2 + 2*6 + 4 + 4 * 4 * 2  + 4* 8 =
            char raw[82];
        } data;

    } StatusResponse;

#pragma pack(pop)
#ifdef __cplusplus
}
#endif

#endif /* __CHASSIS_PROTOCOL_H__ */
