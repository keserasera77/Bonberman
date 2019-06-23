//シーケンス遷移はサンプルのものを一部流用。
//他はオリジナル。
/*
日記

6/13 作業開始！
6/15 シーケンス遷移が概ね完成　ゲーム本体着手
6/17 ステージの描写完了
6/18 ボムの描写、および爆発完了。mBombsの管理はもうちょっと何とかしたい。毎回for文にcntを使うのはめんどい。 何故か2Pが表示されなくなった。
6/19 2Pが表示されないバグの修正（初期座標の設定ミスで画面外にいるだけだった）。ボムの管理をStateではなくPlayerに委託。ボムが貫通ボムではなくなった。　アイテムを落とす処理を追加。
6/20 当たり判定の追加。アイテムを取得しパラメータが上がるようになった。 
6/21 エラーの頻度がかなり減った気がする。ボムの爆風に当たり判定を付けた。clearCheck,failureCheck,resultCheckを実装。Playerクラス同士の当たり判定を追加。
     恐らく後は、エラーを直す作業と、所謂ゲームらしくする作業（今は当たり判定が厳しすぎる等）でそれらしいものができる。
6/23 壁に当たると滑るようにし、間に入りやすくした。ボムの炎が消えるまでブロックが壊れないようにした。　取り敢えず完成。あとはエラーを消すだけ。

*/

/*
確認しているエラー
その日のうちに対処できなかったエラーを並べる。(6/21開始　それ以前のものは思い出して書いてる)

エラーではないが終了時にメモリが残るのが気になる。恐らくボムを追加してから。場合によって出たりでなかったりする。
6/21 永遠に右のようなものが出る　04FFF344		0		unknown		0
6/22 試合中に'q'で終了するとなる気がする

 !(nextSizeWithFlagDebug & FLAG_PREV_EMPTY)という（恐らく本書のフレームワークの）例外が起こる。
 6/19 たくさんボムを置いた試合で、メニュー から'd'を押すと起こる印象。ほかにも条件ありそう。
 6/20 上のエラーはボムの管理の仕方が悪いことが原因だということがほぼほぼ確定。Game::~Parentから飛んでる
 6/23 試合中に'q'で終了するとなる気がする

 Bomb::proceedTime()がthisの読み取り違反を起こすというエラーも発生。ボムの管理の仕方を変える理由が増えた。しかし、いい方法は思いついていない。
 6/19 ボムを置きまくる
 6/21 Player::putBomb内の　if (cnt >= mHaveBomb - mHaveBomb) mBombs[i + 1] = 0; を if (cnt >= mMaxBomb - mHaveBomb) mBombs[i + 1] = 0;　に修正。　恐らく直った。
 (修正完了(?))

例外がスローされました:読み取りアクセス違反。 GameLib::`anonymous namespace'::cast<unsigned int>(...) が 0x9FFE864 を返しました
6/21 タイトルへ戻るとき。等　もう少し前から同じようなエラーがあった気がするが覚えてない…

例外がスローされました:書き込みアクセス違反。 **this** が 0x1FA2338 でした。 (State内でObject::setを使ったときのエラー)
6/21 


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

