#include "Sequence/Game/GameChild.h"
#pragma once

class Image;
class State;

namespace Sequence {
namespace Game {

class Parent;

class Ready : public Child {
public:
  Ready(State* state);
	~Ready();
	Child* update(Parent*);
private:
  Image* mImage;
	Image* mImageGo;
};

}// 	namespace Game 
}// namespace Sequence {
