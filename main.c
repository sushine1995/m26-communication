 
#include<varmain.h> 
#include<string.h>
#define SetPoint_ee 0x0200 //设定
/*********EEPROM设置相关**************/
  sfr ISP_DATA=0xc2;
  sfr ISP_ADDRH=0xc3;
  sfr ISP_ADDRL=0xc4;
  sfr ISP_CMD=0xc5;
  sfr ISP_TRIG=0xc6;
  sfr ISP_CONTR=0xc7;
/*************************************/
/***EEPROM读取相关*****************/
#define ISP_READ_BYTE 1  //EEPROM读
#define ISP_PROG_BYTE 2  //EEPROM编程
#define ISP_SECTOR_ERASE 3 //EEPROM扇区擦除
#define ENABLE_ISP 0x83 //允许编程，对应于12MHz	
/*************************************
 函 数 名 : EEPROM_write_byte
 功    能 : 写一一个字节的EEPROM数据
 入口参数 :  addrH 高位地址
             addrL 低位地址
             wdata  要写入的数据
 返回参数 :  无

*************************************/
void EEPROM_write_byte(unsigned int addr,unsigned char wdata)
{	 unsigned char addrH,addrL;
      addrH=addr/256;
	    addrL=addr%256;
      ISP_CONTR=ENABLE_ISP;//打开IAP功能，设置Flash操作等待时间
      ISP_CMD=ISP_PROG_BYTE;//设置为字节编程模式
      ISP_ADDRH=addrH; //数据地址
      ISP_ADDRL=addrL;
      ISP_DATA=wdata; //送需要存储的数据
      EA=0;
	ISP_TRIG=0x5a; //触发IAP命令
	ISP_TRIG=0xa5;
      _nop_();
      EA=1;
}
/*************************************
 函 数 名 :EEPROM_sector_erase
 功    能 : 删除扇区的内容
 入口参数 :  addrH 高位地址
             addrL 低位地址
 返回参数 :  无

*************************************/
void EEPROM_sector_erase(unsigned int addr)
{	 unsigned char addrH,addrL;
     addrH=addr/256;
	 addrL=addr%256;
     ISP_CONTR=ENABLE_ISP;//打开IAP功能，设置Flash操作等待时间
     ISP_CMD=ISP_SECTOR_ERASE;
     ISP_ADDRH=addrH; //数据地址
     ISP_ADDRL=addrL;
     EA=0;
	ISP_TRIG=0x5a; //触发IAP命令
	ISP_TRIG=0xa5;
     _nop_();
     EA=1;
}
/*************************************
 函 数 名 : EEPROM_read_byte
 功    能 : 读取一个字节的EEPROM数据
 入口参数 :  addrH 高位地址
             addrL 低位地址
 返回参数 :  无 

*************************************/
unsigned char EEPROM_read_byte(unsigned int addr)
{
	unsigned char temp;
	unsigned char addrH,addrL;
	addrH=addr/256;
	 addrL=addr%256;
        ISP_CONTR=ENABLE_ISP;//打开IAP功能，设置Flash操作等待时间
	ISP_CMD=ISP_READ_BYTE;//设置为字节读模式
	ISP_ADDRH=addrH;  //数据地址
	ISP_ADDRL=addrL;
	EA=0;//关中断
	ISP_TRIG=0x5a; //触发IAP命令
	ISP_TRIG=0xa5;
        _nop_();
       	temp=ISP_DATA;
	EA=1;
        return(temp);


}
//****************************************************8
//uint crccheck(uchar *ptr, uchar len)       //CRC校验程序
//{
//uchar uchCRCHi = 0xFF ;   /* high byte of CRC initialized */
//uchar uchCRCLo = 0xFF ;   /* low byte of CRC initialized */
//uchar uIndex ;            /* will index into CRC lookup table */
//while (len--)                  /* pass through message buffer */
//{
//uIndex = uchCRCHi ^ *ptr++ ;   /* calculate the CRC */
//uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
//uchCRCLo = auchCRCLo[uIndex] ;
//}
//return (uchCRCHi<<8|uchCRCLo) ;
//}

//*****************************************************************************************
//函数名：delay(unsigned int s)
//输入：时间
//输出：无
//功能描述：普通廷时,内部用
//*****************************************************************************************  
static void delay(unsigned int s)
{
unsigned int i;
for(i=0; i<s; i++);
for(i=0; i<s; i++);
}

void delayms(unsigned int k)
{
	unsigned int i,j;
	WDT_CONTR=0x3e;
	for(i = 0; i < k; i++)
		for(j = 0; j < 1000; j++)
		;
}


