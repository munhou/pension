#include "GLOBAL.h"

	//��������
//	char goal_ip[24]="192.168.43.243";
	char goal_ip[24]="172.20.10.2";
	u16 goal_port=8080;//�˿�
//	u8 ssid[24]="Honor";//SSID
	u8 ssid[24]="iPhone";//SSID
//	u8 password[24]="12345432";//PASSWORD
	u8 password[24]="1234567890";//PASSWORD

	//¼��ģ��
	u32 count=0;//��������
	u8 *pname;//¼���ļ���
	//ȫ���ź�
	u8 DATA_flag=0;//���ݲɼ��ź�
	u8 HW_flag=0;//���ⴥ���ź�
	u8 wifi_cheak=0;//�������Ӽ���ź�
	u8 wifi_config=0;
	u8 wait=1;
		