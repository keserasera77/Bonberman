#pragma once
#include "Sequence/Game/GameChild.h"

class Image;

namespace Sequence {
namespace Game{

class Parent;

class Clear : public Child {
public:
  Clear();
	~Clear();
  Child* update(Parent*);
private :
  Image* mImage;
};

}// namespace Game
}// namespace Sequence
