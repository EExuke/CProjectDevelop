// exit.cpp : implementation file
//

//#include "stdafx.h"
#include "lib.h"
#include "exit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Cexit

Cexit::Cexit()
{
	PostQuitMessage (0) ;
}

Cexit::~Cexit()
{
}


BEGIN_MESSAGE_MAP(Cexit, CButton)
	//{{AFX_MSG_MAP(Cexit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cexit message handlers
