#include "GameLib/Framework.h"

#include "Image.h"
#include "Object.h"
#include "State.h"

using namespace GameLib;

const int CELL_EDGE = 32;

bool percent(int a) {
	return (Framework::instance().getRandom(100) < a);
}

//背景描画
void Object::drawBackground(int x, int y, const Image* image) const {
	//壁なら壁
	if (mObj == OBJ_WALL) {
		drawCell(x, y, IMAGE_ID_WALL, image);
	}
	else {
	//壁の上には何も来ない
		drawCell(x, y, IMAGE_ID_SPACE, image);
	}
}

void Object::drawForeground(int x, int y, const Image* image) const {
	//動くのは人と荷物だけ。
	if (mObj == OBJ_BLOCK) {
		drawCell(x, y, IMAGE_ID_BLOCK, image);
	}
	else if (mObj == OBJ_BOMB) {
		drawCell(x, y, IMAGE_ID_BOMB, image);
	}
	else if (mObj == OBJ_FIRE_BOTH) {
		drawCell(x, y, IMAGE_ID_FIRE_BOTH, image);
	}
	else if (mObj == OBJ_FIRE_WIDTH) {
		drawCell(x, y, IMAGE_ID_FIRE_WIDTH, image);
	}
	else if (mObj == OBJ_FIRE_HEIGHT) {
		drawCell(x, y, IMAGE_ID_FIRE_HEIGHT, image);
	}
	else if (mObj == OBJ_FIRE_UP) {
		drawCell(x, y, IMAGE_ID_FIRE_UP, image);
	}
	else if (mObj == OBJ_BOMB_UP) {
		drawCell(x, y, IMAGE_ID_BOMB_UP, image);
	}

}

void Object::drawCell(int x, int y, ImageID imageID, const Image* image) const {
	image->drawPicture(CELL_EDGE * x, CELL_EDGE * y, (imageID % 4) * CELL_EDGE, (imageID / 4) * CELL_EDGE, CELL_EDGE, CELL_EDGE);
};

void Object::set(ObjID id) {
	mObj = id;
}


Player::Player(Object::ImageID id, int width, int height) : mId(id), mX(0), mY(0), mSpeed(1), mMaxBomb(2), mFire(3),mHaveBomb(mMaxBomb), mBombs(new Bomb*) {
	Framework f = Framework::instance();

	if (mId == Object::IMAGE_ID_1P) {
		mX = CELL_EDGE; mY = CELL_EDGE;
	}
	else if (mId == Object::IMAGE_ID_2P) {
		mX = (width - 2)*CELL_EDGE; mY = (height - 2)*CELL_EDGE;
	}
	else if (mId == Object::IMAGE_ID_ENEMY) ASSERT(false && "few Constructor's argument");
	else ASSERT(false && "illegal Player");
}

Player::Player(Object::ImageID id, int width, int height, int x, int y) : mId(id), mX(0), mY(0), mSpeed(1), mMaxBomb(0), mFire(0), mHaveBomb(mMaxBomb), mBombs(new Bomb*) {
	Framework f = Framework::instance();

	if (mId == Object::IMAGE_ID_ENEMY) {
		mX = x*CELL_EDGE; mY = y*CELL_EDGE;
	}
	else if (mId == Object::IMAGE_ID_1P || mId == Object::IMAGE_ID_2P) ASSERT(false && "over Constructor's argument");
	else ASSERT(false && "illegal Enemy");
}

Player::~Player() {
	SAFE_DELETE_ARRAY(mBombs);
}

void Player::plusSpeed() {
	Player::mSpeed += 1;
}

void Player::plusMaxBomb() {
	Player::mMaxBomb += 1;
}

void Player::plusFire() {
	Player::mFire += 1;
}

void Player::putBomb(Array2D<Object>& obj) {
	int i = 0, cnt = 0;
	int bombX = (mX + CELL_EDGE / 2) / CELL_EDGE;
	int bombY = (mY + CELL_EDGE / 2) / CELL_EDGE;

	if (mHaveBomb > 0 && obj(bombX,bombY).id() == Object::OBJ_SPACE) {
		//空いてる場所を探す
		while (cnt < mMaxBomb - mHaveBomb) {
			if (mBombs[i] == 0) break;
			else cnt++;
			i++;
		}

		mBombs[i] = new Bomb(this);
		if (cnt >= mHaveBomb) mBombs[i + 1] = 0; // cnt == i
		mHaveBomb--;
		obj(mBombs[i]->x(),mBombs[i]->y()).set(Object::OBJ_BOMB);
	}
}

