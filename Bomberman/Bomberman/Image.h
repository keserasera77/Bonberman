#pragma once

class Image {
public:
	Image(const char* filename);
	~Image();
	//int width() const;
	//int height() const;
	void drawPicture(int dstX, int dstY, int srcX, int srcY, int width, int height) const;
	void drawWithReplacementColor(int dstX, int dstY, int srcX, int srcY, int width, int height, unsigned color) const;
	void drawScreen();
private:
	int mWidth;
	int mHeight;
	unsigned* mData;
};