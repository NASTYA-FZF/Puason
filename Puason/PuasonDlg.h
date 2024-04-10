
// PuasonDlg.h: файл заголовка
//

#pragma once
#include "Work.h"
#include "my_puasson.h"

// Диалоговое окно CPuasonDlg
class CPuasonDlg : public CDialogEx
{
// Создание
public:
	CPuasonDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PUASON_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	model condensator;
//	double my_a;
//	double my_d;
//	double my_l;
	double my_q;
	double my_error;
	int my_a;
	int my_d;
	int my_l;
	int my_width;
	int my_height;
	my_puasson shema;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	my_puasson my_shema;
	CEdit my_cur_error;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	UINT_PTR my_timer;
	double str_error;
//	int my_num_F_line;
	int my_num_izoline;
	CRITICAL_SECTION cs;
	afx_msg void OnBnClickedButton2();
	bool stop;
	double try_er;
	double my_step;
	afx_msg void OnBnClickedButton3();
	int num_power;
	CButton but_ok;
};

DWORD WINAPI rasprostranenie(PVOID p);