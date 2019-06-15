#pragma once
#include "Sequence/Child.h"

class Image;

namespace Sequence{

class Parent;

class Ending : public Child {
public:
  Ending();
	~Ending();
	Child* update(Parent*);
private:
  Image* mImage;
};

}// namespace Sequence
