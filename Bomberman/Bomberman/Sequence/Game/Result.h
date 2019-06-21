#include "Sequence/Game/GameChild.h"
#pragma once

class Image;
class Font;

namespace Sequence {
namespace Game {

class Parent;

class Result : public Child {
public:
  Result(int reult); 
	~Result();
	Child* update(Parent*);
private:
  void initScreen(Font* font);

	int mResult;
  Image* mImage;
	int mCursor;
};

}// 	namespace Game 
}// namespace Sequence {
