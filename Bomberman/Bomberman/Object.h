#pragma once

class Image;

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

		ITEM_BOMB_UP,
		ITEM_FIRE_UP,

		OBJ_PLAYER,
		OBJ_ENEMY,

		OBJ_UNKNOWN
	};
	
	Object() : mObj(OBJ_WALL) {};
	//~Object();

	ObjID mObj;

	//�`��B�w�i�`��
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
	//~Player();

	Object::ImageID id() { return mId; }
	int x() const { return mX; }
	int y() const { return mY; }
	int speed() const { return mSpeed; }
	void plusSpeed();
	int maxBomb() const { return mMaxBomb; }
	void plusMaxBomb();
	int fire() const { return mFire; }
	void plusFire();
	bool putBomb();
	void incBomb();


	void draw(const Image* image) const;
	//void drawSpaceOnPlayer(const Image* image) const;
	void move();
private:
	Object::ImageID mId;
	int mX;
	int mY;

	int mSpeed;
	int mMaxBomb;
	int mFire;

	int mHaveBomb;
};

class Bomb {
public:
  Bomb(Player* player);
	//~Bomb();
	int x() const { return mX; }
	int y() const { return mY; }

  int time() const { return mTime; }
	int fire() const { return mFire; }
	void proceedTime();
private:
  int mX, mY;
  int mTime;
	int mFire;
};