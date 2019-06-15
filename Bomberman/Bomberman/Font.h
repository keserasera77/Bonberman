//�I���W�i��

#pragma once

class Image;

class Font {
public:
	void drawString(int x, int y, const char* str, unsigned color) const;
	static Font* instance();
	static void create(const char* filename);
	static void create(const char* filename, int x, int y, unsigned color, const char** strs, int numOfstrs, int interval, bool dir);
	static void destroy();

	//�I���������Ԍ���dir == true->�c,false->��
	void drawStrings(int x, int y, unsigned colorbool, const char** strs, int numOfStrs, int interval, bool dir) const;
	void drawStrings() const;
private:
	Font(const char* filename);
	Font(const char* filename,int x, int y, unsigned color, const char** strs, int numOfstrs, int interval, bool dir);
	~Font();
  const int charHeight = 16; const int charWidth = 8;
  Image* mImage;
	
	//�ʃN���X�̂ق����ǂ������c
	bool mDirection; //�I���������Ԍ���true->�c,false->��
	int mInterval;
	int mMaxChoices;
	const char** mChoices;
	int mX, mY;
	unsigned mColor;

	static Font* mInstance;
};

