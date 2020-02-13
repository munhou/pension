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

//����PCM ¼��ģʽ
//agc:0,�Զ�����.1024�൱��1��,512�൱��0.5��,���ֵ65535=64��		  
void recoder_enter_rec_mode(u16 agc)
{
	//�����IMA ADPCM,�����ʼ��㹫ʽ����:
 	//������=CLKI/256*d;	
	//����d=0,��2��Ƶ,�ⲿ����Ϊ12.288M.��ôFc=(2*12288000)/256*6=16Khz
	//���������PCM,������ֱ�Ӿ�д����ֵ 
  VS_WR_Cmd(SPI_BASS,0x0000);    
 	VS_WR_Cmd(SPI_AICTRL0,8000);	//���ò�����,����Ϊ8Khz
 	VS_WR_Cmd(SPI_AICTRL1,agc);		//��������,0,�Զ�����.1024�൱��1��,512�൱��0.5��,���ֵ65535=64��	
 	VS_WR_Cmd(SPI_AICTRL2,0);		//�����������ֵ,0,�������ֵ65536=64X
 	VS_WR_Cmd(SPI_AICTRL3,6);		//��ͨ��(MIC����������)
	VS_WR_Cmd(SPI_CLOCKF,0X2000);	//����VS10XX��ʱ��,MULT:2��Ƶ;ADD:������;CLK:12.288Mhz
	VS_WR_Cmd(SPI_MODE,0x1804);		//MIC,¼������    
 	delay_ms(5);					//�ȴ���  ��1.35ms 
 	VS_Load_Patch((u16*)wav_plugin,40);//VS1053��WAV¼����Ҫpatch
}
void recoder_wav_init(__WaveHeader* wavhead) //��ʼ��WAVͷ			   
{
	wavhead->riff.ChunkID=0X46464952;	//"RIFF"
	wavhead->riff.ChunkSize=0;			//��δȷ��,�����Ҫ����
	wavhead->riff.Format=0X45564157; 	//"WAVE"
	wavhead->fmt.ChunkID=0X20746D66; 	//"fmt "
	wavhead->fmt.ChunkSize=16; 			//��СΪ16���ֽ�
	wavhead->fmt.AudioFormat=0X01; 		//0X01,��ʾPCM;0X01,��ʾIMA ADPCM
 	wavhead->fmt.NumOfChannels=1;		//������
 	wavhead->fmt.SampleRate=8000;		//8Khz������ ��������
 	wavhead->fmt.ByteRate=wavhead->fmt.SampleRate*2;//16λ,��2���ֽ�
 	wavhead->fmt.BlockAlign=2;			//���С,2���ֽ�Ϊһ����
 	wavhead->fmt.BitsPerSample=16;		//16λPCM
  wavhead->data.ChunkID=0X61746164;	//"data"
 	wavhead->data.ChunkSize=0;			//���ݴ�С,����Ҫ����  
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
	fmp3=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//�����ڴ�
	databuf=(u8*)mymalloc(SRAMIN,32768);		//����512�ֽڵ��ڴ�����
	if(databuf==NULL||fmp3==NULL)rval=0XFF ;//�ڴ�����ʧ��.
	if(rval==0)
	{
		res=f_open(fmp3,(const TCHAR*)pname,FA_READ);//���ļ�10284
 		if(res==0)//�򿪳ɹ�.
		{

			res=f_read(fmp3,databuf,30000,(UINT*)&br);//����2048���ֽ�  	
			WIFI_SEND(databuf,30000);
			res=f_read(fmp3,databuf,21244,(UINT*)&br);//����2048���ֽ� 
		  WIFI_SEND(databuf,21244);
		  f_close(fmp3);
		}
		else rval=0XFF;//���ִ���
		VS_SPK_Set(0);	//�رհ������� 
	} 
 	myfree(SRAMIN,fmp3);	 
	myfree(SRAMIN,databuf);
	return rval;	  	 		  	    
}	 
//¼����
//����¼���ļ�,��������SD��RECORDER�ļ�����.
u8 recoder_play(void)
{
	u8 res;
	u8 rval=0,a=0,i=0;
	__WaveHeader *wavhead=0;
	u32 sectorsize=0;
	FIL* f_rec=0;					//�ļ�		    
 	DIR recdir;	 					//Ŀ¼
	u8 *recbuf;						//�����ڴ�	 
 	u16 w;
	u16 idx=0;
 	u8 recagc=0;
	 	while(f_opendir(&recdir,"0:/RECORDER"))//��¼���ļ���
 	{	 
		printf("open");	     
		delay_ms(200);
		LED1=~LED1;	
		f_mkdir("0:/RECORDER");				//������Ŀ¼   
	} 
	f_rec=(FIL *)mymalloc(SRAMIN,sizeof(FIL));	//����FIL�ֽڵ��ڴ����� 
	if(f_rec==NULL)rval=1;	//����ʧ��
 	wavhead=(__WaveHeader*)mymalloc(SRAMIN,sizeof(__WaveHeader));//����__WaveHeader�ֽڵ��ڴ�����
	if(wavhead==NULL)rval=1; 
	recbuf=mymalloc(SRAMIN,512); 	
	if(recbuf==NULL)rval=1;	  		   
	pname=mymalloc(SRAMIN,30);					//����30���ֽ��ڴ�,����"0:RECORDER/REC00001.wav"
	if(pname==NULL)rval=1;
	if(rval==0)									//�ڴ�����OK
	{      
 		recoder_enter_rec_mode(1024*recagc);  //����vs1053Ϊ¼��ģʽ				
   	while(VS_RD_Reg(SPI_HDAT1)>>8);//�ȵ�buf ��Ϊ�����ٿ�ʼ
		pname[0]=0;	
		if(rval==0)
		{
						recoder_new_pathname(pname);//�õ��µ�����
			      recoder_wav_init(wavhead);				//��ʼ��wavͷ
	 					res=f_open(f_rec,(const TCHAR*)pname, FA_CREATE_ALWAYS | FA_WRITE);
			      if(res)			//�ļ�����ʧ��
						{
							printf("falt");//�����ļ�ʧ��,����¼��
							rval=0XFE;	//��ʾ�Ƿ����SD��
						}
						else res=f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//д��ͷ����
						count=0;
	   while(sectorsize<100)
			{
				if(HW_flag==1)    //�����ʱ��⵽���ˣ���ֹͣ¼��
						break;
				w=VS_RD_Reg(SPI_HDAT1);  //����С	
				if((w>=256)&&(w<896))
				{
	 				idx=0;				   	 
		  		while(idx<512) 	//һ�ζ�ȡ512�ֽ�
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
	 				res=f_write(f_rec,recbuf,512,&bw);//д���ļ�
				
					if(res)
					break;//д�����.	
					sectorsize++;//����������1,ԼΪ32ms
				}
				
		}printf("%d/n",count);
				
				wavhead->riff.ChunkSize=sectorsize*512+36;	//�����ļ��Ĵ�С-8;
				wavhead->data.ChunkSize=sectorsize*512;		//���ݴ�С
				f_lseek(f_rec,0);							//ƫ�Ƶ��ļ�ͷ.
    		f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//д��ͷ����44
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
