# CarSeat_Recognization
fork
说明:往报警系统写数据，只需调用WriteData(long len, long startnum, VARIANT *WriteData)函数即可，格式如下:
1、先实例化一个OPC类
#include “OPC.h”
OPC *pOCP
2、调用接口
COleVariant writedata[2]; //需要用 COleVariant类型定义数组
plc_flag = 1
writedata[0]=(COleVariant)plc_flag;
pOCP->WriteData(2,startnum,writedata);