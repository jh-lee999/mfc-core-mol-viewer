#pragma once
#include "PropertyGrid.h"

class PropertyGridLink
{
public:
	PropertyGridLink(LPCTSTR name);
	PropertyGridLink(LPCTSTR name, CString* io_value);
	PropertyGridLink(LPCTSTR name, bool* io_value);
	PropertyGridLink(LPCTSTR name, float* io_value);
	PropertyGridLink(LPCTSTR name, long* io_value);
	PropertyGridLink(LPCTSTR name, int32_t* io_value);
	~PropertyGridLink();

	LPCTSTR GetName() const;
	void Set(LPCTSTR value, const CStringArray& options, bool& value_changed);

	LPVOID m_usrData;
	long m_startCommand;
	CallbackPropertyGridButtonClicked m_callbackButtonClicked;

private:
	CString name_;
	long m_type;
	void* m_valuePtr;
};

