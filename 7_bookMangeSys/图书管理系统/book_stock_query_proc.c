#include "book_stock_query_proc.h"
#define FIELD_NUM 3								//�����ж���Ϣ��ɿ��Ƶ��ֶ���
#define CONDITON_NUM 4						//��ѯ�����ֶ���
#define TB_NAME "tb_stock_info"					//��ѯ��
#define BASE_TB_NAME "tb_base_store_info"			//�����
#define	INFO_TB_BOOK_NAME  "tb_book_info"				//�����
#define BOOK_FK		"barcode"
#define STORE_FK	"store"	
BOOL CALLBACK Book_Stock_Query_Proc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char titles[FIELD_NUM][TITLE_LENTH_MAX] = {"������","�����","����"};
	char queryTitles[CONDITON_NUM][TITLE_LENTH_MAX]={"ͼ������","������","����","���"};
	HWND hView = GetDlgItem(hDlg,IDC_LIST_VIEW);
	int selIndex;														 //�ؼ���ǰ��ѡ�е���
	int i;																 //������
	char temp[VALUE_LENTH_MAX];											//����EDIT����ʱ���õ��ַ�����
	char condition[SQL_LENTH_MAX] = "";
	int storeFlag;
	int bookFlag;
	char sql[SQL_LENTH_MAX];
	_int64 nums;
	switch (message)						
	{
	case   WM_INITDIALOG : 
		InitCommonControls();
		SendMessage(hView,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE|LVS_EX_FLATSB);
		InitListViewColumns(hView,titles,FIELD_NUM );
		/*��ʼ��������*/
		sprintf(sql,"select * from %s",BASE_TB_NAME);
		mysql_query(&mysql,sql);
		if(mysql_errno(&mysql))
		{
			MessageBoxEx(GetParent(hView),"��������","��ʾ",MB_ICONERROR|MB_TOPMOST,0);
			return 0;
		}
		result = mysql_store_result(&mysql);	
		nums = mysql_num_rows(result);
		for(i=0;i<nums;i++)
		{
			row = mysql_fetch_row(result);
			SendMessage(GetDlgItem(hDlg,IDC_STORELIST),CB_ADDSTRING,0,(LPARAM)row[0]);
		}
		mysql_free_result(result);
		
		for(i=0;i<CONDITON_NUM;i++)
		{
			SendMessage(GetDlgItem(hDlg,IDC_BOOK_CONDITIONLIST),CB_ADDSTRING,0,(LPARAM)queryTitles[i]);
		}
		return TRUE ;
	case   WM_COMMAND :
		switch(LOWORD(wParam))
		{
		case IDC_QUERY:
		    storeFlag = SendMessage(GetDlgItem(hDlg,IDC_STORE_CHECK),BM_GETCHECK,0,0);
			bookFlag = SendMessage(GetDlgItem(hDlg,IDC_BOOK_CHECK),BM_GETCHECK,0,0);
			/*��ŵز�ѯ*/
			if(storeFlag==BST_CHECKED)
			{
				selIndex = SendMessage(GetDlgItem(hDlg,IDC_STORELIST),CB_GETCURSEL,0,0);
				if(selIndex ==-1)
				{
					MessageBoxEx(hDlg,"��ѡ���ŵص�!","��ʾ",MB_ICONERROR|MB_TOPMOST,0);
					return 0;
				}
				SendMessage(GetDlgItem(hDlg,IDC_STORELIST),CB_GETLBTEXT,selIndex,(LPARAM)temp);
				sprintf(condition,"%s='%s'",STORE_FK,temp);
			}
			/*ͼ���ѯ*/
			if(bookFlag == BST_CHECKED)
			{
				if(storeFlag==BST_CHECKED)
				{
					sprintf(condition,"%s and ",condition);
				}
				selIndex = SendMessage(GetDlgItem(hDlg,IDC_BOOK_CONDITIONLIST),CB_GETCURSEL,0,0);
				if(selIndex ==-1)
				{
					MessageBoxEx(hDlg,"��ѡ��ͼ���ѯ����!","��ʾ",MB_ICONERROR|MB_TOPMOST,0);
					return 0;
				}
				GetWindowText(GetDlgItem(hDlg,IDC_BOOK_PROPERTY),temp,VALUE_LENTH_MAX);
				switch(selIndex)
				{
					case 0:
						if(strlen(temp)==0)
						{
							MessageBoxEx(hDlg,"������ͼ������!","��ʾ",MB_ICONERROR|MB_TOPMOST,0);
							return 0;
						}
						sprintf(condition,"%s%s in (select barcode from %s where bookname='%s')",condition,BOOK_FK,INFO_TB_BOOK_NAME,temp);
						break;
					case 1:
						if(strlen(temp)==0)
						{
							MessageBoxEx(hDlg,"������������!","��ʾ",MB_ICONERROR|MB_TOPMOST,0);
							return 0;
						}
						sprintf(condition,"%s%s='%s'",condition,BOOK_FK,temp);
						break;
					case 2:
						if(strlen(temp)==0)
						{
							MessageBoxEx(hDlg,"��������������!","��ʾ",MB_ICONERROR|MB_TOPMOST,0);
							return 0;
						}
						sprintf(condition,"%s%s in (select barcode from %s where authorname='%s')",condition,BOOK_FK,INFO_TB_BOOK_NAME,temp);
						break;
					case 3:
						if(strlen(temp)==0)
						{
							MessageBoxEx(hDlg,"������ͼ������!","��ʾ",MB_ICONERROR|MB_TOPMOST,0);
							return 0;
						}
						sprintf(condition,"%s%s in (select barcode from %s where kind='%s')",condition,BOOK_FK,INFO_TB_BOOK_NAME,temp);
						break;
					default:
						return 0;
				}
			}
			QueryRecordToView(hView,TB_NAME,FIELD_NUM,condition,1,0);
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


