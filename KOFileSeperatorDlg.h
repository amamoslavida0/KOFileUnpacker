
// KOFileSeperatorDlg.h : header file
//

#pragma once


// CKOFileSeperatorDlg dialog
class CKOFileSeperatorDlg : public CDialogEx
{
// Construction
public:
	CKOFileSeperatorDlg(CWnd* pParent = nullptr);	// standard constructor

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
	CEdit m_editHdrPath;
	CEdit m_editSrcPath;
	CEdit m_editExtractPath;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHelpAbout();
	afx_msg void OnBnClickedButtonSelectHdr();
	afx_msg void OnBnClickedButtonSelectSrc();
};