void DelayMin(uchar m)
{
	uchar i,j;
	for( i = 0; i < m ; i ++)
	{
		for( j =0; j <20; j++)
		{	
			delayms(1000);
			delayms(1000);
			delayms(1000);
			WDT_CONTR = 0x3E;
		}
	}
}
//函 数 名: ClearRecBuff
//函数功能: 清除串口2接收缓存中的数据
//入口参数: 无
//返回参数: 无
//*************************************/
void ClearRecBuff()
{

	uchar i = 0;	
	for(i=0; i < recv_data_count; i++)
	{
		recv_data[i] = '\0';		//在执行命令的部分使用的是recv_data，可以只使用一个
		RxBuf2[i] = '\0';
	}
	recv_data_count = 0;			//表示把数据的记录位置清零,与BlueSendCnt功能相同，但需要整理
	BlueSendCnt=0;	
}


void PowerOnGprs()//初始化15S	    
{

	//Send_String(" &&PowerOnGprs()&& ");
	RESET_M26 = 0; 			//拉高
	delayms(1000);
	WDT_CONTR = 0x3E;		//feed dog
	RESET_M26 = 1; 			//拉低4s
	delayms(1000);
	delayms(1000);
	delayms(1000);
	WDT_CONTR = 0x3E;
	delayms(1000);
	delayms(1000);
	WDT_CONTR = 0x3E;
	RESET_M26 = 0; 			//拉高
	delayms(1000);
	delayms(1000);
	
	WDT_CONTR = 0x3E;
	delayms(1000);
	WDT_CONTR = 0x3E;
	delayms(1000);
	WDT_CONTR = 0x3E;
}




void Send_byte(INT8U odata)
{
 
    //ES=0;
    SBUF = odata;
    while (!TI);
    TI = 0;
   // ES=1;
}

/*************************************************
函数名称：Send_String
函数功能：串口1，发送一个不定长字符串
入口参数：发送字符串地址
返回参数：无
*************************************************/
void Send_String(INT8U *buf_addr)					//发送字符串，不带长度限制
{
    uchar *p;
	  p = buf_addr;
    while( *p != '\0' )
    {
        Send_byte(*p);
		p++;
    }
}

/*************************************************
函数名称：Send_String_Len
函数功能：串口1，发送一个定长字符串
入口参数：发送字符串地址
返回参数：无
*************************************************/
void Send_String_Len(INT8U *buf_addr,INT8U length)
{
    INT8U k;
    for (k=0;k<length;k++)
    {
        Send_byte(*(buf_addr+k));
    }
}

void Send_byte2(INT8U odata)
{
 
 	S2BUF = odata;
    while(!(S2CON&0x02));
	S2CON = S2CON & 0xFD;

}


void Send_String2(INT8U *buf_addr)
{
    uchar *p;
	  p = buf_addr;
    while( *p != '\0' )
    {
        Send_byte2(*p);
		p++;
    }
}
void Send_String2_Len(INT8U *buf_addr,INT8U length)
{
    INT8U k;
    for (k=0;k<length;k++)
    {
        Send_byte2(*(buf_addr+k));
    }
}
///////
void Timer0(void) interrupt 1  using 2	  //10ms
{
//  CNT1++;
//	CNT2++;
  TimeCount++;    	//计时
	//LightCount++;
	//UpLoadCount++;
	if(RecWaitFlag)		//串口1表示要进行判断
	{
		RecWaitCounter ++;
		if(RecWaitCounter >=20)		  //200ms，串口1接收完成标志
		{
			RecWaitFlag = FALSE;
			RecWaitCounter = 0;
			RecFinish = TRUE;
			MobilRECOK=1;				     //串口1接收完成标志
			//Send_String_Len(RxBuf,BlueGetCnt);
		}			
	}
	
	if(RecWaitFlag2)			//对串口2判断
  {
		RecWaitCounter2 ++;
		if (RecWaitCounter2 >=20)
   { 
		 RecWaitFlag2 = FALSE;
		 RecWaitCounter2=0; 
		 RecFinish2 = TRUE;
   }
  }
	if(DelayTimeCounter1 > 0)
			DelayTimeCounter1 --;	
	//if( PingStart == 1 )//开始心跳检测
	
	if(IsRecFlag==1){																//在接收到服务器的数据时，计时器清空，重新计时
		IsRecFlag=0;
		TimeCount=0;
		Second=0;
		Minute=0;
	}
	
	if( IsTcpSendFunc == 1 || (IsRecFlag ==0 && IsConnected == 1))				//进入TCPSENDdata2分钟或者没有收到数据3分钟，则重启
	{
		if( TimeCount >=100)//1S
		{
			TimeCount = 0;
			Second++;
			if( Second >=60 )
			{
				Second = 0;
				Minute++;
				if( Minute == 2 && IsTcpSendFunc == 1)		//TCP_SENDDATA持续2分钟，重启
				{
					Minute = 0;
					//PingFlag = 1;
					//QuitTcpFlag=1;
					
					IsTcpSendFunc =0;
					IAP_CONTR |= 0x20;		//重启					
				}
				else if( Minute == 3 && IsRecFlag == 0)//没有收到M26反馈3分钟，重启
				{
					Minute = 0;
			    IsRecFlag =0;
					IAP_CONTR |= 0x20;		//重启					
				}
				
			}
		}
	}
	else {				//否则重置计时
		TimeCount = 0;
		Second=0;
		Minute=0;
	}
}
///
//void uart_isr() interrupt   4 using 1	 //来着主板的读头信息,10 字节FE E1 TYPE NUM PAI1，PAI2
//{	uchar i;
//    if (RI)
//    {
// 	   RI=0;
//	   for(i=0;i<9;i++)
//			DuoBuf[i]=DuoBuf[i+1];
//		    DuoBuf[9]=SBUF;
//	   if((DuoBuf[0]==0xfe)&&(DuoBuf[1]==0xe1)&&(DuoBuf[3]!=0)&&(PaiRECOK==0))
//	     {
//	       
//		   PaiRECOK=1;
//		  }
//    }
//    else
//        TI=0;
//}	 

