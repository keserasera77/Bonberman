#include "GameLib/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "File.h"

#include <fstream>

Image::Image(const char* filename) {
	//画像の読み込み
	File file(filename);
	ASSERT(file.data()&&filename&&" is not found");
	mHeight = file.getUnsigned(12);
	mWidth = file.getUnsigned(16);
	unsigned* img = new unsigned[mWidth * mHeight];
	for (int i = 0; i < mWidth * mHeight; i++) img[i] = file.getUnsigned(128 + i * 4);
	mData = img;
};

Image::~Image() {
  SAFE_DELETE_ARRAY(mData);
}

void Image::drawPicture(
	int dstX, int dstY,
	int srcX, int srcY,
	int width, int height) const {
	Framework f = Framework::instance();
	unsigned* vram = f.videoMemory();
	int windowHeight = f.height();
	int windowWidth = f.width();

	//画像の位置をsrcX,srcYで特定
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			//画面範囲チェック
			if ((y + dstY) * windowWidth + (x + dstX) < 0) break;
			if ((y + dstY) * windowWidth + (x + dstX) >= windowHeight * windowWidth) break;

			unsigned src = mData[(y + srcY) * mWidth + (x + srcX)];
			unsigned* dst = &vram[(y + dstY) * windowWidth + (x + dstX)];
			unsigned srcA = (src & 0xff000000) >> 24;
			unsigned srcR = src & 0xff0000;
			unsigned srcG = src & 0x00ff00;
			unsigned srcB = src & 0x0000ff;
			unsigned dstR = *dst & 0xff0000;
			unsigned dstG = *dst & 0x00ff00;
			unsigned dstB = *dst & 0x0000ff;
			unsigned r = (srcR - dstR) * srcA / 255 + dstR;
			unsigned g = (srcG - dstG) * srcA / 255 + dstG;
			unsigned b = (srcB - dstB) * srcA / 255 + dstB;
			*dst = (r & 0xff0000) | (g & 0x00ff00) | b;
		}
	}
}

void Image::drawWithReplacementColor(
	int dstX, int dstY,
	int srcX, int srcY,
	int width, int height,
	unsigned color) const {
	Framework f = Framework::instance();
	unsigned* vram = f.videoMemory();
	int windowHeight = f.height();
	int windowWidth = f.width();

	unsigned srcR = color & 0xff0000;
	unsigned srcG = color & 0x00ff00;
	unsigned srcB = color & 0x0000ff;

	//画像の位置をsrcX,srcYで特定
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			//画面範囲チェック
			if ((y + dstY) * windowWidth + (x + dstX) >= windowHeight * windowWidth) break;

			unsigned src = mData[(y + srcY) * mWidth + (x + srcX)];
			unsigned* dst = &vram[(y + dstY) * windowWidth + (x + dstX)];
			unsigned srcA = 0xff - (src & 0xff); 
			unsigned dstR = *dst & 0xff0000;
			unsigned dstG = *dst & 0x00ff00;
			unsigned dstB = *dst & 0x0000ff;
			unsigned r = (srcR - dstR) * srcA / 255 + dstR;
			unsigned g = (srcG - dstG) * srcA / 255 + dstG;
			unsigned b = (srcB - dstB) * srcA / 255 + dstB;
			*dst = (r & 0xff0000) | (g & 0x00ff00) | b;
		}
	}
}



void Image::drawScreen() {
	Framework f = Framework::instance();
	drawPicture(0, 0, 0, 0, f.width(), f.height());
}