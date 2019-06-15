#include "GameLib/Framework.h"
#include "Image.h"
#include "Sequence/Parent.h"
#include "Sequence/Title.h"
#include "Sequence/Game/GameParent.h"

#include "Font.h"

using namespace GameLib;

namespace Sequence {

Title::Title() : mCursor(0) {
	Font::create("Image/font.dds");
	Framework f = Framework::instance();
	mImage = new Image("Image/title.dds");
	initScreen(Font::instance());

}

Title::~Title() {
	SAFE_DELETE(mImage);
	Font::destroy();
}

void Title::initScreen(Font* font) {
	const char* choices[2] = { "1P-MODE","2P-MODE" };
	mImage->drawScreen();
	font->drawStrings(10,12, 0xff0000ff,choices,2, 10, false);
}

Child* Title::update(Parent* parent) {
  Child* next = this;
	Framework f = Framework::instance();
	Font* font = Font::instance();

	unsigned color = 0xff0000ff;
	int x = 10, y = 12;
	int interval = 10;


	if (f.isKeyTriggered('a') || f.isKeyTriggered('d')) {
	   mCursor = (mCursor+1) % 2;
		 initScreen(font);//ŽžŠÔ‚ª‚©‚©‚è‚»‚¤‚È‚Ì‚Å’†‚É“ü‚ê‚½
	}

	font->drawString(x - 1 + mCursor * interval, y, ">", color);

	if (mCursor == 0) {
		if (f.isKeyTriggered(' ')) next = new Game::Parent(Game::Parent::MODE_1P);
	} else {
		if (f.isKeyTriggered(' ')) next = new Game::Parent(Game::Parent::MODE_2P);
	}

	return next;
}

}// namespace Sequence {