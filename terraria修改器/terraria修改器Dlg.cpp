
// terraria修改器Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "terraria修改器.h"
#include "terraria修改器Dlg.h"
#include "afxdialogex.h"
#include "MemoryOperate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD DecAddress = 0;
DWORD SubAddress = 0;
DWORD AddFEAddress = 0;
byte DecByte[] = { 41, 130, 64, 3, 0, 0, 131, 125, 8, 255, 15, 133, 149 };
byte SubByte[] = { 255, 139, 64, 3, 0, 0, 141, 125, 184, 15, 87, 192, 102 };
byte AddFEByte[] = { 139, 69, 220, 51, 210, 137, 144, 88, 3, 0, 0, 137, 144, 180, 2, 0, 0, 131, 128, 64, 3, 0, 0, 254 };
byte AddFEOByte[] = { 0x83, 0x80, 0x40, 0x03, 0x00, 0x00, 0xFE };

byte nop[7] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 ,0x90};

HANDLE hProcessHandle = 0;

// Cterraria修改器Dlg 对话框

void initial(HWND hWnd)
{
	DWORD PID = GetPID(TEXT("Terraria.exe"));
	if (PID == NULL)
	{
		SetWindowTextW(GetDlgItem(hWnd, IDC_MESSAGE_TEXT), L"初始化失败，未获取到游戏进程标识符");
		
		return;
	}

	DecAddress = FindAddress(PID, DecByte, sizeof(DecByte), 0, 0);
	SubAddress = FindAddress(PID, SubByte, sizeof(SubByte), 0, 0);
	AddFEAddress = FindAddress(PID, AddFEByte, sizeof(AddFEByte), 0, 0)+17;
	if (DecAddress == NULL)
	{
		SetWindowTextW(GetDlgItem(hWnd, IDC_MESSAGE_TEXT), L"初始化失败，未获取到伤害函数");
		return;
	}
	if (SubAddress == NULL)
	{
		SetWindowTextW(GetDlgItem(hWnd, IDC_MESSAGE_TEXT), L"初始化失败，未获取到中毒伤害函数");
		return;
	}
	if (AddFEAddress == NULL)
	{
		SetWindowTextW(GetDlgItem(hWnd, IDC_MESSAGE_TEXT), L"初始化失败，未获取到溺水伤害函数");
		return;
	}

	
	SetWindowTextW(GetDlgItem(hWnd, IDC_MESSAGE_TEXT), L"初始化完毕，可以开始使用");

	hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, PID);

	char buf[256] = "";
	sprintf_s(buf,256, "%x |%x |%x |", DecAddress, SubAddress, AddFEAddress);
	OutputDebugStringA(buf);
}



Cterraria修改器Dlg::Cterraria修改器Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cterraria修改器Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cterraria修改器Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cterraria修改器Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Cterraria修改器Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cterraria修改器Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Cterraria修改器Dlg 消息处理程序

BOOL Cterraria修改器Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	initial(CWnd::m_hWnd);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cterraria修改器Dlg::OnPaint()
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
HCURSOR Cterraria修改器Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cterraria修改器Dlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	WriteProcessMemory(hProcessHandle, LPVOID(DecAddress), &nop, 6, 0);
	WriteProcessMemory(hProcessHandle, LPVOID(SubAddress), &nop, 6, 0);
	WriteProcessMemory(hProcessHandle, LPVOID(AddFEAddress), &nop, 7, 0);
	GetDlgItem(IDC_MESSAGE_TEXT)->SetWindowTextW(L"无敌状态:开启");
}


void Cterraria修改器Dlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	WriteProcessMemory(hProcessHandle, LPVOID(DecAddress), DecByte, 6, 0);
	WriteProcessMemory(hProcessHandle, LPVOID(SubAddress), SubByte, 6, 0);
	WriteProcessMemory(hProcessHandle, LPVOID(AddFEAddress), AddFEOByte, 7, 0);
	GetDlgItem(IDC_MESSAGE_TEXT)->SetWindowTextW(L"无敌状态:关闭");
}
