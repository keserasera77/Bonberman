#include "GameLib/Framework.h"
#include "Sequence/Game/GameParent.h"
#include "State.h"
#include "Image.h"
#include "Object.h"

using namespace GameLib;
namespace SG = Sequence::Game;

const int MAX_MOVE_COUNT = 250;

bool percent(int a);

State::State(SG::Parent* parent) : mStageWidth(9), mStageHeight (7), mPlayer1(0), mPlayer2(0), mEnemys(new Player*), mNumOfEnemy(0) {
  //Framework f = Framework::instance();

	mObjects.setSize(mStageWidth, mStageHeight);

	//ステージ自動生成
	int blockRate = 80;
	int enemyRate = 40;

	for (int y = 0 ; y < mStageHeight; y++) {
		for (int x = 0; x < mStageWidth; x++) {
			Object& o = mObjects(x, y);

			//ステージは５＊５以上
		  if (x == 0 || y == 0 || x == mStageWidth - 1 || y == mStageHeight - 1) o.set(Object::OBJ_WALL);//周りの壁
			else if (x % 2 == 0 && y % 2 == 0) o.set(Object::OBJ_WALL); //通路の幅を1にする壁
			else if ((x == 1 && y == 1) || (x == 1 && y == 2) || (x == 2 && y == 1)) o.set(Object::OBJ_SPACE); //1Pの居場所
			else if ((parent->isMode2P()) && (x == mStageWidth-2 && y == mStageHeight-2 || x == mStageWidth-2 && y == mStageHeight-3 || x == mStageWidth-3 && y == mStageHeight-2)) o.set(Object::OBJ_SPACE); //2Pの居場所
			//その他
			else if (percent(blockRate)) o.set(Object::OBJ_BLOCK);
			else { 
			  o.set(Object::OBJ_SPACE);
				if (percent(enemyRate) && parent->isMode1P()){
				  mEnemys[mNumOfEnemy] = new Player(Object::IMAGE_ID_ENEMY, mStageWidth, mStageHeight, x, y);
				  mNumOfEnemy++;
				}
			}
	  }
	}
  
	//敵が一度も生成されなかった時にどこかに一匹敵を生成する。
	if (parent->isMode1P() && mNumOfEnemy <= 0) {
	  int N = (mStageWidth - 4) * (mStageHeight - 4);
		bool breakCheck = false;
		for (int y = 2, i = 0; y < mStageHeight - 2; y++) {
			for (int x = 2; x < mStageWidth - 2; x++,i++) {
			  //全てのマスが等確立で選ばれる。必ずどれかのマスは選ばれる。
				if (percent(100 / ((N+1) - i))) {
				  cout << "Enemy Safty" << endl;
					mObjects(x, y).set(Object::OBJ_SPACE);
					mEnemys[mNumOfEnemy] = new Player(Object::IMAGE_ID_ENEMY, mStageWidth, mStageHeight, x, y);
					mNumOfEnemy++;
					breakCheck = true;
					break;
				}
			}
			if (breakCheck) break;
		}
	}

	mEnemys[mNumOfEnemy] = 0; //番兵の配置
	
	//画像の読み込み
	mImage = new Image("Image/Images.dds");

	//プレイヤーの初期化
	mPlayer1 = new Player(Object::IMAGE_ID_1P, mStageWidth, mStageHeight);
  if (parent->isMode2P())  mPlayer2 = new Player(Object::IMAGE_ID_2P, mStageWidth, mStageHeight);

	//敵はステージ生成時に定義されてる
	//mEnemys[numOfEnemy] = new Player(Object::IMAGE_ID_ENEMY, mStageWidth, mStageHeight, x, y); 
}

State::~State() {
	SAFE_DELETE(mImage); //画像データ
	SAFE_DELETE(mPlayer1);
	SAFE_DELETE(mPlayer2);
  SAFE_DELETE_ARRAY(mEnemys);
}

void State::setSize(int width, int height) {
	mStageWidth = width;
	mStageHeight = height;
}

void State::drawStage() const {
	for (int y = 0; y < mStageHeight; ++y) {
		for (int x = 0; x < mStageWidth; ++x) {
			mObjects(x, y).drawBackground(x, y, mImage);
			mObjects(x, y).drawForeground(x, y, mImage);
	  }
	}
	drawPlayers();
}

