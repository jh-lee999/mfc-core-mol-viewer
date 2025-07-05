#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

#include <map>

// StyleColorMap

class __CUSTOM_CONTROL_CLASS__ StyleColorMap : public CStatic
{
	DECLARE_DYNAMIC(StyleColorMap)

public:
	StyleColorMap();
	virtual ~StyleColorMap();

	void SetRange(float min, float max);

protected:
	float m_min;
	float m_max;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();
};
