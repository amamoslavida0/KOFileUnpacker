#pragma once
#include "afxdialogex.h"
#include <vector>
#include "KOFileUnpackerDlg.h"

// CTabUnpack child dialog to unpack Knight online src files

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
	{
	}
};

class CTabUnpack : public CDialogEx
{
	DECLARE_DYNAMIC(CTabUnpack)

public:
	CKOFileUnpackerDlg* m_pMainDlg;
	CTabUnpack(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTabUnpack();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB_UNPACK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CEdit m_editHdrPath;
	CEdit m_editSrcPath;
	CEdit m_editUnpackPath;
	// paths
	CString m_strPathSrc;
	CString m_strPathHdr;
	CString m_strPathUnpack;

	CProgressCtrl m_progress;
	CButton m_btnUnpack;

	std::vector<file_info> m_vecFileInfo;

	// thread to avoid ui freeze
	static UINT UnpackThread(LPVOID pParam);
	LRESULT OnUnpackDone(WPARAM, LPARAM);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
public:
	void ReadHdr();
	void Unpack();
	inline CString GetText(UINT id) const
	{
		CString str;
		str.LoadString(id);
		return str;
	}
public:
	afx_msg void OnBnClickedButtonSelectHdr();
	afx_msg void OnBnClickedButtonSelectSrc();
	afx_msg void OnBnClickedButtonSelectUnpackPath();
	afx_msg void OnBnClickedButtonUnpack();
};
