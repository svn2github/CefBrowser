#pragma once




// ��ʼ��ȫ�������ж���
void AppInitCommandLine(int argc, const char* const* argv);


// ��ʼ��Cef������
void AppGetSettings(CefSettings& settings);

// ���ر��������ڵ�Ŀ¼
std::wstring AppGetAppPath();
// ���ر����������
std::wstring AppGetAppName();

// ��ʼ���������Flash
void InitWebPlugins();

// ����Exe�е���Դ
bool LoadStringResource(const wchar_t* type, CefString name, CefString& strData);
CefRefPtr<CefStreamReader> GetBinaryResourceReader(const wchar_t* type, CefString name);


void ReplaceToken(std::wstring& str, std::wstring& oldToken, std::wstring& newToken);

