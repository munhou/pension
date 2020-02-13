#include "EXT.h"
u8 ov_sta;
u8 dabuf[500];
u8 link=1;
u16 status;
void EXTI8_Init(void)
{												  
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);//PA8对中断线8
 	   	 
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级0 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器   
}
void EXTI2_Init(void)
{												  
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource2);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2; 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
 	   	 
	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
  NVIC_Init(&NVIC_InitStructure);  	 
}
void EXTI1_Init(void)
{												  
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource1);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1; 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	   	 
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
  NVIC_Init(&NVIC_InitStructure);  	 
}
void EXTI1_IRQHandler(void)
{
	u8 count=0,buf;
	u8 flag=0;
	if(EXTI_GetITStatus(EXTI_Line1)==SET)
	{
		M8266WIFI_SPI_RecvData(dabuf,500,1000,&link,&status);
		printf("%s",dabuf);
		//memset(dabuf,'\0',sizeof(dabuf));
		while(status==0x5124||status==0x1127)
		{
	  M8266WIFI_SPI_RecvData(dabuf,500,1000,&link,&status);
		printf("%s",dabuf);
			//memset(dabuf,'\0',sizeof(dabuf));
		}
	}
	for(count=0;count<strlen((char*)dabuf);count++)
	{
		flag++;
		if(flag==1)
		{
			if(dabuf[count]!='a')
				flag=0;
		}
		if(flag==2)
		{
			if(dabuf[count]!='p')
				flag=0;
		}
		if(flag==3)
		{
			if(dabuf[count]!='p')
				flag=0;
		}
		if(flag==4)
		{
			if(dabuf[count]!='l')
				flag=0;
		}	
		if(flag==5)
		{
			if(dabuf[count]!='y')
			{
				flag=0;	
			}
			else
			HW_flag=1;
		}	

			
	}
	flag=0;

	
	EXTI_ClearITPendingBit(EXTI_Line1);
}
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)==SET)
		{
		HW_flag=1;
		BUUZ=1;
	  }
	EXTI_ClearITPendingBit(EXTI_Line2);
}
void EXTI9_5_IRQHandler(void)
{		 		
	if(EXTI_GetITStatus(EXTI_Line8)==SET)	//是8线的中断
	{      
		OV7670_WRST=0;	//复位写指针		  		 
		OV7670_WRST=1;	
		OV7670_WREN=1;	//允许写入FIFO 	 
		ov_sta++;		//帧中断加1 
	}
	EXTI_ClearITPendingBit(EXTI_Line8);  //清除EXTI8线路挂起位						  
} 
