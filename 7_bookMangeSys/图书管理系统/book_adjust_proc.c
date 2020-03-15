#include "book_adjust_proc.h"
#define FIELD_NUM 7								//窗体中对信息表可控制的字段数
#define PRIMARY "adjustcode"						//主键
#define TB_NAME "tb_book_adjust"					//信息表名
#define DB_FK									//信息表中存在外键
#define BASE_TB_NAME  "tb_base_store_info"			//外键表
#define	INFO_TB_BOOK_NAME  "tb_book_info"				//外键表
#define	INFO_TB_PROVIDER_NAME  "tb_provider_info"			//外键表
#define BOOK_FK		"barcode"
#define PROVIDER_FK	 "provider"
#define SUB_TITLES_BOOK_COUNT	8
#define SUB_TITLES_PROVIDER_COUNT	7	
#define RESULT_TB_NAME "tb_stock_info"			//结果表
#define RESULT_TB_FIELD_NUM 2						//结果表条件字段个数
#define RESULT_TB_BOOK_CONDITION	"barcode"		//结果表更新的条件属性
#define RESULT_TB_STORE_CONDITION	"store"			//结果表更新的条件属性	
#define RESULT_TB_TARGET		"stock"			//结果表更新的目标属性
BOOL CALLBACK Book_Adjust_Proc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char titles[FIELD_NUM][TITLE_LENTH_MAX] = {"操作员","提书仓库","存放仓库","条形码","供应商","数量","时间"};
	char sub_book_titles[SUB_TITLES_BOOK_COUNT][TITLE_LENTH_MAX] = {"条形码","图书名称","助记码","作者","出版社","价格","备注","种类"};
	char sub_provider_titles[SUB_TITLES_PROVIDER_COUNT][TITLE_LENTH_MAX] = {"供应商名称","法人","负责人","联系电话","详细地址","网址","邮箱"};
	HWND hView = GetDlgItem(hDlg,IDC_LIST_VIEW);
	static HWND bookSubView;
	static HWND providerSubView;
	ColumnMessage cms[FIELD_NUM];
	ColumnMessage resultCms[RESULT_TB_FIELD_NUM];
	char values[VALUE_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];					 //inser语句中的值,数组大小为：值总长度和逗号数量加和的最大值
	char fields[FIELD_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];					 //inser语句中的字段,数组大小为：字段总长度和逗号数量加和的最大值
	int selIndex;														 //控件当前被选中的项
	int i;																 //迭代器
	//	int updateFlag;														//更新标识，当没有参数需要更新时，flag为0
	char sets[(VALUE_LENTH_MAX+FIELD_LENTH_MAX)*(FIELD_NUM)+(FIELD_NUM-1)*2+1] = "\0";		 //Update语句中SET的内容，数组大小为：值和字段的总长度加上逗号、加号的数量
	LPNMHDR pNmhdr;														//NMHDR表示WM_NOTIFY消息内容,LPNMHDR则表示指向它的指针类型
	char temp[VALUE_LENTH_MAX];											//设置EDIT文字时所用的字符缓冲
	SYSTEMTIME   systime;												//系统时间结构体
#ifdef	DB_FK
	char sql[SQL_LENTH_MAX];
	_int64 nums;
