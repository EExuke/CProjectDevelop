//ABOUT1.C

/*------------------------------------------------------------------------

  ABOUT1.C -- About Box Demo Program No. 1
  
	(c) Charles Petzold, 1998
	
-------------------------------------------------------------------------*/
#include "operator_m_proc.h"
#include "book_info_proc.h"
#include "provider_info_proc.h"
#include "book_kinds_proc.h"
#include "store_info_proc.h"
#include "desk_info_proc.h"
#include "book_input_proc.h"
#include "book_input_back_proc.h"
#include "book_adjust_proc.h"
#include "book_sale_proc.h"
#include "book_sale_back_proc.h"
#include "book_input_back_query_proc.h"
#include "book_sale_back_query_proc.h"
#include "book_input_query_proc.h"
#include "book_sale_query_proc.h"
#include "book_stock_query_proc.h"
#define LEVEL_COLUMN 2
#define NAME_COLUMN 0


BOOL CALLBACK LoginDlgProc (HWND, UINT, WPARAM, LPARAM) ;
BOOL CALLBACK ClientDlgProc (HWND, UINT, WPARAM, LPARAM) ;
BOOL CALLBACK ConfigDlgProc (HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam);
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					
					PSTR szCmdLine, int iCmdShow)
					
{	
		MSG msg;  
		char host[VALUE_LENTH_MAX];
		char userName[VALUE_LENTH_MAX];
		char password[VALUE_LENTH_MAX];
		char dbName[VALUE_LENTH_MAX];
		FILE *init;
		init = fopen("Init.txt","r");
		if(init==NULL)
		{
			init = fopen("Init.txt","w");
			fprintf(init,"host:\t%s\nusername:\t%s\npassword:\t%s\ndatabase:\t%s","set","set","set","set");
			fclose(init);
			MessageBoxEx(NULL,TEXT("请检查配置文件"),"查找配置文件失败",MB_ICONERROR|MB_TOPMOST,0);
			PostQuitMessage (0) ;
		}
		fscanf(init,"host:\t%s\nusername:\t%s\npassword:\t%s\ndatabase:\t%s",host,userName,password,dbName);
		fclose(init);
		mysql_init(&mysql);
		if(!mysql_real_connect(&mysql,host,userName,password,dbName,0,NULL,0))
		{
			MessageBox(NULL,TEXT("数据库连接失败,请重新设定配置文件参数"),"错误",MB_ICONERROR);
			 return 0;
		}
		/*保留进程句柄*/
		  g_hInstance = hInstance;

	      DialogBox (hInstance, MAKEINTRESOURCE(ID_DIG_LOGIN), NULL, LoginDlgProc) ;
		  while (GetMessage (&msg, NULL, 0, 0))
			  
		  {
			  TranslateMessage (&msg) ;
			  DispatchMessage (&msg) ;	  
          }
		  
		  return msg.wParam ;
		  
}

BOOL CALLBACK LoginDlgProc (HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char name[VALUE_LENTH_MAX]; 
	char pwd[VALUE_LENTH_MAX];
	char sql[SQL_LENTH_MAX];
	switch (message)      
	{
        
	case   WM_INITDIALOG :
        
		return TRUE ;
	case   WM_COMMAND :
		switch (LOWORD (wParam))
		{
		case IDC_BTN_LOGIN :	
			/*获取控件字符串*/
			GetWindowText(GetDlgItem(hDlg,IDC_EDIT_NAME),name,VALUE_LENTH_MAX);
			GetWindowText(GetDlgItem(hDlg,IDC_EDIT_PASSWORD),pwd,VALUE_LENTH_MAX);
			sprintf(sql,"select * from tb_operators where name='%s' and password='%s'",name,pwd);
			/*在数据库查找用户*/
			if(mysql_query(&mysql,sql))
			{
			  MessageBox(NULL,TEXT("操作失败"),"错误",MB_ICONERROR);
			  PostQuitMessage (0);
			}
			result = mysql_store_result(&mysql);
			if(mysql_num_rows(result)!=0)
			{
				/*登陆成功*/
				EndDialog (hDlg, 0) ;
				row = mysql_fetch_row(result);
				sprintf(g_operator,"%s",row[NAME_COLUMN]);
				g_level = atoi(row[LEVEL_COLUMN]);
				ShowWindow(CreateDialog(g_hInstance, MAKEINTRESOURCE(ID_DLG_CLIENT), NULL, ClientDlgProc),SW_SHOW);
			}
			else
			{
			    MessageBox(NULL,TEXT("用户名或密码错误!"),"登陆失败",MB_ICONERROR);
				return 0;
			}
			mysql_free_result(result);
			mysql_query(&mysql,"set names gbk");
			break;
		case IDC_BTN_CANCEL :
			EndDialog (hDlg, 0) ;
			PostQuitMessage (0) ;
			break;
		}
	   return TRUE ; 
       break ;
	case  WM_CLOSE: 
		PostQuitMessage (0) ;
	case  WM_DESTROY :
		//PostQuitMessage (0) ;
		break;
	   return 0 ;
    }
    return 0 ;	
}
BOOL CALLBACK ClientDlgProc (HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)

