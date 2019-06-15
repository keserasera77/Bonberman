#include "GameLib/Framework.h"
#include "State.h"
#include "Image.h"

#include <algorithm>

using namespace GameLib;

const int CELL_EDGE = 32;
const int MAX_MOVE_COUNT = 250;

class State::Object {
public:
	enum Type {
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_GOAL,
		OBJ_BLOCK,
		OBJ_MAN,

		OBJ_UNKNOWN,
	};
	enum ImageID {
		IMAGE_ID_PLAYER,
		IMAGE_ID_WALL,
		IMAGE_ID_BLOCK,
		IMAGE_ID_GOAL,
		IMAGE_ID_SPACE,
	};

	Object() : mType(OBJ_WALL), mGoalFlag(false), mMoveX(0), mMoveY(0) {};
	//~Object();

	Type mType;
	bool mGoalFlag;
	int mMoveX; //-1,0,1
	int mMoveY; //-1,0,1

	void set(char input) {
			switch (input) {
			case '#': mType = OBJ_WALL; break;
			case ' ': mType = OBJ_SPACE; break;
			case 'o': mType = OBJ_BLOCK; break;
			case 'O': mType = OBJ_BLOCK; mGoalFlag = true; break;
			case '.': mType = OBJ_SPACE; mGoalFlag = true; break;
			case 'p': mType = OBJ_MAN; break;
			case 'P': mType = OBJ_MAN; mGoalFlag = true; break;
			case '\n': mType = OBJ_UNKNOWN; break; 
			default: mType = OBJ_UNKNOWN; break;
			}
	}

	//描画。背景描画
	void drawBackground(int x, int y, const Image* image) const {
		ImageID id = IMAGE_ID_SPACE;
		//壁なら壁
		if (mType == OBJ_WALL) {
			drawCell(x, y, IMAGE_ID_WALL, image);
		}
		else {
			if (mGoalFlag) {
				drawCell(x, y, IMAGE_ID_GOAL, image);
			}
			else {
				drawCell(x, y, IMAGE_ID_SPACE, image);
			}
		}
	}
	void drawForeground(int x, int y, const Image* image, int moveCount) const {
		//動くのは人と荷物だけ。
		ImageID id = IMAGE_ID_SPACE; //前景がないフラグとして使う
		if (mType == OBJ_BLOCK) {
			id = IMAGE_ID_BLOCK;
		}
		else if (mType == OBJ_MAN) {
			id = IMAGE_ID_PLAYER;
		}
		if (id != IMAGE_ID_SPACE) { //背景以外なら
			//移動を計算
			const int m =  MAX_MOVE_COUNT;
			int dx = (mMoveX * (m - moveCount) * CELL_EDGE) / m;
			int dy = (mMoveY * (m - moveCount) * CELL_EDGE) / m;
			image->drawPicture(x * CELL_EDGE - dx, y * CELL_EDGE - dy, id * CELL_EDGE, 0, CELL_EDGE, CELL_EDGE);
		}
	}

	void drawCell(int x, int y, ImageID imageID, const Image* image) const {
		image->drawPicture(CELL_EDGE * x, CELL_EDGE * y, imageID * CELL_EDGE, 0, CELL_EDGE, CELL_EDGE);
	};

	//移動をセット。第3引数は置き換わるタイプ
	void move(int dx, int dy, Type replaced) {
		mMoveX = dx;
		mMoveY = dy;
		mType = replaced;
	}

};

State::State(const char* stageData, int stageSize) {

	setSize(stageData, stageSize);
	mObjects.setSize(mStageWidth, mStageHeight);

	int x=0, y=0;
	for (int i = 0; i < stageSize; ++i) {
		Object t;
		bool goalFlag = false;
		switch (stageData[i]) {
		case '#': case ' ': case 'o': case 'O': case '.':
			mObjects(x, y).set(stageData[i]);
			++x;
			break;
		case 'p': case 'P':
		  mPx = x; mPy = y; //人の位置の記憶
			mObjects(x, y).set(stageData[i]);
			++x;
			break;
		case '\n': x = 0; ++y; break; //改行処理
		}
	}

	//画像の読み込み
	mObjectImage = new Image("Image/Images.dds");
}

