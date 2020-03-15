#include "store_info_proc.h"
#define FIELD_NUM 1								//�ֿ���Ϣ����ֶ���
#define PRIMARY "storename"						//�ֿ���Ϣ���key
#define TB_NAME "tb_base_store_info"					//�ֿ���Ϣ����
#define	EDIT_COUNT 1								//EDIT�ؼ�����
#define NO1_EDIT	IDC_EDIT_STORE				//EDIT�ؼ���ID��ֵ��С��һ��
#define RESULT_TB_NAME "tb_stock_info"			//�����
#define RESULT_TB_FIELD_NUM 2						//�������Ӱ����ֶθ���
#define CONFIRM_TB_NAME "tb_book_info"		//ͬ�����ʹ�õ���Ϣ��
#define CONFIRM_COLUMN	"barcode"					//ͬ����
#define ONSALE			"����"
BOOL CALLBACK Store_Info_Proc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char titles[FIELD_NUM][TITLE_LENTH_MAX] = {"�ֿ�����"};
	HWND hView = GetDlgItem(hDlg,IDC_LIST_VIEW);
	ColumnMessage cms[FIELD_NUM];
	ColumnMessage resultCms[RESULT_TB_FIELD_NUM];
	char values[VALUE_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];					 //inser����е�ֵ,�����СΪ��ֵ�ܳ��ȺͶ��������Ӻ͵����ֵ
	char fields[FIELD_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];					 //inser����е��ֶ�,�����СΪ���ֶ��ܳ��ȺͶ��������Ӻ͵����ֵ
	int selIndex;														 //�ؼ���ǰ��ѡ�е���
	int i;																 //������
	int updateFlag;														//���±�ʶ����û�в�����Ҫ����ʱ��flagΪ0
	char sets[(VALUE_LENTH_MAX+FIELD_LENTH_MAX)*(FIELD_NUM)+(FIELD_NUM-1)*2+1] = "\0";		 //Update�����SET�����ݣ������СΪ��ֵ���ֶε��ܳ��ȼ��϶��š��Ӻŵ�����
	LPNMHDR pNmhdr;														//NMHDR��ʾWM_NOTIFY��Ϣ����,LPNMHDR���ʾָ������ָ������
	char temp[VALUE_LENTH_MAX];											//����EDIT����ʱ���õ��ַ�����
	char sql[SQL_LENTH_MAX];
	_int64 nums;
	_int64 bookColumn;
	switch (message)						
	{
	case   WM_INITDIALOG : 
		InitCommonControls();	
		SendMessage(hView,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE|LVS_EX_FLATSB);
		InitListViewColumns(hView,titles,FIELD_NUM );
		sprintf(sql,"storename!='%s'",ONSALE);
		QueryRecordToView(hView,TB_NAME,FIELD_NUM,sql,1,0);
		return TRUE ;
	case   WM_COMMAND :
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_ADD:
			sprintf(cms[0].fieldName,"storename");
			for(i=0;i<FIELD_NUM;i++)
			{
				cms[i].type = DB_CHAR;
			}
			GetDlgItemText(hDlg,NO1_EDIT,cms[0].value,VALUE_LENTH_MAX);
			if(strlen(cms[0].value)<1)
			{
				MessageBox(hDlg,"������ֿ�����","��ʾ",MB_ICONHAND);
				return 0;
			}
			/*��ʽ���ַ���*/
			FomatCMInsert(cms,FIELD_NUM,fields,values);
			/*����Ϣ����в���*/
			mysql_query(&mysql,"BEGIN");									//�����������
			if(!InsertData(TB_NAME,fields,values))
			{
				mysql_query(&mysql,"ROLLBACK");
				return 0;
			}
			/*װ�ؽ������Ϣ*/
			sprintf(resultCms[0].fieldName,"store");
			resultCms[0].type = DB_CHAR;
			sprintf(resultCms[0].value,"%s",cms[0].value);
			sprintf(resultCms[1].fieldName,"barcode");
			resultCms[1].type = DB_CHAR;
			/*�Խ������в���*/
			sprintf(sql,"select %s from %s",CONFIRM_COLUMN,CONFIRM_TB_NAME);
			mysql_query(&mysql,sql);
			result = mysql_store_result(&mysql);
			bookColumn = mysql_num_rows(result);
			for(i=0;i<bookColumn;i++)
			{
				row = mysql_fetch_row(result);
				sprintf(resultCms[1].value,"%s",row[0]);
				FomatCMInsert(resultCms,RESULT_TB_FIELD_NUM,fields,values);
				if(!InsertData(RESULT_TB_NAME,fields,values))
				{
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
			if(bookColumn != nums)
			{
				MessageBox(hDlg,"���������첽����","��ʾ",MB_ICONERROR);
				mysql_query(&mysql,"ROLLBACK");
				return 0;
			}
			mysql_query(&mysql,"COMMIT");
			sprintf(sql,"storename!='%s'",ONSALE);
		    QueryRecordToView(hView,TB_NAME,FIELD_NUM,sql,1,0);
			MessageBox(hDlg,"��ӳɹ�","��ʾ",MB_OK);
			break;
		case IDC_BUTTON_DELETE:
			DeleteFromListView(hView,TB_NAME,PRIMARY,DB_CHAR);
			break;
		case IDC_BUTTON_MODIFY:
			updateFlag = 0;
			sprintf(cms[0].fieldName,"storename");
			for(i=0;i<FIELD_NUM;i++)
			{
				cms[i].type = DB_CHAR;
			}
			for(i =0;i<EDIT_COUNT;i++)
			{
	     		GetDlgItemText(hDlg,NO1_EDIT+i,cms[i].value,VALUE_LENTH_MAX);
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
				selIndex = SendMessage(GetDlgItem(hDlg,IDC_COMBO_KINDS),CB_FINDSTRING,0,(LPARAM)temp);
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_LEVEL),CB_SETCURSEL,selIndex,0);
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


