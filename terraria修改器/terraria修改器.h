
// terraria�޸���.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cterraria�޸���App: 
// �йش����ʵ�֣������ terraria�޸���.cpp
//

class Cterraria�޸���App : public CWinApp
{
public:
	Cterraria�޸���App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cterraria�޸���App theApp;