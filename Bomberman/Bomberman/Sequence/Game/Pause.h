#include "Sequence/Game/GameChild.h"
#pragma once

class Image;
class Font;

namespace Sequence {
namespace Game {

class Parent;

class Pause : public Child{
public:
  Pause();
  ~Pause();
	Child* update(Parent*);
private:
  void initScreen(Font* font);
  Image* mImage;
	int mCursor;
};

}// 	namespace Game 
}// namespace Sequence {
