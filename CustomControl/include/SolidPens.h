#pragma once
#include <map>

class SolidPens
{
public:
	SolidPens();
	~SolidPens();

	CPen* Get(COLORREF color);

private:
	std::map<COLORREF, CPen* > m_pens;
};

