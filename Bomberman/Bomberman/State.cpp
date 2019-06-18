#include "GameLib/Framework.h"
#include "Sequence/Game/GameParent.h"
#include "State.h"
#include "Image.h"
#include "Object.h"

using namespace GameLib;
namespace SG = Sequence::Game;

const int MAX_MOVE_COUNT = 250;

bool percent(int a);

State::State(SG::Parent* parent) : mStageWidth(9), mStageHeight (7), mPlayer1(0), mPlayer2(0), mEnemys(new Player*), mBombs(new Bomb*), mNumOfBomb(0), mNumOfEnemy(0) {
  //Framework f = Framework::instance();

	typedef SG::Parent::Mode Mode0;
	const Mode0 mode1p = SG::Parent::MODE_1P;
	const Mode0 mode2p = SG::Parent::MODE_2P;

	mObjects.setSize(mStageWidth, mStageHeight);
	Mode0 mode = parent->mode();

	//ステージ自動生成
	for (int y = 0 ; y < mStageHeight; y++) {
		for (int x = 0; x < mStageWidth; x++) {
			Object& o = mObjects(x, y);

			//ステージは５＊５以上
		  if (x == 0 || y == 0 || x == mStageWidth - 1 || y == mStageHeight - 1) o.set(Object::OBJ_WALL);//周りの壁
			else if (x % 2 == 0 && y % 2 == 0) o.set(Object::OBJ_WALL); //通路の幅を1にする壁
			else if ((x == 1 && y == 1) || (x == 1 && y == 2) || (x == 2 && y == 1)) o.set(Object::OBJ_SPACE); //1Pの居場所
			else if ((mode == mode2p) && (x == mStageWidth-2 && y == mStageHeight-2 || x == mStageWidth-2 && y == mStageHeight-3 || x == mStageWidth-3 && y == mStageHeight-2)) o.set(Object::OBJ_SPACE); //2Pの居場所
			//その他
			else if (percent(85)) o.set(Object::OBJ_BLOCK);
			else { 
			  o.set(Object::OBJ_SPACE);
				if (percent(40) && mode == mode1p){
				  mEnemys[mNumOfEnemy] = new Player(Object::IMAGE_ID_ENEMY, mStageWidth, mStageHeight, x, y);
				  mNumOfEnemy++;
				}
			}
	  }
	}

	mEnemys[mNumOfEnemy] = 0; //番兵の配置

	mBombs[0] = 0;
	//画像の読み込み
	mImage = new Image("Image/Images.dds");

	//プレイヤーの初期化,描画
	mPlayer1 = new Player(Object::IMAGE_ID_1P, mStageWidth, mStageHeight);

	if (mode == mode2p)  mPlayer2 = new Player(Object::IMAGE_ID_2P, mStageWidth, mStageHeight);

	//ステージ生成時に定義されてる
	//mEnemys[numOfEnemy] = new Player(Object::IMAGE_ID_ENEMY, mStageWidth, mStageHeight, x, y); 
}

State::~State() {
	SAFE_DELETE(mImage); //画像データ
	SAFE_DELETE(mPlayer1);
	SAFE_DELETE(mPlayer2);
	for (int i = 0; i < mNumOfEnemy; i++) SAFE_DELETE(mEnemys);
	for (int i = 0, cnt = 0; cnt < mNumOfBomb; i++)  {
	  if (mBombs[i] != 0){
	    SAFE_DELETE(mBombs[i]);
			cnt++;
		}
	}
}

void State::setSize(int width, int height) {
	mStageWidth = width;
	mStageHeight = height;
}

bool State::clearCheck() const {
	return false;
}

void State::drawStage(/*Player* player*/) const {
	for (int y = 0; y < mStageHeight; ++y) {
		for (int x = 0; x < mStageWidth; ++x) {
			mObjects(x, y).drawBackground(x, y, mImage);
			mObjects(x, y).drawForeground(x, y, mImage);
	  }
	}
}

void State::drawPlayers() const {

	mPlayer1->draw(mImage);
	if (mPlayer2 != 0) mPlayer2->draw(mImage);
	for (int i = 0; mEnemys[i] != 0 ; i++) mEnemys[i]->draw(mImage);
	
}

