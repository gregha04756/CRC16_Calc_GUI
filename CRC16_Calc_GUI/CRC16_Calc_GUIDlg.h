
// CRC16_Calc_GUIDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CCRC16_Calc_GUIDlg dialog
class CCRC16_Calc_GUIDlg : public CDialogEx
{
// Construction
public:
	CCRC16_Calc_GUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CRC16_CALC_GUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit_Sequence;
	CButton m_Btn_Calculate;
	CButton m_Btn_Hex;
	CButton m_Btn_Decimal;
	afx_msg void OnChangeEditSequence();
	afx_msg void OnUpdateEditSequence();
	friend unsigned short calc_CRC16(unsigned char* puchMsg, unsigned short usDataLen);
private:
	int m_i_calls_ctr;
	BOOL Is_Valid_Hex(CString &str_in);
	BOOL Is_Valid_Char_Hex(CString &str_in);
	BOOL Is_Valid_Char_Decimal(CString &str_in);
	BOOL Is_Valid_Sequence(CEdit &e_in);
	CString *cvt_to_decimal(CString &str_in);
	CString *cvt_to_hexadecimal(CString &str_in);
	wchar_t const m_seps[2] = _T(" ");
	uint16_t Base16_to_Base10(wchar_t *u8_arg,int size);
	uint16_t Base10_to_Base16(wchar_t *u8_arg,int size);

public:
	afx_msg void OnClickedBtnHex();
//	afx_msg void OnBtnDecimal();
	afx_msg void OnBtnDecimal();
	afx_msg void OnClickedBtnCalc();
	CStatic m_CRC_Result;
};
