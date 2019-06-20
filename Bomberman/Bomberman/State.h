#pragma once

namespace Sequence { 
namespace Game {
class Parent;
}
}

class Player;
class Image;
class Object;
class Bomb;

template<class T> class Array2D {
public:
	Array2D() : mArray(0),mWidth(0),mHeight(0){}
	~Array2D() {
		delete[] mArray;
		mArray = 0;
	}
	void setSize(int width, int height) {
		mWidth = width;
		mHeight = height;
		mArray = new T[width * height];
	}
	T& operator()(int x, int y) {
		return mArray[y * mWidth + x];
	}
	const T& operator()(int x, int y) const {
		return mArray[y * mWidth + x];
	}
private:
	T* mArray;
	int mWidth;
	int mHeight;
};

class State {
public:
	State(Sequence::Game::Parent* parent);
	~State();
	void update(Sequence::Game::Parent* parent);
	void drawStage() const;
	void drawPlayers() const;
	bool clearCheck() const;
	void movePlayers() const;
private:
	void setSize(int width, int height);

	int mStageWidth;
	int mStageHeight;
	Image* mImage; //âÊëúÉfÅ[É^
	Array2D<Object> mObjects;
	Player* mPlayer1; 
	Player* mPlayer2;
	Player** mEnemys;
	int mNumOfEnemy;
 };
