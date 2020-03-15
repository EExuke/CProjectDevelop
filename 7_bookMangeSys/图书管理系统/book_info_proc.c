#include "book_info_proc.h"
#define FIELD_NUM 8								//ͼ����Ϣ����ֶ���
#define PRIMARY "barcode"						//ͼ����Ϣ���key
#define TB_NAME "tb_book_info"					//ͼ�����
#define	EDIT_COUNT 7								//EDIT�ؼ�����
#define NO1_EDIT	IDC_EDIT_BARCODE			//EDIT�ؼ���ID��ֵ��С��һ��
#define BASE_TB_NAME  "tb_base_book_kind"			//�����
#define RESULT_TB_NAME "tb_stock_info"			//�����
#define RESULT_TB_FIELD_NUM 2						//�������Ӱ����ֶθ���
#define CONFIRM_TB_NAME "tb_base_store_info"		//ͬ�����ʹ�õ���Ϣ��
#define CONFIRM_COLUMN	"storename"					//ͬ����
BOOL CALLBACK Book_Info_Proc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char titles[FIELD_NUM][TITLE_LENTH_MAX] = {"������","ͼ������","������","����","������","�۸�","��ע","����"};
	HWND hView = GetDlgItem(hDlg,IDC_LIST_VIEW);
	ColumnMessage cms[FIELD_NUM];
	ColumnMessage resultCms[RESULT_TB_FIELD_NUM];
	char values[VALUE_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];					 //inser����е�ֵ,�����СΪ��ֵ�ܳ��ȺͶ��������Ӻ͵����ֵ
	char fields[FIELD_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];					 //inser����е��ֶ�,�����СΪ���ֶ��ܳ��ȺͶ��������Ӻ͵����ֵ
	int selIndex;														 //�ؼ���ǰ��ѡ�е���
	int i;																 //������
	int updateFlag;														//���±�ʶ����û�в�����Ҫ����ʱ��flagΪ0
	char sets[(VALUE_LENTH_MAX+FIELD_LENTH_MAX)*(FIELD_NUM)+(FIELD_NUM-1)*2+1] = "\0";		 //Update�����SET�����ݣ������СΪ��ֵ���ֶε��ܳ��ȼ��϶��š��Ӻŵ�����
	LPNMHDR pNmhdr;														//NMHDR��ʾ���ڵ�WM_NOTIFY��Ϣ����,LPNMHDR���ʾָ������ָ������
	char temp[VALUE_LENTH_MAX];											//����EDIT����ʱ���õ��ַ�����
	_int64 stockColumn;													//�������������Ŀ
	char sql[SQL_LENTH_MAX];
	_int64 nums;
	switch (message)						
	{
	case   WM_INITDIALOG : 
		InitCommonControls();	
		SendMessage(hView,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE|LVS_EX_FLATSB);
		InitListViewColumns(hView,titles,FIELD_NUM );
		QueryRecordToView(hView,TB_NAME,FIELD_NUM,"",1,0);
		/*��ʼ��������*/
		sprintf(sql,"select * from %s",BASE_TB_NAME);
		mysql_query(&mysql,sql);
		if(mysql_errno(&mysql))
		{
			MessageBox(GetParent(hView),"��������","��ʾ",MB_ICONERROR);
			return 0;
		}
		result = mysql_store_result(&mysql);	
		nums = mysql_num_rows(result);
		for(i=0;i<nums;i++)
		{
			row = mysql_fetch_row(result);
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_KINDS),CB_ADDSTRING,0,(LPARAM)row[0]);
		}
		mysql_free_result(result);
		return TRUE ;
	case   WM_COMMAND :
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_ADD:
			sprintf(cms[0].fieldName,"barcode");
			sprintf(cms[1].fieldName,"bookname");
			sprintf(cms[2].fieldName,"mncode");
			sprintf(cms[3].fieldName,"authorname");
			sprintf(cms[4].fieldName,"bookconcern");
			sprintf(cms[5].fieldName,"price");
			sprintf(cms[6].fieldName,"memo");
			sprintf(cms[7].fieldName,"kind");
			for(i=0;i<FIELD_NUM;i++)
			{
				cms[i].type = DB_CHAR;
			}
			/*price�Ĳ�������ΪINT*/
			cms[5].type = DB_INT;

			GetDlgItemText(hDlg,IDC_EDIT_BOOKNAME,cms[1].value,VALUE_LENTH_MAX);
			if(strlen(cms[1].value)<1)
			{
				MessageBox(hDlg,"������ͼ������","��ʾ",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_EDIT_MNCODE,cms[2].value,VALUE_LENTH_MAX);
			if(strlen(cms[2].value)<1)
			{
				MessageBox(hDlg,"������������","��ʾ",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_EDIT_BARCODE,cms[0].value,VALUE_LENTH_MAX);
			if(strlen(cms[0].value)<1)
			{
				MessageBox(hDlg,"������������","��ʾ",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_EDIT_AUTHOR,cms[3].value,VALUE_LENTH_MAX);
			if(strlen(cms[3].value)<1)
			{
				MessageBox(hDlg,"��������������","��ʾ",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_EDIT_PUBLIC,cms[4].value,VALUE_LENTH_MAX);
			if(strlen(cms[4].value)<1)
			{
				MessageBox(hDlg,"�����������","��ʾ",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_EDIT_PRICE,cms[5].value,VALUE_LENTH_MAX);
			if(strlen(cms[5].value)<1)
			{
				MessageBox(hDlg,"������ͼ��۸�","��ʾ",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_EDIT_MEMO,cms[6].value,VALUE_LENTH_MAX);
			selIndex = SendMessage(GetDlgItem(hDlg,IDC_COMBO_KINDS),CB_GETCURSEL,0,0);
			if(selIndex == -1)
			{
				MessageBox(hDlg,"��ѡ��ͼ������","��ʾ",MB_ICONHAND);
				return 0;
			} 
		    SendMessage(GetDlgItem(hDlg,IDC_COMBO_KINDS),CB_GETLBTEXT,selIndex,(LPARAM)cms[7].value);
			/*��ʽ���ַ���*/
			FomatCMInsert(cms,FIELD_NUM,fields,values);
			/*����Ϣ����в���*/
			mysql_query(&mysql,"BEGIN");									//�����������
			if(!InsertData(TB_NAME,fields,values))
			{
				MessageBoxEx(GetParent(hView),"��������","��ʾ",MB_ICONERROR|MB_TOPMOST,0);
				mysql_query(&mysql,"ROLLBACK");
				return 0;
			}
			/*װ�ؽ������Ϣ*/
			sprintf(resultCms[0].fieldName,"barcode");
			resultCms[0].type = DB_CHAR;
			sprintf(resultCms[0].value,"%s",cms[0].value);
			sprintf(resultCms[1].fieldName,"store");
			resultCms[1].type = DB_CHAR;
			/*�Խ������в���*/
			sprintf(sql,"select %s from %s",CONFIRM_COLUMN,CONFIRM_TB_NAME);
			mysql_query(&mysql,sql);
			if(mysql_errno(&mysql))
			{
				MessageBoxEx(GetParent(hView),"��������","��ʾ",MB_ICONERROR|MB_TOPMOST,0);
				return 0;
			}
			result = mysql_store_result(&mysql);
			stockColumn = mysql_num_rows(result);
			for(i=0;i<stockColumn;i++)
			{
				row = mysql_fetch_row(result);
				sprintf(resultCms[1].value,"%s",row[0]);
				FomatCMInsert(resultCms,RESULT_TB_FIELD_NUM,fields,values);
				if(!InsertData(RESULT_TB_NAME,fields,values))
				{
					MessageBox(hDlg,"��������","��ʾ",MB_ICONERROR);
					mysql_query(&mysql,"ROLLBACK");
					return 0;
				}
			}
			mysql_free_result(result);
			sprintf(sql,"select count(*) from %s",CONFIRM_TB_NAME);
			mysql_query(&mysql,sql);
			result = mysql_store_result(&mysql);
			row = mysql_fetch_row(result);
			nums = (_int64)atoi(row[0]);
			if(stockColumn != nums)
			{
				MessageBox(hDlg,"���������첽����","��ʾ",MB_ICONERROR);
				mysql_query(&mysql,"ROLLBACK");
				return 0;
			}
			/*�����ύ*/
			mysql_query(&mysql,"COMMIT");
		    QueryRecordToView(hView,TB_NAME,FIELD_NUM,"",1,0);
			MessageBox(hDlg,"��ӳɹ�","��ʾ",MB_OK);
			break;
		case IDC_BUTTON_DELETE:
			DeleteFromListView(hView,TB_NAME,PRIMARY,DB_CHAR);
			break;
		case IDC_BUTTON_MODIFY:
			updateFlag = 0;
			sprintf(cms[0].fieldName,"barcode");
			sprintf(cms[1].fieldName,"bookname");
			sprintf(cms[2].fieldName,"mncode");
			sprintf(cms[3].fieldName,"authorname");
			sprintf(cms[4].fieldName,"bookconcern");
			sprintf(cms[5].fieldName,"price");
			sprintf(cms[6].fieldName,"memo");
			sprintf(cms[7].fieldName,"kind");
			for(i=0;i<FIELD_NUM;i++)
			{
				cms[i].type = DB_CHAR;
			}
			/*price�Ĳ�������ΪINT*/
			cms[5].type = DB_INT;
			for(i =0;i<EDIT_COUNT;i++)
			{
	     		GetDlgItemText(hDlg,NO1_EDIT+i,cms[i].value,VALUE_LENTH_MAX);
			}
			selIndex = SendMessage(GetDlgItem(hDlg,IDC_COMBO_KINDS),CB_GETCURSEL,0,0);
			if(selIndex == -1)
			{
				sprintf(cms[7].value,"");
			}
			else
			{
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KINDS),CB_GETLBTEXT,selIndex,(LPARAM)cms[7].value);
			}
			/*��ʽ���ַ���*/
			if(FomatCMUpdate(cms,FIELD_NUM,sets)>0)
			{
				UpDateDataFromListView(hView,TB_NAME,sets,FIELD_NUM,PRIMARY,DB_CHAR);
			}
			else
			{
				MessageBox(hDlg,"�޸���Ϣ����ȫ��Ϊ��!","��ʾ",MB_ICONERROR);
			}	
			break;
		
		case ID_QUIT:
			EndDialog(hDlg,0);
			break;
		}
	   return TRUE ; 
       break ;
	case WM_NOTIFY:
		if(wParam == IDC_LIST_VIEW)
		{
			pNmhdr = (LPNMHDR)lParam;
			if(NM_DBLCLK == pNmhdr->code)
			{
				selIndex = SendMessage(GetDlgItem(hDlg,IDC_LIST_VIEW),LVM_GETSELECTIONMARK,0,0);
				if(selIndex == -1)
				{
					return 0;
				}
				for(i = 0;i<EDIT_COUNT;i++)
				{
					ListView_GetItemText(hView,selIndex,i,temp,VALUE_LENTH_MAX);
					SetDlgItemText(hDlg,NO1_EDIT+i,temp);				//������EDIT ID
				}
				ListView_GetItemText(hView,selIndex,FIELD_NUM-1,temp,VALUE_LENTH_MAX);
				selIndex = SendMessage(GetDlgItem(hDlg,IDC_COMBO_KINDS),CB_FINDSTRING,0,(LPARAM)temp);
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_KINDS),CB_SETCURSEL,selIndex,0);
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


