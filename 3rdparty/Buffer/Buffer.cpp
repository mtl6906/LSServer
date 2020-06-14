#include "Buffer.h"
#include "string"
#include "cstring"

Buffer::Buffer(int maxBufferSize) : buffer(new char[maxBufferSize + 1]), offset(0), buffersize(0), maxBufferSize(maxBufferSize) {}

int Buffer::Size()
{
	return buffersize - offset;
}

int Buffer::RestSize()
{
	return maxBufferSize - buffersize;
}

char *Buffer::Begin()
{
	return buffer + offset;
}

char *Buffer::End()
{
	return buffer + buffersize;
}

int Buffer::Push(const char *data, int len)
{
	if(len > RestSize())
		len = RestSize();
	strncpy(End(), data, len);
	buffersize += len;
	return len;
}

int Buffer::Push(Buffer &o, int len) {
	if(len > o.Size())
		len = o.Size();
	Push(o.Begin(), len);
	o.offset += len;
	return len;
}

int Buffer::Push(const std::string &data)
{
	return Push(data.c_str(), data.size());
}
int Buffer::Pop(char *data, int len) {
	if(len > Size())
		len = Size();
	strncpy(data, Begin(), len);
	offset += len;
	return len;
}
int Buffer::Pop(std::string &data)
{	
	if(data.size() > Size())
		data.resize(Size());
	strncpy((char *)data.c_str(), Begin(), data.size());
	offset += data.size();
	return data.size();
}
int Buffer::Pop(Buffer &o, int len)
{
	if(len > o.RestSize())
		len = o.RestSize();
	len = Pop(o.End(), len);
	o.buffersize += len;
	return len;
}
int Buffer::Empty()
{
	return buffersize == offset;
}
int Buffer::FindFirstOf(const char *mark, int size)
{
	char *ptr = Begin();
	char *pmark = (char *)mark;
	while(ptr != End()) {
		for(int i=0;i<size;++i)
			if(*ptr == *(pmark + i))
				return ptr - Begin();
		++ptr;
	}
	return -1;
}
int Buffer::Find(const char *mark, int size)
{
	char *ptr = Begin();
	while(ptr != End())
	{
		if(strncmp(mark, ptr, size) == 0)
			return ptr - Begin();
		++ptr;
	}
	return -1;
}
int Buffer::Move()
{
	char *ptr1 = buffer, *ptr2 = Begin();
	while(ptr2 != End())
		*ptr1++ = *ptr2++;
	return 0;
}
int Buffer::MoveBuffersize(int n)
{
	if(buffersize + n >= maxBufferSize)
		buffersize = maxBufferSize;
	else if(buffersize + n < offset)
		buffersize = offset;
	else
		buffersize += n;
	return 0;
}
int Buffer::MoveOffset(int n)
{
	if(offset + n < 0)
		offset = 0;
	if(offset + n > buffersize)
		offset = buffersize;
	else
		offset += n;
	return 0;
}
int Buffer::ClearWhiteSpace(int len)
{
	if(len > Size())
		len = Size();
	int whitespace = 0;
	char *st = Begin(), *ed = Begin() + len;
	bool in_string = false;
	while(st != ed) {
		*st = *(st + whitespace);
		if(*st == '\"') {
		       	in_string == !in_string;
		}
		else if(!in_string && (*st == ' ' || *st == '\n' || *st == '\r' || *st == '\t') ) {
			++whitespace;
			continue;
		}
		++st;
	}
	return whitespace;
}
void Buffer::Clear()
{
	offset = buffersize = 0;	
}