{
	char marks[5][20] = {"图书信息","图书入库","销售查询","系统配置","系统退出"};
	HWND hToolbar;		//工具栏句柄
	u_int i;            //for 语句迭代器
	int ImgID[5];		//存放ImageList中的小图标标识
	HIMAGELIST him;		//ImageList类型数据，用来存放Image或者Icon，在本程序中使用它向工具栏添加图片
	u_long bkColor;		//存放系统默认颜色
	TBBUTTON tbs[5];
	HDC hdc;
	HDC memDC;
	BITMAP mBitmap;
	HBITMAP bmp;
	RECT rect;
	int Width;
	int Height;
	PAINTSTRUCT ps ;
	switch (message)        
	{
	case   WM_INITDIALOG : 
	   	 SetWindowPos(hDlg,HWND_TOP,100,100,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);			//设定窗口的位置
		 /*向主界面设置工具栏*/
		 hToolbar = CreateWindowEx(WS_EX_APPWINDOW,TOOLBARCLASSNAME,"toolbar",WS_VISIBLE|WS_CHILD|CCS_TOP  ,0,0,120,120,hDlg,NULL,g_hInstance,NULL);
		 SendMessage(hToolbar,TB_BUTTONSTRUCTSIZE,sizeof(TBBUTTON),0);				//向空工具条传递按钮数据的大小的信息
		 him = ImageList_Create(24,24,ILC_COLOR24,5,0);
		 bkColor=GetSysColor(COLOR_3DFACE);										//获取系统背景色
		 ImageList_SetBkColor(him,bkColor);											//将图片的背景色设定为默认的背景色;
		 SendMessage(hToolbar,TB_SETIMAGELIST,0,(LPARAM)(HIMAGELIST)him);				//向工具条传递使用当前ImageList的信息
		 //以此图标装载入ImageList中。  
		  for(i=0;i<5;i++)
		  {
			ImgID[i]=ImageList_AddIcon(him,LoadIcon(g_hInstance,MAKEINTRESOURCE(IDI_ICON1+i)));
		  }
		 //将工具条按钮的基本属性依次装填
		 for(i = 0;i<5;i++)
		 {
			 tbs[i].iBitmap = ImgID[i];
			 tbs[i].iString = (int)marks[i];   //iString是一个PTR_INT类型数据，用来存放32位地址
			 tbs[i].fsStyle = TBSTYLE_BUTTON;
			 tbs[i].fsState = TBSTATE_ENABLED ;
		 }
		 //将工具指令与相应菜单栏的指令对应
		 tbs[0].idCommand = ID_BOOK_INFO;
		 tbs[1].idCommand = ID_BOOK_INPUT;
		 tbs[2].idCommand = ID_SALE_QUERY;
		 //工具条最后两个按钮的命令在"resource.h"中自行定义
		 tbs[3].idCommand = ID_TB_CONFIG;
		 tbs[4].idCommand = ID_TB_EXIT;	  
		SendMessage(hToolbar,TB_ADDBUTTONS,5,(LPARAM)(LPTBBUTTON)tbs);				//向工具条传递增添按钮的信息
		SendMessage(hToolbar,TB_SETBUTTONSIZE,0,MAKELONG(80,60));				//设置按钮大小
		SendMessage(hToolbar,TB_AUTOSIZE,0,0);										//根据工具栏和图片的情况，调整按钮大小
		return TRUE ;
	case  WM_COMMAND : 
		switch (LOWORD (wParam))
		{
			case ID_OPERATOR_M:
				if(g_level>1)
				{
					DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_OPERATOR_M),hDlg,Operator_M_Proc);
				}
				else
				{
					MessageBox(hDlg,"您无权进行此操作","权限等级限制",MB_ICONERROR);
				}
				break;
			case ID_BOOK_INFO:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_BOOK_INFO),hDlg,Book_Info_Proc);
				break;
			case ID_PROVIDER_INFO:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_PROVIDER_INFO),hDlg,Provider_Info_Proc);
				break;
			case ID_BOOK_KIND:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_BOOK_KINDS),hDlg,Book_Kinds_Proc);
				break;
			case ID_CK_INFO:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_STORE_INFO),hDlg,Store_Info_Proc);
				break;
			case ID_DESK_INFO:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_DESK_INFO),hDlg,Desk_Info_Proc);
				break;
			case ID_BOOK_INPUT:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_BOOK_INPUT),hDlg,Book_Input_Proc);
				break;
			case ID_INPUT_BACK:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_BOOK_INPUT),hDlg,Book_Input_Back_Proc);
				break;
			case ID_BOOK_ADJUST:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_BOOK_ADJUST),hDlg,Book_Adjust_Proc);
				break;
				break;
			case ID_SALE_M:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_BOOK_SALE),hDlg,Book_Sale_Proc);
				break;
			case ID_SALE_BACK:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_BOOK_SALE),hDlg,Book_Sale_Back_Proc);
				break;
			case ID_INPUT_QUERY:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_QUERY),hDlg,Book_Input_Query_Proc);
				break;
			case ID_INPUT_BACK_QUERY:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_QUERY),hDlg,Book_Input_Back_Query_Proc);
				break;
			case ID_SALE_QUERY:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_QUERY),hDlg,Book_Sale_Query_Proc);
				break;
			case ID_SALE_BACK_QUERY:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_QUERY),hDlg,Book_Sale_Back_Query_Proc);
				break;
			case ID_STOCK_QUERY:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_STOCKQUERY),hDlg,Book_Stock_Query_Proc);
				break;
			case ID_TB_CONFIG:
				DialogBox(g_hInstance,MAKEINTRESOURCE(ID_DLG_CONFIG),hDlg,ConfigDlgProc );
				break;
			case ID_TB_EXIT:
				PostQuitMessage (0) ;
				break;

			case ID_EXIT:
				PostQuitMessage (0) ;
				break;
		}
	   return 1 ; 
       break ;
	case WM_PAINT:
		hdc = BeginPaint(hDlg,&ps);
		/*绘制主界面的背景*/
		GetWindowRect(hDlg,&rect);
		Width = rect.right - rect.left;
		Height = rect.bottom -rect.top;
		memDC = CreateCompatibleDC(hdc);
		bmp = LoadBitmap(g_hInstance,MAKEINTRESOURCE(IDB_BITMAP_CLIENT));
		GetObject(bmp, sizeof (BITMAP), &mBitmap) ;
		SelectObject(memDC,bmp);
		StretchBlt (hdc,0,0,Width,Height,memDC,0,0,mBitmap.bmWidth, mBitmap.bmHeight,SRCCOPY) ;
		DeleteDC (memDC);
		EndPaint(hDlg,&ps);
		return 0;
	case WM_CLOSE: 
		PostQuitMessage (0) ;
	case WM_DESTROY :	
		break;
	   return 0 ;
    }
    return 0 ;	
}
BOOL CALLBACK ConfigDlgProc (HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char host[VALUE_LENTH_MAX];
	char userName[VALUE_LENTH_MAX];
	char password[VALUE_LENTH_MAX];
	char dbName[VALUE_LENTH_MAX];
	FILE *init;
	switch(message)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_BUTTON_CONFIG:
					GetDlgItemText(hDlg,IDC_EDIT_IP,host,VALUE_LENTH_MAX);
					GetDlgItemText(hDlg,IDC_EDIT_USER,userName,VALUE_LENTH_MAX);
					GetDlgItemText(hDlg,IDC_EDIT_PWD,password,VALUE_LENTH_MAX);
					GetDlgItemText(hDlg,IDC_EDIT_DBASE,dbName,VALUE_LENTH_MAX);
					if(strlen(host)==0||strlen(userName)==0||strlen(password)==0||strlen(dbName)==0)
					{
						MessageBoxEx(hDlg,"参数不可为空!","错误",MB_ICONHAND|MB_TOPMOST,0);
						return 0;
					}
					init = fopen("Init.txt","w");
					if(init==NULL)
					{
						MessageBoxEx(hDlg,"配置失败!","错误",MB_ICONHAND|MB_TOPMOST,0);
						return 0;
					}
					fprintf(init,"host:\t%s\nusername:\t%s\npassword:\t%s\ndatabase:\t%s",host,userName,password,dbName);
					MessageBoxEx(hDlg,"配置成功!","提示",MB_OK|MB_TOPMOST,0);
					EndDialog(hDlg,0);
					break;
				case IDC_BUTTON_CANCEL:
					EndDialog(hDlg,0);
			}
		return 1;
		case WM_CLOSE: 
			EndDialog(hDlg,0);
		case WM_DESTROY :	
			break;			
		}
		return 0 ;
}