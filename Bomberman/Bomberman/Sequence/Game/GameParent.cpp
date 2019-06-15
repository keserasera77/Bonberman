#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Title.h"
#include "Sequence/GameOver.h"
#include "Sequence/Ending.h"
#include "Sequence/Game/GameParent.h"
#include "GameLib/Framework.h"
#include "Sequence/Game/GameChild.h"
#include "Sequence/Game/Ready.h"

#include "File.h"
#include "State.h"

namespace Sequence {
namespace Game {

Parent::Parent(Mode mode) : mStageData(0), mStageSize(0), mStageName(0), mLife(3), mMode(MODE_NONE), mNextSequence(NEXT_NONE), mState(0) {
	GameLib::Framework f = GameLib::Framework::instance();
	for (int i = 0; i < f.height() * f.width(); i++) f.videoMemory()[i] = 0;

  if (mode == MODE_1P || mode == MODE_2P) mMode = mode;
	else ASSERT(false);

	File stage("StageData/stageData1.txt");
	setStage(stage.data(), stage.size());
	initState();
	mState->drawStage();
	mChild = new Ready(mState);
  //mStageName = filename;
}

Parent::~Parent() {
	SAFE_DELETE(mChild);
	SAFE_DELETE(mState);
}

const char* Parent::stageName() const{
	return mStageName;
}

void Parent::setState(State* state) {
  SAFE_DELETE(mState);
	mState = state;
}
State* Parent::getState() const{
	return mState;
}

void Parent::setStage(const char* data, int size) {
	mStageData = data;
	mStageSize = size;
}

void Parent::initState() {
  setState(new State(mStageData,mStageSize));
}

Parent::Mode Parent::mode() const{
	return mMode;
}

void Parent::moveTo(NextSequence next) {
	mNextSequence = next;
}

int Parent::life() const {
  return mLife;
}

void Parent::decrementLife() {
  mLife--;
}

Sequence::Child* Parent::update(Sequence::Parent* parent) {
	//Game::Parentとしての役割
	Game::Child* nextChild = mChild->update(this);

	if (nextChild != mChild) {
		SAFE_DELETE(mChild);
		mChild = nextChild;
	}

  //Sequence::Childの派生クラスとしての役割
	Sequence::Child* nextParent = this;
	switch (mNextSequence) {
	case NEXT_TITLE:
	  SAFE_DELETE(mChild);
	  nextParent = new Title();
		break;
	case NEXT_GAMEOVER:
	  SAFE_DELETE(mChild);
	  nextParent = new GameOver();
		break;
	case NEXT_ENDING:
		SAFE_DELETE(mChild);
		nextParent = new Ending();
		break;
	default:
	  break;
	}

	return nextParent; //Sequence::Childの派生クラスとしての役割
}


}//	namespace GameLib 
}// namespace Sequence 