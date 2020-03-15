#include "provider_info_proc.h"
#define FIELD_NUM 7								//供应商信息表的字段数
#define PRIMARY "provider"						//供应商信息表的key
#define TB_NAME "tb_provider_info"					//供应商表名
#define	EDIT_COUNT 7								//EDIT控件数量
#define NO1_EDIT	IDC_PROVIDER
BOOL CALLBACK Provider_Info_Proc(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	char titles[FIELD_NUM][TITLE_LENTH_MAX] = {"供应商名称","法人","负责人","联系电话","详细地址","网址","邮箱"};
	HWND hView = GetDlgItem(hDlg,IDC_LIST_VIEW);
	ColumnMessage cms[FIELD_NUM];
	char values[VALUE_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];											 //insert语句中的值,数组大小为：值总长度和逗号数量加和的最大值
	char fields[FIELD_LENTH_MAX*FIELD_NUM+FIELD_NUM-1];											 //insert语句中的字段,数组大小为：字段总长度和逗号数量加和的最大值
	int selIndex;																				 //控件当前被选中的项
	int i;																						 //迭代器
	int updateFlag;																				//更新标识，当没有参数需要更新时，flag为0
	char sets[(VALUE_LENTH_MAX+FIELD_LENTH_MAX)*(FIELD_NUM)+(FIELD_NUM-1)*2+1] = "\0";			 //Update语句中SET的内容，数组大小为：值和字段的总长度加上逗号、加号的数量
	LPNMHDR pNmhdr;																				//NMHDR表示WM_NOTIFY消息内容,LPNMHDR则表示指向它的指针类型
	char temp[VALUE_LENTH_MAX];																	//设置EDIT文字时所用的字符缓冲
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
				MessageBox(hDlg,"请输入供应商名称","提示",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_PROVIDER_CEO,cms[1].value,VALUE_LENTH_MAX);
			if(strlen(cms[0].value)<1)
			{
				MessageBox(hDlg,"请输入法人名称","提示",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_MANAGER,cms[2].value,VALUE_LENTH_MAX);
			if(strlen(cms[2].value)<1)
			{
				MessageBox(hDlg,"请输入负责人名称","提示",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_PROVIDER_PHONE,cms[3].value,VALUE_LENTH_MAX);
			if(strlen(cms[3].value)<1)
			{
				MessageBox(hDlg,"请输入供应商电话号码","提示",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_PROVIDER_ADDRESS,cms[4].value,VALUE_LENTH_MAX);
			if(strlen(cms[4].value)<1)
			{
				MessageBox(hDlg,"请输入供应商联系地址","提示",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_PROVIDER_NETADDRESS,cms[5].value,VALUE_LENTH_MAX);
			if(strlen(cms[5].value)<1)
			{
				MessageBox(hDlg,"请输入供应商网址","提示",MB_ICONHAND);
				return 0;
			}
			GetDlgItemText(hDlg,IDC_PROVIDER_EMAIL,cms[6].value,VALUE_LENTH_MAX);
		    if(strlen(cms[6].value)<1)
			{
				MessageBox(hDlg,"请输入供应商邮箱","提示",MB_ICONHAND);
				return 0;
			}
			/*格式化字符串*/
			FomatCMInsert(cms,FIELD_NUM,fields,values);
			/*对信息表进行操作*/
			if(!InsertData(TB_NAME,fields,values))
			{
				MessageBoxEx(GetParent(hView),"操作错误","提示",MB_ICONERROR|MB_TOPMOST,0);
				mysql_query(&mysql,"ROLLBACK");
				return 0;
			}
		    QueryRecordToView(hView,TB_NAME,FIELD_NUM,"",1,0);
			MessageBox(hDlg,"添加成功","提示",MB_OK);
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
		   
			/*格式化字符串*/
			if(FomatCMUpdate(cms,FIELD_NUM,sets)>0)
			{
				UpDateDataFromListView(hView,TB_NAME,sets,FIELD_NUM,PRIMARY,DB_CHAR);		//更新数据
			}
			else
			{
				MessageBox(hDlg,"修改信息不可全部为空!","提示",MB_ICONERROR);
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
					SetDlgItemText(hDlg,NO1_EDIT+i,temp);				//相连的EDIT ID
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