void State::setSize(const char* stage, int size) {
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; i++) {
		switch (stage[i]) {
		case '#': case 'o': case 'O': case '.':
		case 'p': case 'P': case ' ':
			x++;
			break;
		case '\n':
			y++;
			mStageWidth = std::max(mStageWidth, x);
			mStageHeight = std::max(mStageHeight, y);
			x = 0;
			break;
		}
	}
}

bool State::clearCheck() const {
	for (int y = 0; y < mStageHeight; y++) {
		for (int x = 0; x < mStageWidth; x++) {
			if (mObjects(x, y).mType == Object::OBJ_BLOCK && mObjects(x, y).mGoalFlag == false) return false;
		}
	}
	return true;
}

void State::drawStage() const {
	//二段階に分けて描画する。まず背景を描画。
	for (int y = 0; y < mStageHeight; ++y) {
		for (int x = 0; x < mStageWidth; ++x) {
			mObjects(x, y).drawBackground(x, y, mObjectImage);
		}
	}
	//次に前景を描画
	for (int y = 0; y < mStageHeight; ++y) {
		for (int x = 0; x < mStageWidth; ++x) {
			mObjects(x, y).drawForeground(x, y, mObjectImage, mMoveCount); //mMoveCountを型変換によってfloorをだしてるのはわざと
		}
	}
}

void State::update(unsigned frameTime) {
	Framework f = Framework::instance();
	int dPx = 0, dPy = 0;

	if (mMoveCount >= MAX_MOVE_COUNT) {
	mMoveCount = 0;
		for (int y = 0; y < mStageHeight; y++) {
			for (int x = 0; x < mStageWidth; x++) {
				mObjects(x, y).mMoveX = 0; 
				mObjects(x, y).mMoveY = 0;
			}
		}
	}
	else if (mMoveCount > 0) { //移動中はステージの更新しない
		mMoveCount += frameTime; 
		if (mMoveCount > MAX_MOVE_COUNT) mMoveCount = MAX_MOVE_COUNT;
		return;
	}
	
	if (f.isKeyTriggered('a')) { dPx = -1;}
	else if (f.isKeyTriggered('s')) { dPy = 1;}
	else if (f.isKeyTriggered('d')) { dPx = 1;}
	else if (f.isKeyTriggered('w')) { dPy = -1; }

//範囲チェック
if (mPx + dPx < 0 || mStageWidth <= mPx + dPx || mPy + dPy < 0 || mStageWidth <= mPy + dPy) return;

switch (mObjects(mPx + dPx, mPy + dPy).mType) {
case Object::OBJ_SPACE:
	mObjects(mPx + dPx, mPy + dPy).move(dPx, dPy, Object::OBJ_MAN);
	mObjects(mPx, mPy).move(dPx, dPy, Object::OBJ_SPACE);
	mPx += dPx; mPy += dPy;
	mMoveCount = 1; //人が動き始める
	break;
case Object::OBJ_BLOCK:

	//範囲チェック
	if (mPx + 2 * dPx < 0 || mStageWidth <= mPx + 2 * dPx || mPy + 2 * dPy < 0 || mStageWidth <= mPy + 2 * dPy) return;

	switch (mObjects(mPx + 2 * dPx, mPy + 2 * dPy).mType) {
		//OBJ_BLOCK,OBJ_WALLの場合はその場に残る
	case Object::OBJ_SPACE:
		mObjects(mPx + 2 * dPx, mPy + 2 * dPy).move(dPx, dPy, Object::OBJ_BLOCK);
		mObjects(mPx + dPx, mPy + dPy).move(dPx, dPy, Object::OBJ_MAN);
		mObjects(mPx, mPy).move(dPx, dPy, Object::OBJ_SPACE);
		mPx += dPx; mPy += dPy;
		mMoveCount = 1; //人が動いているという情報。移動描写時に使う。
		break;
	}
}
}