#ifndef _VESC_
#define _VESC_

#include "sys.h"
#include "param.h"

typedef struct{
//	u16  Pulse;
	float  RATIO;
	float  GearRatio;
	u8 POLE_PAIRS;//电机极对数
}VESCParam;

typedef struct{
	vs16  angle;
	volatile float current;//电流
	vs32  speed;
//	vs32  pulse;
	volatile float duty;//占空比
}VESCValue;

typedef struct{
	bool PosLimit_ON;
	float AngleLimit;
	
	bool PositionSpeedLimit_ON;
	vs32  PosSpLimit;
	
	bool RealseWhenStuck_ON;
	
	float breakCurrent;//刹车电流
}VESCLimits;

typedef struct{
	bool stuck;//电机堵转
	bool timeout;//can报文反馈超时
}VESCStatus;

typedef struct{
    vu16 stuckCnt;//堵转用计数值，[相关于堵转状态stuck]
    u8  timeoutCnt;//超时用计数值，[相关于超时状态timeout]
    u32 lastRxTim;//上次接收到报文的系统时间，[相关于超时状态timeout]
    u16 timeoutTicks;//判断超时用系统节拍数，[相关于超时状态timeout]
}VESCArgum;

typedef struct{
	bool enable;
	bool start;
	u8 mode;
	VESCLimits Limits;
	VESCParam  Param;
	VESCValue  ValueSet,ValueReal;
	VESCStatus Statue;
	VESCArgum  Argum;
}VESC_Motor;

typedef enum{
	CAN_PACKET_SET_DUTY=0,
	CAN_PACKET_SET_CURRENT,								//1	设置电机电流	4字节	有符号整数	mA
	CAN_PACKET_SET_CURRENT_BRAKE,						//2	设置制动电流	4字节	有符号整数	mA
	CAN_PACKET_SET_RPM,									//3	设置（电）转速	4字节	有符号整数	ERPM
	CAN_PACKET_SET_POS,									//4	设置电机转角位置			
	CAN_PACKET_FILL_RX_BUFFER,							//5				
	CAN_PACKET_FILL_RX_BUFFER_LONG,						//6				
	CAN_PACKET_PROCESS_RX_BUFFER,						//7				
	CAN_PACKET_PROCESS_SHORT_BUFFER,					//8				
	CAN_PACKET_STATUS,									//9	查询状态 Request status	N/A		
	CAN_PACKET_SET_CURRENT_REL,							//10				
	CAN_PACKET_SET_CURRENT_BRAKE_REL,					//11
	CAN_PACKET_SET_CURRENT_HANDBRAKE,
	CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
	CAN_PACKET_STATUS_2,
	CAN_PACKET_STATUS_3,
	CAN_PACKET_STATUS_4,
	CAN_PACKET_PING,
	CAN_PACKET_PONG,
	CAN_PACKET_DETECT_APPLY_ALL_FOC,
	CAN_PACKET_DETECT_APPLY_ALL_FOC_RES,
	CAN_PACKET_CONF_CURRENT_LIMITS,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS,
	CAN_PACKET_CONF_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_FOC_ERPMS,
	CAN_PACKET_CONF_STORE_FOC_ERPMS,
	CAN_PACKET_STATUS_5
}VESC_Order;

void VESCInit(void);
void VESC_Set_Duty_Cycle(u8 controller_ID,float duty_cycle,u8 InConGrpFlag);
void VESC_Set_Speed(u8 controller_ID,s32 speed,u8 InConGrpFlag);
void VESC_Set_Current(u8 controller_ID,float current,u8 InConGrpFlag);
void VESC_Set_Position(u8 controller_ID,float angle,u8 InConGrpFlag);
void VESC_Set_Brake_Current(u8 controller_ID,float brake_current,u8 InConGrpFlag);
void VESC_Set_Handbrake_Current(u8 controller_ID,float handbrake_current,u8 InConGrpFlag);

extern VESC_Motor VESCmotor[4];

#endif


