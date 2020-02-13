#include "stm32f10x.h"
#include "DHT11.h"
#include "LED.h"
#include "BMP_SEND.h"
#include "usart.h"
#include "stdio.h"
#include "delay.h"
#include "GLOBAL.h"
#include "string.h"
#include "malloc.h" 
#include "EXT.h"
#include "bmp.h"
#include "exfuns.h"
#include "ff.h"
#include "delay.h"
#include "M8266HostIf.h"
#include "M8266WIFIDrv.h"
#include "M8266WIFI_ops.h"
#include "M8266_init.h"
#include "BUUZ_OUT.h"
#include "HW_GUS_IN.h"
#include "sdio_sdcard.h"
#include "vs10xx.h"
#include "ov7725.h"
#include "ov7670.h"
#include "sound_get.h"
#include "bmp.h"
#include "TIME.h"
 int main(void)
 {	
		u8 temp,humi,connect_check_flag;
		u8 link=1;
		u16 status;
		char pkt[50];
		u8 dabuf[500];
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		delay_init();
	  uart_init(115200);
		VS_Init();
	  VS_Sine_Test();
	  OV7670_Init();	  
		OV7670_Window_Set(12,176,240,320);
		OV7670_CS=0;
//	  OV7725_Init();
//	  OV7725_Window_Set(320,240,0);
//		OV7725_CS=0;
		LED_Init();
		BUUZ_Init();
		DHT11_Init();
	 
	 		
	 
	 	my_mem_init(SRAMIN);
		exfuns_init();
	 	f_mount(fs[0],"0:",1);   //���̹���һ���ļ�ϵͳ
	  f_mkdir("0:/PHOTO");     //��һ��Ŀ¼
		
		
	  M8266_INIT();            //��ʼ��wifiģ��
		
		
		EXTI8_Init();
	  EXTI2_Init();
		EXTI1_Init();
		TIM3_Int_Init(49999,7199);	
		TIM4_Int_Init(49999,7199);
		
		
		
		if(wifi_connect())      //wifi���ӳɹ�����
		{   
			wait=0;
			LED1=0;LED0=0;      //�������WiFi��tcp����˳ɹ���ledϨ��
			delay_ms(100);
			LED1=1;LED0=1;
		}
		else
		{
			LED1=0;LED0=0;
		}
		

		if(wait!=0)
			printf("wait ip");    //���WiFiһֱû������
		while(1){
			
			
			while(wait||wifi_cheak==1)  //��ʱ����һ��ʱ�佫wifi_cheak��1(��һ��ʱ���ѯwifi��״̬�����쳣�ͳ�����������)
				{     
					if(wait||wifi_cheak==1)
					{
						wifi_cheak=0;                 //����
						M8266WIFI_SPI_Query_Connection(1,0,&connect_check_flag,0,0,0,0);  //��ѯwifi������Ϣ
						if(connect_check_flag==1||connect_check_flag==0||connect_check_flag==6) //���wifiΪ�ȴ�����Ч��رվͳ�����������
							{  
							if(wifi_connect())
								{
									wait=0;
									LED1=0;LED0=0;
									delay_ms(100);
									LED1=1;LED0=1;
								}
							else
								{
									LED1=0;LED0=0;
								}	
							}	
					}
				 }

			
			
			
			if(HW_flag==1) //�������⵽��ʱ������һ����Ƭ������λ����32���͡�apply�������ⲿ�жϣ�Ҳ��ʹHW_flag��1
				{   
					BUUZ=1;
					HW_flag=0;	
					memset(pkt,0,50);
					sprintf(pkt,"file123:picture//153666//endF//");     //�ȷ���·��	
					M8266WIFI_SPI_Send_Data((u8*)pkt,sizeof(pkt),1,0);
					delay_ms(5);
					bmp_encode((u8*)"0:/PHOTO/124.bmp",0,0,320,240,0);  //����һ֡��Ƭ������һ����Ƭ��sd��
					BUUZ=0;
//				M8266WIFI_SPI_Send_Data((u8*)pkt,sizeof(pkt),1,0);
// 				send_pt((u8*)"0:/PHOTO/124.bmp");
//				f_unlink("0:/PHOTO/124.bmp");					
				}	
			
			
										
			if(DATA_flag==1)
				{        //��ʪ��
					DATA_flag=0;
					*pkt=0;
					memset(pkt,0,50);
					DHT11_Read_Data(&temp,&humi);
					sprintf(pkt,"DATA:%d//%d//%d//Badroom//endF//",temp,humi,guscheak);
					M8266WIFI_SPI_Send_Data((u8*)pkt,sizeof(pkt),1,0);
					LED1=1;LED0=1;
					delay_ms(100);
					LED1=1;LED0=0;
					delay_ms(100);
					LED1=1;LED0=1;
				}
			
			
				memset(pkt,0,50);     
				recoder_play();      //ÿһ��whileѭ����¼��һ�飬¼�ƴ��50kb����
				if(count>1200)
					{     //�����������1200�ͷ������¼���ļ�����Ȼ�Ͱ���ɾ��
						sprintf(pkt,"file123:sound%d-//51244//endF//",count);		 
						M8266WIFI_SPI_Send_Data((u8*)pkt,sizeof(pkt),1,0);
						rec_play_wav(pname);     //��0:RECORDER/REC0001.wav���¼��ͨ��wifiģ�鷢��ȥ 
				  }			
				f_unlink("0:RECORDER/REC0001.wav");       //ɾ�����¼��
	}

} 
 