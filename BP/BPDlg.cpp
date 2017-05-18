
// BPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BP.h"
#include "BPDlg.h"
#include "afxdialogex.h"
#include "BPANN.h"

#include <iostream>
#include <fstream>
using namespace std;
#include <conio.h>
#include <direct.h>
#include <ctype.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBPDlg 对话框
//clsANN ANN;


CBPDlg::CBPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BP_DIALOG, pParent)
	, input_x1(0)
	, input_x2(0)
	, output_y(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X1, input_x1);
	DDX_Text(pDX, IDC_EDIT_X2, input_x2);
	DDX_Text(pDX, IDC_EDIT_Y, output_y);
}

BEGIN_MESSAGE_MAP(CBPDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CBPDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_TRAIN, &CBPDlg::OnBnClickedButtonTrain)
	ON_BN_CLICKED(IDC_BUTTON_COMPUTE, &CBPDlg::OnBnClickedButtonCompute)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_NET, &CBPDlg::OnBnClickedButtonSaveNet)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_NET, &CBPDlg::OnBnClickedButtonLoadNet)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CBPDlg::OnBnClickedButtonCalculate)
END_MESSAGE_MAP()


// CBPDlg 消息处理程序

BOOL CBPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	AllocConsole();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBPDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filename;
	char *path;
	CString str;
	FILE *fp1, *fp2;
	CFileDialog	FileDlg(true);

	FileDlg.m_ofn.lpstrTitle = _T("打开输入样本训练集");
	FileDlg.m_ofn.lpstrFilter =
		_T("文本文档(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0\0");
	if (FileDlg.DoModal() == IDOK)
	{ 
		filename = FileDlg.GetPathName();
		USES_CONVERSION;
		path = T2A(filename);
		
		CWnd::SetDlgItemTextW(IDC_STATIC_INPUT_PATH, filename);
		
		if ((fp1 = fopen(path, "r")) == NULL)
		{
				printf("can not open the in file\n");
				exit(0);
		}
			
		for (int i = 0; i < SAMPLE_NUM; i++)
		{
			for (int j = 0; j < INPUT_DIM; j++)
			{
				fscanf(fp1, "%lf", &ANN.TrainDataIn[j].TrainData[i]);
			}
		}
		fclose(fp1);
	}

	FileDlg.m_ofn.lpstrTitle = _T("打开输出样本训练集");
	FileDlg.m_ofn.lpstrFilter =
		_T("文本文档(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0\0");
	if (FileDlg.DoModal() == IDOK)
	{
		filename = FileDlg.GetPathName();
		USES_CONVERSION;
		path = T2A(filename);

		CWnd::SetDlgItemTextW(IDC_STATIC_OUTPUT_PATH, filename);

		if ((fp2 = fopen(path, "r")) == NULL)
		{
			printf("can not open the in file\n");
			exit(0);
		}

		for (int i = 0; i < SAMPLE_NUM; i++)
		{
			for (int j = 0; j < OUTPUT_DIM; j++)
			{
				fscanf(fp2, "%lf", &ANN.TrainDataOut[j].TrainData[i]);
			}
		}
		fclose(fp2);
	}
	ANN.is_trained = false;
}


void CBPDlg::OnBnClickedButtonTrain()
{
	// TODO: 在此添加控件通知处理程序代码
	ANN.do_train(0.0003, 0.01, 40000); //学习率，误差限度，最大迭代次数
}

void CBPDlg::OnBnClickedButtonCompute()
{
	// TODO: 在此添加控件通知处理程序代码
	ANN.calc_result();
}


void CBPDlg::OnBnClickedButtonSaveNet()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filename;
	char *path;
	CString str;
	FILE *fp1, *fp2;
	CFileDialog	FileDlg(true);

	FileDlg.m_ofn.lpstrTitle = _T("保存当前权重");
	FileDlg.m_ofn.lpstrFilter =
		_T("文本文档(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0\0");
	if (FileDlg.DoModal() == IDOK)
	{
		filename = FileDlg.GetPathName();
		USES_CONVERSION;
		path = T2A(filename);

		CWnd::SetDlgItemTextW(IDC_STATIC_NET_PATH, filename);

		if ((fp1 = fopen(path, "w")) == NULL)
		{
			printf("can not save the file\n");
			exit(0);
		}

		for (int i = 0; i < HIDE_DIM; i++)
		{
			for (int j = 0; j < INPUT_DIM; j++)
			{
				fprintf(fp1, "%lf ", ANN.NodeHide[i].weight[j]);
			}
		}

		for (int i = 0; i < OUTPUT_DIM; i++)
		{
			for (int j = 0; j < HIDE_DIM; j++)
			{
				fprintf(fp1, "%lf ", ANN.NodeOutput[i].weight[j]);
			}
		}

		//归一时用的参数也存一下
		for (int i = 0; i < INPUT_DIM; i++)
		{
			fprintf(fp1, "%lf ", ANN.TrainDataIn[i].TrainData_Max);
			fprintf(fp1, "%lf ", ANN.TrainDataIn[i].TrainData_Min);
		}

		for (int i = 0; i < OUTPUT_DIM; i++)
		{
			fprintf(fp1, "%lf ", ANN.TrainDataOut[i].TrainData_Max);
			fprintf(fp1, "%lf ", ANN.TrainDataOut[i].TrainData_Min);
		}

		fclose(fp1);
	}
}


void CBPDlg::OnBnClickedButtonLoadNet()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filename;
	char *path;
	CString str;
	FILE *fp1, *fp2;
	CFileDialog	FileDlg(true);

	FileDlg.m_ofn.lpstrTitle = _T("打开网络");
	FileDlg.m_ofn.lpstrFilter =
		_T("文本文档(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0\0");
	if (FileDlg.DoModal() == IDOK)
	{
		filename = FileDlg.GetPathName();
		USES_CONVERSION;
		path = T2A(filename);

		CWnd::SetDlgItemTextW(IDC_STATIC_NET_PATH, filename);

		if ((fp1 = fopen(path, "r")) == NULL)
		{
			printf("can not open the in file\n");
			exit(0);
		}

		for (int i = 0; i < HIDE_DIM; i++)
		{
			for (int j = 0; j < INPUT_DIM; j++)
			{
				fscanf(fp1, "%lf ", &ANN.NodeHide[i].weight[j]);
			}
		}

		for (int i = 0; i < OUTPUT_DIM; i++)
		{
			for (int j = 0; j < HIDE_DIM; j++)
			{
				fscanf(fp1, "%lf ", &ANN.NodeOutput[i].weight[j]);
			}
		}

		for (int i = 0; i < INPUT_DIM; i++)
		{
			fscanf(fp1, "%lf ", &ANN.TrainDataIn[i].TrainData_Max);
			fscanf(fp1, "%lf ", &ANN.TrainDataIn[i].TrainData_Min);
		}

		for (int i = 0; i < OUTPUT_DIM; i++)
		{
			fscanf(fp1, "%lf ", &ANN.TrainDataOut[i].TrainData_Max);
			fscanf(fp1, "%lf ", &ANN.TrainDataOut[i].TrainData_Min);
		}

		fclose(fp1);
	}
}


void CBPDlg::OnBnClickedButtonCalculate()
{
	// TODO: 在此添加控件通知处理程序代码
	double x[2];
	UpdateData(TRUE);
	x[0] = input_x1;
	x[1] = input_x2;
	output_y = ANN.calc_test(x);
	UpdateData(FALSE);
}
