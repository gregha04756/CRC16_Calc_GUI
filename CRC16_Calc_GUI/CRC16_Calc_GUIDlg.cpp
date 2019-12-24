
// CRC16_Calc_GUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CRC16_Calc_GUI.h"
#include "CRC16_Calc_GUIDlg.h"
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


// CCRC16_Calc_GUIDlg dialog



CCRC16_Calc_GUIDlg::CCRC16_Calc_GUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CRC16_CALC_GUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCRC16_Calc_GUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SEQUENCE, m_Edit_Sequence);
	DDX_Control(pDX, IDC_BTN_CALC, m_Btn_Calculate);
	DDX_Control(pDX, IDC_BTN_HEX, m_Btn_Hex);
	DDX_Control(pDX, IDC_BTN_DECIMAL, m_Btn_Decimal);
	DDX_Control(pDX, IDC_TXT_RESULT, m_CRC_Result);
}

BEGIN_MESSAGE_MAP(CCRC16_Calc_GUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_SEQUENCE, &CCRC16_Calc_GUIDlg::OnChangeEditSequence)
	ON_EN_UPDATE(IDC_EDIT_SEQUENCE, &CCRC16_Calc_GUIDlg::OnUpdateEditSequence)
	ON_BN_CLICKED(IDC_BTN_HEX, &CCRC16_Calc_GUIDlg::OnClickedBtnHex)
//	ON_COMMAND(IDC_BTN_DECIMAL, &CCRC16_Calc_GUIDlg::OnBtnDecimal)
ON_COMMAND(IDC_BTN_DECIMAL, &CCRC16_Calc_GUIDlg::OnBtnDecimal)
ON_BN_CLICKED(IDC_BTN_CALC, &CCRC16_Calc_GUIDlg::OnClickedBtnCalc)
END_MESSAGE_MAP()


// CCRC16_Calc_GUIDlg message handlers

BOOL CCRC16_Calc_GUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	m_Btn_Hex.SetCheck((int)BST_CHECKED);
	m_i_calls_ctr = 0;
	m_Btn_Calculate.EnableWindow(false);
	m_Edit_Sequence.SetFocus();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCRC16_Calc_GUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCRC16_Calc_GUIDlg::OnPaint()
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
HCURSOR CCRC16_Calc_GUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCRC16_Calc_GUIDlg::OnClickedBtnCalc()
{
	// TODO: Add your control notification handler code here
	uint16_t u16_CRC;
	uint8_t *p_u8_seq_buf;
	std::list<uint8_t> list_seq;
	std::list<uint8_t>::iterator it_list_seq;
	int i_len;
	int i_x;
	wchar_t *token;
	uint8_t u8_octet;
	CString str_temp(_T(""));
	CString str_Sequence(_T(""));
	PVOID p_v;
	// Establish string and get the first token:  
	m_Edit_Sequence.GetWindowText(str_Sequence);
	token = wcstok(str_Sequence.GetBuffer(), m_seps);
	u8_octet = 0;
	i_len = 0;
	while (NULL != token)
	{
		++i_len;
		// While there are tokens in "string"
		u8_octet = Base16_to_Base10(token, wcslen(token));
		if (m_Btn_Decimal.GetCheck())
		{
			u8_octet = Base10_to_Base16(token, wcslen(token));
		}
		list_seq.push_back(u8_octet);
		str_temp.Format(_T("%s %d "), str_temp.GetBuffer(), u8_octet);
		// Get next token:   
		token = wcstok(NULL, m_seps); // C4996  
	}
	try
	{
		p_u8_seq_buf = (uint8_t*)new uint8_t[i_len];
	}
	catch (std::exception &ee)
	{
		ReportError(ee.what());
		return;
	}
	if (NULL != p_u8_seq_buf)
	{
		p_v = SecureZeroMemory((PVOID)p_u8_seq_buf, (SIZE_T)i_len);

	}
	i_x = 0;
	for (it_list_seq = list_seq.begin(); (i_x < i_len) && (it_list_seq != list_seq.end()); it_list_seq++)
	{
		p_u8_seq_buf[i_x++] = *it_list_seq;
	}
	u16_CRC = calc_CRC16((unsigned char*)p_u8_seq_buf, (unsigned short)i_len);
	str_temp.Format(_T("0x%04x"), u16_CRC);
	m_CRC_Result.SetWindowText(str_temp.GetBuffer());
	if (NULL != p_u8_seq_buf)
	{
		delete[] p_u8_seq_buf;
	}
}

void CCRC16_Calc_GUIDlg::OnChangeEditSequence()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	
	CString str_Sequence(_T(""));
	m_Edit_Sequence.GetWindowText(str_Sequence);
//	AfxMessageBox(str_Sequence.GetString(), 0U, 0U);
	if (m_Btn_Hex.GetCheck())
	{

	}
	if (m_Btn_Decimal.GetCheck())
	{

	}
	m_Btn_Calculate.EnableWindow((0 < str_Sequence.GetLength()));

}


