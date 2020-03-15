#include "provider_info_proc.h"
#define FIELD_NUM 7								//��Ӧ����Ϣ����ֶ���
#define PRIMARY "provider"						//��Ӧ����Ϣ���key
#define TB_NAME "tb_provider_info"					//��Ӧ�̱���
#define	EDIT_COUNT 7								//EDIT�ؼ�����
#define NO1_EDIT	IDC_PROVIDER
BOOL CALLBACK Provider_Info_Proc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char titles[FIELD_NUM][TITLE_LENTH_MAX] = {"��Ӧ������","����","������","��ϵ�绰","��ϸ��ַ","��ַ","����"};
	HWND hView = GetDlgItem(hDlg,IDC_LIST_VIEW);
	ColumnMessage cms[FIELD_NUM];
	char values[VALUE_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];											 //insert����е�ֵ,�����СΪ��ֵ�ܳ��ȺͶ��������Ӻ͵����ֵ
	char fields[FIELD_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];											 //insert����е��ֶ�,�����СΪ���ֶ��ܳ��ȺͶ��������Ӻ͵����ֵ
	int selIndex;																				 //�ؼ���ǰ��ѡ�е���
	int i;																						 //������
	int updateFlag;																				//���±�ʶ����û�в�����Ҫ����ʱ��flagΪ0
	char sets[(VALUE_LENTH_MAX+FIELD_LENTH_MAX)*(FIELD_NUM)+(FIELD_NUM-1)*2+1] = "\0";			 //Update�����SET�����ݣ������СΪ��ֵ���ֶε��ܳ��ȼ��϶��š��Ӻŵ�����
	LPNMHDR pNmhdr;																				//NMHDR��ʾWM_NOTIFY��Ϣ����,LPNMHDR���ʾָ������ָ������
	char temp[VALUE_LENTH_MAX];																	//����EDIT����ʱ���õ��ַ�����
	switch (message)						
	{
	case   WM_INITDIALOG : 
		InitCommonControls();	
		SendMessage(hView,LVM_SETEXTENDEDLISTVIEWSTYLE,0,LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE|LVS_EX_FLATSB);
		InitListViewColumns(hView,titles,FIELD_NUM );
		QueryRecordToView(hView,TB_NAME,FIELD_NUM,"",1,0);
		return TRUE ;
	case   WM_COMMAND :
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_ADD:
			sprintf(cms[0].fieldName,"provider");
			sprintf(cms[1].fieldName,"corporation");
			sprintf(cms[2].fieldName,"principal");
			sprintf(cms[3].fieldName,"phone");
			sprintf(cms[4].fieldName,"addr");
			sprintf(cms[5].fieldName,"web");
			sprintf(cms[6].fieldName,"e_mail");
			for(i=0;i<FIELD_NUM;i++)
			{
				cms[i].type = DB_CHAR;
			}
			GetDlgItemText(hDlg,IDC_PROVIDER,cms[0].value,VALUE_LENTH_MAX);
			if(strlen(cms[0].value)<1)
			{
				MessageBox(hDlg,"�����빩Ӧ������","��ʾ",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_PROVIDER_CEO,cms[1].value,VALUE_LENTH_MAX);
			if(strlen(cms[0].value)<1)
			{
				MessageBox(hDlg,"�����뷨������","��ʾ",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_MANAGER,cms[2].value,VALUE_LENTH_MAX);
			if(strlen(cms[2].value)<1)
			{
				MessageBox(hDlg,"�����븺��������","��ʾ",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_PROVIDER_PHONE,cms[3].value,VALUE_LENTH_MAX);
			if(strlen(cms[3].value)<1)
			{
				MessageBox(hDlg,"�����빩Ӧ�̵绰����","��ʾ",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_PROVIDER_ADDRESS,cms[4].value,VALUE_LENTH_MAX);
			if(strlen(cms[4].value)<1)
			{
				MessageBox(hDlg,"�����빩Ӧ����ϵ��ַ","��ʾ",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_PROVIDER_NETADDRESS,cms[5].value,VALUE_LENTH_MAX);
			if(strlen(cms[5].value)<1)
			{
				MessageBox(hDlg,"�����빩Ӧ����ַ","��ʾ",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_PROVIDER_EMAIL,cms[6].value,VALUE_LENTH_MAX);
		    if(strlen(cms[6].value)<1)
			{
				MessageBox(hDlg,"�����빩Ӧ������","��ʾ",MB_ICONHAND);
				return 0;
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
			sprintf(cms[0].fieldName,"provider");
			sprintf(cms[1].fieldName,"corporation");
			sprintf(cms[2].fieldName,"principal");
			sprintf(cms[3].fieldName,"phone");
			sprintf(cms[4].fieldName,"addr");
			sprintf(cms[5].fieldName,"web");
			sprintf(cms[6].fieldName,"e_mail");
			
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


