#pragma once
#include "afxdialogex.h"
#include <vector>

// CPackDlg dialog

inline const CString FNui = L"ui";
inline const CString FNsnd = L"snd";
inline const CString FNobject = L"object";
inline const CString FNitem = L"item";
inline const CString FNfx = L"fx";


enum class file_type
{
	ui,
	snd,
	object,
	item,
	fx,
	unknown
};

class CPackDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPackDlg)

public:
	CPackDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPackDlg();
	CEdit m_editPackPath;
	CEdit m_editHdrPath;

	CString m_strPathHdr;
	file_type m_eFileType;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB_PACK };
#endif

protected:
	CProgressCtrl m_progressPack;
	std::vector<CString> m_vecFileNames;
	CButton m_btnPack;
	uint8_t m_byPackResult;

	CString GetFileNameWithExtensionFromPath(const CString& strPath);
	CString GetFileNameFromPath(const CString& strPath);
	file_type GetFileType(const CString& strPath);
	void CreateNewFiles();
	uint8_t CreateNewSrcFile();
	uint8_t CreateNewHdrFile();
	void GenerateFileNameVector();
	inline CString GetText(UINT id) const
	{
		CString str;
		str.LoadString(id);
		return str;
	}

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	// thread to avoid ui freeze
	static UINT PackThread(LPVOID pParam);
	LRESULT OnPackDone(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPack();
	afx_msg void OnBnClickedButtonPackSelectExtractPath();
	afx_msg void OnBnClickedButtonPackSelectHdr();
};
