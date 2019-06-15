#pragma once
#include "Sequence/Game/GameChild.h"

class State;
class Image;

namespace Sequence{ 
namespace Game {

class Parent;

class Game : public Child{
public:
  Game();
	~Game();
	Child* update(Parent*);
private:
};

}// namespace Game 
}// namespace Sequence
