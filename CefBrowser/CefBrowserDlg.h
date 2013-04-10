
// CefBrowserDlg.h : ͷ�ļ�
//

#pragma once

#include "TabHost.h"

// CCefBrowserDlg �Ի���
class CCefBrowserDlg : public CDialog
{
// ����
public:
	CCefBrowserDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CEFBROWSER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnClose();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBtnTabClicked();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
};
