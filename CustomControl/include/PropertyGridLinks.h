#pragma once
#include "PropertyGridLink.h"
#include <memory>
#include <vector>

class CGridCtrl;

class PropertyGridLinks
{
public:
	PropertyGridLinks(CGridCtrl* gridCtrl);
	~PropertyGridLinks();

	LPCTSTR GetPropertyName(int32_t row_index);
	void Read(int32_t row_index, bool& value_changed);

	std::vector<std::shared_ptr<PropertyGridLink>> m_items;

	LPVOID grid_item_changed_user_data;
	CallbackPropertyGridItemChanged grid_item_changed_callback;

private:
	CGridCtrl* m_gridCtrl;
};
