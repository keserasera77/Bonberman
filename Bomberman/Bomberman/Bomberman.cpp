//シーケンス遷移はサンプルのものを一部流用。
//他はオリジナル。
/*
日記

6/13 作業開始！
6/15 シーケンス遷移が概ね完成　ゲーム本体着手
6/17 ステージの描写完了
6/18 ボムの描写、および爆発完了。mBombsの管理はもうちょっと何とかしたい。毎回for文にcntを使うのはめんどい。 何故か2Pが表示されなくなった。
6/19 2Pが表示されないバグの修正（初期座標の設定ミスで画面外にいるだけだった）。ボムの管理をStateではなくPlayerに委託。ボムが貫通ボムではなくなった。　アイテムを落とす処理を追加。
     !(nextSizeWithFlagDebug & FLAG_PREV_EMPTY)という（恐らく本書のフレームワークの）例外が起こる。たくさんボムを置いた試合で、メニュー から'd'を押すと起こる印象。ほかにも条件ありそう。

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

