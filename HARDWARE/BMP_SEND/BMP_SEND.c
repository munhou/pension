#include "BMP_SEND.h"
u8 send_pt(u8 *pname)
{	 
 	FIL* fmp3;
  u16 br;
	u8 res,rval=0;	  
	u8 *databuf;	   		    	
	fmp3=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//�����ڴ�
	databuf=(u8*)mymalloc(SRAMIN,32768);		//����512�ֽڵ��ڴ�����
	if(databuf==NULL||fmp3==NULL)rval=0XFF ;//�ڴ�����ʧ��.
	if(rval==0)
	{res=f_open(fmp3,(const TCHAR*)pname,FA_READ);//���ļ�10284
 		if(res==0)//�򿪳ɹ�.
		{
			res=f_read(fmp3,databuf,30000,(UINT*)&br);	
			WIFI_SEND(databuf,30000);
			res=f_read(fmp3,databuf,30000,(UINT*)&br);
			WIFI_SEND(databuf,30000);
			res=f_read(fmp3,databuf,30000,(UINT*)&br);
			WIFI_SEND(databuf,30000);
			res=f_read(fmp3,databuf,30000,(UINT*)&br);
			WIFI_SEND(databuf,30000);
			res=f_read(fmp3,databuf,30000,(UINT*)&br);
			WIFI_SEND(databuf,30000);
			res=f_read(fmp3,databuf,3666,(UINT*)&br);
			WIFI_SEND(databuf,3666);
			}
			f_close(fmp3);
		}else rval=0XFF;//���ִ��� 
 	myfree(SRAMIN,fmp3);	 
	myfree(SRAMIN,databuf);
	return rval;	  	 		  	    
}	
