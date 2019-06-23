#include "GameLib/Framework.h"
#include "Sequence/Game/Game.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Failure.h"
#include "Sequence/Game/Result.h"
#include "Sequence/Game/GameParent.h"

#include "State.h"
#include "Object.h"

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
	State* state = parent->getState();

	if (f.isKeyTriggered(' ')) {
		next = new Pause();
		return next;
	}

	state->update(parent);
	state->drawStage();

	//デバグ用
	#ifndef NDEBUG

	if (parent->isMode1P() && f.isKeyTriggered('c')) next = new Clear;
	if (parent->isMode1P() && f.isKeyTriggered('f')) next = new Failure(parent);
	if (parent->isMode2P() && f.isKeyTriggered('r')) next = new Result(1);
	if (parent->isMode2P() && f.isKeyTriggered('t')) next = new Result(2);

	#endif


	//ステージクリアーかチェック
	if (parent->isMode1P() && state->clearCheck()) {
		next = new Clear;
	};

	if (parent->isMode1P() && state->failureCheck()) {
	  next = new Failure(parent);
	}

	if (parent->isMode2P() && state->resultCheck()) {
		next = new Result(state->resultCheck());
	}

	return next;
}

}// namespace Game 
}// namespace Sequence 
