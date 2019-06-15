#include "GameLib/Framework.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Game.h"
#include "Sequence/Title.h"
#include "Sequence/Game/GameChild.h"

#include "Font.h"
#include "Image.h"
#include <Sequence\Game\GameParent.h>

namespace Sequence {
namespace Game {

Pause::Pause() : mCursor(0) {
	mImage = new Image("Image/pause.dds");
	Font::create("Image/font.dds");
	initScreen(Font::instance());
	Font::instance()->drawString(1, 5, ">", 0x0000ff);
}

Pause::~Pause() {
  Font::destroy();
	SAFE_DELETE(mImage);
}

void Pause::initScreen(Font* font) {
  GameLib::Framework f = GameLib::Framework::instance();
  for (int i = 0; i < f.height()*f.width(); i++) f.videoMemory()[i] = 0;
	mImage->drawScreen();

	const char* choices[2] = {"Return", "Go Title"};
	font->drawStrings(2,5, 0x0000ff, choices,2,1, true);
}

Child* Pause::update(Parent* parent) {
  GameLib::Framework f = GameLib::Framework::instance();
  Child* next = this;

	if (f.isKeyTriggered('w') || f.isKeyTriggered('s')){
	  mCursor = (mCursor+1) % 2;
		initScreen(Font::instance());
		Font::instance()->drawString(1, 5 + mCursor * 1, ">", 0x0000ff);
  }

	if (mCursor == 0) {
		if (f.isKeyTriggered(' ')) next = new Game;
	} else {
		if (f.isKeyTriggered(' ')) { 
		  parent->moveTo(Parent::NEXT_TITLE);
		}
	}

	return next;
}
 
}// namespace Sequence 
}//	namespace Game 