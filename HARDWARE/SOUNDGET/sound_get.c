#include "stm32f10x.h"
#include "sound_get.h"
#include "malloc.h" 
#include "vs10xx.h"	
#include "exfuns.h"
#include "ff.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "M8266WIFIDrv.h"
#include "GLOBAL.h"
#include "M8266_init.h"
u16 index1=0;
const u16 wav_plugin[40]=/* Compressed plugin */ 
{ 
0x0007, 0x0001, 0x8010, 0x0006, 0x001c, 0x3e12, 0xb817, 0x3e14, /* 0 */ 
0xf812, 0x3e01, 0xb811, 0x0007, 0x9717, 0x0020, 0xffd2, 0x0030, /* 8 */ 
0x11d1, 0x3111, 0x8024, 0x3704, 0xc024, 0x3b81, 0x8024, 0x3101, /* 10 */ 
0x8024, 0x3b81, 0x8024, 0x3f04, 0xc024, 0x2808, 0x4800, 0x36f1, /* 18 */ 
0x9811, 0x0007, 0x0001, 0x8028, 0x0006, 0x0002, 0x2a00, 0x040e,  
}; 

//激活PCM 录音模式
//agc:0,自动增益.1024相当于1倍,512相当于0.5倍,最大值65535=64倍		  
void recoder_enter_rec_mode(u16 agc)
{
	//如果是IMA ADPCM,采样率计算公式如下:
 	//采样率=CLKI/256*d;	
	//假设d=0,并2倍频,外部晶振为12.288M.那么Fc=(2*12288000)/256*6=16Khz
	//如果是线性PCM,采样率直接就写采样值 
  VS_WR_Cmd(SPI_BASS,0x0000);    
 	VS_WR_Cmd(SPI_AICTRL0,8000);	//设置采样率,设置为8Khz
 	VS_WR_Cmd(SPI_AICTRL1,agc);		//设置增益,0,自动增益.1024相当于1倍,512相当于0.5倍,最大值65535=64倍	
 	VS_WR_Cmd(SPI_AICTRL2,0);		//设置增益最大值,0,代表最大值65536=64X
 	VS_WR_Cmd(SPI_AICTRL3,6);		//左通道(MIC单声道输入)
	VS_WR_Cmd(SPI_CLOCKF,0X2000);	//设置VS10XX的时钟,MULT:2倍频;ADD:不允许;CLK:12.288Mhz
	VS_WR_Cmd(SPI_MODE,0x1804);		//MIC,录音激活    
 	delay_ms(5);					//等待至  少1.35ms 
 	VS_Load_Patch((u16*)wav_plugin,40);//VS1053的WAV录音需要patch
}
void recoder_wav_init(__WaveHeader* wavhead) //初始化WAV头			   
{
	wavhead->riff.ChunkID=0X46464952;	//"RIFF"
	wavhead->riff.ChunkSize=0;			//还未确定,最后需要计算
	wavhead->riff.Format=0X45564157; 	//"WAVE"
	wavhead->fmt.ChunkID=0X20746D66; 	//"fmt "
	wavhead->fmt.ChunkSize=16; 			//大小为16个字节
	wavhead->fmt.AudioFormat=0X01; 		//0X01,表示PCM;0X01,表示IMA ADPCM
 	wavhead->fmt.NumOfChannels=1;		//单声道
 	wavhead->fmt.SampleRate=8000;		//8Khz采样率 采样速率
 	wavhead->fmt.ByteRate=wavhead->fmt.SampleRate*2;//16位,即2个字节
 	wavhead->fmt.BlockAlign=2;			//块大小,2个字节为一个块
 	wavhead->fmt.BitsPerSample=16;		//16位PCM
  wavhead->data.ChunkID=0X61746164;	//"data"
 	wavhead->data.ChunkSize=0;			//数据大小,还需要计算  
}
void recoder_new_pathname(u8 *pname)
{	 					 
	index1=0;
	sprintf((char*)pname,"0:RECORDER/REC0001.wav");
}