void State::drawPlayers() const {

	mPlayer1->draw(mImage);
	if (mPlayer2 != 0) mPlayer2->draw(mImage);
	for (int i = 0; mEnemys[i] != 0 ; i++) mEnemys[i]->draw(mImage);
	
}

void State::moveEnemys() {
	for (int i = 0; mEnemys[i] != 0; i++) {
		int dX = 0, dY = 0;
		if (percent(10)) { dX = -1; dY = 0; }
		else if (percent(12)) { dX = 1; dY = 0; }
		else if (percent(14)) { dX = 0; dY = -1; }
		else if (percent(16)) { dX = 0; dY = 1; }
		//else 動かない確率60%

		if (!mEnemys[i]->isCollidedWithObject(mObjects, dX, dY)) mEnemys[i]->move(mObjects, dX, dY);
	}
}

//1Pモードのみ
bool State::failureCheck() {
	return mPlayer1->isDead();
}


bool State::clearCheck() {
	return (mNumOfEnemy <= 0);
}

void State::fireEnemys() {
	for (int i = 0, cnt = 0; cnt < mNumOfEnemy; i++) {
		if(mEnemys[i] != 0){
		  if(mEnemys[i]->isFired(mObjects)) {
			  SAFE_DELETE(mEnemys[i]);
			  mNumOfEnemy--;
			}
			cnt++;
		}
	}
}

//2Pモードのみ
int State::resultCheck() {
  int result = 0;
	if (mPlayer1->isDead()) result = 2;
	if (mPlayer2->isDead()) result = 1;

	return result;
}



void State::update(SG::Parent* parent) {
	Framework f = Framework::instance();

	bool canMove1P = true, canMove2P = true;
	int dX1 = 0, dY1 = 0;
	int dX2 = 0, dY2 = 0;

	//共通 
	//1Pの移動
	if (f.isKeyOn('a')) { dX1 = -1; dY1 = 0; }
	else if (f.isKeyOn('d')) { dX1 = 1; dY1 = 0; }
	else if (f.isKeyOn('w')) { dX1 = 0; dY1 = -1; }
	else if (f.isKeyOn('s')) { dX1 = 0; dY1 = 1; }

	canMove1P = canMove1P && !mPlayer1->isCollidedWithObject(mObjects, dX1, dY1);

  //ボムの処理
	if (f.isKeyTriggered('e')) mPlayer1->putBomb(mObjects);
	mPlayer1->proceedBombTime();
	mPlayer1->fireBomb(mObjects);

	//アイテムの取得
	mPlayer1->pickItem(mObjects, Object::OBJ_FIRE_UP);
	mPlayer1->pickItem(mObjects, Object::OBJ_BOMB_UP);

	if (mPlayer1->isFired(mObjects)) mPlayer1->die();

	//1Pモード
	if (parent->isMode1P()) {
		if (mPlayer1->isCollidedWithEnemys(dX1, dY1, mEnemys, mNumOfEnemy)) mPlayer1->die();

		//敵の移動
	  moveEnemys();
	  fireEnemys();
	}
	//2Pモード
	else if (parent->isMode2P()){

		if (f.isKeyOn('j')) { dX2 = -1; dY2 = 0; }
		else if (f.isKeyOn('l')) { dX2 = 1; dY2 = 0; }
		else if (f.isKeyOn('i')) { dX2 = 0; dY2 = -1; }
		else if (f.isKeyOn('k')) { dX2 = 0; dY2 = 1; }

		canMove2P = canMove2P && !mPlayer2->isCollidedWithObject(mObjects, dX2, dY2) && !mPlayer2->isCollidedWithPlayer(dX2, dY2, mPlayer1);
		canMove1P = canMove1P && !mPlayer1->isCollidedWithPlayer(dX1, dY1, mPlayer2);

		if (mPlayer2->isFired(mObjects)) mPlayer2->die();

		//ボムの処理
		if (f.isKeyTriggered('o'))  mPlayer2->putBomb(mObjects);
		mPlayer2->proceedBombTime();
		mPlayer2->fireBomb(mObjects);

		//アイテムの処理
		mPlayer2->pickItem(mObjects, Object::OBJ_FIRE_UP);
		mPlayer2->pickItem(mObjects, Object::OBJ_BOMB_UP);

		if (canMove2P) mPlayer2->move(mObjects, dX2, dY2);
	}
	else ASSERT(false && "illegal mode : State::update");

	if (canMove1P) mPlayer1->move(mObjects, dX1, dY1);
	}