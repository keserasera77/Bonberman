//オリジナル

#pragma once

class Image;

class Font {
public:
	void drawString(int x, int y, const char* str, unsigned color) const;
	static Font* instance();
	static void create(const char* filename);
	static void create(const char* filename, int x, int y, unsigned color, const char** strs, int numOfstrs, int interval, bool dir);
	static void destroy();

	//選択肢が並ぶ向きdir == true->縦,false->横
	void drawStrings(int x, int y, unsigned colorbool, const char** strs, int numOfStrs, int interval, bool dir) const;
	void drawStrings() const;
private:
	Font(const char* filename);
	Font(const char* filename,int x, int y, unsigned color, const char** strs, int numOfstrs, int interval, bool dir);
	~Font();
  const int charHeight = 16; const int charWidth = 8;
  Image* mImage;
	
	//別クラスのほうが良いかも…
	bool mDirection; //選択肢が並ぶ向きtrue->縦,false->横
	int mInterval;
	int mMaxChoices;
	const char** mChoices;
	int mX, mY;
	unsigned mColor;

	static Font* mInstance;
};

