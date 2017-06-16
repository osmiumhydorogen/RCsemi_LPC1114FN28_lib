/*
 * rcsemi_ringbuf.cc
 *
 *  Created on: 2016/10/18
 *      Author: osmium
 */

#include "../inc/ringbuf.hpp"

namespace RCsemi
{
RingBuf::RingBuf(int size)
{
	size_ = size;
	buf_ = new uint8_t[size];
	ptr_ = 0;
	remain_ = 0;
	last_ = size-1;
	is_external_buf_ = false;
	accept_overwrite_ = false;
}

RingBuf::RingBuf(uint8_t* buf,int size)
{
	buf_    = buf;
	size_   = size;
	last_   = size-1;
	ptr_    = 0;
	remain_ = 0;
	is_external_buf_ = true;
	accept_overwrite_ = false;
}

int RingBuf::PushData(uint8_t data)
{
	int target_ptr = 0;
    static bool first_push = true;
	if((last_+1) < size_)
	{
		target_ptr = last_+1;
	}
	else
	{
		last_ = 0;
		target_ptr  = 0;
	}

	//オーバーラン検出
	//if(!first_push && target_ptr == ptr_)
	if(remain_ && target_ptr == ptr_)
	{
		if(!accept_overwrite_)
			return 0;
		else
			++ptr_;
	}
	else
	{
		++remain_;
		buf_[target_ptr] = data;
	}

    first_push = false;

    last_ = target_ptr;
	return 1;
}

int RingBuf::PushData(const uint8_t* data,int nbytes)
{
	for(int i=0;i<nbytes;i++)
	{
		if(!PushData(data[i]))return 0;
	}
	return 1;
}

int RingBuf::GetData(uint8_t* data,int max_nbytes)
{
	int ptr = ptr_,num_of_data = 0;
	for(num_of_data = 0;(num_of_data < max_nbytes) && remain_;remain_--)
	{
		data[num_of_data++] = buf_[ptr++];
		if(ptr >= size_)
			ptr = 0;
	}
	ptr_ = ptr;
	return num_of_data;
}

int RingBuf::emptyBytes() const
{
	return size_ - remain_;
}

void RingBuf::acceptOverwrite(bool accept_overwrite)
{
	accept_overwrite_ = accept_overwrite;
}

RingBuf::~RingBuf()
{
	if(!is_external_buf_)
		delete[] buf_;
}
}