/**
  * @brief 本杰明CAN接收处理代码(参考）

			if (rxmsg.IDE == CAN_IDE_EXT) {  			//接收处理代码
				uint8_t id = rxmsg.EID & 0xFF;			//接收低八位作为id
				CAN_PACKET_ID cmd = rxmsg.EID >> 8;     //右移8位 其他位为指令cmd
				can_status_msg *stat_tmp;

				if (id == 255 || id == app_get_configuration()->controller_id) {			//判断id
					switch (cmd) {															//判断指令
					case CAN_PACKET_SET_DUTY:												//设置占空比
						ind = 0;															//数组位
						mc_interface_set_duty(buffer_get_float32(rxmsg.data8, 1e5f, &ind));	//接收data除以1e5f(100000)设为占空比 (50000->0.5–>50%)
						timeout_reset();													//重置时间
						break;

					case CAN_PACKET_SET_CURRENT:
						ind = 0;
						mc_interface_set_current(buffer_get_float32(rxmsg.data8, 1e3f, &ind));//接收data除以1e3f(1000)设为电流 mA->A
						timeout_reset();
						break;

					case CAN_PACKET_SET_CURRENT_BRAKE:
						ind = 0;
						mc_interface_set_brake_current(buffer_get_float32(rxmsg.data8, 1e3f, &ind));//接收data除以1e3f(1000)设为刹车电流 mA->A
						timeout_reset();
						break;

					case CAN_PACKET_SET_RPM:
						ind = 0;
						mc_interface_set_pid_speed(buffer_get_float32(rxmsg.data8, 1e0f, &ind)); //接收data除以1e0f(1)设为速度 ERPM=RPM*极对数
						timeout_reset();
						break;

					case CAN_PACKET_SET_POS:
						ind = 0;
						mc_interface_set_pid_pos(buffer_get_float32(rxmsg.data8, 1e6f, &ind)); //接收data除以1e6f(1000000)设为位置
						timeout_reset();
						break;

					case CAN_PACKET_FILL_RX_BUFFER:
						memcpy(rx_buffer + rxmsg.data8[0], rxmsg.data8 + 1, rxmsg.DLC - 1);
						break;

					case CAN_PACKET_FILL_RX_BUFFER_LONG:
						rxbuf_ind = (unsigned int)rxmsg.data8[0] << 8;
						rxbuf_ind |= rxmsg.data8[1];
						if (rxbuf_ind < RX_BUFFER_SIZE) {
							memcpy(rx_buffer + rxbuf_ind, rxmsg.data8 + 2, rxmsg.DLC - 2);
						}
						break;

					case CAN_PACKET_PROCESS_RX_BUFFER:
						ind = 0;
						rx_buffer_last_id = rxmsg.data8[ind++];
						commands_send = rxmsg.data8[ind++];
						rxbuf_len = (unsigned int)rxmsg.data8[ind++] << 8;
						rxbuf_len |= (unsigned int)rxmsg.data8[ind++];

						if (rxbuf_len > RX_BUFFER_SIZE) {
							break;
						}

						crc_high = rxmsg.data8[ind++];
						crc_low = rxmsg.data8[ind++];

						if (crc16(rx_buffer, rxbuf_len)
								== ((unsigned short) crc_high << 8
										| (unsigned short) crc_low)) {

							if (commands_send) {
								commands_send_packet(rx_buffer, rxbuf_len);
							} else {
								commands_set_send_func(send_packet_wrapper);
								commands_process_packet(rx_buffer, rxbuf_len);
							}
						}
						break;

					case CAN_PACKET_PROCESS_SHORT_BUFFER:
						ind = 0;
						rx_buffer_last_id = rxmsg.data8[ind++];
						commands_send = rxmsg.data8[ind++];

						if (commands_send) {
							commands_send_packet(rxmsg.data8 + ind, rxmsg.DLC - ind);
						} else {
							commands_set_send_func(send_packet_wrapper);
							commands_process_packet(rxmsg.data8 + ind, rxmsg.DLC - ind);
						}
						break;

					case CAN_PACKET_SET_CURRENT_REL:
						ind = 0;
						mc_interface_set_current_rel(buffer_get_float32(rxmsg.data8, 1e5f, &ind));
						timeout_reset();
						break;

					case CAN_PACKET_SET_CURRENT_BRAKE_REL:
						ind = 0;
						mc_interface_set_brake_current_rel(buffer_get_float32(rxmsg.data8, 1e5f, &ind));
						timeout_reset();
						break;

					case CAN_PACKET_SET_CURRENT_HANDBRAKE:
						ind = 0;
						mc_interface_set_handbrake(buffer_get_float32(rxmsg.data8, 1e3f, &ind));
						timeout_reset();
						break;

					case CAN_PACKET_SET_CURRENT_HANDBRAKE_REL:
						ind = 0;
						mc_interface_set_handbrake_rel(buffer_get_float32(rxmsg.data8, 1e5f, &ind));
						timeout_reset();
						break;

					default:
						break;
					}
				}
*/
