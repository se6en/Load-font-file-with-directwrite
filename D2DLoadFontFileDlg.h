
// D2DLoadFontFileDlg.h : header file
//

#pragma once

#include "D2DDrawTextCtrl.h"


// CD2DLoadFontFileDlg dialog
class CD2DLoadFontFileDlg : public CDialogEx
{
// Construction
public:
	CD2DLoadFontFileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_D2DLOADFONTFILE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
   CD2DDrawTextCtrl           m_ctrlD2DDrawText;
};