void CCRC16_Calc_GUIDlg::OnUpdateEditSequence()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
	CString str_Sequence(_T(""));
	CString temp;
	DWORD dw_les;
	DWORD dw_cut;
	DWORD dw_sel = m_Edit_Sequence.CharFromPos(m_Edit_Sequence.GetCaretPos());
	temp.Format(_T("%d"), dw_sel);
//	AfxMessageBox(temp, 0U, 0U);
	m_Edit_Sequence.GetWindowText(str_Sequence);
	int i_len;
	if (0 < m_i_calls_ctr)
	{
		return;
	}
	m_i_calls_ctr++;
//	AfxMessageBox(str_Sequence.GetString(), 0U, 0U);
	if (!Is_Valid_Sequence(m_Edit_Sequence))
	{
		dw_sel = m_Edit_Sequence.CharFromPos(m_Edit_Sequence.GetCaretPos());
		dw_les = MAKELONG((WORD)dw_sel, (WORD)dw_sel);
		m_Edit_Sequence.SetSel(MAKELONG((WORD)dw_sel, (WORD)(dw_sel+1)));
		dw_cut = m_Edit_Sequence.GetSel();
		m_Edit_Sequence.Cut();
#if _DEBUG
		m_Edit_Sequence.GetWindowText(temp);
#endif
	}
	--m_i_calls_ctr;
}

BOOL CCRC16_Calc_GUIDlg::Is_Valid_Sequence(CEdit &e_in)
{
	BOOL b_r = true;
	DWORD dw_sel;
	CString str_Sequence(_T(""));
	int i_len = str_Sequence.GetLength();
	wchar_t *token;
	uint16_t u16_octet;
	CString str_temp(_T(""));

	e_in.GetWindowText(str_Sequence);
	dw_sel = e_in.CharFromPos(m_Edit_Sequence.GetCaretPos());
	str_temp = CString(str_Sequence.GetBuffer()[dw_sel]);
	if (m_Btn_Decimal.GetCheck())
	{
		if (!Is_Valid_Char_Decimal(str_temp))
		{
			return (b_r = false);
		}
	}
	if (m_Btn_Hex.GetCheck())
	{
		if (!Is_Valid_Char_Hex(str_temp))
		{
			return (b_r = false);
		}
	}
	// Establish string and get the first token:  
	token = wcstok(str_Sequence.GetBuffer(), m_seps);
	while (NULL != token)
	{
		u16_octet = 0;
		// While there are tokens in "string"
		u16_octet = m_Btn_Decimal.GetCheck() ? Base10_to_Base16(token, wcslen(token)) : u16_octet;
		u16_octet = m_Btn_Hex.GetCheck() ? Base16_to_Base10(token, wcslen(token)) : u16_octet;
		if (255 < u16_octet)
		{
			b_r = false;
			break;
		}
		token = wcstok(NULL, m_seps); // C4996  
	}
	return b_r;
}

BOOL CCRC16_Calc_GUIDlg::Is_Valid_Hex(CString &str_in)
{
	BOOL b_r = true;
	int i_len = str_in.GetLength();
	if (0 == i_len)
	{
		return (b_r = true);
	}
	if (1 == i_len && !Is_Valid_Char_Hex(CString(str_in[i_len - 1])))
	{
		return (b_r = false);
	}
	return b_r;
}

BOOL CCRC16_Calc_GUIDlg::Is_Valid_Char_Hex(CString &str_in)
{
	return (str_in == CString(_T(" ")) ||
		str_in >= CString(_T("A")) && str_in <= CString(_T("F")) ||
		str_in >= CString(_T("a")) && str_in <= CString(_T("f")) ||
		str_in >= CString(_T("0")) && str_in <= CString(_T("9")));
}

BOOL CCRC16_Calc_GUIDlg::Is_Valid_Char_Decimal(CString &str_in)
{
	return (str_in == CString(_T(" ")) || str_in >= CString(_T("0")) && str_in <= CString(_T("9")));
}

void CCRC16_Calc_GUIDlg::OnClickedBtnHex()
{
	// TODO: Add your control notification handler code here
	//	AfxMessageBox(_T("Hex Clicked"));
	CString str_Sequence;
	PVOID p_v;
#ifdef _DEBUG
	int i_r = m_Btn_Hex.GetCheck();
#endif
	if (!m_Btn_Hex.GetCheck())
	{
		return;
	}
	m_Edit_Sequence.GetWindowText(str_Sequence);
	p_v = (PVOID)cvt_to_hexadecimal(str_Sequence);
	m_Edit_Sequence.SetWindowText(str_Sequence);
}


void CCRC16_Calc_GUIDlg::OnBtnDecimal()
{
	// TODO: Add your command handler code here
	//	AfxMessageBox(_T("Decimal Clicked"));
	CString str_Sequence;
	PVOID p_v;
#ifdef _DEBUG
	int i_r = m_Btn_Decimal.GetCheck();
#endif
	if (!m_Btn_Decimal.GetCheck())
	{
		return;
	}
	m_Edit_Sequence.GetWindowText(str_Sequence);
	p_v = (PVOID)cvt_to_decimal(str_Sequence);
	m_Edit_Sequence.SetWindowText(str_Sequence);
}

