#include "book_sale_back_proc.h"
#define FIELD_NUM 7								//�����ж���Ϣ��ɿ��Ƶ��ֶ���
#define PRIMARY "backcode"						//����
#define TB_NAME "tb_book_sale_back"					//��Ϣ����
#define	EDIT_COUNT 3								//����EDIT�ؼ�����
#define DB_FK									//��Ϣ���д������
#define BASE_TB_NAME  "tb_base_desk_info"			//�����
#define	INFO_TB_BOOK_NAME  "tb_book_info"				//�����
#define BOOK_FK		"barcode"
#define SUB_TITLES_BOOK_COUNT	8	
#define RESULT_TB_NAME "tb_stock_info"			//�����
#define RESULT_TB_FIELD_NUM 2						//����������ֶθ���
#define RESULT_TB_BOOK_CONDITION	"barcode"		//�������µ���������
#define RESULT_TB_STORE_CONDITION	"store"			//�������µ���������	
#define RESULT_TB_TARGET		"stock"			//�������µ�Ŀ������
BOOL CALLBACK Book_Sale_Back_Proc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char titles[FIELD_NUM][TITLE_LENTH_MAX] = {"����Ա","������","��̨","�˿�����","�˻�����","����","ʵ�����"};
	char sub_book_titles[SUB_TITLES_BOOK_COUNT][TITLE_LENTH_MAX] = {"������","ͼ������","������","����","������","�۸�","��ע","����"};
	HWND hView = GetDlgItem(hDlg,IDC_LIST_VIEW);
	static HWND bookSubView;
	ColumnMessage cms[FIELD_NUM];
	char values[VALUE_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];					 //inser����е�ֵ,�����СΪ��ֵ�ܳ��ȺͶ��������Ӻ͵����ֵ
	char fields[FIELD_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];					 //inser����е��ֶ�,�����СΪ���ֶ��ܳ��ȺͶ��������Ӻ͵����ֵ
	int selIndex;														 //�ؼ���ǰ��ѡ�е���
	int i;																 //������
	//	int updateFlag;														//���±�ʶ����û�в�����Ҫ����ʱ��flagΪ0
	char sets[(VALUE_LENTH_MAX+FIELD_LENTH_MAX)*(FIELD_NUM)+(FIELD_NUM-1)*2+1] = "\0";		 //Update�����SET�����ݣ������СΪ��ֵ���ֶε��ܳ��ȼ��϶��š��Ӻŵ�����
	LPNMHDR pNmhdr;														//NMHDR��ʾWM_NOTIFY��Ϣ����,LPNMHDR���ʾָ������ָ������
	char temp[VALUE_LENTH_MAX];											//����EDIT����ʱ���õ��ַ�����
	SYSTEMTIME   systime;												//ϵͳʱ��ṹ��
	double rebate;														//�ۿ�
	double pay;															//ʵ��
#ifdef	DB_FK
	char sql[SQL_LENTH_MAX];
	_int64 nums;
