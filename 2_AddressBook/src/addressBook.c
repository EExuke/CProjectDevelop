/**************************************************************************** **
 * Copyright (C) 2010-2011 Cameo Communications, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : addressBook.c
 *     FIRST CREATION DATE    : 2020/02/01
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : EExuke
 *     Last Change            : 2020/02/01
 *     FILE DESCRIPTION       :
** ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "../inc/addressBook.h"

/****************************************************************************
 *  Function Name : main
 *  Description   : The Main Function 调用各子模块.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : 保持菜单显示，和接收用户按键输入.
 *  Returns       : 0
 ****************************************************************************/
void main()
{
	link l;
	l = (Node*)malloc(sizeof(Node));
	if (!l)
	{
		printf("\n allocate memory failure ");
		return 0;
	}
	l->next = NULL;
	load(l);    /* 启动时读入默认通讯录文件信息 */
	system("clear");
	while(1)
	{
		system("clear");
		switch(menu_select())
		{
			case 1:
				enter(l);       /* 数据录入 */
				break;
			case 2:
				del(l);         /* 删除数据 */
				break;
			case 3:
				list(l);        /* 显示列表 */
				break;
			case 4:
				search(l);      /* 数据查询 */
				break;
			case 5:
				save(l);        /* 数据保存 */
				break;
			case 6:
				load(l);        /* 加载数据 */
				break;
			case 7:
				exit(0);
		}
	}
}

/****************************************************************************
 *  Function Name : stringinput
 *  Description   : The Function 检测输入字符串，并将合理的字符串复制到指定的位置.
 *  Input(s)      : t 最后接收输入字符串的位置
 *                : lens 长度限制
 *                : notice 提示信息
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void stringinput(char *t, int lens, char *notice)
{
	char n[50];
	do{
		printf("%s", notice);           /* 显示提示信息 */
		scanf("%s", &n);                /* 输入字符串 */
		if(strlen(n) > lens)
			printf("\n exceed the required length! \n");    /* 超过lens长度，则重新输入 */
	}while(strlen(n) > lens);
	strcpy(t, n);                      /* 输入的合法字符串复制到t中 */
}

/****************************************************************************
 *  Function Name : enter
 *  Description   : The Function 录入模块，完成数据添加.
 *  Input(s)      : l 保存通讯录数据的链表
 *                :
 *  Output(s)     ：将添加的数据插入到链表中保存.
 *  Returns       : 0
 ****************************************************************************/
void enter(link l)
{
	Node *p, *q;
	q = l;
	while(1)
	{
		p = (Node*)malloc(sizeof(Node));    /* 为新数据申请节点空间 */
		if(!p)
		{
			printf("memory malloc fail\n");
			return;
		}
		stringinput(p->data.name, 15, "enter name (0-return):");        /* 输入姓名 */
		if(strcmp(p->data.name, "0") == 0)               /* 检查输入姓名是否为0 */
			break;
		stringinput(p->data.city, 10, "enter city:");        /* 输入城市 */
		stringinput(p->data.province, 10, "enter province:");        /* 输入省 */
		stringinput(p->data.state, 10, "enter status:");     /* 输入国家 */
		stringinput(p->data.tel, 15, "enter telephone:");        /* 输入电话号码 */
		p->next = NULL;    /* 尾插法插入新节点 */
		q->next = p;
		q = p;
	}
}

/****************************************************************************
 *  Function Name : del
 *  Description   : The Function 为删除模块，用于删除指定的数据信息,使用了链表的删除方法.
 *  Input(s)      : l 保存通讯录数据的链表
 *                :
 *  Output(s)     : 显示删除是否成功.
 *  Returns       : NULL
 ****************************************************************************/
void del(link l)
{
	Node *p, *q;
	char s[20];
	int delFlag = 0;
	q = l;
	p = q->next;
	printf("enter name:");
	scanf("%s", s);
	while(p)
	{
		if(strcmp(s, p->data.name) == 0)      /* 通过p和q顺序查找链表，直到找到与输入姓名匹配的信息 */
		{
			q->next = p->next;    /* 从链表中删除找到的p节点 */
			free(p);              /* 释放p节点空间 */
			printf("delete successfully!");
			delFlag = 1;
			break;
		}
		else
		{
			q = p;        /* q始终指向p的前一个节点 */
			p = q->next;  /* 往后遍历链表 */
		}
	}
	if (delFlag == 0)
		printf("can not find '%s' ! \n", s);
	getchar();
	getchar();
}

/****************************************************************************
 *  Function Name : display
 *  Description   : The Function 显示模块，用于显示节点中的具体信息.
 *  Input(s)      : p 要显示的信息节点
 *                :
 *  Output(s)     : 输出节点保存的数据信息.
 *  Returns       : NULL
 ****************************************************************************/
