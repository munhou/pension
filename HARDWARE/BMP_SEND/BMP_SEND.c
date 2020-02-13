#include "BMP_SEND.h"
u8 send_pt(u8 *pname)
{	 
 	FIL* fmp3;
  u16 br;
	u8 res,rval=0;	  
	u8 *databuf;	   		    	
	fmp3=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//申请内存
	databuf=(u8*)mymalloc(SRAMIN,32768);		//开辟512字节的内存区域
	if(databuf==NULL||fmp3==NULL)rval=0XFF ;//内存申请失败.
	if(rval==0)
	{res=f_open(fmp3,(const TCHAR*)pname,FA_READ);//打开文件10284
 		if(res==0)//打开成功.
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
		}else rval=0XFF;//出现错误 
 	myfree(SRAMIN,fmp3);	 
	myfree(SRAMIN,databuf);
	return rval;	  	 		  	    
}	
