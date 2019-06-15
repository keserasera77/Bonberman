#include "GameLib/Framework.h"
#include "Sequence/Title.h"
//#include "Sequence/Child.h"
#include "Sequence/Parent.h"

using namespace GameLib;

namespace Sequence {

Parent* Parent::mInstance = 0;

Parent::Parent() : mStageName(0), mChild(0) {
  mChild = new Title();
}

Parent::~Parent() {
	SAFE_DELETE(mChild);
}

void Parent::create() {
	ASSERT(!mInstance);
	mInstance = new Parent();
}

void Parent::destroy() {
	ASSERT(mInstance);
	SAFE_DELETE(mInstance);
}

Parent* Parent::instance() {
	return mInstance;
}

/*
void Parent::moveTo(NextSequence next) {
	Framework f = Framework::instance();

	ASSERT(mNextSequence == NEXT_NONE); //‚±‚êˆÈŠO‚ ‚è‚¦‚È‚¢
	mNextSequence = next;
}
*/

const char* Parent::getStageName() const{
	return mStageName;
}

void Parent::setStageName(const char* filename) {
	mStageName = filename;
}

void Parent::update() {
  Child* next = mChild->update(this);
	if (next != mChild) {
		SAFE_DELETE(mChild);
		mChild = next;
	}
}

}//namespace Sequence 