void uart_isr() interrupt   4 using 1	 //
{	//uchar i;
    if (RI)
    {
 	    RI=0;
			RxBuf[BlueGetCnt]=SBUF;	     //与recv_data相对
			temp[BlueGetCnt]=SBUF;			 //
			BlueGetCnt++;
			
			RecWaitFlag=TRUE;						//判断是否接收完毕			
			LED=~LED;	
			
			RecWaitCounter=0;				//只要接收到数据，等待计时就清零
			if( BlueGetCnt > MAX_RECV_LEN )//超过最大长度，重新开始
			{
				BlueGetCnt = 0;
			}			
    }
    else
        TI=0;
}

void uart_isr2(void) interrupt 8 using 1  //串口2，接收和发送M26
{	
   	if(S2CON&0x01)												//S2RI=1，接收来自M26的数据,reve_data
	{	
		S2CON = S2CON & 0xFE;	  //bit0
		RxBuf2[BlueSendCnt]=S2BUF;
		BlueSendCnt++;
		
		recv_data[recv_data_count] = S2BUF;
		recv_data_count++;
		RecWaitFlag2 = TRUE;
		RecWaitCounter2 = 0;
		//IsRecFlag=1;												//接收到M26的数据,改在Main-While中，只有服务器没有发送数据时才计时
		if( recv_data_count > MAX_RECV_LEN )//超过最大长度，重新开始
		{
			recv_data_count = 0;
		}
		if( BlueSendCnt > MAX_RECV_LEN )		//超过最大长度，重新开始
		{
			BlueSendCnt = 0;
		}
	}	   
//    else
//        S2CON = S2CON & 0xFd;
}
		

/////
void UartInit(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xE8;		//设定定时初值
	TH1 = 0xFF;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
//串口2,9600
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xE0;		//设定定时初值
	T2H = 0xFE;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
}

//void UartInit(void)		//115200bps@11.0592MHz
//{
//	SCON = 0x50;		//8位数据,可变波特率
//	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
//	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
//	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
//	TL1 = 0xFE;		//设定定时初值
//	TH1 = 0xFF;		//设定定时初值
//	ET1 = 0;		//禁止定时器1中断
//	TR1 = 1;		//启动定时器1
////串口2,9600
//	S2CON = 0x50;		//8位数据,可变波特率
//	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
//	T2L = 0xE8;		//设定定时初值
//	T2H = 0xFE;		//设定定时初值
//	AUXR |= 0x10;		//启动定时器2
//}

//////
void Timer0Init(void)		//10毫秒@11.0592MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xDC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}


