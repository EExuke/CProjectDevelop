/**************************************************************************** **
 * Copyright (C) 2010-2011 Cameo Communications, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : consume.c
 *     FIRST CREATION DATE    : 2020/02/06
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : EExuke
 *     Last Change            : 2020/02/06
 *     FILE DESCRIPTION       :
** ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <string.h>
#include "../inc/consume.h"

/****************************************************************************
 *  Function Name : main
 *  Description   : The Main Function.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void main()
{
	int num;
	score *head = NULL, *stu = NULL;
	menu();
	scanf("%d", &num);
	while(1)
	{
		switch(num)
		{
			case 1:
				head = creat();
				break;
			case 2:
				head = load(head);
				break;
			case 3:
				head = search(head);
				break;
			case 4:
				head = del(head);
				break;
			case 5:
				head = add(head, stu);
				break;
			case 6:
				print(head);
				break;
			case 7:
				save(head);
				break;
			case 0:
				exit(0);
			defaultult:
				printf("Input error,please again!");
		}
		menu();
		scanf("%d", &num);
	}
}

/****************************************************************************
 *  Function Name : creat
 *  Description   : The Function 录入学生消费信息.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : head 保存消费信息的链表头节点
 ****************************************************************************/
score *creat()
{
	score *head;
	score *p1, *p2, *p3, *max;
	int i, j;
	char t[10];
	n = 0;
	p1 = p2 = p3 = (score *)malloc(LEN);    /* 开辟新单元 */
	printf("please input student's information, input 0 exit!\n");
	printf("please input student's number(number>0):");    /* 输入学号，学号应大于0 */
	scanf(" %d", &p1->number);
	while(p1->number < 0)
	{
		getchar();
		printf("error, please input number again:");
		scanf("%d", &p1->number);
	}
	/* 当输入学号为0时，跳转到末尾，结束创建链表 */
	if(p1->number == 0)
		goto end;
	/* 当学号已存在时，返回重新输入，（遍历查重） */
	else
	{
		printf("please input student's name:");
		scanf("%s", p1->name);
		printf("please input student's consume money:");
		scanf("%d", &p1->xiaofei);
	}
	/* 继续判断学号是否为0,若不为0,则节点+1,依次创建信息节点,p1指向新创建的节点 */
	while(p1->number != 0)
	{
		n = n + 1;
		if(n == 1)
			head = p1;
		else
			p2->next = p1;    /* p2指向尾节点，尾插法建立链表 */
		p2 = p1;
		p1 = (score *)malloc(LEN);
		printf("please input student's information,input 0 exit!\n");
		repeat2:
		printf("please input student's number(number>0):");
		scanf("%d", &p1->number);
		/* 输入学号不得小于0 */
		while(p1->number < 0)
		{
			getchar();
			printf("error,please input number again:");
			scanf("%d", &p1->number);
		}
		/* 当输入学号为0时，跳转到末尾，结束创建链表 */
		if(p1->number == 0)
			goto end;
		/* p3遍历查重 */
		else
		{
			p3 = head;
			if(n > 0)
			{
				for(i=0; i<n; i++)
				{
					if(p1->number != p3->number)
						p3 = p3->next;
					else
					{
						printf("number repeate,please input again!\n");
						goto repeat2;
					}
				}
			}
			printf("please input student's name:");
			scanf("%s", p1->name);
			printf("please input student's consume money:");
			scanf("%d", &p1->xiaofei);
		}
	}
	/* 最后将信息按照学号大小顺序排序，并返回头指针 */
end:
	p1 = head;
	p3 = p1;
	for(i=1; i<n; i++)
	{
		for(j = i+1; j <= n; j++)
		{
			max = p1;
			p1 = p1->next;
			/* 简单的冒泡交换排序，按内容交换，（可改进为节点交换） */
			if(max->number > p1->number)
			{
				/* 交换学号 */
				k = max->number;
				max->number = p1->number;
				p1->number = k;
				/* 交换姓名 */
				strcpy(t, max->name);
				strcpy(max->name, p1->name);
				strcpy(p1->name, t);
				/* 交换消费信息 */
				k = max->xiaofei;
				max->xiaofei = p1->xiaofei;
				p1->xiaofei = k;
			}
		}
		/* 两指针重新指向头节点，准备下一趟冒泡 */
		max = head;
		p1 = head;
	}
	p2->next = NULL;    /* 链表补尾 */
	printf("input student's num: %d \n", n);
	getchar();
	getchar();
	return(head);
}

/****************************************************************************
 *  Function Name : load
 *  Description   : The Function 加载文件信息.
 *  Input(s)      : head 要保存消费信息的链表头节点
 *                :
 *  Output(s)     : NULL
 *  Returns       : head 加载信息后的信息链表头
 ****************************************************************************/
