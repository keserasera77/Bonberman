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


Player::Player(Object::ImageID id, int width, int height) : mId(id), mX(0), mY(0), mSpeed(1), mMaxBomb(2), mFire(2),mHaveBomb(mMaxBomb), mBombs(new Bomb*), mIsDead(false) {
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

Player::Player(Object::ImageID id, int width, int height, int x, int y) : mId(id), mX(0), mY(0), mSpeed(1), mMaxBomb(0), mFire(0), mHaveBomb(mMaxBomb), mBombs(0), mIsDead(false) {
	Framework f = Framework::instance();

	if (mId == Object::IMAGE_ID_ENEMY) {
		mX = x*CELL_EDGE; mY = y*CELL_EDGE;
	}
	else if (mId == Object::IMAGE_ID_1P || mId == Object::IMAGE_ID_2P) ASSERT(false && "over Constructor's argument");
	else ASSERT(false && "illegal Enemy");
}

Player::~Player() {
	//SAFE_DELETE_ARRAY(mBombs); //!(nextSizeWithFlagDebug & FLAG_PREV_EMPTY) の原因っぽい
	/*
	for (int i = 0, cnt = 0; cnt < mMaxBomb - mHaveBomb; i++) {
		if (mBombs[i] != 0) {
			SAFE_DELETE(mBombs[i]);
			cnt++;
		}
	} // 書いても残りメモリは変わらなかった
	*/
}

void Player::plusSpeed() {
	mSpeed += 1;
}

void Player::plusMaxBomb() {
	mMaxBomb += 1;
	mHaveBomb += 1; //所持数も増加
}

void Player::plusFire() {
	mFire += 1;
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
		if (cnt >= mMaxBomb - mHaveBomb) mBombs[i + 1] = 0; // cnt == i
		mHaveBomb--;
		obj(mBombs[i]->x(),mBombs[i]->y()).set(Object::OBJ_BOMB);
	}
}

void Player::proceedBombTime() {
	for (int i = 0, cnt = 0; cnt < mMaxBomb - mHaveBomb; i++) {
		if (mBombs[i] != 0) {
			mBombs[i]->proceedTime();
			cnt++;
		}
	}
}

void Player::drawFire(Array2D<Object>& obj, int i, int x, int y, int flag) {
  if (flag != 1 && flag != 2 && flag != 4 && flag != 8) ASSERT(false && "illegal flag : Player::drawFire");
	int dx = 0, dy = 0;
	Object::ObjID fireid = Object::OBJ_FIRE_BOTH;
	if (flag == 1) {dx += 1; fireid = Object::OBJ_FIRE_WIDTH;}
	else if (flag == 2) {dx -= 1; fireid = Object::OBJ_FIRE_WIDTH;}
	else if (flag == 4) {dy += 1; fireid = Object::OBJ_FIRE_HEIGHT;}
	else if (flag == 8) {dy -= 1; fireid = Object::OBJ_FIRE_HEIGHT;}

	x += dx; y += dy;

	for (int j = 1; j <= mBombs[i]->fire() && obj(x, y).id() != Object::OBJ_WALL; j++, x += dx, y += dy) {
		bool cond = obj(x, y).id() == Object::OBJ_BLOCK || obj(x, y).id() == Object::OBJ_FIRE_UP || obj(x, y).id() == Object::OBJ_BOMB_UP;
		obj(x, y).set(fireid);
		if (cond) {
			if (obj(x, y).id() == Object::OBJ_BLOCK) mBombs[i]->flagItem(flag); //フラグを立てる
			break;
		}
	}
}

