#ifndef __BATTERY_H
#define __BATTERY_H

#include "stm32f4xx.h"
#include "system.h"

//��ص�Ԥ������
#define BAT_ELECTRIC_VALID_LOW	20

//������ߺ궨��
#define BAT_CHARGEENERGY_1      1350
#define BAT_CHARGEENERGY_21     1500
#define BAT_CHARGEENERGY_81     1600
#define BAT_CHARGEENERGY_100    1670

//�ŵ����ߺ궨��
#define BAT_DISCHARGEENERGY_100 1600
#define BAT_DISCHARGEENERGY_99  1599
#define BAT_DISCHARGEENERGY_1   1300

//��ص���������
#define BAT_ENERGY_100          100
#define BAT_ENERGY_0            0

//���״̬
#define BAT_STATE_CHARGE	1	//���ڳ��
#define BAT_STATE_FULL		2	//������
#define BAT_STATE_ERROR		3	//��ع��ϡ���ѹ������˯�ߵ�

void Bat_ChargeManager(void);
void Bat_InformationRead(void);
bool Bat_InsertStateChangeMinitor(void);
bool Bat_StateChangeMinitor(void);
uint8_t Get_BatteryChargeState(void);
#endif
