#pragma once
#include "Sequence/Child.h"

class Image;

namespace Sequence {

class Parent;

class GameOver : public Child {
public:
	GameOver();
	~GameOver();
	Child* update(Parent*);
private:
	Image* mImage;
};

}// namespace Sequence