void Player::eraseFire(Array2D<Object>& obj, int i, int x, int y, int flag) {
	if (flag != 1 && flag != 2 && flag != 4 && flag != 8) ASSERT(false && "illegal flag : Player::drawFire");
	int p = 30; //アイテムが落ちる確率　単位は%
	int dx = 0, dy = 0;
	Object::ObjID fireid = Object::OBJ_FIRE_BOTH;
	if (flag == 1) { dx += 1; fireid = Object::OBJ_FIRE_WIDTH; }
	else if (flag == 2) { dx -= 1; fireid = Object::OBJ_FIRE_WIDTH; }
	else if (flag == 4) { dy += 1; fireid = Object::OBJ_FIRE_HEIGHT; }
	else if (flag == 8) { dy -= 1; fireid = Object::OBJ_FIRE_HEIGHT; }

	x += dx; y += dy;

	for (;obj(x, y).id() == fireid; x += dx, y += dy) {
		Object::ObjID id = Object::OBJ_SPACE;

		if ((mBombs[i]->itemFlag() & flag) && obj(x + dx, y + dy).id() != fireid) {//炎の先端にアイテムを落とす
			//各アイテムが落ちる確率を等しくしている 上下左右で同じことをしている
			if (percent(p)) id = Object::OBJ_FIRE_UP;
			else if (percent(100 * p / (100 - p))) id = Object::OBJ_BOMB_UP;
		}

		obj(x, y).set(id);

	}
}

void Player::fireBomb(Array2D<Object>& obj) {
	for (int i = 0, cnt = 0; cnt < mMaxBomb - mHaveBomb; i++) {
		if (mBombs[i] != 0) {
			int x = mBombs[i]->x(), y = mBombs[i]->y(); //ボムの位置

			if (mBombs[i]->time() >= 3000) { //消火条件

				obj(x,y).set(Object::OBJ_SPACE);
				eraseFire(obj, i, x, y, 1);
				eraseFire(obj, i, x, y, 2);
				eraseFire(obj, i, x, y, 4);
				eraseFire(obj, i, x, y, 8);

				mHaveBomb++;
				SAFE_DELETE(mBombs[i]);
			}
			else if (mBombs[i]->time() >= 2000 && obj(x,y).id() != Object::OBJ_FIRE_BOTH) { //発火条件 発火は一回

				cout << mBombs[i]->time() << " ms : in 2000 !!!" << endl;
				obj(x, y).set(Object::OBJ_FIRE_BOTH);
				drawFire(obj, i, x, y, 1);
				drawFire(obj, i, x, y, 2);
				drawFire(obj, i, x, y, 4);
				drawFire(obj, i, x, y, 8);
			}
			cnt++; /**/
		}
	}
}


void Player::draw(const Image* image) const {
	image->drawPicture(mX, mY, (mId % 4) * CELL_EDGE, (mId / 4)* CELL_EDGE, CELL_EDGE, CELL_EDGE);
}

void Player::move(Array2D<Object>& obj,int dX, int dY) {
  GameLib::Framework f = GameLib::Framework::instance();
	mX += mSpeed*dX;
	mY += mSpeed*dY;
}

void Player::pickItem(Array2D<Object>& obj, Object::ObjID id) {
  if (id != Object::OBJ_FIRE_UP && id != Object::OBJ_BOMB_UP) ASSERT(false && "id is Not item");
	Object& objLU = obj(mX / CELL_EDGE, mY / CELL_EDGE);
	Object& objRU = obj((mX + CELL_EDGE - 1) / CELL_EDGE, mY / CELL_EDGE);
	Object& objLD = obj(mX / CELL_EDGE, (mY + CELL_EDGE - 1) / CELL_EDGE);
	Object& objRD = obj((mX + CELL_EDGE - 1) / CELL_EDGE, (mY + CELL_EDGE - 1) / CELL_EDGE);

	Object::ObjID lu = objLU.id(); //left up
	Object::ObjID ru = objRU.id();
	Object::ObjID ld = objLD.id();
	Object::ObjID rd = objRD.id();

	if (lu == id) objLU.set(Object::OBJ_SPACE); 
	if (ru == id) objRU.set(Object::OBJ_SPACE); 
	if (ld == id) objLD.set(Object::OBJ_SPACE);
	if (rd == id) objRD.set(Object::OBJ_SPACE);
	else return;

	//一度にアイテムは2つ取らない前提
	if (id == Object::OBJ_BOMB_UP) plusMaxBomb();
  if (id == Object::OBJ_FIRE_UP) plusFire(); 

}