#endif
	switch (message)						
	{
	case   WM_INITDIALOG : 
		InitCommonControls();
		SendMessage(hView,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE|LVS_EX_FLATSB);
		InitListViewColumns(hView,titles,FIELD_NUM );
		/*初始化下拉条*/
		sprintf(sql,"select * from %s",BASE_TB_NAME);
		mysql_query(&mysql,sql);
		if(mysql_errno(&mysql))
		{
			MessageBox(GetParent(hView),"操作错误","提示",MB_ICONERROR);
			return 0;
		}
		result = mysql_store_result(&mysql);	
		nums = mysql_num_rows(result);
		for(i=0;i<nums;i++)
		{
			row = mysql_fetch_row(result);
			SendMessage(GetDlgItem(hDlg,IDC_TAKELIST),CB_ADDSTRING,0,(LPARAM)row[0]);
			SendMessage(GetDlgItem(hDlg,IDC_STORELIST),CB_ADDSTRING,0,(LPARAM)row[0]);
		}
		mysql_free_result(result);
		/*初始化折扣*/
		SetWindowText(GetDlgItem(hDlg,IDC_REBATE),"1.00");
		/*初始化辅助窗口*/
		CreateSubViewProc(hDlg,&bookSubView,"图书",sub_book_titles,SUB_TITLES_BOOK_COUNT,0);
		CreateSubViewProc(hDlg,&providerSubView,"供应商",sub_provider_titles,SUB_TITLES_PROVIDER_COUNT,1);
		/*初始化操作员*/
		SetWindowText(GetDlgItem(hDlg,IDC_ADJUST_OPERATOR),g_operator);
		return TRUE ;
	case   WM_COMMAND :
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_ADD:
			sprintf(cms[0].fieldName,"operator");
			sprintf(cms[1].fieldName,"take");
			sprintf(cms[2].fieldName,"store");
			sprintf(cms[3].fieldName,"barcode");
			sprintf(cms[4].fieldName,"provider");
			sprintf(cms[5].fieldName,"count");
			sprintf(cms[6].fieldName,"date");
			for(i=0;i<FIELD_NUM;i++)
			{
				cms[i].type = DB_CHAR;
			}
			cms[5].type = DB_INT;
			GetDlgItemText(hDlg,IDC_ADJUST_OPERATOR,cms[0].value,VALUE_LENTH_MAX);
			/*获取提书仓库*/
			selIndex = SendMessage(GetDlgItem(hDlg,IDC_TAKELIST),CB_GETCURSEL,0,0);
			if(selIndex == -1)
			{
				MessageBox(hDlg,"请选择提书仓库","提示",MB_ICONHAND);
				return 0;
			}
			SendMessage(GetDlgItem(hDlg,IDC_TAKELIST),CB_GETLBTEXT,selIndex,(LPARAM)cms[1].value);
			/*获取存放仓库*/
			selIndex = SendMessage(GetDlgItem(hDlg,IDC_STORELIST),CB_GETCURSEL,0,0);
			if(selIndex == -1)
			{
				MessageBox(hDlg,"请选择存放仓库","提示",MB_ICONHAND);
				return 0;
			}
			SendMessage(GetDlgItem(hDlg,IDC_STORELIST),CB_GETLBTEXT,selIndex,(LPARAM)cms[2].value);
			if(strcmp(cms[1].value,cms[2].value)==0)
			{
				MessageBox(hDlg,"选择了同一仓库","提示",MB_ICONHAND);
				return 0;
			}
			/*获取条形码*/
			GetDlgItemText(hDlg,IDC_ADJUST_BARCODE,cms[3].value,VALUE_LENTH_MAX);
			if(strlen(cms[3].value)<1)
			{
				MessageBox(hDlg,"请输入条形码","提示",MB_ICONHAND);
				return 0;
			}
			/*获取供应商*/
			GetDlgItemText(hDlg,IDC_ADJUST_PROVIDER,cms[4].value,VALUE_LENTH_MAX);
			if(strlen(cms[4].value)<1)
			{
				MessageBox(hDlg,"请输入供应商","提示",MB_ICONHAND);
				return 0;
			}
			 /*获取数量*/
			GetDlgItemText(hDlg,IDC_ADJUST_COUNT,cms[5].value,VALUE_LENTH_MAX);
			if(strlen(cms[5].value)<1)
			{
				MessageBox(hDlg,"请输入数量","提示",MB_ICONHAND);
				return 0;
			}
			/*获取的系统时间*/
			GetSystemTime(&systime);
			sprintf(cms[6].value,"%d/%d/%d %d:%d:%d",systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond); 
			/*格式化字符串*/
			FomatCMInsert(cms,FIELD_NUM,fields,values);
			/*对信息表进行操作*/
			mysql_query(&mysql,"BEGIN");									//开启事务管理
			if(!InsertData(TB_NAME,fields,values))
			{
				MessageBoxEx(GetParent(hView),"操作错误","提示",MB_ICONERROR|MB_TOPMOST,0);
				mysql_query(&mysql,"ROLLBACK");
				return 0;
			}
			/*对结果表进行操作*/
			FomatCMUpdate(resultCms,RESULT_TB_FIELD_NUM,sets);
			sprintf(sql,"update %s set %s=%s-%s where %s='%s' and %s='%s'",RESULT_TB_NAME,RESULT_TB_TARGET,RESULT_TB_TARGET,cms[5].value,RESULT_TB_BOOK_CONDITION,cms[3].value,RESULT_TB_STORE_CONDITION,cms[1].value);
			mysql_query(&mysql,sql);
			if(mysql_errno(&mysql))
			{
				MessageBoxEx(GetParent(hView),"操作错误","提示",MB_ICONERROR|MB_TOPMOST,0);
				mysql_query(&mysql,"ROLLBACK");
				return 0;
			}
			sprintf(sql,"update %s set %s=%s+%s where %s='%s' and %s='%s'",RESULT_TB_NAME,RESULT_TB_TARGET,RESULT_TB_TARGET,cms[5].value,RESULT_TB_BOOK_CONDITION,cms[3].value,RESULT_TB_STORE_CONDITION,cms[2].value);
			mysql_query(&mysql,sql);
			if(mysql_errno(&mysql))
			{
				MessageBoxEx(GetParent(hView),"操作错误","提示",MB_ICONERROR|MB_TOPMOST,0);
				mysql_query(&mysql,"ROLLBACK");
				return 0;
			}
			/*事务提交*/
			mysql_query(&mysql,"COMMIT");
		    HoldInsertIDCondition(sql,PRIMARY);
		    QueryRecordToView(hView,TB_NAME,FIELD_NUM,sql,1,1);
			MessageBox(hDlg,"操作成功","提示",MB_OK);
			break;
		case IDC_BUTTON_SUB:
			if(IsWindow(bookSubView)==0)
			{
				CreateSubViewProc(hDlg,&bookSubView,"图书",sub_book_titles,SUB_TITLES_BOOK_COUNT,0);
			}
			if(IsWindow(providerSubView)==0)
			{
				CreateSubViewProc(hDlg,&providerSubView,"供应商",sub_provider_titles,SUB_TITLES_PROVIDER_COUNT,1);
			}
			break;
		case ID_QUIT:
			EndDialog(hDlg,0);
			break;
		case IDC_ADJUST_BARCODE:
			if(HIWORD(wParam)==EN_CHANGE)
			{
				SetSubViewFromEdit(GetDlgItem(hDlg,IDC_ADJUST_BARCODE),bookSubView,INFO_TB_BOOK_NAME,BOOK_FK,SUB_TITLES_BOOK_COUNT);
			}
			break;
		case IDC_ADJUST_PROVIDER:
			if(HIWORD(wParam)==EN_CHANGE)
			{
				SetSubViewFromEdit(GetDlgItem(hDlg,IDC_ADJUST_PROVIDER),providerSubView,INFO_TB_PROVIDER_NAME,PROVIDER_FK,SUB_TITLES_PROVIDER_COUNT);
			}
			break;
		}
	   return TRUE ; 
       break ;
	case WM_NOTIFY:
		pNmhdr = (LPNMHDR)lParam;
		if(NM_DBLCLK == pNmhdr->code)
		{
			selIndex = SendMessage(pNmhdr->hwndFrom,LVM_GETSELECTIONMARK,0,0);
			if(selIndex == -1)
			{
				return 0;
			}
			ListView_GetItemText(pNmhdr->hwndFrom,selIndex,0,temp,VALUE_LENTH_MAX);
			if(pNmhdr->hwndFrom==bookSubView)
			{
				SetWindowText(GetDlgItem(hDlg,IDC_ADJUST_BARCODE),temp);
			}
			if(pNmhdr->hwndFrom==providerSubView)
			{
				SetWindowText(GetDlgItem(hDlg,IDC_ADJUST_PROVIDER),temp);
			}
		}
		return 1;
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


