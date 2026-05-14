
// KOFileUnpackerDlg.h : header file
//

#pragma once
#include <vector>

class CPackDlg;
class CUnpackDlg;

// CKOFileUnpackerDlg dialog
class CKOFileUnpackerDlg : public CDialogEx
{
// Construction
public:
	CKOFileUnpackerDlg(CWnd* pParent = nullptr);	// standard constructor
	~CKOFileUnpackerDlg();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KOFILESEPERATOR_DIALOG
	};
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	// tab control
	CTabCtrl m_tabMain;
	// tab pages, dialogs
	CUnpackDlg* m_tabUnpackDlg;
	CPackDlg* m_tabPackDlg;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()	
public:
	afx_msg void OnHelpAbout();
	afx_msg void OnTcnSelchangeTabPages(NMHDR* pNMHDR, LRESULT* pResult);
};
