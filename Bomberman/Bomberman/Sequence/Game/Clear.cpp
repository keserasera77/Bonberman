#include "GameLib/Framework.h"
#include "Sequence/Game/GameChild.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/GameParent.h"
#include "Sequence/Game/Clear.h"

#include "Image.h"

using namespace GameLib;

namespace Sequence{
namespace Game{

Clear::Clear() {
  mImage = new Image("Image/clear.dds");
	mImage->drawScreen();
}

Clear::~Clear() {
	SAFE_DELETE(mImage);
}

Child* Clear::update(Parent* parent) {
	Framework f = Framework::instance();
	Child* next = this;

	if (f.isKeyTriggered(' ')) {
		parent->moveTo(Parent::NEXT_ENDING);
	}

	return next;
}

}// namespace Game
}// namespace Sequence