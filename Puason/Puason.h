
// Puason.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CPuasonApp:
// Сведения о реализации этого класса: Puason.cpp
//

class CPuasonApp : public CWinApp
{
public:
	CPuasonApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CPuasonApp theApp;
