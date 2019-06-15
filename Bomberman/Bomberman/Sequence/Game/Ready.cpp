#include "GameLib/Framework.h"
#include "Sequence/Game/Ready.h"
#include "Sequence/Game/Game.h"
//#include "Sequence/Game/GameChild.h"

#include "State.h"
#include "Image.h"

namespace Sequence{
namespace Game {

class Child;

Ready::Ready(State* state) {
	GameLib::Framework f = GameLib::Framework::instance();
	for (int i = 0; i < f.height() * f.width(); i++) f.videoMemory()[i] = 0;

  mImage = new Image("Image/ready.dds");
	state->drawStage();
	mImage->drawScreen();
	mImageGo = new Image("Image/go.dds");
}

Ready::~Ready() {
	SAFE_DELETE(mImage);
}

Child* Ready::update(Parent* parent) {
  Child* next = this;
	if (GameLib::Framework::instance().isKeyTriggered(' ')) {
	  mImageGo->drawScreen(); // ‘S‘RŒ©‚¦‚È‚¢
		next = new Game;
	}

	return next;
}

}// namespace Sequence 
}//	namespace Game 