void display(Node * p)
{
	printf("MESSAGE \n");
	printf("name:%15s\n", p->data.name);
	printf("city:    %10s\n", p->data.city);
	printf("province:%10s\n", p->data.province);
	printf("state:   %10s\n", p->data.state);
	printf("telphone:%15s\n", p->data.tel);
}

/****************************************************************************
 *  Function Name : search
 *  Description   : The Function 用于查询的模块.
 *  Input(s)      : l 保存通讯录数据的链表
 *                :
 *  Output(s)     : 输出需要查询的数据信息.
 *  Returns       : NULL
 ****************************************************************************/
void search(link l)
{
	char name[20];
	Node * p;
	p = l->next;    /* p 指向第一个数据节点 */
	printf("enter name to find: ");
	scanf("%s", name);    /* 输入要查找的姓名 */
	while(p)
	{
		if(strcmp(p->data.name, name) == 0)    /* 通过p遍历链表，匹配与输入姓名对应的数据节点 */
		{
			display(p);    /* 调用显示函数显示查找到的数据信息 */
			getchar();
			getchar();
			break;        /* 使用break则只匹配第一处合适的姓名，去掉可以匹配多处 */
		}
		else
			p = p->next;
	}
}

/****************************************************************************
 *  Function Name : list
 *  Description   : The Function 显示模块，用于逐个显示所有已保存的通讯录信息.
 *  Input(s)      : l 保存通讯录数据的链表
 *                :
 *  Output(s)     : 逐个输出显示通讯录信息.
 *  Returns       : NULL
 ****************************************************************************/
void list(link l)
{
	Node * p;
	p = l->next;
	while(p != NULL)     /* 通过p遍历整个链表l */
	{
		display(p);    /* 逐个显示节点信息 */
		p = p->next;
	}
	getchar();
	getchar();
}

/****************************************************************************
 *  Function Name : save
 *  Description   : The Function 用于保存通讯录数据到文件中.
 *  Input(s)      : l 保存通讯录数据的链表
 *                :
 *  Output(s)     : 将链表保存通讯录数据，存储到文件中.
 *  Returns       : NULL
 ****************************************************************************/
void save(link l)
{
	Node *p;
	FILE *fp;
	p = l->next;
	if((fp = fopen("adresslist", "wb")) == NULL)
	{
		printf("can not open file\n");
		exit(1);
	}
	printf("\nSaving file...\n");
	/* 将节点信息逐个写入磁盘文件中 */
	while(p)
	{
		fwrite(p, sizeof(Node), 1, fp);
		p = p->next;
	}
	fclose(fp);
	printf("Save successfully! \n");
	getchar();
	getchar();
}

/****************************************************************************
 *  Function Name : load
 *  Description   : The Function 加载模块，用于将磁盘文件中存储的信息读入到链表中.
 *  Input(s)      : l 要保存通讯录数据的链表
 *                :
 *  Output(s)     : 将文件中保存的信息输出到链表中.
 *  Returns       : NULL
 ****************************************************************************/
void load(link l)
{
	Node *p, *r;
	FILE *fp;
	l->next = NULL;
	r = l;
	if((fp = fopen("adresslist", "rb")) == NULL)
	{
		printf("can not open file\n");
		exit(1);
	};
	printf("\nLoading file...\n");
	/* 逐个读出文件中的节点信息 */
	while(!feof(fp))
	{
		p = (Node*)malloc(sizeof(Node));    /* 申请节点空间，构造新节点 */
		if(!p)
		{
			printf("memory malloc fail!");
			return;
		}
		if(fread(p, sizeof(Node), 1, fp) != 1)    /* 读记录到节点p中 */
			break;        /* 失败则跳出 */
		else
		{
			/* 尾插法建立链表 */
			p->next = NULL;
			r->next = p;
			r = p;
		}
	}
	fclose(fp);
	printf("Load file successfully! \n");
	sleep(2);
	getchar();
}

/****************************************************************************
 *  Function Name : menu_select
 *  Description   : The Function 显示各个功能的菜单，并接收输入的选项，返回给主函数.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     ：i 用户输入的选项.
 *  Returns       : 0
 ****************************************************************************/
int menu_select()
{
	int i;
	printf("\n\n\t*************************ADDRESS LIST*************************\n");
	printf("\t|*    1.input record                                        *|\n");
	printf("\t|*    2.delete receivedord                                  *|\n");
	printf("\t|*    3.list record                                         *|\n");
	printf("\t|*    4.search record                                       *|\n");
	printf("\t|*    5.save record                                         *|\n");
	printf("\t|*    6.load record                                         *|\n");
	printf("\t|*    7.Quit                                                *|\n");
	printf("\t**************************************************************\n");
	do
	{
		printf("\n\tEnter your choice:");
		scanf("%d", &i);
	}while((i < 1) || (i > 7));    /* 有效范围限制 */
	return i;
}

