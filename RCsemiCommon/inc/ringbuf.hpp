/*
 * rcsemi_ringbuf.hpp
 *
 *  Created on: 2016/10/14
 *      Author: osmium
 */
 #include <stdint.h>

#ifndef INCLUDE_RCSEMI_RINGBUF_HPP_
#define INCLUDE_RCSEMI_RINGBUF_HPP_
namespace RCsemi
{
class RingBuf
{
private:
	uint8_t* buf_;
	int      last_;
	int      ptr_;
	int      size_;
	int      remain_;
	bool is_external_buf_;
public:
	RingBuf(int size);
	RingBuf(uint8_t* buf,int size);
	int PushData(uint8_t data);
	int GetData(uint8_t* data,int max);
	~RingBuf();
};
}

#endif /* INCLUDE_RCSEMI_RINGBUF_HPP_ */
