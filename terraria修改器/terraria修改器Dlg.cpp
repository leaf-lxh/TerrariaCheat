
// terraria�޸���Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "terraria�޸���.h"
#include "terraria�޸���Dlg.h"
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

// Cterraria�޸���Dlg �Ի���

void initial(HWND hWnd)
{
	DWORD PID = GetPID(TEXT("Terraria.exe"));
	if (PID == NULL)
	{
		SetWindowTextW(GetDlgItem(hWnd, IDC_MESSAGE_TEXT), L"��ʼ��ʧ�ܣ�δ��ȡ����Ϸ���̱�ʶ��");
		
		return;
	}

	DecAddress = FindAddress(PID, DecByte, sizeof(DecByte), 0, 0);
	SubAddress = FindAddress(PID, SubByte, sizeof(SubByte), 0, 0);
	AddFEAddress = FindAddress(PID, AddFEByte, sizeof(AddFEByte), 0, 0)+17;
	if (DecAddress == NULL)
	{
		SetWindowTextW(GetDlgItem(hWnd, IDC_MESSAGE_TEXT), L"��ʼ��ʧ�ܣ�δ��ȡ���˺�����");
		return;
	}
	if (SubAddress == NULL)
	{
		SetWindowTextW(GetDlgItem(hWnd, IDC_MESSAGE_TEXT), L"��ʼ��ʧ�ܣ�δ��ȡ���ж��˺�����");
		return;
	}
	if (AddFEAddress == NULL)
	{
		SetWindowTextW(GetDlgItem(hWnd, IDC_MESSAGE_TEXT), L"��ʼ��ʧ�ܣ�δ��ȡ����ˮ�˺�����");
		return;
	}

	
	SetWindowTextW(GetDlgItem(hWnd, IDC_MESSAGE_TEXT), L"��ʼ����ϣ����Կ�ʼʹ��");

	hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, PID);

	char buf[256] = "";
	sprintf_s(buf,256, "%x |%x |%x |", DecAddress, SubAddress, AddFEAddress);
	OutputDebugStringA(buf);
}



Cterraria�޸���Dlg::Cterraria�޸���Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cterraria�޸���Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cterraria�޸���Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cterraria�޸���Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Cterraria�޸���Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cterraria�޸���Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Cterraria�޸���Dlg ��Ϣ�������

BOOL Cterraria�޸���Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	initial(CWnd::m_hWnd);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cterraria�޸���Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cterraria�޸���Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cterraria�޸���Dlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	WriteProcessMemory(hProcessHandle, LPVOID(DecAddress), &nop, 6, 0);
	WriteProcessMemory(hProcessHandle, LPVOID(SubAddress), &nop, 6, 0);
	WriteProcessMemory(hProcessHandle, LPVOID(AddFEAddress), &nop, 7, 0);
	GetDlgItem(IDC_MESSAGE_TEXT)->SetWindowTextW(L"�޵�״̬:����");
}


void Cterraria�޸���Dlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	WriteProcessMemory(hProcessHandle, LPVOID(DecAddress), DecByte, 6, 0);
	WriteProcessMemory(hProcessHandle, LPVOID(SubAddress), SubByte, 6, 0);
	WriteProcessMemory(hProcessHandle, LPVOID(AddFEAddress), AddFEOByte, 7, 0);
	GetDlgItem(IDC_MESSAGE_TEXT)->SetWindowTextW(L"�޵�״̬:�ر�");
}
