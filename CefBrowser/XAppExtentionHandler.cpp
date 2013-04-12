#include "StdAfx.h"
#include "XAppExtentionHandler.h"


#include "algorithm"

XAppExtentionHandler::XAppExtentionHandler(void)
{
}

XAppExtentionHandler::~XAppExtentionHandler(void)
{
}

//////////////////////////////////////////////////////////////////////////
// CefV8Handler
bool XAppExtentionHandler::Execute(const CefString& name,
                     CefRefPtr<CefV8Value> object,
                     const CefV8ValueList& arguments,
                     CefRefPtr<CefV8Value>& retval,
                     CefString& exception)
{
    if(name == "setCallback")
    {
        SetCallback(object, arguments, retval, exception);
    }
    else if(name == "UpdateString")
    {
        UpdateString(object, arguments, retval, exception);
    }
    else
    {
        exception.FromWString(L"���app����Ч��������");
    }
    return true;
}


void XAppExtentionHandler::SetCallback(CefRefPtr<CefV8Value> object,
                 const CefV8ValueList& arguments,
                 CefRefPtr<CefV8Value>& retval,
                 CefString& exception)
{
    if(arguments.size() == 1 && arguments[0]->IsFunction())
    {
        CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
        ASSERT(browser.get());

        stCallbackInfo info;
        info.context = CefV8Context::GetCurrentContext();
        info.nFrameId = info.context->GetFrame()->GetIdentifier();
        info.nBrowserId = browser->GetIdentifier();
        info.function = arguments[0];
        m_vctCallbackInfo.push_back(info);
    }
}

void XAppExtentionHandler::UpdateString(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& exception)
{
    if(arguments.size() == 1 && arguments[0]->IsString())
    {
        CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
        CefRefPtr<CefBrowser> browser = context->GetBrowser();
        int nBrowserId = browser->GetIdentifier();
        CefRefPtr<CefFrame> frame = context->GetFrame();

        CallbackInfoVector::iterator ite = m_vctCallbackInfo.begin();
        for(; ite != m_vctCallbackInfo.end(); ++ ite)
        {
            stCallbackInfo& info = *ite;
            if(info.nBrowserId == nBrowserId
                && info.nFrameId == frame->GetIdentifier()
                && info.context->IsSame(context))
            {
                break;
            }
        }
        if(ite == m_vctCallbackInfo.end())
            return;

        stCallbackInfo& info = *ite;
        info.context->Enter();

        std::wstring str = arguments[0]->GetStringValue().ToWString();
        std::reverse(str.begin(), str.end());

        CefV8ValueList data;
        data.push_back(CefV8Value::CreateString(str));
        info.function->ExecuteFunction(object, data);

        info.context->Exit();
    }
}

