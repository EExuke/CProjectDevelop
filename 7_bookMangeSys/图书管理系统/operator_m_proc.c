#include "operator_m_proc.h"
#define FIELD_NUM 3  //����ֶ���
#define PRIMARY "name"   //���key
#define TB_NAME "tb_operators"
BOOL CALLBACK Operator_M_Proc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char titles[FIELD_NUM][TITLE_LENTH_MAX] = {"����Ա��½��","����Ա����","����Ա�ȼ�"};
	HWND hView = GetDlgItem(hDlg,IDC_LIST_VIEW);
	ColumnMessage cms[FIELD_NUM];
	char values[VALUE_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];	 //inser����е�ֵ,�����СΪ��ֵ�ܳ��ȺͶ��������Ӻ͵����ֵ
	char fields[FIELD_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];  //inser����е��ֶ�,�����СΪ���ֶ��ܳ��ȺͶ��������Ӻ͵����ֵ
	int selIndex;													 //�ؼ���ǰ��ѡ�е���
	int updateFlag;													//���±�ʶ����û�в�����Ҫ����ʱ��flagΪ0
	char sets[(VALUE_LENTH_MAX+FIELD_LENTH_MAX)*(FIELD_NUM)+(FIELD_NUM-1)*2+1] = "\0"; //Update�����SET�����ݣ������СΪ��ֵ���ֶε��ܳ��ȼ��϶��š��Ӻŵ����� 
	switch (message)    
	{
	case   WM_INITDIALOG : 
		InitCommonControls();	
		SendMessage(hView,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE|LVS_EX_FLATSB);
		InitListViewColumns(hView,titles,FIELD_NUM );
		QueryRecordToView(hView,TB_NAME,FIELD_NUM,"",1,0);
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_LEVEL),CB_ADDSTRING,0,(LPARAM)"1");
		SendMessage(GetDlgItem(hDlg,IDC_COMBO_LEVEL),CB_ADDSTRING,0,(LPARAM)"2");
		return TRUE ;
	case   WM_COMMAND :
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_ADD:
			sprintf(cms[0].fieldName,"name");
			sprintf(cms[1].fieldName,"password");
			sprintf(cms[2].fieldName,"level");
			cms[0].type = DB_CHAR;
			cms[1].type = DB_CHAR;
			cms[2].type = DB_INT;
			GetDlgItemText(hDlg,IDC_EDIT_NAME,cms[0].value,VALUE_LENTH_MAX);
			if(strlen(cms[0].value)<1)
			{
				MessageBox(hDlg,"���������Ա����","��ʾ",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_EDIT_PWD,cms[1].value,VALUE_LENTH_MAX);
			selIndex = SendMessage(GetDlgItem(hDlg,IDC_COMBO_LEVEL),CB_GETCURSEL,0,0);
			if(selIndex == -1)
			{
				strcpy(cms[2].value,"");
			}
			else
			{
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_LEVEL),CB_GETLBTEXT,selIndex,(LPARAM)cms[2].value);
			}
			/*��ʽ���ַ���*/
			FomatCMInsert(cms,FIELD_NUM,fields,values);
			/*����Ϣ����в���*/
			if(!InsertData(TB_NAME,fields,values))
			{
				MessageBoxEx(GetParent(hView),"��������","��ʾ",MB_ICONERROR|MB_TOPMOST,0);
				mysql_query(&mysql,"ROLLBACK");
				return 0;
			}
		    QueryRecordToView(hView,TB_NAME,FIELD_NUM,"",1,0);
			MessageBox(hDlg,"��ӳɹ�","��ʾ",MB_OK);
			break;
		case IDC_BUTTON_DELETE:
			DeleteFromListView(hView,TB_NAME,PRIMARY,DB_CHAR);
			break;
		case IDC_BUTTON_MODIFY:
			updateFlag = 0;
			sprintf(cms[0].fieldName,"name");
			sprintf(cms[1].fieldName,"password");
			sprintf(cms[2].fieldName,"level");
			cms[0].type = DB_CHAR;
			cms[1].type = DB_CHAR;
			cms[2].type = DB_INT;
			GetDlgItemText(hDlg,IDC_EDIT_NAME,cms[0].value,VALUE_LENTH_MAX);
			GetDlgItemText(hDlg,IDC_EDIT_PWD,cms[1].value,VALUE_LENTH_MAX);
			selIndex = SendMessage(GetDlgItem(hDlg,IDC_COMBO_LEVEL),CB_GETCURSEL,0,0);
			if(selIndex == -1)
			{
				strcpy(cms[2].value,"");
			}
			else
			{
				SendMessage(GetDlgItem(hDlg,IDC_COMBO_LEVEL),CB_GETLBTEXT,selIndex,(LPARAM)cms[2].value);
			}
			/*��ʽ���ַ���*/
			if(FomatCMUpdate(cms,FIELD_NUM,sets)>0)
			{
				UpDateDataFromListView(hView,TB_NAME,sets,FIELD_NUM,PRIMARY,DB_CHAR);		//��������
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
	case WM_NOTIFY:
		return 1;
	case WM_CLOSE: 
		EndDialog(hDlg,0);
	case  WM_DESTROY :
		break;
	   return 0 ;
    }
    return FALSE ;	
}
