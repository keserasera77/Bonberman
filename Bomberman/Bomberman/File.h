#pragma once

class File{
public:
	File(const char* filename);
	~File();
	int size() const;
	char* data() const;
	unsigned getUnsigned(int i);
private:
  int mSize;
  char* mData;
};

