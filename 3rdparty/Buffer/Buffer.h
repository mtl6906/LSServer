#ifndef BUFFER_H
#define BUFFER_H

#include "string"

class Buffer {
	char *buffer;
	int offset;
	int buffersize;
	int maxBufferSize;
public:
	Buffer(int maxBufferSize = 1 << 20);
	int Push(const char *data, int len);
	int Push(Buffer &o, int len);
	int Push(const std::string &data);
	int Pop(char *data, int len);
	int Pop(Buffer &o, int len);
	int Pop(std::string &data);
	char *Begin();
	char *End();
	int Size();
	int RestSize();
	int Empty();
	int Find(const char *mark, int size);
	int FindFirstOf(const char *mark, int size);
	int Move();
	int MoveBuffersize(int n);
	int MoveOffset(int n);
	int ClearWhiteSpace(int len);
	void Clear();
};

#endif
