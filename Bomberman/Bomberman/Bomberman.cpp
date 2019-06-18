//シーケンス遷移はサンプルのものを一部流用。
//他はオリジナル。
/*
日記

6/13 作業開始！
6/16 シーケンス遷移が概ね完成　ゲーム本体着手
6/18 ステージの描写完了
6/19 ボムの描写、および爆発完了。mBombsの管理はもうちょっと何とかしたい。毎回for文にcntを使うのはめんどい。
*/

#include "GameLib/Framework.h"
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

