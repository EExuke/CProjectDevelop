#include "book_sale_query_proc.h"
#define FIELD_NUM 7								//窗体中对信息表可控制的字段数
#define CONDITON_NUM 4						//查询条件字段数
#define PRIMARY "backcode"						//主键
#define TB_NAME "tb_book_input_back"					//信息表名
BOOL CALLBACK Book_Sale_Query_Proc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char titles[FIELD_NUM][TITLE_LENTH_MAX] = {"操作员","条形码","柜台","顾客姓名","数量","日期","实付金额"};
	char queryTitles[CONDITON_NUM][TITLE_LENTH_MAX]= {"操作员","顾客姓名","条形码","柜台"};
	HWND hView = GetDlgItem(hDlg,IDC_LIST_VIEW);
	int selIndex;														 //控件当前被选中的项
	int i;																 //迭代器
	char temp[VALUE_LENTH_MAX];											//设置EDIT文字时所用的字符缓冲
	char end[VALUE_LENTH_MAX];
	char condition[SQL_LENTH_MAX] = "";
	int conditionFlag;
	int timeFlag;
	switch (message)						
	{
	case   WM_INITDIALOG : 
		InitCommonControls();
		SetWindowText(hDlg,"销售查询");
		SendMessage(hView,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE|LVS_EX_FLATSB);
		InitListViewColumns(hView,titles,FIELD_NUM );
		/*初始化下拉条*/
		for(i=0;i<CONDITON_NUM;i++)
		{
			SendMessage(GetDlgItem(hDlg,IDC_CONDITIONLIST),CB_ADDSTRING,0,(LPARAM)queryTitles[i]);
		}
		return TRUE ;
	case   WM_COMMAND :
		switch(LOWORD(wParam))
		{
		case IDC_QUERY:
			conditionFlag = SendMessage(GetDlgItem(hDlg,IDC_SELECT_CHECK),BM_GETCHECK,0,0);
			timeFlag = SendMessage(GetDlgItem(hDlg,IDC_TIME_CHECK),BM_GETCHECK,0,0);
			if(conditionFlag==BST_CHECKED)
			{
				selIndex = SendMessage(GetDlgItem(hDlg,IDC_CONDITIONLIST),CB_GETCURSEL,0,0);
				GetWindowText(GetDlgItem(hDlg,IDC_SQ_VALUE),temp,VALUE_LENTH_MAX);
				if(strlen(temp)==0)
				{
					MessageBoxEx(hDlg,"请输入查询条件","提示",MB_ICONERROR|MB_TOPMOST,0);
					return 0;
				}
				switch(selIndex)
				{
					case 0:
						sprintf(condition,"operator='%s'",temp);
						break;
					case 1:
						sprintf(condition,"customer='%s'",temp);
						break;
					case 2:
						sprintf(condition,"barcode='%s'",temp);
						break;
					case 3:
						sprintf(condition,"desk='%s'",temp);
						break;
					default:
						return 0;
				}	
				if(timeFlag == conditionFlag)
				{
					sprintf(condition,"%s and ",condition);
				}
			}
			if(timeFlag ==BST_CHECKED)
			{
				GetWindowText(GetDlgItem(hDlg,IDC_STARTTIME),temp,VALUE_LENTH_MAX);
				GetWindowText(GetDlgItem(hDlg,IDC_ENDTIME),end,VALUE_LENTH_MAX);
				sprintf(condition,"%sdate>='%s/00/00/00' and date<='%s/23/59/59'",condition,temp,end);
			}
			QueryRecordToView(hView,TB_NAME,FIELD_NUM,condition,1,1);
			break;
		case ID_QUIT:
			EndDialog(hDlg,0);
			break;
		case IDC_CLEAR:
			ListView_DeleteAllItems(hView);
		}
	   return TRUE ; 
       break ;
	case  WM_CLOSE: 
		EndDialog(hDlg,0);
		//PostQuitMessage (0) ;
	case  WM_DESTROY :
		//PostQuitMessage (0) ;
		break;
	   return 0 ;
    }
    return FALSE ;	
}