void Player::proceedBombTime() {
	for (int i = 0, cnt = 0; cnt < mMaxBomb - mHaveBomb; i++) {
		if (bomb(i) != 0) {
			bomb(i)->proceedTime();
			cnt++;
		}
	}
}

void Player::fireBomb(Array2D<Object>& obj) {
	for (int i = 0, cnt = 0; cnt < mMaxBomb - mHaveBomb; i++) {
		if (mBombs[i] != 0) {
			int x = mBombs[i]->x(), y = mBombs[i]->y(); //ボムの位置

			if (mBombs[i]->time() >= 3000) { //消火条件

				obj(x,y).set(Object::OBJ_SPACE);
				cout << "Flag = " << mBombs[i]->itemFlag() << endl;

				for (int j = 1; j <= mBombs[i]->fire() && obj(x + j, y).id() == Object::OBJ_FIRE_WIDTH; j++) {
				  if ((mBombs[i]->itemFlag() & 8) && obj(x + j + 1, y).id() != Object::OBJ_FIRE_WIDTH) {//炎の先端にアイテムを落とす
					  obj(x + j, y).set(Object::OBJ_FIRE_UP);
					}
				  else obj(x + j, y).set(Object::OBJ_SPACE);
				}
				for (int j = 1; j <= mBombs[i]->fire() && obj(x - j, y).id() == Object::OBJ_FIRE_WIDTH; j++) {
					if ((mBombs[i]->itemFlag() & 4) && obj(x - j - 1, y).id() != Object::OBJ_FIRE_WIDTH) {
					  obj(x - j, y).set(Object::OBJ_FIRE_UP);
					}
				  else obj(x - j, y).set(Object::OBJ_SPACE);
				}
				for (int j = 1; j <= mBombs[i]->fire() && obj(x, y + j).id() == Object::OBJ_FIRE_HEIGHT; j++) {
					if ((mBombs[i]->itemFlag() & 2) && obj(x, y + j + 1).id() != Object::OBJ_FIRE_HEIGHT) {
					  obj(x, y + j).set(Object::OBJ_FIRE_UP);
					}
				  else obj(x, y + j).set(Object::OBJ_SPACE);
				}
				for (int j = 1; j <= mBombs[i]->fire() && obj(x, y - j).id() == Object::OBJ_FIRE_HEIGHT; j++) {
					if ((mBombs[i]->itemFlag() & 1) && obj(x, y - j - 1).id() != Object::OBJ_FIRE_HEIGHT) {
					  obj(x, y - j).set(Object::OBJ_FIRE_UP);
					}
				  else obj(x, y - j).set(Object::OBJ_SPACE);
				}

				mHaveBomb++;
				SAFE_DELETE(mBombs[i]);
			}
			else if (mBombs[i]->time() >= 2000 && obj(x,y).id() != Object::OBJ_FIRE_BOTH) { //発火条件 発火は一回

				cout << mBombs[i]->time() << " ms : in 2000 !!!" << endl;
				obj(x, y).set(Object::OBJ_FIRE_BOTH);

				for (int j = 1; j <= mBombs[i]->fire() && obj(x + j, y).id() != Object::OBJ_WALL; j++) {
				  bool cond = obj(x + j, y).id() == Object::OBJ_BLOCK;
					obj(x + j, y).set(Object::OBJ_FIRE_WIDTH);
					if (cond){
						mBombs[i]->flagItem(8); //右フラグを立てる
					  break;
					}
				}
				for (int j = 1; j <= mBombs[i]->fire() && obj(x - j, y).id() != Object::OBJ_WALL; j++) {
				  bool cond = obj(x - j, y).id() == Object::OBJ_BLOCK;
					obj(x - j, y).set(Object::OBJ_FIRE_WIDTH);
					if (cond) {
						mBombs[i]->flagItem(4); //左フラグを立てる
					  break;
					}
				}
				for (int j = 1; j <= mBombs[i]->fire() && obj(x, y + j).id() != Object::OBJ_WALL; j++) {
				  bool cond = obj(x, y + j).id() == Object::OBJ_BLOCK;
				  obj(x, y + j).set(Object::OBJ_FIRE_HEIGHT);
					if (cond) {
						mBombs[i]->flagItem(2); //下フラグを立てる
					  break;
					}
				}
				for (int j = 1; j <= mBombs[i]->fire() && obj(x, y - j).id() != Object::OBJ_WALL; j++) {
				  bool cond = obj(x, y - j).id() == Object::OBJ_BLOCK;
				  obj(x, y - j).set(Object::OBJ_FIRE_HEIGHT);
			    if (cond) {
						mBombs[i]->flagItem(1); //上フラグを立てる
					  break;
					}
				}
			}
			cnt++; /**/
		}
	}
}


