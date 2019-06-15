#include "GameLib/Framework.h"
#include "Sequence/Title.h"
#include "Sequence/Ending.h"

#include "Image.h"

namespace Sequence {

Ending::Ending(){
  mImage = new Image("Image/ending.dds");
}

Ending::~Ending() {
	SAFE_DELETE(mImage);
}

Child* Ending::update(Parent* parent) {
	Child* next = this;
	GameLib::Framework f = GameLib::Framework::instance();
	if (f.isKeyTriggered(' ')) {
		next = new Title;
	}

	return next;
}

}// namespace Sequence