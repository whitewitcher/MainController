#ifndef _VESC_
#define _VESC_

#include "sys.h"
#include "param.h"

typedef struct{
//	u16  Pulse;
	float  RATIO;
	float  GearRatio;
	u8 POLE_PAIRS;//���������
}VESCParam;

typedef struct{
	vs16  angle;
	volatile float current;//����
	vs32  speed;
//	vs32  pulse;
	volatile float duty;//ռ�ձ�
}VESCValue;

typedef struct{
	bool PosLimit_ON;
	float AngleLimit;
	
	bool PositionSpeedLimit_ON;
	vs32  PosSpLimit;
	
	bool RealseWhenStuck_ON;
	
	float breakCurrent;//ɲ������
}VESCLimits;

typedef struct{
	bool stuck;//�����ת
	bool timeout;//can���ķ�����ʱ
}VESCStatus;

typedef struct{
    vu16 stuckCnt;//��ת�ü���ֵ��[����ڶ�ת״̬stuck]
    u8  timeoutCnt;//��ʱ�ü���ֵ��[����ڳ�ʱ״̬timeout]
    u32 lastRxTim;//�ϴν��յ����ĵ�ϵͳʱ�䣬[����ڳ�ʱ״̬timeout]
    u16 timeoutTicks;//�жϳ�ʱ��ϵͳ��������[����ڳ�ʱ״̬timeout]
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
	CAN_PACKET_SET_CURRENT,								//1	���õ������	4�ֽ�	�з�������	mA
	CAN_PACKET_SET_CURRENT_BRAKE,						//2	�����ƶ�����	4�ֽ�	�з�������	mA
	CAN_PACKET_SET_RPM,									//3	���ã��磩ת��	4�ֽ�	�з�������	ERPM
	CAN_PACKET_SET_POS,									//4	���õ��ת��λ��			
	CAN_PACKET_FILL_RX_BUFFER,							//5				
	CAN_PACKET_FILL_RX_BUFFER_LONG,						//6				
	CAN_PACKET_PROCESS_RX_BUFFER,						//7				
	CAN_PACKET_PROCESS_SHORT_BUFFER,					//8				
	CAN_PACKET_STATUS,									//9	��ѯ״̬ Request status	N/A		
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
  * @brief ������CAN���մ������(�ο���

			if (rxmsg.IDE == CAN_IDE_EXT) {  			//���մ������
				uint8_t id = rxmsg.EID & 0xFF;			//���յͰ�λ��Ϊid
				CAN_PACKET_ID cmd = rxmsg.EID >> 8;     //����8λ ����λΪָ��cmd
				can_status_msg *stat_tmp;

				if (id == 255 || id == app_get_configuration()->controller_id) {			//�ж�id
					switch (cmd) {															//�ж�ָ��
					case CAN_PACKET_SET_DUTY:												//����ռ�ձ�
						ind = 0;															//����λ
						mc_interface_set_duty(buffer_get_float32(rxmsg.data8, 1e5f, &ind));	//����data����1e5f(100000)��Ϊռ�ձ� (50000->0.5�C>50%)
						timeout_reset();													//����ʱ��
						break;

					case CAN_PACKET_SET_CURRENT:
						ind = 0;
						mc_interface_set_current(buffer_get_float32(rxmsg.data8, 1e3f, &ind));//����data����1e3f(1000)��Ϊ���� mA->A
						timeout_reset();
						break;

					case CAN_PACKET_SET_CURRENT_BRAKE:
						ind = 0;
						mc_interface_set_brake_current(buffer_get_float32(rxmsg.data8, 1e3f, &ind));//����data����1e3f(1000)��Ϊɲ������ mA->A
						timeout_reset();
						break;

					case CAN_PACKET_SET_RPM:
						ind = 0;
						mc_interface_set_pid_speed(buffer_get_float32(rxmsg.data8, 1e0f, &ind)); //����data����1e0f(1)��Ϊ�ٶ� ERPM=RPM*������
						timeout_reset();
						break;

					case CAN_PACKET_SET_POS:
						ind = 0;
						mc_interface_set_pid_pos(buffer_get_float32(rxmsg.data8, 1e6f, &ind)); //����data����1e6f(1000000)��Ϊλ��
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
