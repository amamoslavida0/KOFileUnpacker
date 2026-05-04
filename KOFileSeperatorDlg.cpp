
// KOFileSeperatorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "KOFileSeperator.h"
#include "KOFileSeperatorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKOFileSeperatorDlg dialog



CKOFileSeperatorDlg::CKOFileSeperatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KOFILESEPERATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strPathSrc = L"";
	m_strPathHdr = L"";
	m_strPathExtract = L"";
}

void CKOFileSeperatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HDR_PATH, m_editHdrPath);
	DDX_Control(pDX, IDC_EDIT_SRC_PATH, m_editSrcPath);
	DDX_Control(pDX, IDC_EDIT_EXTRACT_PATH, m_editExtractPath);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_BUTTON_EXTRACT, m_btnExtract);
}

BEGIN_MESSAGE_MAP(CKOFileSeperatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_HELP_ABOUT, &CKOFileSeperatorDlg::OnHelpAbout)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_HDR, &CKOFileSeperatorDlg::OnBnClickedButtonSelectHdr)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_SRC, &CKOFileSeperatorDlg::OnBnClickedButtonSelectSrc)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_EXTRACT_PATH, &CKOFileSeperatorDlg::OnBnClickedButtonSelectUnpackPath)
	ON_BN_CLICKED(IDC_BUTTON_EXTRACT, &CKOFileSeperatorDlg::OnBnClickedButtonUnpack)
	ON_MESSAGE(WM_USER + 1, &CKOFileSeperatorDlg::OnUnpackDone)
END_MESSAGE_MAP()


// CKOFileSeperatorDlg message handlers

BOOL CKOFileSeperatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// hide progressbar at the start
	m_progress.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKOFileSeperatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKOFileSeperatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKOFileSeperatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKOFileSeperatorDlg::OnHelpAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CKOFileSeperatorDlg::OnBnClickedButtonSelectHdr()
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

void CKOFileSeperatorDlg::OnBnClickedButtonSelectSrc()
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

void CKOFileSeperatorDlg::OnBnClickedButtonSelectUnpackPath()
{
	CFolderPickerDialog dlg;

	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		m_editExtractPath.SetWindowText(path);
	}
}

void CKOFileSeperatorDlg::OnBnClickedButtonUnpack()
{
	m_editHdrPath.GetWindowText(m_strPathHdr);
	m_editSrcPath.GetWindowText(m_strPathSrc);
	m_editExtractPath.GetWindowText(m_strPathExtract);

	CString strError = L"";
	if (m_strPathHdr.IsEmpty())
	{
		strError += GetText(IDS_WARNING_1) + L"\r\n";
	}

	if (m_strPathSrc.IsEmpty())
	{
		strError += GetText(IDS_WARNING_2) + L"\r\n";
	}

	if (m_strPathExtract.IsEmpty())
	{
		strError += GetText(IDS_WARNING_3) + L"\r\n";
	}

	if (strError.IsEmpty() == false)
	{
		AfxMessageBox(strError, MB_OK);
		return;
	}

	// make button unclickable
	m_btnExtract.EnableWindow(FALSE);
	// start thread to avoid UI freeze.
	AfxBeginThread(UnpackThread, this);
}

UINT CKOFileSeperatorDlg::UnpackThread(LPVOID pParam)
{
	CKOFileSeperatorDlg* pDlg = (CKOFileSeperatorDlg*) pParam;

	pDlg->ReadHdr();
	pDlg->Unpack();

	pDlg->PostMessage(WM_USER + 1); // done signal

	return 0;
}

LRESULT CKOFileSeperatorDlg::OnUnpackDone(WPARAM, LPARAM)
{
	m_btnExtract.EnableWindow(TRUE);

	AfxMessageBox(GetText(IDS_SUCCESS_UNPACK), MB_OK | MB_ICONINFORMATION);

	return 0;
}

void CKOFileSeperatorDlg::ReadHdr() 
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

void CKOFileSeperatorDlg::Unpack() 
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
		strPathOut.Format(L"%s\\%s", m_strPathExtract, fileInfo.strFN.GetString());
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
