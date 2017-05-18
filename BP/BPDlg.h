
// BPDlg.h : 头文件
//

#pragma once

#include "BPANN.h"

// CBPDlg 对话框
class CBPDlg : public CDialogEx
{
// 构造
public:
	CBPDlg(CWnd* pParent = NULL);	// 标准构造函数
	CString train_filepath;
	clsANN ANN;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonTrain();
	afx_msg void OnBnClickedButtonCompute();
	afx_msg void OnBnClickedButtonSaveNet();
	afx_msg void OnBnClickedButtonLoadNet();
	afx_msg void OnBnClickedButtonCalculate();
	double input_x1;
	double input_x2;
	double output_y;
};
