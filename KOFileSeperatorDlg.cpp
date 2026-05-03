
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
}

void CKOFileSeperatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HDR_PATH, m_editHdrPath);
	DDX_Control(pDX, IDC_EDIT_SRC_PATH, m_editSrcPath);
	DDX_Control(pDX, IDC_EDIT_EXTRACT_PATH, m_editExtractPath);

}

BEGIN_MESSAGE_MAP(CKOFileSeperatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_HELP_ABOUT, &CKOFileSeperatorDlg::OnHelpAbout)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_HDR, &CKOFileSeperatorDlg::OnBnClickedButtonSelectHdr)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_SRC, &CKOFileSeperatorDlg::OnBnClickedButtonSelectSrc)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_EXTRACT_PATH, &CKOFileSeperatorDlg::OnBnClickedButtonSelectExtractPath)
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

void CKOFileSeperatorDlg::OnBnClickedButtonSelectExtractPath()
{
	CFolderPickerDialog dlg;

	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		m_editExtractPath.SetWindowText(path);
	}
}