CString * CCRC16_Calc_GUIDlg::cvt_to_decimal(CString &str_in)
{
	wchar_t *token;
	uint8_t u16_octet;
	CString str_temp(_T(""));
	// Establish string and get the first token:  
	token = wcstok(str_in.GetBuffer(), m_seps); // C4996  
								  // Note: strtok is deprecated; consider using strtok_s instead  
	while (NULL != token)
	{
		// While there are tokens in "string"
		u16_octet = Base16_to_Base10(token,wcslen(token));
		str_temp.Format(_T("%s %d "), str_temp.GetBuffer(), u16_octet);
		// Get next token:   
		token = wcstok(NULL, m_seps); // C4996  
	}
	str_in = str_temp;
	return (CString*)NULL;
}

CString * CCRC16_Calc_GUIDlg::cvt_to_hexadecimal(CString &str_in)
{
	wchar_t *token;
	uint8_t u16_octet;
	CString str_temp(_T(""));
	// Establish string and get the first token:  
	token = wcstok(str_in.GetBuffer(), m_seps); // C4996  
												// Note: strtok is deprecated; consider using strtok_s instead  
	while (NULL != token)
	{
		// While there are tokens in "string"
		u16_octet = Base10_to_Base16(token, wcslen(token));
		str_temp.Format(_T("%s %02x "), str_temp.GetBuffer(), u16_octet);
		// Get next token:   
		token = wcstok(NULL, m_seps); // C4996  
	}
	str_in = str_temp;
	return (CString*)NULL;
}

uint16_t CCRC16_Calc_GUIDlg::Base16_to_Base10(wchar_t *u8_arg,int size)
{
	int i_i;
	uint16_t u16_result;
	uint16_t u16_factor = 1;
	u16_result = 0;
	for (i_i = size - 1; i_i >= 0; --i_i)
	{
		switch (u8_arg[i_i])
		{
		case _T('0'):
			break;
		case _T('1'):
			u16_result += u16_factor;
			break;
		case _T('2'):
			u16_result += 2*u16_factor;
			break;
		case _T('3'):
			u16_result += 3 * u16_factor;
			break;
		case _T('4'):
			u16_result += 4 * u16_factor;
			break;
		case _T('5'):
			u16_result += 5 * u16_factor;
			break;
		case _T('6'):
			u16_result += 6 * u16_factor;
			break;
		case _T('7'):
			u16_result += 7 * u16_factor;
			break;
		case _T('8'):
			u16_result += 8 * u16_factor;
			break;
		case _T('9'):
			u16_result += 9 * u16_factor;
			break;
		case _T('A'):
			u16_result += 10 * u16_factor;
			break;
		case _T('B'):
			u16_result += 11 * u16_factor;
			break;
		case _T('C'):
			u16_result += 12 * u16_factor;
			break;
		case _T('D'):
			u16_result += 13 * u16_factor;
			break;
		case _T('E'):
			u16_result += 14 * u16_factor;
			break;
		case _T('F'):
			u16_result += 15 * u16_factor;
			break;
		case _T('a'):
			u16_result += 10 * u16_factor;
			break;
		case _T('b'):
			u16_result += 11 * u16_factor;
			break;
		case _T('c'):
			u16_result += 12 * u16_factor;
			break;
		case _T('d'):
			u16_result += 13 * u16_factor;
			break;
		case _T('e'):
			u16_result += 14 * u16_factor;
			break;
		case _T('f'):
			u16_result += 15 * u16_factor;
			break;
		default:
			AfxMessageBox(_T("Invalid character in sequence, please correct"));
			break;
		}
		u16_factor *= 16;
	}
	return u16_result;
}

uint16_t CCRC16_Calc_GUIDlg::Base10_to_Base16(wchar_t *u8_arg,int size)
{
	int i_i;
	uint16_t u16_result;
	uint16_t u16_factor = 1;
	u16_result = 0;
	for (i_i = size - 1; i_i >= 0; --i_i)
	{
		switch (u8_arg[i_i])
		{
		case _T('0'):
			break;
		case _T('1'):
			u16_result += u16_factor;
			break;
		case _T('2'):
			u16_result += 2 * u16_factor;
			break;
		case _T('3'):
			u16_result += 3 * u16_factor;
			break;
		case _T('4'):
			u16_result += 4 * u16_factor;
			break;
		case _T('5'):
			u16_result += 5 * u16_factor;
			break;
		case _T('6'):
			u16_result += 6 * u16_factor;
			break;
		case _T('7'):
			u16_result += 7 * u16_factor;
			break;
		case _T('8'):
			u16_result += 8 * u16_factor;
			break;
		case _T('9'):
			u16_result += 9 * u16_factor;
			break;
		default:
			AfxMessageBox(_T("Invalid character in sequence, please correct"));
			break;
		}
		u16_factor *= 10;
	}
	return u16_result;
}