score *load(score *head)
{
	score *p1, *p2;
	int m = 0;
	char filepn[10];
	FILE *fp;

	printf("please input file's postion and its name:");
	scanf("%s", filepn);    /* 输入文件路径及名字 */
	if((fp = fopen(filepn, "r+")) == NULL)
	{
		printf("can't open this file!\n");
		getchar();
		return 0;
	}
	else
	{
		p1 = (score *)malloc(LEN);    /* 申请节点空间 */
		fscanf(fp, "%d,%s,%d\n", &p1->number, p1->name, &p1->xiaofei);    /* 读取信息 */
		printf("|%d\t|%s\t|%d\t\n", p1->number, p1->name, p1->xiaofei);    /* 显示读取到的节点信息 */
		head = NULL;
		/* 读取信息到节点，并建立链表 */
		while(!feof(fp))
		{
			n = n+1;
			if(n == 1)
				head = p1;
			else
				p2->next = p1;    /* p2始终指向尾节点 */
			p2 = p1;
			p1 = (score *)malloc(LEN);    /* p1创建新节点 */
			fscanf(fp, "%d,%s,%d\n", &p1->number, p1->name, &p1->xiaofei);
			printf("|%d\t|%s\t|%d\t\n", p1->number, p1->name, p1->xiaofei);
		}
		free(p1);
		p2->next = NULL;
	}
	printf("-----------------------------------------\n");
	getchar();
	getchar();
	fclose(fp);    /* 关闭文件 */
	return (head);
}

/****************************************************************************
 *  Function Name : search
 *  Description   : The Function 根据学号查询学生消费信息.
 *  Input(s)      : head 保存消费信息的链表头节点
 *                :
 *  Output(s)     : 查询到的节点信息
 *  Returns       : head 保存消费信息的链表头节点
 ****************************************************************************/
score *search(score *head)
{
	int number;
	score *p1, *p2;
	printf("input the student's number of searching:");
	scanf("%d", &number);
	while(number != 0)
	{
		if(head == NULL)
		{
			printf("\n nobody information!\n");
			return(head);
		}
		printf("-----------------------------------------\n");
		printf("|number\t|name\t|consume\t \n");
		printf("-----------------------------------------\n");
		p1 = head;
		while(number != p1->number && p1->next != NULL)
		{
			p2 = p1;        /* p2始终指向p1前一个节点 */
			p1 = p1->next;
		}
		if(number == p1->number)
		{
			printf("|%d\t|%s\t|%d\t\n", p1->number, p1->name, p1->xiaofei);
			printf("-----------------------------------------\n");
		}
		else
			printf("%dthis student not exist!\n", number);
		printf("input the student's number of searching:");
		scanf("%d", &number);
		getchar();
		getchar();
	}
	printf("already exit!\n");
	getchar();
	return(head);
}

/****************************************************************************
 *  Function Name : del
 *  Description   : The Function 删除学生消费信息.
 *  Input(s)      : head 保存消费信息的链表头节点
 *                :
 *  Output(s)     : 删除是否成功
 *  Returns       : head 删除后消费信息的链表头节点
 ****************************************************************************/
score *del(score *head)
{
	score *p1, *p2;
	int number;
	printf("input the student's number of deleting(input 0 exit):");
	scanf("%d", &number);
	/* 输入为0时退出 */
	while(number != 0)
	{

		if(head == NULL)
		{
			printf("\nnobody information!\n");
			return(head);
		}
		p1 = head;
		/* p1查找要删除的信息节点 */
		while(number != p1->number && p1->next != NULL)
		{
			p2 = p1;        /* p2始终指向p1前一个节点 */
			p1 = p1->next;
		}
		/* 找到要删除的信息节点 */
		if(number == p1->number)
		{
			if(p1 == head)    /* 删除首节点的情况 */
				head = p1->next;
			else
				p2->next = p1->next;
			free(p1);    /* 释放空间 */
			printf("delete number:%d \n", number);
			n = n - 1;    /* 节点总个数减1 */
		}
		else
			printf("%d student not exist! \n",number);
		printf("input the student's number of deletinging:");
		scanf("%d", &number);
	}
#ifdef DEBUG
	printf("already exit \n");
#endif
	printf("now students number is: %d \n", n);
	getchar();
	getchar();
	return(head);
}

/****************************************************************************
 *  Function Name : add
 *  Description   : The Function 添加消费信息节点.
 *  Input(s)      : head 保存消费信息的链表头节点
 *                : stu 新添加的信息节点
 *  Output(s)     : NULL
 *  Returns       : head 添加信息节点后的链表头节点
 ****************************************************************************/
score *add(score *head, score *stu)

