#include "book_adjust_proc.h"
#define FIELD_NUM 7								//�����ж���Ϣ��ɿ��Ƶ��ֶ���
#define PRIMARY "adjustcode"						//����
#define TB_NAME "tb_book_adjust"					//��Ϣ����
#define DB_FK									//��Ϣ���д������
#define BASE_TB_NAME  "tb_base_store_info"			//�����
#define	INFO_TB_BOOK_NAME  "tb_book_info"				//�����
#define	INFO_TB_PROVIDER_NAME  "tb_provider_info"			//�����
#define BOOK_FK		"barcode"
#define PROVIDER_FK	 "provider"
#define SUB_TITLES_BOOK_COUNT	8
#define SUB_TITLES_PROVIDER_COUNT	7	
#define RESULT_TB_NAME "tb_stock_info"			//�����
#define RESULT_TB_FIELD_NUM 2						//����������ֶθ���
#define RESULT_TB_BOOK_CONDITION	"barcode"		//�������µ���������
#define RESULT_TB_STORE_CONDITION	"store"			//�������µ���������	
#define RESULT_TB_TARGET		"stock"			//�������µ�Ŀ������
BOOL CALLBACK Book_Adjust_Proc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char titles[FIELD_NUM][TITLE_LENTH_MAX] = {"����Ա","����ֿ�","��Ųֿ�","������","��Ӧ��","����","ʱ��"};
	char sub_book_titles[SUB_TITLES_BOOK_COUNT][TITLE_LENTH_MAX] = {"������","ͼ������","������","����","������","�۸�","��ע","����"};
	char sub_provider_titles[SUB_TITLES_PROVIDER_COUNT][TITLE_LENTH_MAX] = {"��Ӧ������","����","������","��ϵ�绰","��ϸ��ַ","��ַ","����"};
	HWND hView = GetDlgItem(hDlg,IDC_LIST_VIEW);
	static HWND bookSubView;
	static HWND providerSubView;
	ColumnMessage cms[FIELD_NUM];
	ColumnMessage resultCms[RESULT_TB_FIELD_NUM];
	char values[VALUE_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];					 //inser����е�ֵ,�����СΪ��ֵ�ܳ��ȺͶ��������Ӻ͵����ֵ
	char fields[FIELD_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];					 //inser����е��ֶ�,�����СΪ���ֶ��ܳ��ȺͶ��������Ӻ͵����ֵ
	int selIndex;														 //�ؼ���ǰ��ѡ�е���
	int i;																 //������
	//	int updateFlag;														//���±�ʶ����û�в�����Ҫ����ʱ��flagΪ0
	char sets[(VALUE_LENTH_MAX+FIELD_LENTH_MAX)*(FIELD_NUM)+(FIELD_NUM-1)*2+1] = "\0";		 //Update�����SET�����ݣ������СΪ��ֵ���ֶε��ܳ��ȼ��϶��š��Ӻŵ�����
	LPNMHDR pNmhdr;														//NMHDR��ʾWM_NOTIFY��Ϣ����,LPNMHDR���ʾָ������ָ������
	char temp[VALUE_LENTH_MAX];											//����EDIT����ʱ���õ��ַ�����
	SYSTEMTIME   systime;												//ϵͳʱ��ṹ��
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
			SendMessage(GetDlgItem(hDlg,IDC_TAKELIST),CB_ADDSTRING,0,(LPARAM)row[0]);
			SendMessage(GetDlgItem(hDlg,IDC_STORELIST),CB_ADDSTRING,0,(LPARAM)row[0]);
		}
		mysql_free_result(result);
		/*��ʼ���ۿ�*/
		SetWindowText(GetDlgItem(hDlg,IDC_REBATE),"1.00");
		/*��ʼ����������*/
		CreateSubViewProc(hDlg,&bookSubView,"ͼ��",sub_book_titles,SUB_TITLES_BOOK_COUNT,0);
		CreateSubViewProc(hDlg,&providerSubView,"��Ӧ��",sub_provider_titles,SUB_TITLES_PROVIDER_COUNT,1);
		/*��ʼ������Ա*/
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
			/*��ȡ����ֿ�*/
			selIndex = SendMessage(GetDlgItem(hDlg,IDC_TAKELIST),CB_GETCURSEL,0,0);
			if(selIndex == -1)
			{
				MessageBox(hDlg,"��ѡ������ֿ�","��ʾ",MB_ICONHAND);
				return 0;
			}
			SendMessage(GetDlgItem(hDlg,IDC_TAKELIST),CB_GETLBTEXT,selIndex,(LPARAM)cms[1].value);
			/*��ȡ��Ųֿ�*/
			selIndex = SendMessage(GetDlgItem(hDlg,IDC_STORELIST),CB_GETCURSEL,0,0);
			if(selIndex == -1)
			{
				MessageBox(hDlg,"��ѡ���Ųֿ�","��ʾ",MB_ICONHAND);
				return 0;
			}
			SendMessage(GetDlgItem(hDlg,IDC_STORELIST),CB_GETLBTEXT,selIndex,(LPARAM)cms[2].value);
			if(strcmp(cms[1].value,cms[2].value)==0)
			{
				MessageBox(hDlg,"ѡ����ͬһ�ֿ�","��ʾ",MB_ICONHAND);
				return 0;
			}
			/*��ȡ������*/
			GetDlgItemText(hDlg,IDC_ADJUST_BARCODE,cms[3].value,VALUE_LENTH_MAX);
			if(strlen(cms[3].value)<1)
			{
				MessageBox(hDlg,"������������","��ʾ",MB_ICONHAND);
				return 0;
			}
			/*��ȡ��Ӧ��*/
			GetDlgItemText(hDlg,IDC_ADJUST_PROVIDER,cms[4].value,VALUE_LENTH_MAX);
			if(strlen(cms[4].value)<1)
			{
				MessageBox(hDlg,"�����빩Ӧ��","��ʾ",MB_ICONHAND);
				return 0;
			}
			 /*��ȡ����*/
			GetDlgItemText(hDlg,IDC_ADJUST_COUNT,cms[5].value,VALUE_LENTH_MAX);
			if(strlen(cms[5].value)<1)
			{
				MessageBox(hDlg,"����������","��ʾ",MB_ICONHAND);
				return 0;
			}
			/*��ȡ��ϵͳʱ��*/
			GetSystemTime(&systime);
			sprintf(cms[6].value,"%d/%d/%d %d:%d:%d",systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond); 
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
			/*�Խ������в���*/
			FomatCMUpdate(resultCms,RESULT_TB_FIELD_NUM,sets);
			sprintf(sql,"update %s set %s=%s-%s where %s='%s' and %s='%s'",RESULT_TB_NAME,RESULT_TB_TARGET,RESULT_TB_TARGET,cms[5].value,RESULT_TB_BOOK_CONDITION,cms[3].value,RESULT_TB_STORE_CONDITION,cms[1].value);
			mysql_query(&mysql,sql);
			if(mysql_errno(&mysql))
			{
				MessageBoxEx(GetParent(hView),"��������","��ʾ",MB_ICONERROR|MB_TOPMOST,0);
				mysql_query(&mysql,"ROLLBACK");
				return 0;
			}
			sprintf(sql,"update %s set %s=%s+%s where %s='%s' and %s='%s'",RESULT_TB_NAME,RESULT_TB_TARGET,RESULT_TB_TARGET,cms[5].value,RESULT_TB_BOOK_CONDITION,cms[3].value,RESULT_TB_STORE_CONDITION,cms[2].value);
			mysql_query(&mysql,sql);
			if(mysql_errno(&mysql))
			{
				MessageBoxEx(GetParent(hView),"��������","��ʾ",MB_ICONERROR|MB_TOPMOST,0);
				mysql_query(&mysql,"ROLLBACK");
				return 0;
			}
			/*�����ύ*/
			mysql_query(&mysql,"COMMIT");
		    HoldInsertIDCondition(sql,PRIMARY);
		    QueryRecordToView(hView,TB_NAME,FIELD_NUM,sql,1,1);
			MessageBox(hDlg,"�����ɹ�","��ʾ",MB_OK);
			break;
		case IDC_BUTTON_SUB:
			if(IsWindow(bookSubView)==0)
			{
				CreateSubViewProc(hDlg,&bookSubView,"ͼ��",sub_book_titles,SUB_TITLES_BOOK_COUNT,0);
			}
			if(IsWindow(providerSubView)==0)
			{
				CreateSubViewProc(hDlg,&providerSubView,"��Ӧ��",sub_provider_titles,SUB_TITLES_PROVIDER_COUNT,1);
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


