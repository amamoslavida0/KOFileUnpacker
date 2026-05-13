// CTabPack.cpp : implementation file
//

#include "pch.h"
#include "KOFileUnpacker.h"
#include "afxdialogex.h"
#include "CTabPack.h"


// CTabPack dialog

IMPLEMENT_DYNAMIC(CTabPack, CDialogEx)

CTabPack::CTabPack(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TAB_PACK, pParent)
{
	m_strPathHdr = L"";
	m_eFileType = file_type::unknown;
	m_vecFileNames = {};
}

CTabPack::~CTabPack()
{
}

void CTabPack::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_PACK, m_progressPack);
	DDX_Control(pDX, IDC_EDIT_PACK_EXTRACT_PATH, m_editPackPath);
	DDX_Control(pDX, IDC_EDIT_PACK_HDR_PATH, m_editHdrPath);
}

BOOL CTabPack::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_progressPack.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(CTabPack, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PACK, &CTabPack::OnBnClickedButtonPack)
	ON_BN_CLICKED(IDC_BUTTON_PACK_SELECT_EXTRACT_PATH, &CTabPack::OnBnClickedButtonPackSelectExtractPath)
	ON_BN_CLICKED(IDC_BUTTON_PACK_SELECT_HDR, &CTabPack::OnBnClickedButtonPackSelectHdr)
END_MESSAGE_MAP()

// helper functions

CString CTabPack::GetFileNameFromPath(const CString& strPath)
{
	CString strFN = PathFindFileName(strPath);
	LPTSTR p = strFN.GetBuffer();
	PathRemoveExtension(p);
	strFN.ReleaseBuffer();

	return strFN;
}

CString CTabPack::GetFileNameWithExtensionFromPath(const CString& strPath)
{
	return PathFindFileName(strPath);
}

file_type CTabPack::GetFileType(const CString& strPath)
{
	if (strPath.IsEmpty())
	{
		return file_type::unknown;
	}

	CString strFileName = GetFileNameFromPath(strPath);
	TRACE(L"strFileName = %s \n", strFileName.GetString());
	if (strFileName == FNui)
	{
		return file_type::ui;
	}
	else if (strFileName == FNsnd)
	{
		return file_type::snd;
	}
	else if (strFileName == FNobject)
	{
		return file_type::object;
	}
	else if (strFileName == FNitem)
	{
		return file_type::item;
	}
	else if (strFileName == FNfx)
	{
		return file_type::fx;
	}
	
	return file_type::unknown;	
}