u8 rec_play_wav(u8 *pname)
{	 
 	FIL* fmp3;
  u16 br;
	u8 res,rval=0;	  
	u8 *databuf;	   		    	
	fmp3=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//申请内存
	databuf=(u8*)mymalloc(SRAMIN,32768);		//开辟512字节的内存区域
	if(databuf==NULL||fmp3==NULL)rval=0XFF ;//内存申请失败.
	if(rval==0)
	{
		res=f_open(fmp3,(const TCHAR*)pname,FA_READ);//打开文件10284
 		if(res==0)//打开成功.
		{

			res=f_read(fmp3,databuf,30000,(UINT*)&br);//读出2048个字节  	
			WIFI_SEND(databuf,30000);
			res=f_read(fmp3,databuf,21244,(UINT*)&br);//读出2048个字节 
		  WIFI_SEND(databuf,21244);
		  f_close(fmp3);
		}
		else rval=0XFF;//出现错误
		VS_SPK_Set(0);	//关闭板载喇叭 
	} 
 	myfree(SRAMIN,fmp3);	 
	myfree(SRAMIN,databuf);
	return rval;	  	 		  	    
}	 
//录音机
//所有录音文件,均保存在SD卡RECORDER文件夹内.
u8 recoder_play(void)
{
	u8 res;
	u8 rval=0,a=0,i=0;
	__WaveHeader *wavhead=0;
	u32 sectorsize=0;
	FIL* f_rec=0;					//文件		    
 	DIR recdir;	 					//目录
	u8 *recbuf;						//数据内存	 
 	u16 w;
	u16 idx=0;
 	u8 recagc=0;
	 	while(f_opendir(&recdir,"0:/RECORDER"))//打开录音文件夹
 	{	 
		printf("open");	     
		delay_ms(200);
		LED1=~LED1;	
		f_mkdir("0:/RECORDER");				//创建该目录   
	} 
	f_rec=(FIL *)mymalloc(SRAMIN,sizeof(FIL));	//开辟FIL字节的内存区域 
	if(f_rec==NULL)rval=1;	//申请失败
 	wavhead=(__WaveHeader*)mymalloc(SRAMIN,sizeof(__WaveHeader));//开辟__WaveHeader字节的内存区域
	if(wavhead==NULL)rval=1; 
	recbuf=mymalloc(SRAMIN,512); 	
	if(recbuf==NULL)rval=1;	  		   
	pname=mymalloc(SRAMIN,30);					//申请30个字节内存,类似"0:RECORDER/REC00001.wav"
	if(pname==NULL)rval=1;
	if(rval==0)									//内存申请OK
	{      
 		recoder_enter_rec_mode(1024*recagc);  //设置vs1053为录音模式				
   	while(VS_RD_Reg(SPI_HDAT1)>>8);//等到buf 较为空闲再开始
		pname[0]=0;	
		if(rval==0)
		{
						recoder_new_pathname(pname);//得到新的名字
			      recoder_wav_init(wavhead);				//初始化wav头
	 					res=f_open(f_rec,(const TCHAR*)pname, FA_CREATE_ALWAYS | FA_WRITE);
			      if(res)			//文件创建失败
						{
							printf("falt");//创建文件失败,不能录音
							rval=0XFE;	//提示是否存在SD卡
						}
						else res=f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//写入头数据
						count=0;
	   while(sectorsize<100)
			{
				if(HW_flag==1)    //如果此时检测到有人，就停止录音
						break;
				w=VS_RD_Reg(SPI_HDAT1);  //读大小	
				if((w>=256)&&(w<896))
				{
	 				idx=0;				   	 
		  		while(idx<512) 	//一次读取512字节
					{	 
			 			w=VS_RD_Reg(SPI_HDAT0);				   	    
		 				recbuf[idx++]=w&0XFF;
						i++;
						
						
						if(i==5)
						{
							i=0;
							count+=(a>recbuf[idx]?(a-recbuf[idx]):(recbuf[idx]-a))/100;
							a=recbuf[idx];
						}
						
						
						recbuf[idx++]=w>>8;
					}	  		 
	 				res=f_write(f_rec,recbuf,512,&bw);//写入文件
				
					if(res)
					break;//写入出错.	
					sectorsize++;//扇区数增加1,约为32ms
				}
				
		}printf("%d/n",count);
				
				wavhead->riff.ChunkSize=sectorsize*512+36;	//整个文件的大小-8;
				wavhead->data.ChunkSize=sectorsize*512;		//数据大小
				f_lseek(f_rec,0);							//偏移到文件头.
    		f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//写入头数据44
				f_close(f_rec);
				sectorsize=0;
	}
	}

 //rec_play_wav(pname);
  myfree(SRAMIN,wavhead);
	myfree(SRAMIN,recbuf);	  
 	myfree(SRAMIN,f_rec);	 
	myfree(SRAMIN,pname);

	return rval;
}
