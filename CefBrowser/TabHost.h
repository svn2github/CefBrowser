#pragma once

#include <vector>

struct stTabInfo
{
    HWND hWndButton;
    HWND hWndBrowser;
    int  BrowserId;
    CefRefPtr<CefBrowser>   browser;
};
typedef std::vector<stTabInfo> TabInfoVector;

class CTabHost
{
public:
    CTabHost();
    ~CTabHost(void);

    void Init(HWND hWndHost);
    void Destroy();

    // Tab����
    void ShowTab(HWND hTabButton);

    void OpenUrl(LPCTSTR szUrl);
    void CreateTab(LPCTSTR szUrl);
    void AddTab(CefRefPtr<CefBrowser> browser, CefWindowInfo* info);
    // ����ҳ�������ɾ���Լ�ʱ�����ñ�����
    void RemoveTab(CefRefPtr<CefBrowser> browser);
    // �û�Ҫ��ر�Tabʱ�����ñ�����
    void CloseTab(HWND hWndButton);

    // �¼���Ӧ
    void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward);
    void OnAddressChange(CefRefPtr<CefBrowser> browser, const CefString& url);
    void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title);

    // ���������
    void GoBack();
    void GoForward();
    void Reload(BOOL bIgnoreCache);

protected:
    void GetBrowserRect(RECT& rcBrowser);
    void AddTabImpl(CefRefPtr<CefBrowser> browser);
    static void AddTabTask(CTabHost* pTabHost, CefRefPtr<CefBrowser> browser);

    stTabInfo* GetTabInfoByButton(HWND hWndButton);
    stTabInfo* GetTabInfoByBrowser(const CefRefPtr<CefBrowser> browser);
    TabInfoVector::iterator GetTabInfoByButtonEx(HWND hWndButton);
    TabInfoVector::iterator GetTabInfoByBrowserEx(const CefRefPtr<CefBrowser> browser);

protected:
    HWND    m_hWndHost;
    HWND    m_hEditUrlWnd;

    HWND    m_hBtnBack;
    HWND    m_hBtnForward;
    HWND    m_hBtnRefresh;

    HWND    m_hVisibleTabButton;
    HWND    m_hVisibleBrowser;

    TabInfoVector  m_vctTabInfo;

    HWND        m_hBtnContainer;
    CWndContainer m_BtnLayout;

    HWND        m_hBrowContainer;
    CWndLayout  m_BrowLayout;
};
