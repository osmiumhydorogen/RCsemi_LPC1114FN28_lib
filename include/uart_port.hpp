/*
 * uart_port.hpp
 *
 *  Created on: 2016/10/14
 *      Author: osmium
 */

#ifndef INCLUDE_UART_PORT_HPP_
#define INCLUDE_UART_PORT_HPP_

#include <LPC11xx.h>
#include <../RCsemiCommon/inc/ringbuf.hpp>
#include <RCsemi_typedef.hpp>
#include <stdint.h>

namespace RCsemi
{

namespace uart_port_conf
{
	const uint32_t kBufSize = 64;
}

namespace uart_port_bufs
{
	static RingBuf* UARTRcvBuf0 = 0;
}

class UARTPort
{
private:
	RingBuf* buf_;
	SystemTimer* timer_;
	int timeout_10usec_;
public:
	enum PortList
	{
		UART0
	};
	UARTPort(PortList prt);
	LPC_UART_TypeDef* port_;
	int setTimeout(int timeout_10usec);
	int SetBaud(int PCLK,int BR);
	int Send(const uint8_t* data,const int num);
	int Recieve(uint8_t* data,const int max);
	uint8_t RecieveByte();
	~UARTPort();
};

}

#endif /* INCLUDE_UART_PORT_HPP_ */
