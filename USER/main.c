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
	 	f_mount(fs[0],"0:",1);   //立刻挂载一个文件系统
	  f_mkdir("0:/PHOTO");     //建一个目录
		
		
	  M8266_INIT();            //初始化wifi模块
		
		
		EXTI8_Init();
	  EXTI2_Init();
		EXTI1_Init();
		TIM3_Int_Init(49999,7199);	
		TIM4_Int_Init(49999,7199);
		
		
		
		if(wifi_connect())      //wifi连接成功？？
		{   
			wait=0;
			LED1=0;LED0=0;      //如果连接WiFi和tcp服务端成功，led熄灭
			delay_ms(100);
			LED1=1;LED0=1;
		}
		else
		{
			LED1=0;LED0=0;
		}
		

		if(wait!=0)
			printf("wait ip");    //如果WiFi一直没有连上
		while(1){
			
			
			while(wait||wifi_cheak==1)  //定时器过一段时间将wifi_cheak置1(过一段时间查询wifi的状态，如异常就尝试重新连接)
				{     
					if(wait||wifi_cheak==1)
					{
						wifi_cheak=0;                 //清零
						M8266WIFI_SPI_Query_Connection(1,0,&connect_check_flag,0,0,0,0);  //查询wifi连接信息
						if(connect_check_flag==1||connect_check_flag==0||connect_check_flag==6) //如果wifi为等待，无效或关闭就尝试重新连接
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

			
			
			
			if(HW_flag==1) //当红外检测到人时，就拍一张照片。若上位机给32发送“apply”触发外部中断，也会使HW_flag置1
				{   
					BUUZ=1;
					HW_flag=0;	
					memset(pkt,0,50);
					sprintf(pkt,"file123:picture//153666//endF//");     //先发送路径	
					M8266WIFI_SPI_Send_Data((u8*)pkt,sizeof(pkt),1,0);
					delay_ms(5);
					bmp_encode((u8*)"0:/PHOTO/124.bmp",0,0,320,240,0);  //发送一帧照片并保存一张照片到sd卡
					BUUZ=0;
//				M8266WIFI_SPI_Send_Data((u8*)pkt,sizeof(pkt),1,0);
// 				send_pt((u8*)"0:/PHOTO/124.bmp");
//				f_unlink("0:/PHOTO/124.bmp");					
				}	
			
			
										
			if(DATA_flag==1)
				{        //温湿度
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
				recoder_play();      //每一个while循环都录音一遍，录制大概50kb左右
				if(count>1200)
					{     //如果音量大于1200就发送这个录音文件，不然就把它删掉
						sprintf(pkt,"file123:sound%d-//51244//endF//",count);		 
						M8266WIFI_SPI_Send_Data((u8*)pkt,sizeof(pkt),1,0);
						rec_play_wav(pname);     //把0:RECORDER/REC0001.wav这个录音通过wifi模块发出去 
				  }			
				f_unlink("0:RECORDER/REC0001.wav");       //删掉这个录音
	}

} 
 