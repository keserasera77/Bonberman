#include "Sequence/Game/GameChild.h"
#pragma once

class Image;
class Font;

namespace Sequence {
namespace Game {

class Parent;

class Result : public Child {
public:
  Result(); 
	~Result();
	Child* update(Parent*);
private:
  void initScreen(Font* font);
  Image* mImage;
	int mCursor;
};

}// 	namespace Game 
}// namespace Sequence {
