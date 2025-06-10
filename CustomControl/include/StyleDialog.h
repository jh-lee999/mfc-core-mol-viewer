#pragma once

#ifdef __CUSTOM_CONTROL_CLASS_EXPORT__
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#else
#define __CUSTOM_CONTROL_CLASS__ __declspec(dllexport)
#endif

#include <afxmt.h>
#include <memory>
#include <queue>

class PostObj;

class __CUSTOM_CONTROL_CLASS__ StyleDialog : public CDialogEx
{
public:
	DECLARE_DYNAMIC(StyleDialog)

	StyleDialog(UINT resourceID, CWnd* parent = nullptr);

	bool Create();

	void Post(long command, std::shared_ptr<PostObj> obj);
	virtual void ProcPostMessage(long command, std::shared_ptr<PostObj> obj);

protected:
	void Close();
	void EnableOkCancel(bool enable);
	long GetDlgItemWidth(CWnd* item);
	long GetDlgItemHeight(CWnd* item);
	void UpdateLayerControl();
	virtual void UpdateLayerControl(int cx, int cy);

private:
	UINT m_resourceID;
	CWnd* m_parent;
	bool m_usingUpdateLayer;
	bool m_enableOkCancel;

	CCriticalSection m_csPostQueue;
	std::queue<std::pair<long, std::shared_ptr<PostObj>>> m_postQueue; //�߻��Ǵ� �޼������� �ֱ����� ť

protected:
	// ���Ƿ� Escape ���� �ʰ� ��
	virtual void OnOK() override;
	virtual void OnCancel() override;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnPostProcEvent(WPARAM, LPARAM);
};
