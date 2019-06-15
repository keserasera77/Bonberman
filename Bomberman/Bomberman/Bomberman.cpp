//シークエンス遷移はサンプルのものを一部流用。
//他はオリジナル。
//6/13作業開始！ -> 6/15ゲーム本体着手

#include "GameLib/Framework.h"
#include <iostream>
#include "State.h"
#include "Sequence/Parent.h"

using namespace GameLib;

int gCount = 0; //mainLoopのループ数

namespace GameLib {
	void Framework::update() {
	Framework f = Framework::instance();

	f.sleep(1); //少しは寝かせる

	//フレームレート計測
	 unsigned currentTime = f.time();
	 if (gCount % 60 == 0) cout << "frameRate :: " << f.frameRate() << " (fps)" << endl;
	 gCount++;
	 //f.setFrameRate(60) // 固定フレームレート

	 if (!Sequence::Parent::instance()) {
		 Sequence::Parent::create();
	 }

	 //終了
	 if (f.isKeyOn('q')) {
		 f.requestEnd();
	 }

	 if (f.isEndRequested()) {
		 Sequence::Parent::destroy();
		 return;
	 }

	 Sequence::Parent::instance()->update();
	 }
}

