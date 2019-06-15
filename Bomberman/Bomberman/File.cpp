#include "GameLib/GameLib.h"
#include "File.h"

#include <fstream>

using namespace std;

File::File(const char* filename) : mSize(0), mData(0) {
  int size;
	char *buffer = 0;
	ifstream in(filename,ifstream::binary /* 超重要！！！ */);
	if (!in) {
		buffer = 0;
		size = 0;
	} else {
		in.seekg(0, ifstream::end);
		size = static_cast<int>(in.tellg());
		in.seekg(0, ifstream::beg);
		buffer = new char[size];
		in.read(buffer, size);
		mSize = size;
		mData = buffer;
	}
}


File::~File(){
}

int File::size() const {
	return mSize;
}

char* File::data() const {
	return mData;
}

//unsigned取り出し
unsigned File::getUnsigned(int i) {
	const unsigned char* up;
	up = reinterpret_cast<const unsigned char*>(mData);
	ASSERT(mData != 0);
	unsigned r = up[i];
	r |= up[i+1] << 8;
	r |= up[i+2] << 16;
	r |= up[i+3] << 24;
	return r;
};