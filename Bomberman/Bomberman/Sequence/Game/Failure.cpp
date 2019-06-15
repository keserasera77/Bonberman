#include "GameLib/Framework.h"
#include "Sequence/Game/Failure.h"
#include "Sequence/Game/Ready.h"
#include "Sequence/GameOver.h"
#include "Sequence/Game/GameChild.h"
#include "Sequence/Game/GameParent.h"

#include "Font.h"
#include "Image.h"
#include <string>

namespace Sequence {
namespace Game {

Failure::Failure(Parent* parent) {
	mImage = new Image("Image/failure.dds");

	mImage->drawScreen();
  Font::create("Image/font.dds");
	std::string str = "Life : " + (std::to_string(parent->life())) + " -> " + (std::to_string(parent->life()-1));
	Font::instance()->drawString(22,9,str.c_str(), 0xff0000);
	Font::destroy();
}

Failure::~Failure() {
	SAFE_DELETE(mImage);

}

Child* Failure::update(Parent* parent) {
  GameLib::Framework f = GameLib::Framework::instance();
	Child* next = this;

	if (f.isKeyTriggered(' ')){
	if (parent->life() <= 1) {
	  parent->moveTo(Parent::NEXT_GAMEOVER);
	}else {
		parent->initState();
	  parent->decrementLife();
	  next = new Ready(parent->getState());
	}
	}

	return next;
}

}// namespace Sequence 
}//	namespace Game 