/*
 * uart_port.cc
 *
 *  Created on: 2016/10/14
 *      Author: osmium
 */
#include "../rcsemi_lib/inc/uart_port.hpp"

#include <LPC11xx.h>
#include <../RCsemiCommon/inc/ringbuf.hpp>
#include <math.h>
#include "../rcsemi_lib/inc/RCsemi_typedef.hpp"

namespace RCsemi
{
UARTPort* UARTPort::getObject(UARTPort::PortList port)
{
	if(!instance_)instance_ = new UARTPort(port);
	return instance_;
}

UARTPort::UARTPort(UARTPort::PortList prt)
{
	using namespace uart_port_conf;
	timeout_10usec_ = 0;
	timer_ = SystemTimer::getObject();
	tick_counter_ = 0;
	switch(prt)
	{
	case UART0:
		port_ = LPC_UART;

		LPC_IOCON->PIO1_6 &= ~0x07;
		LPC_IOCON->PIO1_6 |= 0x01;

		LPC_IOCON->PIO1_7 &= ~0x07;
		LPC_IOCON->PIO1_7 |= 0x01;

		LPC_SYSCON->SYSAHBCLKCTRL |= (0x01UL << 12);

		LPC_SYSCON->UARTCLKDIV    =  (0x01UL);

		uart_port_bufs::UARTRcvBuf0 = recieve_buf_ = new RingBuf(kBufSize);
		uart_port_bufs::UARTSndBuf0 = send_buf_    = new RingBuf(kBufSize);
		break;
	}
	port_->IER |= 0x03;
	port_->FCR |= 0x07;
	port_->LCR |= 0x03;
	NVIC_EnableIRQ(UART_IRQn);
}

int UARTPort::Recieve(uint8_t* data,const int max)
{
	int i=0;
	int ptr=0,recieved_bytes_num = 0;
	unsigned int start_time = timer_->counter();
	for(i=0;recieved_bytes_num < max;i++)
	{
		if((port_->IIR & 0x01) && (port_->LSR & 0x01))
		{
			LockInterrupt(this->RBR_interrupt_);
			while(port_->LSR & 0x01)
			{
				recieve_buf_->PushData((uint8_t)port_->RBR);
			}
			UnlockInterrupt(this->RBR_interrupt_);
		}
		LockInterrupt(this->RBR_interrupt_);
		recieved_bytes_num += recieve_buf_->GetData(data+ptr,max-recieved_bytes_num);
		UnlockInterrupt(this->RBR_interrupt_);

		ptr = recieved_bytes_num;
		if((timer_->counter()-start_time) >= timeout_10usec_)
			break;
	}
	return recieved_bytes_num;
}

int UARTPort::RecieveLine(void* data,const int max_bytes)
{
	unsigned int start_time = timer_->counter();
	uint8_t* p_data = (uint8_t*)data;
	int num_of_data=0;
	while(timer_->counter()-start_time <= timeout_10usec_
			&& num_of_data <= max_bytes)
	{
		if(!Recieve(p_data,1)) continue;
		++p_data;
		if(*(p_data-1) == '\r' || *(p_data-1) == '\n') break;
		num_of_data++;
	}
	*p_data = '\0';
	return num_of_data;
}

int UARTPort::Send(const void* data,const int num_of_data_bytes)
{
	const uint8_t* p_data = (uint8_t*)data;
	uint8_t tmp;
	if(num_of_data_bytes > send_buf_->emptyBytes())
	{
		return 1;
	}
	else
	{
		LockInterrupt(THRE_interrupt_);
		send_buf_->PushData(p_data,num_of_data_bytes);
		UnlockInterrupt(THRE_interrupt_);

		if((port_->IIR & 0x1) && (port_->LSR & (0x01 << 5)) )
		{
			LockInterrupt(THRE_interrupt_);
			if(send_buf_->GetData(&tmp))
			{
				LPC_UART->THR = tmp;
			}
			UnlockInterrupt(THRE_interrupt_);
		}
		return 0;
	}

}

//See UM!!
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


void UARTPort::LockInterrupt(UARTPort::UARTInterruptType_t interrupt_type)
{
	switch(interrupt_type)
	{
	case RBR_interrupt_:
		port_->IER &= ~0x01;
		break;
	case THRE_interrupt_:
		port_->IER &= ~0x02;
	}
}

void UARTPort::UnlockInterrupt(UARTPort::UARTInterruptType_t interrupt_type)
{
	switch(interrupt_type)
	{
	case RBR_interrupt_:
		port_->IER |= 0x01;
		break;
	case THRE_interrupt_:
		port_->IER |= 0x02;
	}
}

UARTPort* UARTPort::instance_ = 0;

}

extern "C"
{
void UART_IRQHandler(void)
{
	using namespace RCsemi::uart_port_bufs;
	uint8_t tmp;
	switch(((LPC_UART->IIR) >> 1) & 0x7)
	{
	case 0x02:
		if(UARTRcvBuf0)
		{
			UARTRcvBuf0->PushData((uint8_t)(LPC_UART->RBR));
		}
		else tmp = (uint8_t)LPC_UART->RBR;
		break;
	case 0x01:
		if(UARTSndBuf0)
		{
			if(UARTSndBuf0->GetData(&tmp))
			{
				LPC_UART->THR = tmp;
			}
		}
		break;
	}
}
}