{
	score *p0, *p1, *p2, *p3, *max;
	int i, j;
	char t[10];
	p3 = stu = (score *)malloc(LEN);    /* 创建新节点 */
	printf("\ninput the student's information of adding!");
repeat4:
	printf("please input the student's number(number>0):");
		scanf("%d", &stu->number);
		while(stu->number < 0)
		{
			 getchar();
			 getchar();
			 printf("error,please input number again:");
			 scanf("%d", &stu->number);
		}
		/* 输入0结束 */
		if(stu->number == 0)
			 goto end2;
		/* 检查该学号是否已存在 */
		else
		{
			p3 = head;
			if(n > 0)
			{
				for(i=0; i<n; i++)
				{
					if(stu->number != p3->number)
						 p3 = p3->next;
					else
					{
						 printf("number repeat,please input again!\n");
						 goto repeat4;
					}
				}
			}
		}
		/* 查重通过，输入信息 */
		printf("input the student's name:");
		scanf("%s", stu->name);
		printf("please input the consuming:");
		scanf("%d", &stu->xiaofei);
		p1 = head;
		p0 = stu;
		if(head == NULL)    /* 头节点为空的情况 */
		{
			 head = p0;
			 p0->next = NULL;
		}
		else
		{
			if(p1->next == NULL)    /* 只有头节点的情况 */
			{
				 p1->next = p0;
				 p0->next = NULL;
			}
			else
			{
				/* 一般情况，后移到尾 */
				while(p1->next != NULL)
				{
					 p2 = p1;    /* p2始终指向p1前一个节点 */
					 p1 = p1->next;    /* p1后移 */
				}
				p1->next = p0;    /* 尾插法插入新节点 */
				p0->next = NULL;
			}
		}
		n = n+1;
		p1 = head;
		p0 = stu;
		/* 插入新节点后，重新进行冒泡排序 */
		for(i=1; i<n; i++)
		{
			for(j=i+1; j<=n; j++)
			{
				max = p1;
				p1 = p1->next;
				if(max->number > p1->number)
				{
					/* 交换学号 */
					k = max->number;
					max->number = p1->number;
					p1->number = k;
					/* 交换姓名 */
					strcpy(t, max->name);
					strcpy(max->name, p1->name);
					strcpy(p1->name, t);
					/* 交换消费信息 */
					k = max->xiaofei;
					max->xiaofei = p1->xiaofei;
					p1->xiaofei = k;
				}
				max = head;    /* 回头，为下一趟做准备 */
				p1 = head;
			}
		}
end2:
	printf("now how many students are they: %d \n",n);
	getchar();
	return(head);
}

/****************************************************************************
 *  Function Name : print
 *  Description   : The Function 显示消费信息记录.
 *  Input(s)      : head 保存消费信息的链表头节点
 *                :
 *  Output(s)     : 输出消费信息记录
 *  Returns       : NULL
 ****************************************************************************/
void print(score *head)
{
	score *p;
	if(head == NULL)
		printf("\nnobody information!\n");
	else
	{
		printf("%d\n",n);
		printf("-----------------------------------------\n");
		printf("|number\t|name\t|consume\t |\n");
		printf("-----------------------------------------\n");
		p = head;
		/* 逐个显示每条信息 */
		do
		{
			printf("|%d\t|%s\t|%d\t\n", p->number, p->name, p->xiaofei);
			printf("-----------------------------------------\n");
			p = p->next;
		}while(p != NULL);
		getchar();
		getchar();
	}
}

/****************************************************************************
 *  Function Name : save
 *  Description   : The Function 用于保存链表信息到文件.
 *  Input(s)      : p1 保存消费信息的链表头节点
 *                :
 *  Output(s)     : 保存消费信息到磁盘文件中
 *  Returns       : 0
 ****************************************************************************/
int save(score *p1)
{
	FILE *fp;
	if((fp = fopen("consumingme", "wb")) == NULL)
	{
		printf("can't open this file!\n");
		return 0;
	}
	else
	{
		while(p1 != NULL)
		{
			/* 格式化输出到文件 */
			fprintf(fp,"%d,%s,%d\n", p1->number, p1->name, p1->xiaofei);
			p1 = p1->next;
		}
		printf("file save complete! please enter return!\n");
		getchar();
		getchar();
	}
	fclose(fp);
	return 0;
}

/****************************************************************************
 *  Function Name : menu
 *  Description   : The Function 显示菜单选项.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : 输出显示菜单选项
 *  Returns       : NULL
 ****************************************************************************/
void menu()
{
	system("clear");
	printf("\n\n\n");
	printf("\t\t-------------STUDENT CONSUME-------------\n");
	printf("\t\t\t0  exit                                \n");
	printf("\t\t\t1  creat                               \n");
	printf("\t\t\t2  load                                \n");
	printf("\t\t\t3  search                              \n");
	printf("\t\t\t4  delete                              \n");
	printf("\t\t\t5  add                                 \n");
	printf("\t\t\t6  show                                \n");
	printf("\t\t\t7  save                                \n");
	printf("\t\t-----------------------------------------\n\n");
	printf("\t\tchoose(0-7):");
}

