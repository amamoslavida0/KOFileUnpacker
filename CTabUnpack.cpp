// CTabUnpack.cpp : implementation file
//

#include "pch.h"
#include "KOFileUnpacker.h"
#include "afxdialogex.h"
#include "CTabUnpack.h"


// CTabUnpack dialog

IMPLEMENT_DYNAMIC(CTabUnpack, CDialogEx)

CTabUnpack::CTabUnpack(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TAB_UNPACK, pParent)
{
	m_strPathSrc = L"";
	m_strPathHdr = L"";
	m_strPathUnpack = L"";
	m_pMainDlg = nullptr;
}

CTabUnpack::~CTabUnpack()
{	
}

void CTabUnpack::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HDR_PATH, m_editHdrPath);
	DDX_Control(pDX, IDC_EDIT_SRC_PATH, m_editSrcPath);
	DDX_Control(pDX, IDC_EDIT_EXTRACT_PATH, m_editUnpackPath);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_BUTTON_EXTRACT, m_btnUnpack);
}


BOOL CTabUnpack::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_progress.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTabUnpack::OnDestroy() 
{
	if (m_pMainDlg != nullptr)
	{
		delete m_pMainDlg;
		m_pMainDlg = nullptr;
	}
}


BEGIN_MESSAGE_MAP(CTabUnpack, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_HDR, &CTabUnpack::OnBnClickedButtonSelectHdr)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_SRC, &CTabUnpack::OnBnClickedButtonSelectSrc)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_EXTRACT_PATH, &CTabUnpack::OnBnClickedButtonSelectUnpackPath)
	ON_BN_CLICKED(IDC_BUTTON_EXTRACT, &CTabUnpack::OnBnClickedButtonUnpack)
	ON_MESSAGE(WM_USER + 1, &CTabUnpack::OnUnpackDone)
END_MESSAGE_MAP()


// CTabUnpack message handlers

void CTabUnpack::OnBnClickedButtonSelectHdr()
{
	CFileDialog dlg(
		TRUE,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"Header Files (*.hdr)|*.hdr|All Files (*.*)|*.*||"
	);

	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		m_editHdrPath.SetWindowText(path);
	}
}

void CTabUnpack::OnBnClickedButtonSelectSrc()
{
	CFileDialog dlg(
		TRUE,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"Source Files (*.src)|*.src|All Files (*.*)|*.*||"
	);

	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		m_editSrcPath.SetWindowText(path);
	}
}

void CTabUnpack::OnBnClickedButtonSelectUnpackPath()
{
	CFolderPickerDialog dlg;

	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		m_editUnpackPath.SetWindowText(path);
	}
}

void CTabUnpack::OnBnClickedButtonUnpack()
{
	m_editHdrPath.GetWindowText(m_strPathHdr);
	m_pMainDlg->SetHdrPath(m_strPathHdr);

	m_editSrcPath.GetWindowText(m_strPathSrc);
	m_pMainDlg->SetSrcPath(m_strPathSrc);

	m_editUnpackPath.GetWindowText(m_strPathUnpack);
	m_pMainDlg->SetUnpackPath(m_strPathUnpack);


	CString strError = L"";
	if (m_strPathHdr.IsEmpty())
	{
		strError += GetText(IDS_WARNING_1) + L"\r\n";
	}

	if (m_strPathSrc.IsEmpty())
	{
		strError += GetText(IDS_WARNING_2) + L"\r\n";
	}

	if (m_strPathUnpack.IsEmpty())
	{
		strError += GetText(IDS_WARNING_3) + L"\r\n";
	}

	if (strError.IsEmpty() == false)
	{
		AfxMessageBox(strError, MB_OK);
		return;
	}

	// make button unclickable
	m_btnUnpack.EnableWindow(FALSE);
	// start thread to avoid UI freeze.
	AfxBeginThread(UnpackThread, this);
}

UINT CTabUnpack::UnpackThread(LPVOID pParam)
{
	CTabUnpack* pDlg = (CTabUnpack*) pParam;

	pDlg->ReadHdr();
	pDlg->Unpack();

	pDlg->PostMessage(WM_USER + 1); // done signal

	return 0;
}

LRESULT CTabUnpack::OnUnpackDone(WPARAM, LPARAM)
{
	m_btnUnpack.EnableWindow(TRUE);

	AfxMessageBox(GetText(IDS_SUCCESS_UNPACK), MB_OK | MB_ICONINFORMATION);

	return 0;
}

void CTabUnpack::ReadHdr()
{

	HANDLE hFile = CreateFile(
		m_strPathHdr,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		uint32_t dwHeader = 0;
		uint32_t dwUnknown = 0;
		DWORD bytesRead = 0;

		// read only once
		ReadFile(hFile, &dwHeader, sizeof(uint32_t), &bytesRead, NULL);
		ReadFile(hFile, &dwUnknown, sizeof(uint32_t), &bytesRead, NULL);

		while (true)
		{
			file_info fileInfo;

			if (!ReadFile(hFile, &fileInfo.wLenFN, sizeof(uint16_t), &bytesRead, NULL)
				|| bytesRead != sizeof(uint16_t))
			{
				break;
			}

			if (fileInfo.wLenFN > 0)
			{
				std::vector<char> buf(fileInfo.wLenFN + 1, 0);

				ReadFile(hFile, buf.data(), fileInfo.wLenFN, &bytesRead, NULL);

				fileInfo.strFN = CString(buf.data());
			}

			ReadFile(hFile, &fileInfo.dwOffset, sizeof(uint32_t), &bytesRead, NULL);
			ReadFile(hFile, &fileInfo.dwBytes, sizeof(uint32_t), &bytesRead, NULL);

			m_vecFileInfo.push_back(fileInfo);
		}


		CloseHandle(hFile);
	}

#ifdef _DEBUG
	for (const auto& fileInfo : m_vecFileInfo)
	{
		TRACE(L"FileName: %s | Len : %hu | Offset : %u | Bytes : %u\n",
			fileInfo.strFN.GetString(),
			fileInfo.wLenFN,
			fileInfo.dwOffset,
			fileInfo.dwBytes);
	}
#endif
}

void CTabUnpack::Unpack()
{
	// progress bar
	m_progress.ShowWindow(SW_SHOW);
	m_progress.SetRange(0, (int) m_vecFileInfo.size());
	m_progress.SetPos(0);

	HANDLE hFile = CreateFile(
		m_strPathSrc,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		CString msg;
		msg.Format(GetText(IDS_WARNING_4), err);
		AfxMessageBox(msg);
		return;
	}

	DWORD bytesRead = 0;

	int iCount = 0;
	for (const auto& fileInfo : m_vecFileInfo)
	{
		CString strPathOut;
		strPathOut.Format(L"%s\\%s", m_strPathUnpack, fileInfo.strFN.GetString());
		HANDLE hOut = CreateFile(
			strPathOut,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hOut == INVALID_HANDLE_VALUE)
		{
			continue;
		}

		SetFilePointer(hFile, fileInfo.dwOffset, NULL, FILE_BEGIN);
		std::vector<char> buffer(fileInfo.dwBytes);
		ReadFile(hFile, buffer.data(), fileInfo.dwBytes, &bytesRead, NULL);
		DWORD bytesWritten = 0;
		WriteFile(hOut, buffer.data(), bytesRead, &bytesWritten, NULL);
		CloseHandle(hOut);
		m_progress.SetPos(++iCount);
	}

	m_progress.ShowWindow(SW_HIDE);
	CloseHandle(hFile);
}



