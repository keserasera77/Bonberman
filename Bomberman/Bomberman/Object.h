#pragma once

class Image;
class Bomb;
template<class T> class Array2D;

class Object {
public:
	enum ImageID {
		IMAGE_ID_1P,
		IMAGE_ID_2P,
		IMAGE_ID_BOMB,
		IMAGE_ID_FIRE_UP,
		IMAGE_ID_FIRE_WIDTH,
		IMAGE_ID_FIRE_BOTH,
		IMAGE_ID_FIRE_HEIGHT,
		IMAGE_ID_ENEMY,
		IMAGE_ID_BLOCK,
		IMAGE_ID_WALL,
		IMAGE_ID_SPACE,
		IMAGE_ID_BOMB_UP
	};

	enum ObjID {
		OBJ_BOMB,
		OBJ_FIRE_HEIGHT,
		OBJ_FIRE_WIDTH,
		OBJ_FIRE_BOTH,
		OBJ_BLOCK,
		OBJ_SPACE,
		OBJ_WALL,

		OBJ_BOMB_UP,
		OBJ_FIRE_UP,

		OBJ_PLAYER,
		OBJ_ENEMY,

		OBJ_UNKNOWN
	};
	
	Object() : mObj(OBJ_WALL) {};
	//~Object();

	ObjID mObj;

	//描画。背景描画
  void drawBackground(int x, int y, const Image* image) const;
	void drawForeground(int x, int y, const Image* image) const;
  void drawCell(int x, int y, ImageID imageID, const Image* image) const;
	void set(ObjID id);
	ObjID id() { return mObj; }

};


class Player {
public:
	Player(Object::ImageID id, int width, int height);
	Player(Object::ImageID id, int width, int height, int x, int y);
	~Player();

	Object::ImageID id() { return mId; }
	int x() const { return mX; }
	int y() const { return mY; }
	int speed() const { return mSpeed; }
	void plusSpeed();
	int maxBomb() const { return mMaxBomb; }
	void plusMaxBomb();
	int fire() const { return mFire; }
	void plusFire();
	void eraseFire(Array2D<Object>& obj, Bomb* bomb, int x, int y, int flag);
	void drawFire(Array2D<Object>& obj, Bomb* bomb, int x, int y, int flag);
	void putBomb(Array2D<Object>& obj);
	void fireBomb(Array2D<Object>& obj);
	void proceedBombTime();
	int haveBomb() const { return mHaveBomb;}

	Bomb* bomb(int i) { return mBombs[i]; };


	void draw(const Image* image) const;
	void move(Array2D<Object>& obj,int dX,int dY);
  int isCollided(Array2D<Object>& obj, int dX, int dY, Object::ObjID id) const; //0 当たってない 1,lu 2,ru 3,rd 4,ld に当たってる
	bool isCollidedWithObject(Array2D<Object>& obj, int dX, int dY);
	bool isCollidedWithPlayer(int dX, int dY, Player* player) const;
	bool isCollidedWithEnemys(int dX, int dY, Player** enemys, int numOfEnemys) const;
	void pickItem(Array2D<Object>& obj, Object::ObjID id);
	bool isFired(Array2D<Object>& obj) const;
	void die();
	bool isDead() const { return mIsDead; }

private:
	Object::ImageID mId;
	int mX;
	int mY;

	int mSpeed;
	int mMaxBomb;
	int mFire;

	int mHaveBomb;
	Bomb** mBombs;

	bool mIsDead;
};

class Bomb {
public:
  Bomb(Player* player);
	//~Bomb();
	int x() const { return mX; }
	int y() const { return mY; }

  int time() const { return mTime; }
	int fire() const { return mFire; }
	int itemFlag() const { return mItemFlag; }
	void flagItem(int f);
	void proceedTime();
private:
  int mX, mY;
  int mTime;
	int mFire;
	int mItemFlag; // 4bit 上下左右の順で爆発後にアイテムを落とすかを決定。
};