/***********************************************************
函 数 名: SendCommand
函数功能: 向串口2发送一条命令
入口参数: command  存放命令的指针
		  stat  命令执行完后是否要清除接收缓存
返回参数: 命令的执行结果
***********************************************************/
uchar SendCommand(unsigned char *command, bit stat)
{
	uchar Result = FALSE;
	uchar LoopCount = 0;			    //循环发送的次数
	ClearRecBuff();					      //清零接收的缓存
	Send_String2( command );
	RecFinish2 = FALSE;
	//DelayTimeCounter1 = 100;		//延时20S,等待命令是否有响应
	DelayTimeCounter1 = 2000;
	delayms(200);
	while(LoopCount < 2)
	{
		WDT_CONTR = 0x3E;			 //feed dog

		if(RecFinish2 || (DelayTimeCounter1 == 0))	//表示已经接收完数据或延时时间到
		{
			RecFinish2 = FALSE;
//			//接收数据后，使用串口1转发到电脑,用于调试
//			if(recv_data[0]!='\0'){				
//				Send_String("-----");
//				Send_String(recv_data);
//				Send_String("-----\r\n");
//			}

			if(strstr(recv_data,"OK") != NULL)				//表示OK,数据返回正确
			{
//				Send_String("getOk\r\n");
				Result = TRUE;
				break;
			}						

			else if(DelayTimeCounter1 == 0)						//到达延时限定的时间,重新发送指令
			{
				ClearRecBuff();													//清零接收的缓存
				Send_String2(command); 
				DelayTimeCounter1 = 2000;
				LoopCount ++;
			}						
//			if(LoopCount >= 2 )
//			{
////				Send_String("LoopCount>=2,error");
//				//ClearRecBuff();			//062409注释
//			}
		}
	}
	RecFinish2 = FALSE;
	if(stat == CL_REC_BUF)			//表示要清零接收的缓冲
		ClearRecBuff();				//清零接收的缓存		 
	return Result;	
}
/********************************************************
函 数 名：InitGprs
函数功能：初始化GPRS模块
入口参数：无
返 回 值：初始化是否成功
********************************************************/
int InitGprs()
{	
	uchar result = FALSE;
	uchar i = 0;
	uchar icc_H=0x00;
	uchar icc_L=0x00;
//	Send_String(" **Before start** ");			//向电脑端发送调试信息
	//DelayMin(LoopTimes%5);//
START:
	DelayMin(LoopTimes%5);//
	LoopTimes++;
	if(LoopTimes >= 5)
	{	
		LoopTimes = 0;
		IAP_CONTR |= 0x20;		//重启
	}

	result = SendCommand("AT\r\n",CL_REC_BUF);						//自适应波特率
	
	result = SendCommand("AT\r\n",CL_REC_BUF);
	if( result )
	{
		result = SendCommand("AT+IPR=9600\r\n",CL_REC_BUF);		//设置固定波特率9600
	}
	delayms(100);
	if( result )
	{		
		result = SendCommand("AT&W\r\n",CL_REC_BUF);	
	}
	delayms(100);
	if( result )
	{
		result = SendCommand("AT+QIHEAD=0\r\n",CL_REC_BUF);			//增加头部信息	
	}
	delayms(100);
	//-----------------------------------------------------------------------------------------------
	if( result )
	{	
		while( (result = SendCommand("AT+CPIN?\r\n",CL_REC_BUF)) == FALSE )	//查询sim卡状态，有ok成功
		{
			 CheckSimTimes++;	
			 if( CheckSimTimes >= 5)										//10s钟未读到sim卡则重启
			 {
				CheckSimTimes = 0;
				goto START;		//重新初始化
			 }
		}														
	}
	delayms(100);

	//--------------------------------------------------------------------------------------------------------------------
	//20171017Start-增加-发送ICCID码
	if(result)
	{
	CheckSimTimes=0;
	while( 1 )	 
		{
			if( SendCommand("AT+QCCID\r\n",NO_CL_REC_BUF) == FALSE ) 	//查询SIM卡的ICCID码，30秒内查询失败，强制下一步
			{
				CheckSimTimes++;
				if( CheckSimTimes >= 3 )
				{
					CheckSimTimes = 0;
					break;
				}
			}
			else
			{
				for(i = 0;i<20 ;i++)
				{
					//iccid2[i]=recv_data[i+11];
					iccid[i+5]=recv_data[i+11];														//recv_data从第12位开始是iccid码		
				}
//				for(i = 0; i<20;i++)			//先取出并从ASCII转为标准的数值
//						{
//							if(iccid[i+5]>=48 && iccid[i+5] <= 57)					//将0-9和A-F的字符转成数值
//								iccid[5+i] = iccid[5+i]-48;
//							else if(iccid[i+5]>=65 && iccid[i+5]<= 70)
//								iccid[5+i] = iccid[i+5]-55;
//						}
//				for(i = 0;i<10 ;i++)
//				{
//					icc_H=iccid[5+2*i]<<4;
//					icc_L=iccid[6+2*i];
//					iccid_bak[i]=icc_H+icc_L;
//					//iccid_bak[i]=(((iccid[5+2*i])<<4)+(iccid[5+2*i+1])&0x0F);														//recv_data从第12位开始是iccid码		
//				}
				iccid[25]='\0';
//			iccid_bak[11]='\0';
//			Send_String(iccid);
				ClearRecBuff();
				break;
			}
		}	
		
	}
	//20171017End
	
	if( result )
	{
		while( 1 )	 
		{
			if( (result = SendCommand("AT+CREG?\r\n",CL_REC_BUF)) == FALSE )	 //查询GSM网络状态，30秒内GSM注册失败就重启
			{
				CheckGsmTimes++;
				if( CheckGsmTimes >= 6 )
				{
					CheckGsmTimes = 0;				
					goto START;		//重新初始化
				}
			}
			else
			{
				if( strstr(recv_data,"+CREG: 0,0") != NULL )	//没有注册成功，+CREG: 0,1-5成功
				{
					CheckGsmTimes++;
					if( CheckGsmTimes >= 6 )
					{						
						CheckGsmTimes = 0;
						goto START;	//重新初始化
					}
					else
						continue;
				}
				else
				{
					break;
				}
			}
		}
	}
	delayms(100);
	//--------------------------------------------------------------------------------------------------------------------
	if( result )
	{
		while( 1 )	 
		{
			if( SendCommand("AT+CGREG?\r\n",CL_REC_BUF) == FALSE ) 	//查询GPRS网络状态，30秒内查询失败，强制下一步
			{
				CheckGsmTimes++;
				if( CheckGsmTimes >= 6 )
				{
					CheckGsmTimes = 0;
					break;
				}
			}
			else
			{
				if( strstr(recv_data,"+CGREG: 0,0") != NULL )		//没有查询成功,+CGREG: 0,1-5成功
				{
					CheckGsmTimes++;
					if( CheckGsmTimes >= 6 )
					{
						CheckGsmTimes = 0;
						break;
					}
				}
				else
				{
					break;
				}	
			}
		}
	}
	delayms(20);
	//--------------------------------------------------------------------------------------------------------------------
	if( result )
	{
		result = SendCommand("AT+QIFGCNT=0\r\n",CL_REC_BUF);			//将context0设为前台context
	}
	//--------------------------------------------------------------------------------------------------------------------
	if( result )
	{
		result = SendCommand("AT+QICSGP=1,\"CMNET\"\r\n",CL_REC_BUF); 	//设置GPRS的APN,移动联通卡可不用	
	}
	delayms(20);
	//--------------------------------------------------------------------------------------------------------------------
	if( result )
	{
		while( 1 ) 
		{
			if(	(result = SendCommand("AT+QIREGAPP\r\n",NO_CL_REC_BUF)) == FALSE )	  //启动任务，180s未收到返回ok则重启
			{
				StartTaskTimes++;
				if( StartTaskTimes >= 12 ) 	//24s，这里设置为24秒，以加快速度
				{
					StartTaskTimes = 0;
					SendCommand("AT+QIDEACT\r\n",CL_REC_BUF);				
					goto START;				//重新初始化
				}
			}
			else
			{
				//SendCommand("AT+QISTAT\r\n",NO_CL_REC_BUF); 	//查询当前连接状态
				//delayms(300);			
//			Send_String("REGAPP-OK-STAT-");
				break;
			}
		}
	}
	delayms(200);
	//延时,经常连接GPRS失败--------------------------------------------------------------------------------------------------------------------
	if( result )
	{
		while( 1 ) 
		{
			if( (result = SendCommand("AT+QIACT\r\n",CL_REC_BUF)) == FALSE )//激活移动场景或发起gprs无线连接，3次失败则重启
			{
				ActivateTaskTimes++;
				delayms(1000);
				if( ActivateTaskTimes >= 2&& ActivateTaskTimes <4) 					//2次失败则尝试关闭ACT，并重新激活REGAPP。这种方法成功率较高
				{
					//ActivateTaskTimes = 0;
					SendCommand("AT+QIDEACT\r\n",NO_CL_REC_BUF);
					delayms(1000);
					delayms(1000);
					WDT_CONTR = 0x3E;
					delayms(1000);
					delayms(1000);
					delayms(1000);
					WDT_CONTR = 0x3E;
					SendCommand("AT+QIREGAPP\r\n",NO_CL_REC_BUF);
					delayms(1000);
					delayms(1000);
//					Send_String("--try -DEACT-RegAPP--");
					//goto START;		//重新初始化
				}
				else if( ActivateTaskTimes >= 4 ) 
				{
					ActivateTaskTimes = 0;
					goto START;		//重新初始
				}
			}
			else
			{
//				Send_String("---QIACT-OK---");
				break;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------
	if( result )
	{
		while(1)
		{
			ClearRecBuff();
			
			Send_String2("AT+QILOCIP\r\n");				//获取本地ip地址
			//SendCommand("AT+QILOCIP\r\n",NO_CL_REC_BUF);  //发送命令后不清空接收区，否则下面不能跳出循环
			delayms(1000);
			delayms(1000);
			RecFinish = FALSE;
			WDT_CONTR = 0x3E;
//			Send_String(recv_data);						//回显获取IP的结果
			if( strstr(recv_data,"AT+QILOCIP") != NULL ) //返回成功
			{
				ClearRecBuff();
				break;
			}
			else
			{
				InquryIpTimes++;
				if( strstr(recv_data,"10.") != NULL )				//可删除，一般获取的IP为10开头，重复验证是否获取成功
				{
						break;
				}
				if(	InquryIpTimes >= 5)					//查询5次
				{				
					InquryIpTimes = 0;
					if(SendCommand("AT+QIDEACT\r\n",CL_REC_BUF)) //关闭GPRS场景
						goto START;						//重新开GPRS
					else
						goto START;
				}
			}
		}
	}
		if( result )
	{
	 if(IPMODE==1) result = SendCommand("AT+QIDNSIP=0\r\n",CL_REC_BUF);			//设置连接方式，AT+QIDNSIP = 1为域名连接，0为IP连接
		  else result = SendCommand("AT+QIDNSIP=1\r\n",CL_REC_BUF);						//设置连接方式，1为域名连接，0为IP连接
	}
	delayms(200);
		if( result )
	{
		result = SendCommand("AT+QSCLK=0\r\n",CL_REC_BUF);			//关闭睡眠模式
	}
	delayms(200);
		if( result )
	{
		result = SendCommand("AT+QISDE=0\r\n",CL_REC_BUF); 	//发送数据不回显
	}
	
	//--------------------------------------------------------------------------------------------------------------------
    return result;
}



/********************************************************
函 数 名：TCP_Connection
函数功能：建立TCP连接
入口参数：无//IP地址，Port端口
返 回 值：连接是否成功
********************************************************/
uchar TCP_Connection()
{
	uchar result = FALSE;

	while(1)
	{
		ClearRecBuff();
		//Send_String2("AT+QIOPEN=\"TCP\",\"111.144.112.93\",\"6003\"\r\n"); 			//IP连接
		 if(IPMODE==0) 	Send_String2("AT+QIOPEN=\"TCP\",\"k1q7338645.iask.in\",\"30162\"\r\n");  //使用域名服务器
	  else Send_String2("AT+QIOPEN=\"TCP\",\"222.76.151.66\",\"6900\"\r\n");  //使用IP服务器
		delayms(1000);
		delayms(1000);
				
		RecFinish = FALSE;
		WDT_CONTR = 0x3E;
		if( strstr(recv_data,"CONNECT OK") != NULL )
		{
			Send_String("CONNECT OK\r\n");
			//向主板发送原始的ICCID码
			Send_String_Len(iccid,25);
			//Send_String("\r\n");
			//Send_String_Len(iccid2,20);
			//Send_String("\r\n");
			//Send_String_Len(iccid_bak,10);
			ClearRecBuff();
			ConnectionTimes = 0;
			result = TRUE;
			break;
		}
		//错误处理
		else if( strstr(recv_data,"ERROR") != NULL )					//1 ERROR	
		{
			SendCommand("AT+QIMUX?\r\n",NO_CL_REC_BUF);
			delayms(200);
			SendCommand("AT+QISTAT\r\n",NO_CL_REC_BUF); 		//查询当前连接状态
			if( strstr(recv_data,"+QIMUX: 1") != NULL )
			{
				SendCommand("AT+QIMUX=0\r\n",CL_REC_BUF);		//设为单连接
				SendCommand("AT+QISTAT\r\n",NO_CL_REC_BUF); 		//查询当前连接状态
			}			
							
			if(  strstr(recv_data,"IP INITAL") == NULL &&  strstr(recv_data,"IP STATUS") == NULL && strstr(recv_data,"IP CLOSE") == NULL )
			//
			{
				if( strstr(recv_data,"TCP CONNECTING") != NULL ) //TCP连接中
				{
					SendCommand("AT+QICLOSE\r\n",CL_REC_BUF); 	//关闭当前连接
				}
				else		         //PDP DEACT
				{
					SendCommand("AT+QIDEACT\r\n",CL_REC_BUF); 	//关闭GPRS场景
					delayms(1000);
				  break;
				}
			}
		}
		else if( strstr(recv_data,"ALREADY CONNECT") != NULL ) 		//2 ALREADY CONNECT
		{
			SendCommand("AT+QICLOSE\r\n",CL_REC_BUF); 			//关闭当前连接
		}
		else
		//------------------------------------------------------------------
		if( strstr(recv_data,"CONNECT FAIL") != NULL )			// CONNECT FAIL
		{
			if(	 strstr(recv_data,"TCP CONNECTING") != NULL )
			{
				SendCommand("AT+QICLOSE\r\n",CL_REC_BUF); 		//关闭当前连接
			}
			else
			{
				SendCommand("AT+QIDEACT\r\n",CL_REC_BUF);		//关闭GPRS场景
				delayms(1000);
				//break;
			}
		}


		//-----------------------------
		ConnectionTimes++;
		if(	ConnectionTimes >= 5)								//重连超过5次还不行
		{
			ConnectionTimes = 0;
			SendCommand("AT+QIDEACT\r\n",CL_REC_BUF); 			//关闭GPRS场景
			break;	
		}
	}
	IsConnected = result;
	return result;	
}

/********************************************************
函 数 名：TCP_SendData
函数功能：通过TCP发送数据
入口参数：data 发送的数据指针
返 回 值：发送是否成功
********************************************************/

uchar TCP_SendData(uchar *Data,uint Len)// 
{
	uchar result = TRUE;
	//1117：增加发送iccid后，长度由22增加为32
	uchar senddata[32];		//单独保存需要发送的数据，避免被AT指令的信息覆盖
	IsTcpSendFunc=1;
	for(count=0;count<Len;count++)
	{
		senddata[count]=Data[count];
	}
	senddata[Len]='\0';
	while(1)
	{
		
		RecFinish = FALSE;
		
		if(IsSendState==0)
	 {			
		 SendCommand("AT+QISTAT\r\n",NO_CL_REC_BUF); 									//查询当前连接状态
		 delayms(200);
		if( !((strstr(recv_data,"CONNECT OK") != NULL) || (strstr(recv_data,"TCP CONNECTING") != NULL)) )	//没有连接，尝试连接
		{		
			while(1) 			//连接失败，再次初始化
			{	
				if( TCP_Connection() == FALSE )
				{
					InitGprs();
				}
				else
				{
					LoopTimes = 0;
					break;
				}
			}
			SendCommand("AT+QISTAT\r\n",NO_CL_REC_BUF); 									//查询当前连接状态
		}	
	 }
		//当前连接成功
		delayms(200);
		if( (strstr(recv_data,"CONNECT OK") != NULL) || (strstr(recv_data,"TCP CONNECTING") != NULL) )
		{
			if(IsSendState==0){				
				if(Len == 21)		Send_String2("AT+QISEND=21\r\n"); 			    //发送定长数据
				else if(Len == 8) Send_String2("AT+QISEND=8\r\n"); 				  //发送心跳包
				else if(Len == 31) Send_String2("AT+QISEND=31\r\n"); 				//发送注册信息
				else 
				{
					result = FALSE;
					break;																			//如果不是正常数据或者心跳包，则抛弃
				}
				delayms(300);
				IsSendState=1;																//标志为1，不再发送AT指令
			}
			if( strstr(recv_data,">")!= NULL )			    		//开始接受发送数据
			{
				//-------------------------
				Send_String2_Len(senddata,Len);
				IsSendState=0;				
				delayms(300);							//
				if( strstr(recv_data,"SEND OK") != NULL )
				{
					ResponseFlag = 1;
					ClearRecBuff();
					break;
				}
				else break;
//				else	//否则，查看数据是否发送成功
//				{
//					if( Query_Send_Success() == TRUE )
//					{
//						SendDataTimes = 0;
//						break;
//					}
//				}
			}
		}
	 	//连接不成功 | 数据未发送成功
		else {
			SendDataTimes++;
			if( SendDataTimes >= 2 )
			{	
				SendDataTimes = 0;
				result = FALSE;
				IsSendState=0;
				break;
			}
		}
	}
	IsTcpSendFunc=0;
	return result;
}


//void SendPing()
//{
//	uchar ret = FALSE;
//	SendCommand("AT+QISTAT\r\n",NO_CL_REC_BUF); 	//查询当前连接状态
////	Send_String("Ping inqury ");
////	Send_String(recv_data);

//	if( strstr(recv_data,"IP CLOSE") != NULL || strstr(recv_data,"IP STATUS") != NULL)		//连接断开了
//	{	
////		Send_String("^^deact,try_reconnect^^\r\n");
//		TCP_Connection();			//尝试重新连接
//	}
//	else //否则，发送心跳数据
//	{
//		ClearRecBuff();
//		Send_String2("AT+QISEND=4\r\n"); 				  //发送定长数据
//		delayms(200);
//		//Send_String2("ping");
//		Send_String2(ping);
//		delayms(200);
//		if(strstr(recv_data,"SEND OK") != NULL)
//		{
//			ret=TRUE;
//		  ClearRecBuff();
//		}
//			else ret = FALSE;
//		if(ret == FALSE)
//		{
//			PingNormal--;
//			//Send_String("Send_Ping_Error,wait_next_time\r\n");
//			if( PingNormal == 0)
//			{
//				PingNormal = 5;
//				SendCommand("AT+QICLOSE\r\n",CL_REC_BUF); 	//关闭当前连接  IAP_CONTR |= 0x20;//重启
//			if(IPMODE==0) 	Send_String2("AT+QIOPEN=\"TCP\",\"k1q7338645.iask.in\",\"30162\"\r\n");  //使用域名服务器
//	    else Send_String2("AT+QIOPEN=\"TCP\",\"222.76.151.66\",\"6900\"\r\n");  //使用IP服务器
////				Send_String("Send_Ping_Failed_5times,try_reconnect_TCP \r\n");
//			}
//		}
//		else
//		{
//			  PingNormal = 5;
////			Send_String("Send_Ping_Success \r\n");
//		}
//	}
//}

  ////////////////////////
void security(void)
 {		uint temp16;
        uchar i;
		for (i=0;i<7;i++)
		 {
		 temp16=origin[i]*bbp[i]+origin[i];
	     SecuValue[i] =(uchar)temp16;
		 SecuValue[i]=SecuValue[i]^birthp[i];
		 }
 }
void read_password(void)
{
  uchar i;
  for(i=0;i<7;i++)
  {
   password[i]=EEPROM_read_byte(PASEE+i);
  }
}

void take_code(void)
{
	 //uchar idata *idata_point;
	 uchar code *idata_point;
	 uchar data i,tempp[7];

	 idata_point = 0x1ff9;
	 for(i=0;i<7;i++)
	 {
		 tempp[i] = *idata_point;	
	   origin[i] = tempp[i];
		 idata_point++;
	 }
}

////////////////
void main(void)
{
	
	P2M1=0;
	P2M0=0x80;
	P5M1=0;
	P5M0=0x10;
	LED=1;
	POWER=0;
	take_code(); //
	security();
	read_password();
	delayms(200);
	
	UartInit();
	Timer0Init();
	
	//启动M26模块
	RESET_M26=0;
	
	delayms(200); 
  delayms(1000);  	
	RESET_M26=1;
	
	delayms(1000); 			
	delayms(1000); 		
	delayms(1000); 	
	
	RESET_M26=0;	
	ET0=1;
	ES=1;
	IE2  |= 1;//启动串口2 
	EA=1;
//	Send_String_Len(password,7);
	delayms(1000);  
  delayms(1000); 
	
	//Send_String("____________programa start______________"); 
	
	if( InitGprs() == FALSE )
	{
		//Send_String(" ^^^ main restart ^^^ \r\n");
		IAP_CONTR |= 0x20;//重启
	}
	TCP_Connection();
//  iccid放在TCP_connection中发送
//	Send_String(iccid);
	
	PingStart = 1;
 while(1)
 {	 
   WDT_CONTR=0x3e;
	 delayms(100);
	 //将M26返回的内容，使用串口1发送到电脑
		 if (RecFinish2==1) {Send_String_Len(RxBuf2,BlueSendCnt);BlueSendCnt=0; RecFinish2=0;IsRecFlag=1;}		 		

	//将串口1接收的数据转发到串口2
		 if(MobilRECOK==1)											  			   //当串口1接收完数据，转发给串口2
		 {	
		   BlueGetCnt_old = BlueGetCnt;										 //记录当前串口1缓冲区的大小
			 //手动补充换行符
//			 temp[BlueGetCnt]='\r';
//			 temp[BlueGetCnt+1]='\n';
//			 temp[BlueGetCnt+2]='\0';			 
//			if(temp[0]=='A'&&temp[1]=='T')				         //支持输入AT指令，用于调试
//			 SendCommand(temp,1);
//			else 

			 TCP_SendData(temp,BlueGetCnt_old);							 //否则作为数据转发

			 delayms(200);
			 if(BlueGetCnt !=BlueGetCnt_old)								 //新进入数据，多为心跳包和数据在较短间隔内进入
			 {
          TCP_SendData(temp+BlueGetCnt_old,BlueGetCnt-BlueGetCnt_old);
			 }
			 for( count=0;count<BlueGetCnt+1;count++){
				 temp[count]='\0';
			 }			 

		   BlueGetCnt=0;
			 MobilRECOK=0;				//串口1接收完成标志
		 }		 
 }  //while
}//main