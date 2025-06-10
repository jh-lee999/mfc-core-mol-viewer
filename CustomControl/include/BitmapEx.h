#pragma once

class BitmapEx : CBitmap
{
public:
	BitmapEx();
	~BitmapEx();

	CBitmap* GetBitmap();
	void DeleteObject();

	int GetSizeX() const;
	int GetSizeY() const;
	bool IsNull() const;
	void Alloc(CDC* pDC, int imageSizeX, int imageSizeY);

private:
	int m_imageSizeX;
	int m_imageSizeY;
};