void Player::draw(const Image* image) const {
	image->drawPicture(mX, mY, (mId % 4) * CELL_EDGE, (mId / 4)* CELL_EDGE, CELL_EDGE, CELL_EDGE);
}

void Player::move() {
  GameLib::Framework f = GameLib::Framework::instance();
	switch (mId) {
	case Object::IMAGE_ID_1P:
		if (f.isKeyOn('a')) {
			mX += mSpeed * (-1);
			//mY = mSpeed * 0;
		}
		else if (f.isKeyOn('d')) {
			mX += mSpeed * 1;
			//mY = mSpeed * 0;
		}
		else if (f.isKeyOn('w')) {
			//mX = mSpeed * 0;
			mY += mSpeed * (-1);
		}
		else if (f.isKeyOn('s')) {
			//mX = mSpeed * 0;
			mY += mSpeed * 1;
		}
		break;
	case Object::IMAGE_ID_2P:
		if (f.isKeyOn('j')) {
			mX += mSpeed * (-1);
			//mY = mSpeed * 0;
		}
		else if (f.isKeyOn('l')) {
			mX += mSpeed * 1;
			//mY = mSpeed * 0;
		}
		else if (f.isKeyOn('i')) {
			//mX = mSpeed * 0;
			mY += mSpeed * (-1);
		}
		else if (f.isKeyOn('k')) {
			//mX = mSpeed * 0;
			mY += mSpeed * 1;
		}
		break;
	case Object::IMAGE_ID_ENEMY:
		if (percent(10)) {
			mX += mSpeed * (-1);
			//mY = mSpeed * 0;
		}
		else if (percent(12)) {
			mX += mSpeed * 1;
			//mY = mSpeed * 0;
		}
		else if (percent(14)) {
			//mX = mSpeed * 0;
			mY += mSpeed * (-1);
		}
		else if (percent(16)) {
			//mX = mSpeed * 0;
			mY += mSpeed * 1;
		}
		//全ての選択肢が選ばれる確率を概ね等しくした
		//p_n = p_1 / (1 - (n-1)*p_1)
		//何もしない確率は約(1-n*p_1)のつもり。今回は60%かな…
		break;
	default:
	  ASSERT(false && "in Player::move()");
		break;
	}
	
}

/*
void Player::drawSpaceOnPlayer(const Image* image) const {
	Object::ImageID id = Object::IMAGE_ID_SPACE;
	int x = mX / CELL_EDGE, y = mY / CELL_EDGE;
	//床には模様があるのでマス目ごとに塗る必要がある
  image->drawPicture(x, y,(id % 4) * CELL_EDGE,(id / 4) *CELL_EDGE, CELL_EDGE, CELL_EDGE);

	// どのマスに被っているかを、x,yに切り捨てがあったかどうかで判断
	if (x == mX && y == mY){//この場合は1マスだけでよい}
	else if (x == mX) {
		image->drawPicture(x, y+1, (id % 4) * CELL_EDGE, (id / 4) * CELL_EDGE, CELL_EDGE, CELL_EDGE);
	} else if (y == mY) {
		image->drawPicture(x, y + 1, (id % 4) * CELL_EDGE, (id / 4) * CELL_EDGE, CELL_EDGE, CELL_EDGE);
	} 
}
*/

Bomb::Bomb(Player* player) : mTime(0), mFire(player->fire()), mX(0), mY(0), mItemFlag(0) {
  //player->x()は画素で数えるが、mXはマスで数える。
  mX = (player->x() + CELL_EDGE/2) / CELL_EDGE;
	mY = (player->y() + CELL_EDGE/2) / CELL_EDGE;
}

void Bomb::proceedTime() {
	mTime += Framework::instance().previousFrameInterval();
}

void Bomb::flagItem(int f) {
  ASSERT(0 <= f && f < 16);
  mItemFlag |= f;
}