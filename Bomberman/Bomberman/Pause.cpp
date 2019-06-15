#include "GameLib/GameLib.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Child.h"

#include "Image.h"

namespace Sequence {
namespace Game {

Pause::Pause() {
	mImage = Image("Image/pause.dds");
	mImage->drawScreen();
}

Pause::~Pause() {
	SAFE_DELETE(mImage);
}

Child* Ready::update(Parent* parent) {

}

}// namespace Game
}//	namespace Sequence 