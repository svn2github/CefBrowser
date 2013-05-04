#pragma once



// �ṩBrowser��App������߼�ʵ��
class XAppExtentionHandler : public CefV8Handler
{
public:
    XAppExtentionHandler(void);
    ~XAppExtentionHandler(void);


    // CefV8Handler
    virtual bool Execute(const CefString& name,
        CefRefPtr<CefV8Value> object,
        const CefV8ValueList& arguments,
        CefRefPtr<CefV8Value>& retval,
        CefString& exception);

protected:
    void SetCallback(CefRefPtr<CefV8Value> object,
        const CefV8ValueList& arguments,
        CefRefPtr<CefV8Value>& retval,
        CefString& exception);

    void UpdateString(CefRefPtr<CefV8Value> object,
        const CefV8ValueList& arguments,
        CefRefPtr<CefV8Value>& retval,
        CefString& exception);

    // JS��Ҫ��ִ������thunder://Э���Ӧ������
    void ExecProtocal(CefRefPtr<CefV8Value> object,
        const CefV8ValueList& arguments,
        CefRefPtr<CefV8Value>& retval,
        CefString& exception);

protected:
    // 
    struct stCallbackInfo
    {
        int nBrowserId;
        int64 nFrameId;
        CefRefPtr<CefV8Value> function;
        CefRefPtr<CefV8Context> context;
    };
    typedef std::vector<stCallbackInfo> CallbackInfoVector;
    CallbackInfoVector  m_vctCallbackInfo;


    IMPLEMENT_REFCOUNTING(XAppExtentionHandler);
};
