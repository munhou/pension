#include "M8266_init.h"
void M8266_INIT()
{
	u32 success=0;
	M8266HostIf_Init();
	success = M8266WIFI_Module_Init_Via_SPI();
	if(success){
		LED0=0;LED1=1;
	}	
	else{
		 while(1){				
	     LED0=0;LED1=1; LED0=1;LED1=1;  M8266WIFI_Module_delay_ms(250);
		   LED0=0;LED1=0; LED0=1;LED1=0;  M8266WIFI_Module_delay_ms(250);
		}
	}
		
}
u32 wifi_connect()
{
	u16 res;
	u32 success=0;
	success=M8266WIFI_SPI_Set_Opmode(3,0,&res);
	success=M8266WIFI_SPI_STA_Connect_Ap(ssid,password,0,10,&res);
	success=M8266WIFI_SPI_Setup_Connection(1,4321,goal_ip,goal_port,1,5,&res);
	return success;
	
}
void WIFI_SEND(u8* buf,int lent)   //每次最多发送512个字节
{
	 #define M8266WIFI_SPI_SEND_DATA_BLOCK_MAX_LOOPS   2000
   #define tcp_packet_size 512
	 u16 res;
   u16 loops     = 0;
	 u32 len       = lent; 
	 u16 sent = 0;
   for(sent=0, loops=0; (sent<len)&&(loops<=M8266WIFI_SPI_SEND_DATA_BLOCK_MAX_LOOPS); loops++)
   {		
		 sent += M8266WIFI_SPI_Send_Data(buf+sent, ((len-sent)>tcp_packet_size)?tcp_packet_size:(len-sent), 1, &res);
     if(sent>=len)  break;
		 if((res&0xFF) == 0x00)
		 {
		 }
		 else
		 {
			 if(   ((res&0xFF) == 0x14)      // 0x14 = connection of link_no not present
          || ((res&0xFF) == 0x15)    // 0x15 = connection of link_no closed
          || ((res&0xFF) == 0x18) )  // 0x18 = TCP server in listening states and no tcp clients connecting to so far
			 {
                 // add some failure process here
					printf("connect false!");
					break;
	     }
	     else
	     {
	       M8266HostIf_delay_us(100);
	     }
			}
		}
	}
