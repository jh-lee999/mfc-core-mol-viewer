// ViewProgramTopMenu.cpp : implementation file
//

#include "pch.h"
#include "SW.h"
#include "afxdialogex.h"
#include "SWDlg.h"
#include "ViewMain.h"
#include "MoleculeParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(ViewMain, StyleDialog) 

ViewMain::ViewMain(CWnd* pParent /*=nullptr*/)
	: StyleDialog(IDD_VIEW_MAIN, pParent)
{
	m_pParent = pParent;
}

ViewMain::~ViewMain()
{
}

void ViewMain::UpdateLayerControl(int cx, int cy)
{
	m_pictureCtl.MoveWindow(10, 10, cx-20, cy  - 20);

}

BOOL ViewMain::OnInitDialog()
{
	StyleDialog::OnInitDialog();
	SetBackgroundColor(RGB(64, 64, 64));  // 배경 색상 

	HWND hWndGL = m_pictureCtl.GetSafeHwnd();
	m_gl.init(hWndGL, Viewer3D);
	m_gl.Render();
	SetTimer(1, 16, nullptr);
    initDB();
    g_molDB.LoadFromIni(Path::GetDbFilePath());
	StyleDialog::UpdateLayerControl(); // 리소스 배치 함수	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void ViewMain::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	StyleDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_OPENCV, m_pictureCtl);
}
BEGIN_MESSAGE_MAP(ViewMain, StyleDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &ViewMain::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_MESSAGE(WM_USER_ADD_ATOM_MODE, &ViewMain::OnUpdateAtomAdd)
	ON_MESSAGE(WM_LOAD_MOLECULE_FILE, &ViewMain::OnLoadMolculeData)
END_MESSAGE_MAP()


LRESULT ViewMain::OnUpdateAtomAdd(WPARAM wParam, LPARAM lParam)
{
	bool isEnabled = (wParam != 0);  // 1 → true, 0 → false
	m_gl.SetAddMode(isEnabled);
	return 0;
}
LRESULT ViewMain::OnLoadMolculeData(WPARAM wParam, LPARAM lParam)
{
	std::wstring* pStr = reinterpret_cast<std::wstring*>(lParam);
	if (pStr)
	{
		std::string path(pStr->begin(), pStr->end());
		delete pStr;

		MoleculeParser::LoadFromFile(path);
		Invalidate();  // 혹시 화면 갱신 필요시
	}
}

void ViewMain::OnBnClickedButton1()
{
	static bool isClicked = false;
	isClicked = !isClicked;
	m_mouseCtrl.ResetViewPoint();
	if (isClicked == true)
	{
		m_gl.d_ChangeViewer(Viewer2D);
	}
	else 
	{
		m_gl.d_ChangeViewer(Viewer3D);
	}
}


void ViewMain::OnTimer(UINT_PTR nIDEvent)
{

	m_gl.Render();
	StyleDialog::OnTimer(nIDEvent);
}


void ViewMain::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_gl.GetAddMode())
	{
		float glX = 0.0f, glY = 0.0f, glZ = -5.0f;

		if (m_gl.GetCurrentStyle() == Viewer2D) {
			m_gl.ScreenToGL(point.x, point.y, glX, glY);
			glZ = 0.0f;  // 2D는 z 고정
		}
		else if (m_gl.GetCurrentStyle() == Viewer3D) {
			if (!m_gl.ScreenToGL_3D(point.x, point.y, glX, glY, glZ))
			{
				glX = glY = 0.0f;
				glZ = -5.0f;
			}
		}

		ObjectContainer::Get().AddAtomObject("C", glX, glY, glZ);
		m_gl.Render();
		return;
	}
	m_gl.HandleMouseClick(point.x, point.y);
	m_mouseCtrl.StartDrag(point);
	SetCapture();
	StyleDialog::OnLButtonDown(nFlags, point);
}


void ViewMain::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_mouseCtrl.EndDrag();
	ReleaseCapture();
	StyleDialog::OnLButtonUp(nFlags, point);
}


