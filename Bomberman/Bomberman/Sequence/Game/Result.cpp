#include "GameLib/Framework.h"
#include "Sequence/Game/Result.h"
#include "Sequence/Game/Ready.h"
//#include "Sequence/Game/GameChild.h"
#include "Sequence/Game/GameParent.h"

#include "Font.h"
#include "Image.h"

namespace Sequence {
namespace Game {

class Child;

Result::Result(int result) : mCursor(0) {
  mImage = new Image("Image/result.dds"); 

	const char* strs[2] = { "Go To Title", "Continue" };
	Font::create("Image/font.dds",9,12,0xff,strs,2,13,false);
	initScreen(Font::instance());
	Font::instance()->drawString(9 - 1, 12, ">", 0xff);

	//Œ‹‰Ê”­•\
	mResult = result;
	Font::instance()->drawString(10, 10, "Winner : ", 0xff);
	if (result == 1)  Font::instance()->drawString(19, 10, "1P", 0xff);
	if (result == 2)  Font::instance()->drawString(19, 10, "2P", 0xff);

}

Result::~Result() {
  Font::destroy();
	SAFE_DELETE(mImage);
} 

void Result::initScreen(Font* font) {
  GameLib::Framework f = GameLib::Framework::instance();
  for (int i=0; i < f.height()*f.width(); i++) f.videoMemory()[i] = 0;
  mImage->drawScreen();
	const char* strs[2] = { "Go To Title", "Continue" };
	font->drawStrings(9, 12, 0xff, strs, 2, 13, false);
}

Child* Result::update(Parent* parent) {
  GameLib::Framework f = GameLib::Framework::instance();
	Child* next = this;

	if (f.isKeyTriggered('a') || f.isKeyTriggered('d')) {
	  mCursor = (mCursor + 1) % 2;
		initScreen(Font::instance());
		Font::instance()->drawString(9-1+ mCursor*13,12,">",0xff);

			Font::instance()->drawString(10, 10, "Winner : ", 0xff);
	if (mResult == 1)  Font::instance()->drawString(19, 10, "1P", 0xff);
	if (mResult == 2)  Font::instance()->drawString(19, 10, "2P", 0xff);
	}

	if (mCursor == 0) {
		if (f.isKeyTriggered(' ')) parent->moveTo(Parent::NEXT_TITLE);
	} else {
	  parent->initState();
		if (f.isKeyTriggered(' ')) next = new Ready(parent->getState());
	}
	return next;
}

}// namespace Sequence 
}//	namespace Game 