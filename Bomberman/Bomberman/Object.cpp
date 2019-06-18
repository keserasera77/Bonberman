#include "GameLib/Framework.h"

#include "Image.h"
#include "Object.h"

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
	
}

void Object::drawCell(int x, int y, ImageID imageID, const Image* image) const {
	image->drawPicture(CELL_EDGE * x, CELL_EDGE * y, (imageID % 4) * CELL_EDGE, (imageID / 4) * CELL_EDGE, CELL_EDGE, CELL_EDGE);
};

void Object::set(ObjID id) {
	mObj = id;
}


Player::Player(Object::ImageID id, int width, int height) : mId(id), mX(0), mY(0), mSpeed(1), mMaxBomb(2), mFire(1),mHaveBomb(mMaxBomb) {
	Framework f = Framework::instance();

	if (mId == Object::IMAGE_ID_1P) {
		mX = CELL_EDGE; mY = CELL_EDGE;
	}
	else if (mId == Object::IMAGE_ID_2P) {
		mX = width - 2*CELL_EDGE; mY = height - 2*CELL_EDGE;
	}
	else if (mId == Object::IMAGE_ID_ENEMY) ASSERT(false && "few Constructor's argument");
	else ASSERT(false && "illegal Player");
}

Player::Player(Object::ImageID id, int width, int height, int x, int y) : mId(id), mX(0), mY(0), mSpeed(1), mMaxBomb(0), mFire(0), mHaveBomb(mMaxBomb) {
	Framework f = Framework::instance();

	if (mId == Object::IMAGE_ID_ENEMY) {
		mX = x*CELL_EDGE; mY = y*CELL_EDGE;
	}
	else if (mId == Object::IMAGE_ID_1P || mId == Object::IMAGE_ID_2P) ASSERT(false && "over Constructor's argument");
	else ASSERT(false && "illegal Enemy");
}

//Player::~Player() {}

void Player::plusSpeed() {
	Player::mSpeed += 1;
}

void Player::plusMaxBomb() {
	Player::mMaxBomb += 1;
}

void Player::plusFire() {
	Player::mFire += 1;
}

bool Player::putBomb() {
  if (mHaveBomb > 0) {
	  mHaveBomb--;
		return true;
	}
	return false;
} 

void Player::incBomb() {
	if (mMaxBomb > mHaveBomb) mHaveBomb++;
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
		if (percent(22)) {
			mX += mSpeed * (-1);
			//mY = mSpeed * 0;
		}
		else if (percent(29)) {
			mX += mSpeed * 1;
			//mY = mSpeed * 0;
		}
		else if (percent(22)) {
			//mX = mSpeed * 0;
			mY += mSpeed * (-1);
		}
		else if (percent(29)) {
			//mX = mSpeed * 0;
			mY += mSpeed * 1;
		}
		//全ての選択肢が選ばれる確率を概ね等しくした
		//何もしない確率は約68.64%
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

Bomb::Bomb(Player* player) : mTime(0), mFire(player->fire()), mX(0), mY(0) {
  //player->x()は画素で数えるが、mXはマスで数える。
  mX = (player->x() + CELL_EDGE/2) / CELL_EDGE;
	mY = (player->y() + CELL_EDGE/2) / CELL_EDGE;
}

void Bomb::proceedTime() {
	mTime += Framework::instance().previousFrameInterval();
}