#include "GameLib/Framework.h"
#include "Sequence/Game/Game.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Failure.h"
#include "Sequence/Game/Result.h"
#include "Sequence/Game/GameParent.h"

#include "State.h"

using namespace GameLib;

namespace Sequence {
namespace Game {

Game::Game() {
  Framework f = Framework::instance();

	//画面初期化
  for (int i = 0; i < f.height()*f.width(); i++) f.videoMemory()[i] = 0;
}

Game::~Game() {}

Child* Game::update(Parent* parent) {
	Framework f = Framework::instance();
	Child* next = this;

	if (f.isKeyTriggered(' ')) {
		next = new Pause();
		return next;
	}

	parent->getState()->update(parent);
	parent->getState()->drawStage();
	parent->getState()->drawPlayers();


	//ステージクリアーかチェック
	if (parent->getState()->clearCheck()) {
		next = new Clear;
	};

	if (f.isKeyTriggered('f')/*failure 未定*/) {
	  next = new Failure(parent);
	}

	if (f.isKeyTriggered('r')/*result 未定*/) {
		next = new Result;
	}

	return next;
}

}// namespace Game 
}// namespace Sequence 
