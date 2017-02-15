/*
 * uart_port.cc
 *
 *  Created on: 2016/10/14
 *      Author: osmium
 */
#include <LPC11xx.h>
#include <RCsemi_typedef.hpp>
#include <../RCsemiCommon/inc/ringbuf.hpp>
#include <uart_port.hpp>
#include <math.h>

namespace RCsemi
{
UARTPort::UARTPort(UARTPort::PortList prt)
{
	using namespace uart_port_bufs;
	using namespace uart_port_conf;
	timeout_10usec_ = 0;
	timer_ = SystemTimer::getObject();
	switch(prt)
	{
	case UART0:
		port_ = LPC_UART;
		if(UARTRcvBuf0)
		{
			buf_ = UARTRcvBuf0;
		}
		else
		{
			buf_ = new RingBuf(kBufSize);
			UARTRcvBuf0 = buf_;
		}
		break;
	}
}

int UARTPort::Recieve(uint8_t* data,const int max)
{
	int i=0;
	int ptr=0,recieved_bytes_num = 0;
	unsigned int start_time = timer_->counter();
	for(i=0;recieved_bytes_num < max;i++)
	{
		recieved_bytes_num += buf_->GetData(data+ptr,max-recieved_bytes_num);
		ptr = recieved_bytes_num;
		if((timer_->counter()-start_time) >= timeout_10usec_)
			break;
	}
	return recieved_bytes_num;
}

int UARTPort::SetBaud(int PCLK,int BR)
{
		uint8_t MULVAL    = 1;
		uint8_t DIVADDVAL = 0;
		int tmp;
		int cnt;
		float FRest;
		float DLest;

		DLest = ((float)PCLK)/(16*BR);
		if((float)((int)DLest) == DLest)
		{
			MULVAL   = 1;
			DIVADDVAL= 0;
		}
		else
		{
			FRest=1.5;
			DLest=(int)(PCLK/(16*BR*FRest));
			FRest=PCLK/(16*BR*DLest);
			if(FRest > 1.9 || FRest < 1.1)
			{
				for(FRest=1.1;FRest > 1.9 || FRest < 1.1;FRest += 0.1)
				{
					if(FRest > 1.9) return -1;
					DLest=(int)(PCLK/(16*BR*FRest));
					FRest=PCLK/(16*BR*DLest);
				}
			}
			for(cnt=1;cnt<=15;cnt++)
			{
				tmp=((FRest-1)*cnt-((int)((FRest-1)*cnt)) < 0.5) ? ((int)((FRest-1)*cnt)):((int)((FRest-1)*cnt)+1);
				if(fabs((FRest-1)-(float)tmp/cnt) < (fabs((FRest-1)-(float)DIVADDVAL/MULVAL)))
				{
					DIVADDVAL=tmp;
					MULVAL=cnt;
				}

			}
		}


		port_->LCR |= (0x1UL << 7);
		port_->DLL  = 0xffUL & (int)DLest;
		port_->DLM  = 0xffUL & (((int)DLest) >> 8);
		port_->FDR &= ~0xffUL;
		port_->FDR |= (MULVAL << 4) | (DIVADDVAL & 0x0fUL);
		port_->LCR &= ~(0x1UL << 7);

		return 0;

}

int UARTPort::setTimeout(int timeout_10usec)
{
	return timeout_10usec_ = timeout_10usec;
}

}

extern "C"
{
void UART_IRQHandler(void)
{
	using namespace RCsemi::uart_port_bufs;
	if((((LPC_UART->IIR) >> 1) & 0x7) == 0x2)
	{
		if(UARTRcvBuf0)
		{
			UARTRcvBuf0->PushData((uint8_t)(LPC_UART->RBR));
		}
	}
}
}
