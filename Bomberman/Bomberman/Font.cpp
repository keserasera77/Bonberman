#include "GameLib/GameLib.h"
#include "Image.h"

#include "Font.h"

Font* Font::mInstance = 0;

Font::Font(const char* filename) : mX(0), mY(0), mColor(0), mDirection(0), mInterval(0), mMaxChoices(0),mChoices(0) {
  mImage = new Image(filename);
}

Font::Font(const char* filename,int x, int y, unsigned color, const char** strs, int numOfstrs, int interval, bool dir ) {
	mImage = new Image(filename);
	mChoices = strs;
	mMaxChoices = numOfstrs;
	mInterval = interval;
	mDirection = dir;
	mX = x; mY = y;
	mColor = color;
}

Font::~Font() {
  SAFE_DELETE(mImage);
}

Font* Font::instance() {
	return mInstance;
}

void Font::create(const char* filename) {
  STRONG_ASSERT(!mInstance && "Font::create");  //二個目はダメ
  mInstance = new Font(filename);
}

void Font::create(const char* filename, int x, int y, unsigned color, const char** strs, int numOfstrs, int interval, bool dir) {
	STRONG_ASSERT(!mInstance && "Font::create2");  //二個目はダメ
	mInstance = new Font(filename,x,y,color,strs,numOfstrs,interval,dir);
}

void Font::destroy() {
  ASSERT(mInstance && "Font::destroy"); //余計なデリート
	SAFE_DELETE(mInstance);
}

void Font::drawString(int x, int y, const char* str, unsigned color) const{
	for (int i = 0; str[i] != '\0'; i++) {
	  int srcX = ((str[i] - ' ') % 10)*charWidth, srcY = ((str[i] - ' ') / 10)*charHeight;
		mImage->drawWithReplacementColor((x+i)*charWidth, y*charHeight, srcX, srcY, charWidth, charHeight, color);
	}
}

//オリジナル
void Font::drawStrings(int x, int y, unsigned color, const char** strs, int numOfStrs, int interval, bool dir) const{
	for (int i = 0; i < numOfStrs; i++) {
		drawString(x,y,strs[i],color);
		if (dir) y += interval;
		else x += interval;
	}
}

//なぜか使えない(mChoices[i] の2回目(コンストラクタ内ではなくupdate内)に読み込みでエラーが起こる(drawString内))
void Font::drawStrings() const {
  //GameLib::cout << "in drawStrings()!!" << GameLib::endl;
  int x = mX, y = mY;
	for (int i = 0; i < mMaxChoices; i++) {
		drawString(x, y, mChoices[i], mColor);
		if (mDirection) y += mInterval;
		else x += mInterval;
	}
}