void State::movePlayers() const{
	mPlayer1->move();
	if (mPlayer2 != 0) mPlayer2->move();
	for (int i = 0; mEnemys[i] != 0; i++) mEnemys[i]->move();
}

void State::proceedBombTime() {
	for (int i = 0, cnt = 0; cnt < mNumOfBomb; i++) {
	  if (mBombs[i] != 0) {
		  mBombs[i]->proceedTime();
		  cnt++; 
		}
	}
}

void State::putBomb(Player* player) {
  int i = 0;
	if (player->putBomb()) {
		while (i < mNumOfBomb) {
		  if (mBombs[i] == 0) break;
			i++;
		}
		mBombs[i] = new Bomb(player);
		if (i >= mNumOfBomb) mBombs[i+1] = 0;
		mNumOfBomb++;
		mObjects(mBombs[i]->x(),mBombs[i]->y()).set(Object::OBJ_BOMB);
	}
}

void State::fireBomb(Player* player) {
	for (int i = 0, cnt = 0; cnt < mNumOfBomb; i++) {
	  cout << "i = " << i << ", mNumOfBomb = " << mNumOfBomb << endl;
		if (mBombs[i] != 0) {
		  cout << mBombs[i]->time() << " ms" << endl;
			int x = mBombs[i]->x(), y = mBombs[i]->y(); //ボムの位置

			if (mBombs[i]->time() >= 3000) { //消火条件
				cout << mBombs[i]->time() << " ms : in 3000 !!!" << endl;

				mObjects(mBombs[i]->x(), mBombs[i]->y()).set(Object::OBJ_SPACE);
				for (int j = 1; j <= mBombs[i]->fire() && mObjects(x + j, y).id() != Object::OBJ_WALL; j++) mObjects(x + j, y).set(Object::OBJ_SPACE);
				for (int j = 1; j <= mBombs[i]->fire() && mObjects(x - j, y).id() != Object::OBJ_WALL; j++) mObjects(x - j, y).set(Object::OBJ_SPACE);
				for (int j = 1; j <= mBombs[i]->fire() && mObjects(x, y + j).id() != Object::OBJ_WALL; j++) mObjects(x, y + j).set(Object::OBJ_SPACE);
				for (int j = 1; j <= mBombs[i]->fire() && mObjects(x, y - j).id() != Object::OBJ_WALL; j++) mObjects(x, y - j).set(Object::OBJ_SPACE);

				mNumOfBomb--;
				SAFE_DELETE(mBombs[i]);
			}
			else if (mBombs[i]->time() >= 2000) { //発火条件

				cout << mBombs[i]->time() << " ms : in 2000 !!!" << endl;
				mObjects(x, y).set(Object::OBJ_FIRE_BOTH);
				
				for (int j = 1; j <= mBombs[i]->fire() && mObjects(x + j, y).id() != Object::OBJ_WALL; j++) mObjects(x + j, y).set(Object::OBJ_FIRE_WIDTH);
				for (int j = 1; j <= mBombs[i]->fire() && mObjects(x - j, y).id() != Object::OBJ_WALL; j++) mObjects(x - j, y).set(Object::OBJ_FIRE_WIDTH);
				for (int j = 1; j <= mBombs[i]->fire() && mObjects(x, y + j).id() != Object::OBJ_WALL; j++) mObjects(x, y + j).set(Object::OBJ_FIRE_HEIGHT);
				for (int j = 1; j <= mBombs[i]->fire() && mObjects(x, y - j).id() != Object::OBJ_WALL; j++) mObjects(x, y - j).set(Object::OBJ_FIRE_HEIGHT);
			  
				player->incBomb();
			}
			cnt++; /**/
		}
	}
	
}

void State::update(SG::Parent* parent) {
	Framework f = Framework::instance();

	movePlayers();

	if (f.isKeyTriggered('e')) putBomb(mPlayer1);
	if (parent->mode() == SG::Parent::MODE_2P) if (f.isKeyTriggered('o')) putBomb(mPlayer2);

	proceedBombTime();

	fireBomb(mPlayer1);
  if (parent->mode() == SG::Parent::MODE_2P) fireBomb(mPlayer2);
}