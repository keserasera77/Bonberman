#include "Sequence/Child.h"

class State;

namespace Sequence {
class Parent;
namespace Game {

class Child;

class Parent : public Sequence::Child {
public:
	enum NextSequence {
	  //Parent‚Ö
		NEXT_TITLE,
		NEXT_GAMEOVER,
		NEXT_ENDING,

		NEXT_NONE
	};
	enum Mode {
		MODE_1P,
		MODE_2P,

		MODE_NONE
	};

	Parent(Mode mode);
	~Parent();

	const char* stageName() const;
	Child* update(Sequence::Parent*);
	void moveTo(NextSequence);
	void setState(State* state);
	State* getState() const;
  void initState();
	Mode mode() const;
	bool isMode1P() const;
	bool isMode2P() const;
	int life() const;
	void decrementLife();



private:
	NextSequence mNextSequence;
	Game::Child* mChild;
	const char* mStageName;
	const char* mStageData;
	int         mStageSize;
	State* mState;
	Mode mMode;
	int mLife;

	void setStage(const char* data, int size);
};

}// namespace Sequence 
}//	namespace GameLib 