#endif
	switch (message)						
	{
	case   WM_INITDIALOG : 
		InitCommonControls();
		SetWindowText(hDlg,"�����˻�");
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
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_DESK),CB_ADDSTRING,0,(LPARAM)row[0]);
		}
		mysql_free_result(result);
		/*��ʼ���ۿ�*/
		SetWindowText(GetDlgItem(hDlg,IDC_SALE_REBATE),"1.00");
		/*��ʼ����������*/
		CreateSubViewProc(hDlg,&bookSubView,"ͼ��",sub_book_titles,SUB_TITLES_BOOK_COUNT,0);
		/*��ʼ������Ա����*/
		SetWindowText(GetDlgItem(hDlg,IDC_DISEDIT_OPERATOR),g_operator);
		return TRUE ;
	case   WM_COMMAND :
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_ADD:
			sprintf(cms[0].fieldName,"operator");
			sprintf(cms[1].fieldName,"barcode");
			sprintf(cms[2].fieldName,"desk");
			sprintf(cms[3].fieldName,"customer");
			sprintf(cms[4].fieldName,"count");
			sprintf(cms[5].fieldName,"date");
			sprintf(cms[6].fieldName,"pay");
			for(i=0;i<FIELD_NUM;i++)
			{
				cms[i].type = DB_CHAR;
			}
			cms[4].type = DB_INT;
			cms[6].type = DB_INT;
			GetDlgItemText(hDlg,IDC_DISEDIT_OPERATOR,cms[0].value,VALUE_LENTH_MAX);
			GetDlgItemText(hDlg,IDC_SALE_BARCODE,cms[1].value,VALUE_LENTH_MAX);
			if(strlen(cms[1].value)<1)
			{
				MessageBox(hDlg,"������������","��ʾ",MB_ICONHAND);
				return 0;
			}
			/*��ȡ��̨*/
			selIndex = SendMessage(GetDlgItem(hDlg,IDC_COMBO_DESK),CB_GETCURSEL,0,0);
			if(selIndex == -1)
			{
				MessageBox(hDlg,"��ѡ���̨","��ʾ",MB_ICONHAND);
				return 0;
			}
			SendMessage(GetDlgItem(hDlg,IDC_COMBO_DESK),CB_GETLBTEXT,selIndex,(LPARAM)cms[2].value);
			GetDlgItemText(hDlg,IDC_SALE_CUSTOMER,cms[3].value,VALUE_LENTH_MAX);
			 /*��ȡ����*/
			GetDlgItemText(hDlg,IDC_SALE_COUNT,cms[4].value,VALUE_LENTH_MAX);
			if(strlen(cms[4].value)<1)
			{
				MessageBox(hDlg,"����������","��ʾ",MB_ICONHAND);
				return 0;
			}
			/*��ȡ��ϵͳʱ��*/
			GetSystemTime(&systime);
			sprintf(cms[5].value,"%d/%d/%d %d:%d:%d",systime.wYear,systime.wMonth,systime.wDay,systime.wHour,systime.wMinute,systime.wSecond); 	
			/*ͨ���������ۿۡ����ۻ�ȡʵ�����*/
			GetDlgItemText(hDlg,IDC_SALE_REBATE,temp,VALUE_LENTH_MAX);
			if(strlen(temp)<1)
			{
				MessageBox(hDlg,"�������ۿ�","��ʾ",MB_ICONHAND);
				return 0;
			}
			rebate = atof(temp);
			if((int)(rebate*10)<3)
			{
				MessageBox(hDlg,"�ۿ۹���","��ʾ",MB_ICONHAND);
				return 0;
			}
			if((int)(rebate*10)>10)
			{
				MessageBox(hDlg,"�ۿ۲��ܳ���1","��ʾ",MB_ICONHAND);
				return 0;
			}
			sprintf(sql,"select price from %s where %s='%s'",INFO_TB_BOOK_NAME,BOOK_FK,cms[1].value);
			mysql_query(&mysql,sql);
			result = mysql_store_result(&mysql);
			if(mysql_num_rows(result)==0)
			{
				MessageBoxEx(hDlg,"��������������","��ʾ",MB_ICONHAND|MB_TOPMOST,0);
				return 0;
			}
			row = mysql_fetch_row(result);
			pay =atof(row[0])*atof(cms[4].value);
			sprintf(temp,"%3.2f",pay);
			SetDlgItemText(hDlg,IDC_SUMMONEY,temp);
			pay = pay*rebate;
			mysql_free_result(result);
			sprintf(cms[6].value,"%3.2f",pay);
			SetDlgItemText(hDlg,IDC_FACTMONEY,cms[6].value);
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
			sprintf(sql,"update %s set %s=%s+%s where %s='%s' and %s='%s'",RESULT_TB_NAME,RESULT_TB_TARGET,RESULT_TB_TARGET,cms[4].value,RESULT_TB_BOOK_CONDITION,cms[1].value,RESULT_TB_STORE_CONDITION,"����");
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
			break;
		case ID_QUIT:
			EndDialog(hDlg,0);
			break;
		case IDC_SALE_BARCODE:
			if(HIWORD(wParam)==EN_CHANGE)
			{
				SetSubViewFromEdit(GetDlgItem(hDlg,IDC_SALE_BARCODE),bookSubView,INFO_TB_BOOK_NAME,BOOK_FK,SUB_TITLES_BOOK_COUNT);
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
				SetWindowText(GetDlgItem(hDlg,IDC_SALE_BARCODE),temp);
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


