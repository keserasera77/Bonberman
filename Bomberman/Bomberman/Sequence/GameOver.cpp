#include "GameLib/Framework.h"
#include "Sequence/GameOver.h"
#include "Sequence/Title.h"

#include "Image.h"

namespace Sequence {

	GameOver::GameOver() {
		mImage = new Image("Image/gameover.dds");
		mImage->drawScreen();
	}

	GameOver::~GameOver() {
		SAFE_DELETE(mImage);
	}

	Child* GameOver::update(Parent* parent) {
		Child* next = this;
		GameLib::Framework f = GameLib::Framework::instance();
		if (f.isKeyTriggered(' ')) {
			next = new Title;
		}

		return next;
	}

}// namespace Sequence