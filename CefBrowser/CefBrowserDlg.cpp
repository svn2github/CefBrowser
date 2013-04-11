
// CefBrowserDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CefBrowser.h"
#include "CefBrowserDlg.h"

#include "XGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCefBrowserDlg �Ի���


//////////////////////////////////////////////////////////////////////////

WNDPROC g_OldEditProc = NULL;
LRESULT WINAPI EditWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_CHAR && wParam == VK_RETURN)
    {
        ::PostMessage(::GetParent(hWnd), WM_COMMAND, MAKELONG(IDOK, BN_CLICKED), (LPARAM)hWnd);
        return 0;
    }
    return ::CallWindowProc(g_OldEditProc, hWnd, message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////


CCefBrowserDlg::CCefBrowserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCefBrowserDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCefBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCefBrowserDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_TAB_BUTTON, &CCefBrowserDlg::OnBtnTabClicked)
    ON_BN_DOUBLECLICKED(IDC_TAB_BUTTON, &CCefBrowserDlg::OnBtnTabDoubleClicked)
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CCefBrowserDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BTN_BACK, &CCefBrowserDlg::OnBnClickedBtnBack)
    ON_BN_CLICKED(IDC_BTN_REFRESH, &CCefBrowserDlg::OnBnClickedBtnRefresh)
    ON_BN_CLICKED(IDC_BTN_FORWARD, &CCefBrowserDlg::OnBnClickedBtnForward)
END_MESSAGE_MAP()


// CCefBrowserDlg ��Ϣ�������

BOOL CCefBrowserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������

    // 
    HWND hEditUrlWnd = GetDlgItem(IDC_EDIT_URL)->GetSafeHwnd();
    g_OldEditProc = (WNDPROC)::SetWindowLongPtr(hEditUrlWnd, GWLP_WNDPROC, (LONG_PTR)&EditWindowProc);

    // 
    XGlobal::inst().WndLayout.Init(m_hWnd);
    XGlobal::inst().WndLayout.AddControlById(IDC_EDIT_URL, Layout_HFill | Layout_Top);
    XGlobal::inst().WndLayout.AddControlById(IDOK, Layout_Top | Layout_Right);
    XGlobal::inst().WndLayout.AddControlById(IDC_FRAME_BROWSER, Layout_HFill | Layout_VFill);

    XGlobal::inst().TabHost.Init(m_hWnd);
    XGlobal::inst().TabHost.OpenUrl(_T("http://www.youku.com"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCefBrowserDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCefBrowserDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);

        RECT rcClient;
        ::GetClientRect(m_hWnd, &rcClient);
        ::FillRect(dc.GetSafeHdc(), &rcClient, (HBRUSH)::GetStockObject(WHITE_BRUSH));
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCefBrowserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCefBrowserDlg::OnClose()
{
    XGlobal::inst().TabHost.Destroy();
    ::PostQuitMessage(0);
    CDialog::OnClose();
}

BOOL CCefBrowserDlg::OnEraseBkgnd(CDC* pDC)
{
    return 1;
}

HBRUSH CCefBrowserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if(nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)::GetStockObject(NULL_BRUSH);
    }
    else
    {
        return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
}

void CCefBrowserDlg::OnBtnTabClicked()
{
    XGlobal::inst().TabHost.ShowTab(::GetFocus());
}

void CCefBrowserDlg::OnBtnTabDoubleClicked()
{
    XGlobal::inst().TabHost.CloseTab(::GetFocus());
}

void CCefBrowserDlg::OnBnClickedOk()
{
    CString strUrl;
    GetDlgItemText(IDC_EDIT_URL, strUrl);
    XGlobal::inst().TabHost.OpenUrl(strUrl);
}

void CCefBrowserDlg::OnBnClickedBtnBack()
{
    XGlobal::inst().TabHost.GoBack();
}

void CCefBrowserDlg::OnBnClickedBtnRefresh()
{
    XGlobal::inst().TabHost.Refresh();
}

void CCefBrowserDlg::OnBnClickedBtnForward()
{
    XGlobal::inst().TabHost.GoForward();
}
