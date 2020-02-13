#include "GLOBAL.h"

	//网络链接
//	char goal_ip[24]="192.168.43.243";
	char goal_ip[24]="172.20.10.2";
	u16 goal_port=8080;//端口
//	u8 ssid[24]="Honor";//SSID
	u8 ssid[24]="iPhone";//SSID
//	u8 password[24]="12345432";//PASSWORD
	u8 password[24]="1234567890";//PASSWORD

	//录音模块
	u32 count=0;//能量计数
	u8 *pname;//录音文件名
	//全局信号
	u8 DATA_flag=0;//数据采集信号
	u8 HW_flag=0;//红外触发信号
	u8 wifi_cheak=0;//网络链接检测信号
	u8 wifi_config=0;
	u8 wait=1;
		