bool Player::isCollided(Array2D<Object>& obj, int dX,int dY, Object::ObjID id) const {
  Object::ObjID lu = obj((mX + dX) / CELL_EDGE,(mY + dY) / CELL_EDGE).id(); //left up
	Object::ObjID ru = obj((mX + dX + CELL_EDGE - 1) / CELL_EDGE, (mY + dY) / CELL_EDGE).id();
	Object::ObjID ld = obj((mX + dX) / CELL_EDGE, (mY + dY + CELL_EDGE - 1) / CELL_EDGE).id();
	Object::ObjID rd = obj((mX + dX + CELL_EDGE - 1) / CELL_EDGE, (mY + dY + CELL_EDGE - 1) / CELL_EDGE).id();

	/*
	cout << "mX = " << mX << ", mY = " << mY << endl;
	cout << "dX = " << dX << ", dY = " << dY << endl;
	cout << (mX + dX + CELL_EDGE - 1) / CELL_EDGE << " " << (mY + dY + CELL_EDGE - 1) / CELL_EDGE << endl;
	cout << "lu = " << lu << ", ru = " << ru << ", ld = " << ld << ", rd = " << rd << endl;
	cout << "wall = " << wall << endl << endl;
	*/

	return (lu == id || ru == id || ld == id || rd == id);
}

bool Player::isCollidedWithObject(Array2D<Object>& obj, int dX, int dY) const {
  bool wall  = !isCollided(obj, 0, 0, Object::OBJ_WALL) && isCollided(obj, dX, dY, Object::OBJ_WALL); 
  bool block = !isCollided(obj, 0, 0, Object::OBJ_BLOCK) && isCollided(obj, dX, dY, Object::OBJ_BLOCK);
	bool bomb  = !isCollided(obj, 0, 0, Object::OBJ_BOMB) && isCollided(obj, dX, dY, Object::OBJ_BOMB);
	return wall || block || bomb;
}

bool Player::isCollidedWithPlayer(int dX, int dY, Player* player) const {
  ASSERT(player && "Player::isCollidedWithPlayer");
  int l = mX + dX;
	int u = mY + dY;
	int r = mX + dX + CELL_EDGE - 1;
	int d = mY + dY + CELL_EDGE - 1;

	int l2 = player->x();
	int u2 = player->y();
	int r2 = player->x() + CELL_EDGE - 1;
	int d2 = player->y() + CELL_EDGE - 1;

	bool lu = (l2 <= l) && (l <= r2) && (u2 <= u) && (u <= d2);
	bool ld = (l2 <= l) && (l <= r2) && (u2 <= d) && (d <= d2);
	bool ru = (l2 <= r) && (r <= r2) && (u2 <= u) && (u <= d2);
	bool rd = (l2 <= r) && (r <= r2) && (u2 <= d) && (d <= d2);

	cout << "l = " << l << ", u = " << u << ", r = " << r << ", d = " << d << endl;
	cout << "l2 = " << l2 << ", u2 = " << u2 << ", r2 = " << r2 << ", d2 = " << d2 << endl;
	cout << "lu = " << lu << ", ld = " << ld << ", ru = " << ru << ", rd = " << rd << endl << endl;

	return (lu || ld || ru || rd);
}

bool Player::isCollidedWithEnemys(int dX, int dY, Player** enemys, int numOfEnemys) const {
	for (int i = 0, cnt = 0; cnt < numOfEnemys; i++) {
		if (enemys[i] != 0) {
		  if (isCollidedWithPlayer(dX, dY, enemys[i])) return true;
			cnt++;
		}
	}
	return false;
}

bool Player::isFired(Array2D<Object>& obj) const{
  return (isCollided(obj,0,0,Object::OBJ_FIRE_WIDTH) || isCollided(obj, 0, 0, Object::OBJ_FIRE_HEIGHT) || isCollided(obj, 0, 0, Object::OBJ_FIRE_BOTH));
}

void Player::die() {
	mIsDead = true;
}

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