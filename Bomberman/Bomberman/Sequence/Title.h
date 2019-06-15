#pragma once
#include "Sequence/Child.h"

class Image;
class Parent;
class Font;

namespace Sequence {

class Title : public Child {
public:
  Title();
	~Title();
  
	Child* update(Parent*);
	void initScreen(Font* font);
private:
  Image* mImage;
	int mCursor; //0:1P,1:2P
 

};

}// namespace Sequence 