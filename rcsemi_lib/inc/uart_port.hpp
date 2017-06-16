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
#include <rcsemi_system_timer.hpp>
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
	static RingBuf* UARTSndBuf0 = 0;
}

class UARTPort
{
public:
	enum PortList
	{
		UART0
	};
	LPC_UART_TypeDef* port_;
	int setTimeout(int timeout_10usec);
	int SetBaud(int PCLK,int BR);

	/*
	 * dataをnum_of_data_bytes[bytes]だけ送る
	 * 一度に遅れるデータは最大uart_port_conf::kBufSize[bytes]まで
	 * 返り値:
	 * 	0:成功 1:バッファオーバーランしそうなので何も送らなかった
	 */
	int Send(const void* data,const int num_of_data_bytes);

	/*
	 * dataにmax_recieve_bytes[bytes]までデータを受け取ろうとする
	 * 返り値:
	 * 	受け取ったデータの数[bytes]
	 */
	int Recieve(uint8_t* data,const int max_recieve_bytes);

	/*
	 * データを1byte受け取って返す
	 */
	uint8_t RecieveByte();

	/*
	 * 改行コードが来るか受け取ったデータ数がmax_recieve_bytesになるまでデータを受信する
	 * 改行コードは捨てられて,'\0'が挿入される
	 *
	 *返り値:
	 *	受け取ったデータ数('\0'は含まない)
	 */
	int RecieveLine(void* data,const int max_recieve_bytrs);

	~UARTPort(){};
	static UARTPort* getObject(PortList port);

	void _tick();

	int tick_counter_;
private:
	RingBuf* recieve_buf_;
	RingBuf* send_buf_;
	SystemTimer* timer_;
	unsigned int timeout_10usec_;
	UARTPort(PortList prt);
	static UARTPort* instance_;
	enum UARTInterruptType_t
	{
		RBR_interrupt_,
		THRE_interrupt_
	};
	void LockInterrupt(UARTPort::UARTInterruptType_t interrupt_type);
	void UnlockInterrupt(UARTPort::UARTInterruptType_t interrupt_type);
};
}

#endif /* INCLUDE_UART_PORT_HPP_ */