void ViewMain::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_mouseCtrl.EndDrag();
	ReleaseCapture();
	StyleDialog::OnRButtonUp(nFlags, point);
}


void ViewMain::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_mouseCtrl.StartDrag(point);
	SetCapture();
	StyleDialog::OnRButtonDown(nFlags, point);
}


void ViewMain::OnMouseMove(UINT nFlags, CPoint point)
{
	m_mouseCtrl.UpdateDrag(nFlags, point, m_gl.GetCurrentStyle(), m_gl);
	m_gl.HandleMouseMove(point.x, point.y);
	StyleDialog::OnMouseMove(nFlags, point);
}




BOOL ViewMain::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    POINT cursorPos;
    ::GetCursorPos(&cursorPos);

    CRect rect;
    GetDlgItem(IDC_STATIC_OPENCV)->GetWindowRect(&rect);

    if (rect.PtInRect(cursorPos))
    {
        ScreenToClient(&cursorPos);
        float glX = 0, glY = 0;
        m_gl.ScreenToGL(cursorPos.x, cursorPos.y, glX, glY);

        float prevZoom = m_gl.Getzoom();
        float newZoom = prevZoom + ((zDelta > 0) ? 0.5f : -0.5f);
        m_gl.Setzoom(newZoom);

        float deltaZoom = newZoom - prevZoom;
        m_mouseCtrl.m_offsetX -= glX * deltaZoom * 0.1f;
        m_mouseCtrl.m_offsetY -= glY * deltaZoom * 0.1f;

        m_gl.Setoffset(m_mouseCtrl.m_offsetX, m_mouseCtrl.m_offsetY);
        m_gl.Render();
    }

    return StyleDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void ViewMain::initDB()
{
    CString path = Path::GetDbFilePath();
    CFileStatus status;
    BOOL exists = CFile::GetStatus(path, status);

    if (!exists || status.m_size == 0)
    {
        WritePrivateProfileString(_T("H"), _T("Radius"), _T("1.2"), path);
        WritePrivateProfileString(_T("H"), _T("Color"), _T("White"), path);
        WritePrivateProfileString(_T("He"), _T("Radius"), _T("1.4"), path);
        WritePrivateProfileString(_T("He"), _T("Color"), _T("Cyan"), path);
        WritePrivateProfileString(_T("Li"), _T("Radius"), _T("1.82"), path);
        WritePrivateProfileString(_T("Li"), _T("Color"), _T("Purple"), path);
        WritePrivateProfileString(_T("Be"), _T("Radius"), _T("1.53"), path);
        WritePrivateProfileString(_T("Be"), _T("Color"), _T("Green"), path);
        WritePrivateProfileString(_T("B"), _T("Radius"), _T("1.92"), path);
        WritePrivateProfileString(_T("B"), _T("Color"), _T("Salmon"), path);
        WritePrivateProfileString(_T("C"), _T("Radius"), _T("1.7"), path);
        WritePrivateProfileString(_T("C"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("N"), _T("Radius"), _T("1.55"), path);
        WritePrivateProfileString(_T("N"), _T("Color"), _T("Blue"), path);
        WritePrivateProfileString(_T("O"), _T("Radius"), _T("1.52"), path);
        WritePrivateProfileString(_T("O"), _T("Color"), _T("Red"), path);
        WritePrivateProfileString(_T("F"), _T("Radius"), _T("1.47"), path);
        WritePrivateProfileString(_T("F"), _T("Color"), _T("Green"), path);
        WritePrivateProfileString(_T("Ne"), _T("Radius"), _T("1.54"), path);
        WritePrivateProfileString(_T("Ne"), _T("Color"), _T("Cyan"), path);
        WritePrivateProfileString(_T("Na"), _T("Radius"), _T("2.27"), path);
        WritePrivateProfileString(_T("Na"), _T("Color"), _T("Violet"), path);
        WritePrivateProfileString(_T("Mg"), _T("Radius"), _T("1.73"), path);
        WritePrivateProfileString(_T("Mg"), _T("Color"), _T("Green"), path);
        WritePrivateProfileString(_T("Al"), _T("Radius"), _T("1.84"), path);
        WritePrivateProfileString(_T("Al"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Si"), _T("Radius"), _T("2.1"), path);
        WritePrivateProfileString(_T("Si"), _T("Color"), _T("Orange"), path);
        WritePrivateProfileString(_T("P"), _T("Radius"), _T("1.8"), path);
        WritePrivateProfileString(_T("P"), _T("Color"), _T("Orange"), path);
        WritePrivateProfileString(_T("S"), _T("Radius"), _T("1.8"), path);
        WritePrivateProfileString(_T("S"), _T("Color"), _T("Yellow"), path);
        WritePrivateProfileString(_T("Cl"), _T("Radius"), _T("1.75"), path);
        WritePrivateProfileString(_T("Cl"), _T("Color"), _T("Green"), path);
        WritePrivateProfileString(_T("Ar"), _T("Radius"), _T("1.88"), path);
        WritePrivateProfileString(_T("Ar"), _T("Color"), _T("Cyan"), path);
        WritePrivateProfileString(_T("K"), _T("Radius"), _T("2.75"), path);
        WritePrivateProfileString(_T("K"), _T("Color"), _T("Violet"), path);
        WritePrivateProfileString(_T("Ca"), _T("Radius"), _T("2.31"), path);
        WritePrivateProfileString(_T("Ca"), _T("Color"), _T("Green"), path);
        WritePrivateProfileString(_T("Sc"), _T("Radius"), _T("2.11"), path);
        WritePrivateProfileString(_T("Sc"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Ti"), _T("Radius"), _T("1.87"), path);
        WritePrivateProfileString(_T("Ti"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("V"), _T("Radius"), _T("1.79"), path);
        WritePrivateProfileString(_T("V"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Cr"), _T("Radius"), _T("1.89"), path);
        WritePrivateProfileString(_T("Cr"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Mn"), _T("Radius"), _T("1.97"), path);
        WritePrivateProfileString(_T("Mn"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Fe"), _T("Radius"), _T("1.94"), path);
        WritePrivateProfileString(_T("Fe"), _T("Color"), _T("Orange"), path);
        WritePrivateProfileString(_T("Co"), _T("Radius"), _T("1.92"), path);
        WritePrivateProfileString(_T("Co"), _T("Color"), _T("Blue"), path);
        WritePrivateProfileString(_T("Ni"), _T("Radius"), _T("1.63"), path);
        WritePrivateProfileString(_T("Ni"), _T("Color"), _T("Green"), path);
        WritePrivateProfileString(_T("Cu"), _T("Radius"), _T("1.4"), path);
        WritePrivateProfileString(_T("Cu"), _T("Color"), _T("Orange"), path);
        WritePrivateProfileString(_T("Zn"), _T("Radius"), _T("1.39"), path);
        WritePrivateProfileString(_T("Zn"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Ga"), _T("Radius"), _T("1.87"), path);
        WritePrivateProfileString(_T("Ga"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Ge"), _T("Radius"), _T("2.11"), path);
        WritePrivateProfileString(_T("Ge"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("As"), _T("Radius"), _T("1.85"), path);
        WritePrivateProfileString(_T("As"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Se"), _T("Radius"), _T("1.9"), path);
        WritePrivateProfileString(_T("Se"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Br"), _T("Radius"), _T("1.85"), path);
        WritePrivateProfileString(_T("Br"), _T("Color"), _T("Red"), path);
        WritePrivateProfileString(_T("Kr"), _T("Radius"), _T("2.02"), path);
        WritePrivateProfileString(_T("Kr"), _T("Color"), _T("Cyan"), path);
        WritePrivateProfileString(_T("Rb"), _T("Radius"), _T("3.03"), path);
        WritePrivateProfileString(_T("Rb"), _T("Color"), _T("Violet"), path);
        WritePrivateProfileString(_T("Sr"), _T("Radius"), _T("2.49"), path);
        WritePrivateProfileString(_T("Sr"), _T("Color"), _T("Green"), path);
        WritePrivateProfileString(_T("Y"), _T("Radius"), _T("2.12"), path);
        WritePrivateProfileString(_T("Y"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Zr"), _T("Radius"), _T("2.06"), path);
        WritePrivateProfileString(_T("Zr"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Nb"), _T("Radius"), _T("1.98"), path);
        WritePrivateProfileString(_T("Nb"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Mo"), _T("Radius"), _T("1.9"), path);
        WritePrivateProfileString(_T("Mo"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Tc"), _T("Radius"), _T("1.83"), path);
        WritePrivateProfileString(_T("Tc"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Ru"), _T("Radius"), _T("1.78"), path);
        WritePrivateProfileString(_T("Ru"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Rh"), _T("Radius"), _T("1.73"), path);
        WritePrivateProfileString(_T("Rh"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Pd"), _T("Radius"), _T("1.69"), path);
        WritePrivateProfileString(_T("Pd"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Ag"), _T("Radius"), _T("1.65"), path);
        WritePrivateProfileString(_T("Ag"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Cd"), _T("Radius"), _T("1.61"), path);
        WritePrivateProfileString(_T("Cd"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("In"), _T("Radius"), _T("2.2"), path);
        WritePrivateProfileString(_T("In"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Sn"), _T("Radius"), _T("2.17"), path);
        WritePrivateProfileString(_T("Sn"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Sb"), _T("Radius"), _T("2.06"), path);
        WritePrivateProfileString(_T("Sb"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Te"), _T("Radius"), _T("2.06"), path);
        WritePrivateProfileString(_T("Te"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("I"), _T("Radius"), _T("1.98"), path);
        WritePrivateProfileString(_T("I"), _T("Color"), _T("Purple"), path);
        WritePrivateProfileString(_T("Xe"), _T("Radius"), _T("2.16"), path);
        WritePrivateProfileString(_T("Xe"), _T("Color"), _T("Cyan"), path);
        WritePrivateProfileString(_T("Cs"), _T("Radius"), _T("3.43"), path);
        WritePrivateProfileString(_T("Cs"), _T("Color"), _T("Violet"), path);
        WritePrivateProfileString(_T("Ba"), _T("Radius"), _T("2.68"), path);
        WritePrivateProfileString(_T("Ba"), _T("Color"), _T("Green"), path);
        WritePrivateProfileString(_T("La"), _T("Radius"), _T("2.4"), path);
        WritePrivateProfileString(_T("La"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Ce"), _T("Radius"), _T("2.35"), path);
        WritePrivateProfileString(_T("Ce"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Pr"), _T("Radius"), _T("2.47"), path);
        WritePrivateProfileString(_T("Pr"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Nd"), _T("Radius"), _T("2.06"), path);
        WritePrivateProfileString(_T("Nd"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Pm"), _T("Radius"), _T("2.05"), path);
        WritePrivateProfileString(_T("Pm"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Sm"), _T("Radius"), _T("2.38"), path);
        WritePrivateProfileString(_T("Sm"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Eu"), _T("Radius"), _T("2.31"), path);
        WritePrivateProfileString(_T("Eu"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Gd"), _T("Radius"), _T("2.33"), path);
        WritePrivateProfileString(_T("Gd"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Tb"), _T("Radius"), _T("2.25"), path);
        WritePrivateProfileString(_T("Tb"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Dy"), _T("Radius"), _T("2.28"), path);
        WritePrivateProfileString(_T("Dy"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Ho"), _T("Radius"), _T("2.26"), path);
        WritePrivateProfileString(_T("Ho"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Er"), _T("Radius"), _T("2.26"), path);
        WritePrivateProfileString(_T("Er"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Tm"), _T("Radius"), _T("2.22"), path);
        WritePrivateProfileString(_T("Tm"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Yb"), _T("Radius"), _T("2.22"), path);
        WritePrivateProfileString(_T("Yb"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Lu"), _T("Radius"), _T("2.17"), path);
        WritePrivateProfileString(_T("Lu"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Hf"), _T("Radius"), _T("2.08"), path);
        WritePrivateProfileString(_T("Hf"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Ta"), _T("Radius"), _T("2.0"), path);
        WritePrivateProfileString(_T("Ta"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("W"), _T("Radius"), _T("1.93"), path);
        WritePrivateProfileString(_T("W"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Re"), _T("Radius"), _T("1.88"), path);
        WritePrivateProfileString(_T("Re"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Os"), _T("Radius"), _T("1.85"), path);
        WritePrivateProfileString(_T("Os"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Ir"), _T("Radius"), _T("1.8"), path);
        WritePrivateProfileString(_T("Ir"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Pt"), _T("Radius"), _T("1.77"), path);
        WritePrivateProfileString(_T("Pt"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Au"), _T("Radius"), _T("1.74"), path);
        WritePrivateProfileString(_T("Au"), _T("Color"), _T("Orange"), path);
        WritePrivateProfileString(_T("Hg"), _T("Radius"), _T("1.71"), path);
        WritePrivateProfileString(_T("Hg"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Tl"), _T("Radius"), _T("1.56"), path);
        WritePrivateProfileString(_T("Tl"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Pb"), _T("Radius"), _T("1.54"), path);
        WritePrivateProfileString(_T("Pb"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Bi"), _T("Radius"), _T("1.43"), path);
        WritePrivateProfileString(_T("Bi"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Po"), _T("Radius"), _T("1.35"), path);
        WritePrivateProfileString(_T("Po"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("At"), _T("Radius"), _T("1.27"), path);
        WritePrivateProfileString(_T("At"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Rn"), _T("Radius"), _T("1.2"), path);
        WritePrivateProfileString(_T("Rn"), _T("Color"), _T("Cyan"), path);
        WritePrivateProfileString(_T("Fr"), _T("Radius"), _T("3.48"), path);
        WritePrivateProfileString(_T("Fr"), _T("Color"), _T("Violet"), path);
        WritePrivateProfileString(_T("Ra"), _T("Radius"), _T("2.83"), path);
        WritePrivateProfileString(_T("Ra"), _T("Color"), _T("Green"), path);
        WritePrivateProfileString(_T("Ac"), _T("Radius"), _T("2.6"), path);
        WritePrivateProfileString(_T("Ac"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Th"), _T("Radius"), _T("2.37"), path);
        WritePrivateProfileString(_T("Th"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Pa"), _T("Radius"), _T("2.43"), path);
        WritePrivateProfileString(_T("Pa"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("U"), _T("Radius"), _T("2.4"), path);
        WritePrivateProfileString(_T("U"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Np"), _T("Radius"), _T("2.21"), path);
        WritePrivateProfileString(_T("Np"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Pu"), _T("Radius"), _T("2.43"), path);
        WritePrivateProfileString(_T("Pu"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Am"), _T("Radius"), _T("2.44"), path);
        WritePrivateProfileString(_T("Am"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Cm"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Cm"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Bk"), _T("Radius"), _T("2.44"), path);
        WritePrivateProfileString(_T("Bk"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Cf"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Cf"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Es"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Es"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Fm"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Fm"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Md"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Md"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("No"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("No"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Lr"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Lr"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Rf"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Rf"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Db"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Db"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Sg"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Sg"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Bh"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Bh"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Hs"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Hs"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Mt"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Mt"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Ds"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Ds"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Rg"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Rg"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Cn"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Cn"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Fl"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Fl"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Lv"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Lv"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Ts"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Ts"), _T("Color"), _T("Gray"), path);
        WritePrivateProfileString(_T("Og"), _T("Radius"), _T("2.45"), path);
        WritePrivateProfileString(_T("Og"), _T("Color"), _T("Gray"), path);

    }
}
