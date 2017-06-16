/*
 * im315xx_sample.cc
 *
 *  Created on: 2017/03/06
 *      Author: osmium
 */

#include "sample_conf.hpp"
#ifdef BUILD_4_SAMPLE

#include "../../rcsemi_lib/inc/im315xx_lpc1114.hpp"

#ifdef IM315_SAMPLE
int main()
{
	SystemCoreClockUpdate();
	unsigned int start_time;
	uint8_t data[64];
	int k;
	LPC_GPIO0->DIR |= 0x01 << 3;
	LPC_GPIO0->DATA &= ~(0x01 << 3);
	//	LPC_GPIO0->DATA ^= (0x01 << 3);

	RCsemi::IM315xx& im315 = *(RCsemi::IM315xx::getObject());
	RCsemi::IM315xxPacket_t packet;
	RCsemi::UARTPort& uport = *(RCsemi::UARTPort::getObject(RCsemi::UARTPort::UART0));

	RCsemi::SystemTimer&    tim   = *RCsemi::SystemTimer::getObject();

	RCsemi::TickingEntry::InitTimer();


	k = uport.RecieveLine(data,32);

	uport.Send(data,k);
	for(int i=4;i<12;i++)
		packet.byte[i] = i;

	while(true)
	{
		start_time = tim.counter();
		while(im315.sendPacket(packet));
		while(im315.recievePacket(&packet))
				LPC_GPIO0->DATA ^= (0x01 << 3);
		//++packet.by_name.data[7];
		//while(tim.counter() - start_time < 100000);
	}

	RCsemi::IM315xx::destroyObject();
}
#endif
#endif
