#include "Sequence/Game/GameChild.h"
#pragma once

class Image;

namespace Sequence {
	namespace Game {

		class Parent;

class Failure :public Child {
public:
	Failure(Parent* parent);
	~Failure();
	Child* update(Parent*);
private:
  Image* mImage;
};

}// 	namespace Game 
}// namespace Sequence {