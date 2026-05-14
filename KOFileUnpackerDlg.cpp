
// KOFileSeperatorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "KOFileUnpacker.h"
#include "KOFileUnpackerDlg.h"
#include "afxdialogex.h"
#include "CUnpackDlg.h"
#include "CPackDlg.h"

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



CKOFileUnpackerDlg::CKOFileUnpackerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KOFILESEPERATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_tabUnpackDlg = new CUnpackDlg;
	m_tabPackDlg = new CPackDlg;
}

CKOFileUnpackerDlg::~CKOFileUnpackerDlg()
{
}

void CKOFileUnpackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PAGES, m_tabMain);	
}

BEGIN_MESSAGE_MAP(CKOFileUnpackerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_HELP_ABOUT, &CKOFileUnpackerDlg::OnHelpAbout)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PAGES, &CKOFileUnpackerDlg::OnTcnSelchangeTabPages)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CKOFileSeperatorDlg message handlers

BOOL CKOFileUnpackerDlg::OnInitDialog()
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

	// Add extra initialization here
	// hide progressbar at the start

	// display page names on tabctrl
	m_tabMain.InsertItem(0, L"Unpack");
	m_tabMain.InsertItem(1, L"Pack");

	// display unpack page as default at the opening
	m_tabUnpackDlg->Create(IDD_DIALOG_TAB_UNPACK, &m_tabMain);
	m_tabUnpackDlg->m_pMainDlg = this;

	m_tabPackDlg->Create(IDD_DIALOG_TAB_PACK, &m_tabMain);
	CRect rc;
	m_tabMain.GetClientRect(&rc);
	// tab header offset
	rc.top += 20;

	m_tabUnpackDlg->MoveWindow(&rc);
	m_tabPackDlg->MoveWindow(&rc);

	m_tabUnpackDlg->SetWindowPos(&m_tabMain, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	m_tabPackDlg->SetWindowPos(&m_tabMain, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	m_tabUnpackDlg->ShowWindow(SW_SHOW);
	m_tabPackDlg->ShowWindow(SW_HIDE);

	// return TRUE  unless you set the focus to a control
	return TRUE;
}

void CKOFileUnpackerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_tabUnpackDlg)
	{
		m_tabUnpackDlg->DestroyWindow();
		delete m_tabUnpackDlg;
		m_tabUnpackDlg = nullptr;
	}

	if (m_tabPackDlg)
	{
		m_tabPackDlg->DestroyWindow();
		delete m_tabPackDlg;
		m_tabPackDlg = nullptr;
	}
}

void CKOFileUnpackerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CKOFileUnpackerDlg::OnPaint()
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
HCURSOR CKOFileUnpackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKOFileUnpackerDlg::OnHelpAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CKOFileUnpackerDlg::OnTcnSelchangeTabPages(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_tabUnpackDlg->ShowWindow(SW_HIDE);
	m_tabPackDlg->ShowWindow(SW_HIDE);

	int iSelected = m_tabMain.GetCurSel();
	TRACE("iSelected %d\n", iSelected);
	switch (iSelected)
	{
		case 0:
			m_tabUnpackDlg->ShowWindow(SW_SHOW);
			break;
		case 1:
			m_tabPackDlg->ShowWindow(SW_SHOW);
			break;
		default:
			m_tabUnpackDlg->ShowWindow(SW_SHOW);
			break;
	}

	*pResult = 0;
}
