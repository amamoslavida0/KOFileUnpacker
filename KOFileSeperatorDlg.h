
// KOFileSeperatorDlg.h : header file
//

#pragma once
#include <vector>

struct file_info
{
	uint16_t wLenFN;
	CString strFN;
	uint32_t dwOffset;
	uint32_t dwBytes;

	file_info() 
		: wLenFN(0)
		, strFN(L"")
		, dwOffset(0)
		, dwBytes(0)
	{}
};

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
	CEdit m_editUnpackPath;
	CString m_strPathSrc;
	CString m_strPathHdr;
	CString m_strPathUnpack;
	CProgressCtrl m_progress;
	CButton m_btnUnpack;
	std::vector<file_info> m_vecFileInfo;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	// thread to avoid ui freeze
	static UINT UnpackThread(LPVOID pParam);
	LRESULT OnUnpackDone(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
protected:
	void ReadHdr();
	void Unpack();
	inline CString GetText(UINT id) const
	{
		CString str;
		str.LoadString(id);
		return str;
	}
public:
	afx_msg void OnHelpAbout();
	afx_msg void OnBnClickedButtonSelectHdr();
	afx_msg void OnBnClickedButtonSelectSrc();
	afx_msg void OnBnClickedButtonSelectUnpackPath();
	afx_msg void OnBnClickedButtonUnpack();
};
