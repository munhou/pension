#ifndef _M8266INIT_H
#define _M8266INIT_H
#include "sys.h"
#include "led.h"
#include "GLOBAL.h"
#include "usart.h"
#include "M8266HostIf.h"
#include "M8266WIFIDrv.h"
#include "M8266WIFI_ops.h"
void M8266_INIT(void);
u32 wifi_connect(void);
void WIFI_SEND(u8* buf,int lent);
#endif

