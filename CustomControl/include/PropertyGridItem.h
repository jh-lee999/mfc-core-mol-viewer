#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

class __CUSTOM_CONTROL_CLASS__ PropertyGridItem
{
public:
	PropertyGridItem();
	~PropertyGridItem();

	LPCTSTR GetDescription() const;
	void SetDescription(LPCTSTR text);

private:
	CString m_description;
};

