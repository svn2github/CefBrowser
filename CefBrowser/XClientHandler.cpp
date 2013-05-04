#include "StdAfx.h"
#include "XClientHandler.h"


#include "XGlobal.h"
#include "XUtil.h"
#include <locale>
#include "XRegUtil.h"


XClientHandler::XClientHandler(void)
{
    // ����֧�ֵ�Э�����ݣ��Ǵ�E:\Src\Chromium\src\src\net\url_request\url_request_job_manager.cc�ļ��г�ȡ������
    m_ProtocalSet.insert(_T("http"));
    m_ProtocalSet.insert(_T("https"));
    m_ProtocalSet.insert(_T("file"));
    m_ProtocalSet.insert(_T("ftp"));
    m_ProtocalSet.insert(_T("about"));
    m_ProtocalSet.insert(_T("data"));
    m_ProtocalSet.insert(_T("chrome"));
}

XClientHandler::~XClientHandler(void)
{
}


// CefKeyboardHandler
bool XClientHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                           const CefKeyEvent& event,
                           CefEventHandle os_event,
                           bool* is_keyboard_shortcut)
{
    if(event.windows_key_code == VK_F5)
    {
        // ����F5ˢ��
        BOOL bIgnoreCache = (event.modifiers == EVENTFLAG_CONTROL_DOWN);
        XGlobal::inst().TabHost.Reload(bIgnoreCache);
    }
    return false;
}

bool XClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  const CefString& target_url,
                                  const CefString& target_frame_name,
                                  const CefPopupFeatures& popupFeatures,
                                  CefWindowInfo& windowInfo,
                                  CefRefPtr<CefClient>& client,
                                  CefBrowserSettings& settings,
                                  bool* no_javascript_access)
{
    if (browser->GetHost()->IsWindowRenderingDisabled()) {
        // Cancel popups in off-screen rendering mode.
        return true;
    }

    XGlobal::inst().TabHost.AddTab(browser, &windowInfo);

    return false;
}

void XClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    XGlobal::inst().TabHost.AddTab(browser, NULL);
}

bool XClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
    return false;
}

void XClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    XGlobal::inst().TabHost.RemoveTab(browser);
}

bool XClientHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
    return false;
}

void XClientHandler::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
{
}

void XClientHandler::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
{
}

void XClientHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                     PaintElementType type,
                     const RectList& dirtyRects,
                     const void* buffer,
                     int width,
                     int height)
{
    ;
}

//////////////////////////////////////////////////////////////////////////
// CefDisplayHandler methods
void XClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                  bool isLoading,
                                  bool canGoBack,
                                  bool canGoForward)
{
    XGlobal::inst().TabHost.OnLoadingStateChange(browser, isLoading, canGoBack, canGoForward);
}

void XClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             const CefString& url)
{
    if(!frame->IsMain())
        return;

    XGlobal::inst().TabHost.OnAddressChange(browser, url.c_str());
}

void XClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                           const CefString& title)
{
    XGlobal::inst().TabHost.OnTitleChange(browser, title.c_str());
}


//////////////////////////////////////////////////////////////////////////
// CefRequestHandler
bool XClientHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  CefRefPtr<CefRequest> request)
{
    std::wstring strUrl = request->GetURL().ToWString();
    size_t pos = strUrl.find(_T("://"));
    if(pos == std::wstring::npos)
        return false;

    std::wstring strScheme = strUrl.substr(0, pos);
    std::transform(strScheme.begin(), strScheme.end(), strScheme.begin(), &::_totlower);
    if(m_ProtocalSet.find(strScheme) != m_ProtocalSet.end())
        return false;

    // ��ѯ��scheme��Ϣ
    CRegUtil reg;
    std::wstring strTemp;
    if(!reg.Open(HKEY_CLASSES_ROOT, strScheme.c_str(), RM_ReadOnly)
        || !reg.GetValue(L"URL Protocol", strTemp))
    {
        // ��ʾû�й�����Э��
        std::wstring strJs = L"alert(\"û�в��ҵ���������Э��ĳ���\\r\\n";
        strJs += strUrl.c_str();
        strJs += L"\");";
        browser->GetMainFrame()->ExecuteJavaScript(strJs.c_str(), "", 0);
        return false;
    }

    // ����������
    size_t stubPos = 0;
    std::wstring strShell;
    strTemp = strScheme;
    strTemp += L"\\Shell\\Open\\command";
    if(!reg.Open(HKEY_CLASSES_ROOT, strTemp.c_str(), RM_ReadOnly)
        || !reg.GetValue(L"", strShell)
        || ((stubPos = strShell.find(L"%1")) == std::wstring::npos))
    {
        // ��ʾЭ��ע���������
        std::wstring strJs = L"alert(\"����Э��Ĵ������ע�����\\r\\n";
        strJs += strUrl.c_str();
        strJs += L"\");";
        browser->GetMainFrame()->ExecuteJavaScript(strJs.c_str(), "", 0);
        return false;
    }

    strShell.replace(stubPos, 2, strUrl.c_str());

    {
        std::wstring oldToken = _T("\\");
        std::wstring newToken = _T("\\\\");
        ReplaceToken(strShell, oldToken, newToken);
    }

    // ƴװ��ʾ���
    // var r = confirm(....); if(r) exec_protocal(exe, param)
    std::wstring strMsg = L"var msg='���������ⲿӦ�ò��ܴ���";
    strMsg += strScheme;
    strMsg += L"���ӡ������������\\r\\n";
    strMsg += strUrl;
    strMsg += L"��\\r\\n\\r\\n�����������һ���󣬾ͻ���������Ӧ�ã�\\r\\n";
    strMsg += strShell;
    strMsg += L"\';\r\n";

    std::wstring strJs;
    strJs += strMsg;
    strJs += L"var r = confirm(msg);";
    strJs += L"\r\nif(r) app.exec_protocal(\'";
    strJs += strShell;
    strJs += L"\');";

    // ��ʾЭ��ע���������
    browser->GetMainFrame()->ExecuteJavaScript(strJs.c_str(), "", 0);

    return true;
}

CefRefPtr<CefResourceHandler> XClientHandler::GetResourceHandler(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefRequest> request)
{
    const wchar_t* const prefix = L"http://test/";
    const size_t prefix_length = 12;
    std::wstring strUrl = request->GetURL().ToWString();
    if(strUrl.find(prefix) == 0)
    {
        strUrl.erase(0, prefix_length);

        CefRefPtr<CefStreamReader> stream = GetBinaryResourceReader(RT_HTML, strUrl.c_str());
        ASSERT(stream.get());
        return new CefStreamResourceHandler("text/html", stream);
    }

    return NULL;
}

void XClientHandler::OnProtocolExecution(CefRefPtr<CefBrowser> browser,
                                 const CefString& url,
                                 bool& allow_os_execution)
{
    allow_os_execution = false;
    // ::ShellExecute(browser->GetHost()->GetWindowHandle(), _T("open"), url.ToWString().c_str(), NULL, NULL, SW_SHOW);
}