uint8_t CTabPack::CreateNewSrcFile() 
{
	// find path
	CString strPath = L"";
	m_editPackPath.GetWindowText(strPath);
	CString strFileName = GetFileNameFromPath(m_strPathHdr);
	strPath += L"\\" + strFileName + L".src";

	HANDLE hFile = CreateFile(
		strPath,
		GENERIC_WRITE,
		0,
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	DWORD written = 0;
	BYTE buffer[8192] = {};

	for (const auto& filePath : m_vecFileNames)
	{
		HANDLE hSplitFile = CreateFile(
			filePath,
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		if (hSplitFile == INVALID_HANDLE_VALUE) 
		{
			continue;
		}

		DWORD readBytes = 0;

		while (ReadFile(hSplitFile, buffer, sizeof(buffer), &readBytes, nullptr) 
			&& readBytes > 0)
		{
			WriteFile(hFile, buffer, readBytes, &written, nullptr);
		}

		CloseHandle(hSplitFile);
	}


	CloseHandle(hFile);
	return 1;
}

uint8_t CTabPack::CreateNewHdrFile() 
{
	// find path
	CString strPath = L"";
	m_editPackPath.GetWindowText(strPath);
	CString strFileName = GetFileNameFromPath(m_strPathHdr);
	strPath += L"\\" + strFileName + L".hdr";

	TRACE(L"strPath : %s \n", strPath);

	HANDLE hFile = CreateFile(
		strPath,
		GENERIC_WRITE,
		0,
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);

	if (hFile == INVALID_HANDLE_VALUE) 
	{
		return 0;
	}

	DWORD dwWritten = 0;

	// first 4 bytes same in all files
	uint16_t wFirst = 3392;
	WriteFile(hFile, &wFirst, sizeof(wFirst), &dwWritten, nullptr);

	uint16_t wSecond = 3;
	WriteFile(hFile, &wSecond, sizeof(wSecond), &dwWritten, nullptr);

	uint16_t wThird = 0;

	switch (m_eFileType)
	{
		case file_type::ui: wThird = 21598; break;
		case file_type::snd: wThird = 20116; break;
		case file_type::object: wThird = 24788; break;
		case file_type::item: wThird = 24891; break;
		case file_type::fx: wThird = 26777; break;
		default: break;
	}

	WriteFile(hFile, &wThird, sizeof(wThird), &dwWritten, nullptr);

	uint16_t wForth = 8015;
	WriteFile(hFile, &wForth, sizeof(wForth), &dwWritten, nullptr);

	// add file infos...
	uint32_t dwCurrentOffset = 0;
	for (const auto& filePath : m_vecFileNames)
	{
		CString strFileName = GetFileNameWithExtensionFromPath(filePath);

		// file size
		HANDLE hSrc = CreateFile(filePath, 
								GENERIC_READ, 
								FILE_SHARE_READ, 
								nullptr, 
								OPEN_EXISTING, 
								0, 
								nullptr);

		DWORD dwFileSize = GetFileSize(hSrc, nullptr);
		CloseHandle(hSrc);

		// name length
		uint16_t wNameLen = (uint16_t) strFileName.GetLength();
		WriteFile(hFile, &wNameLen, sizeof(wNameLen), &dwWritten, nullptr);

		// name bytes (ANSI)
		CStringA fileNameA(strFileName);
		WriteFile(hFile, fileNameA.GetString(), wNameLen, &dwWritten, nullptr);

		// offset
		WriteFile(hFile, &dwCurrentOffset, sizeof(dwCurrentOffset), &dwWritten, nullptr);

		// size
		WriteFile(hFile, &dwFileSize, sizeof(dwFileSize), &dwWritten, nullptr);

		dwCurrentOffset += dwFileSize;
	}


	CloseHandle(hFile);

	return 1;
}

void CTabPack::GenerateFileNameVector() 
{
	// find all files inside folder
	m_vecFileNames.clear();
	CString strPath;
	m_editPackPath.GetWindowText(strPath);
	CString strSearchPath = strPath + L"\\*.*";

	CFileFind finder;
	BOOL bWorking = finder.FindFile(strSearchPath);
	TRACE(L"bWorking %d \n", bWorking);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots()
			|| finder.IsDirectory())
		{
			continue;
		}

		// get extension of file
		PCWSTR ext = PathFindExtension(finder.GetFileName());

		// skip old .hdr or .src files
		if (_wcsicmp(ext, L".hdr") == 0
			|| _wcsicmp(ext, L".src") == 0)
		{
			continue;
		}

		m_vecFileNames.push_back(finder.GetFilePath());
	}

	finder.Close();
}

uint8_t CTabPack::CreateNewFiles() 
{
	GenerateFileNameVector();
	uint8_t byResHdr = CreateNewHdrFile();

	if (byResHdr != 1)
	{
		return byResHdr;
	}

	return CreateNewSrcFile();
}

// CTabPack message handlers

void CTabPack::OnBnClickedButtonPack()
{
	m_editHdrPath.GetWindowText(m_strPathHdr);
	
	m_eFileType = GetFileType(m_strPathHdr);
	
	TRACE(L"eFileType %d\n", m_eFileType);

	uint8_t byResult = CreateNewFiles();

	switch (byResult)
	{
	case 1: 
		AfxMessageBox(L".hdr and .src files have been created by success.", MB_OK);
		break;
	default:
		AfxMessageBox(L"Something went wrong !", MB_OK);
		break;
	}

}

void CTabPack::OnBnClickedButtonPackSelectExtractPath()
{
	CFolderPickerDialog dlg;

	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		m_editPackPath.SetWindowText(path);
	}
}

void CTabPack::OnBnClickedButtonPackSelectHdr()
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
