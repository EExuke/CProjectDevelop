/**************************************************************************** **
 * Copyright (C) 2010-2011 Cameo Communications, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : train.c
 *     FIRST CREATION DATE    : 2020/01/31
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : EExuke
 *     Last Change            : 2020/01/31
 *     FILE DESCRIPTION       :
** ************************************************************************** */

#include "../inc/train.h"

/****************************************************************************
 *  Function Name : main
 *  Description   : The Main Function 调用各功能函数.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : 0
 ****************************************************************************/
int main()
{
	FILE*fp1, *fp2;
	Node *p, *r;
	char ch1, ch2;
	Link l;
	bookLink k;
	book *t, *h;
	int sel;

	l = (Node*)malloc(sizeof(Node));
	l->next = NULL;
	r = l;
	k = (book*)malloc(sizeof(book));
	k->next = NULL ;
	h = k;
	fp1 = fopen("train.txt", "ab+");    /*打开存储火车信息的文件*/
	if ((fp1 == NULL))
	{
		printf("can't open the file!");
		return 0;
	}
	while(!feof(fp1))
	{
		p = (Node*)malloc(sizeof(Node));
		if (fread(p, sizeof(Node), 1, fp1) == 1)/* 从指定磁盘文件读取记录 */
		{
			p->next = NULL;
			r->next = p;/* 尾插法构造链表 */
			r = p;
		}
	}
	fclose(fp1);
	/* 订票人信息 */
	fp2 = fopen("man.txt", "ab+");
	if ((fp2 == NULL))
	{
		printf("can't open the file!");
		return 0;
	}

	while(!feof(fp2))
	{
		t = (book*)malloc(sizeof(book));
		if (fread(t, sizeof(book), 1, fp2) == 1)
		{
			t->next = NULL;
			h->next = t;
			h = t;
		}
	}
	fclose(fp2);

	/* 进入程序界面 */
	while(1)
	{
		system("clear");    /* 清屏幕 */
		menu();    /* 显示选择菜单 */
		printf("\tplease choose (0~6):  ");
		scanf("%d", &sel);
		system("clear");
		if (sel == 0)
		{
			if (saveflag == 1)    /* 退出时确认信息是否保存 */
			{
				getchar();
				printf("\nthe file have been changed!do you want to save it(y/n)?\n");
				scanf("%c", &ch1);
				if ((ch1 == 'y') || (ch1 == 'Y'))
				{
					SaveBookInfo(k);
					SaveTrainInfo(l);
				}
			}
			printf("\nThank you!!You are welcome too\n");
			break;
		}
		switch(sel)    /* 根据不同的sel来选择相应的操作 */
		{
			case 1 :
				Traininfo(l);
				break;
			case 2 :
				searchtrain(l);
				break;
			case 3 :
				Bookticket(l,k);
				break;
			case 4 :
				Modify(l);
				break;
			case 5 :
				showtrain(l);
				break;
			case 6 :
				SaveTrainInfo(l);
				SaveBookInfo(k);
				break;
			case 0:
				return 0;
		}
		printf("\nplease press any key to continue.......");
		getchar();    /* 去除上一次键盘Enter输入的 \n 字符 */
		getchar();    /* 阻塞等待按键 */
	}
    return 0;
}

/****************************************************************************
 *  Function Name : menu
 *  Description   : The Function 显示选择菜单.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void menu()
{
	puts("\n\n");
	puts("\t\t|------------------------------------------------------|");
	puts("\t\t|                   Booking Tickets                    |");
	puts("\t\t|------------------------------------------------------|");
	puts("\t\t|       0: quit the system                             |");
	puts("\t\t|       1: Insert a train information                  |");
	puts("\t\t|       2: Search a train information                  |");
	puts("\t\t|       3: Book a train ticket                         |");
	puts("\t\t|       4: Modify the train information                |");
	puts("\t\t|       5: Show the train information                  |");
	puts("\t\t|       6: save information to file                    |");
	puts("\t\t|------------------------------------------------------|");
}

/****************************************************************************
 *  Function Name : Traininfo
 *  Description   : The Function 用来添加火车票信息.
 *  Input(s)      : linkhead 储存火车信息的链表头
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void Traininfo(Link linkhead)
{
	struct node *p,*r,*s;
	char num[10];
	r = linkhead;
	s = linkhead->next;
	while(r->next!=NULL)
		r=r->next;
	/* 进入死循环 */
	while(1)
	{
		printf("please input the number of the train( 0-return )");
		scanf("%s", num);
		if(strcmp(num, "0") == 0)
			break;
		/* 判断车次是否已存在 */
		while(s)
		{
			/* 车次已存在 */
			if (strcmp(s->data.num, num) == 0)
			{
				printf("the train '%s'is existing!\n",num);
				return;
			}
			s = s->next;
		}
		p = (struct node*)malloc(sizeof(struct node));
		strcpy(p->data.num, num);    /* 复制车号 */
		printf("Input the city where the train will start:");
		scanf("%s", p->data.startcity);    /* 输入出发城市 */
		printf("Input the city where the train will reach:");
		scanf("%s", p->data.reachcity);    /* 输入到站城市 */
		printf("Input the time which the train take off:");
		scanf("%s", p->data.takeofftime);    /* 输入发车时间 */
		printf("Input the time which the train receive:");
		scanf("%s", &p->data.receivetime);    /* 输入到站时间 */
		printf("Input the price of ticket:");
		scanf("%d", &p->data.price);    /* 输入火车票价 */
		printf("Input the number of booked tickets:");
		scanf("%d", &p->data.ticketnum);    /* 输入火车票数 */
		p->next = NULL;
		r->next = p;    /* 尾插法插入火车票信息链表 */
		r = p;
		saveflag = 1;    /* 保存标志 */
	}
}

/****************************************************************************
 *  Function Name : printheader
 *  Description   : The Function 打印表格抬头.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void printheader()     /* 打印表格抬头 */
{
	printf(HEADER1);
	printf(HEADER2);
	printf(HEADER3);
}

/****************************************************************************
 *  Function Name : printdata
 *  Description   : The Function 打印数据信息.
 *  Input(s)      : argc - The numbers of input value.
 *                : argv - The pointer to input specific parameters.
 *  Output(s)     : NULL
 *  Returns       : 0
 ****************************************************************************/
void printdata(Node *q)     /* 打印信息 */
{
	Node * p;
	p = q;
	printf(FORMAT, DATA);
}

/****************************************************************************
 *  Function Name : searchtrain
 *  Description   : The Function 查询模块，用于查询火车票信息.
 *  Input(s)      : l 已保存的信息链表
 *                :
 *  Output(s)     : 打印输出要查询的火车票信息.
 *  Returns       : NULL
 ****************************************************************************/
void searchtrain(Link l)

{
	Node *s[10], *r;
	int sel, k;
	int i = 0;
	char str1[5], str2[10];
	if(!l->next)
	{
		printf("There is not any record !");
		return;
	}
	printf("Choose the way:\n1:according to the number of train;\n2:according to the city:\n");
	scanf("%d", &sel);    /* 输入选择的序号 */
	if(sel == 1)
	{
		printf("Input the the number of train:");
		scanf("%s", str1);
		r = l->next;
		/* 用r遍历链表 */
		while(r != NULL)
		{
			if (strcmp(r->data.num,str1) == 0)    /* 检索与输入车次相匹配的火车 */
			{
				s[i] = r;
				i++;
				break;
			}
			else
				r = r->next;
		}
	}
	else if(sel == 2)    /* 根据城市查询 */
	{
		printf("Input the city  you want to go:");
		scanf("%s",str2);
		r = l->next;
		while(r != NULL)
		{
			if(strcmp(r->data.reachcity, str2) == 0)    /* 检索与输入到达城市相匹配的火车 */
			{
				s[i] = r;    /* 匹配成功放入数组记录 */
				i++;
				r = r->next;
			}
			else
				r = r->next;
		}
	}
	if(i == 0)
		printf("can not find!");
	else
	{
		printheader();    /* 输出表头 */
		for(k = 0; k < i; k++)
			printdata(s[k]);    /* 输出火车信息 */
	}
}

/****************************************************************************
 *  Function Name : Bookticket
 *  Description   : The Function 为订票模块，根据输入的城市查询，显示满足条件的火车票信息，
 *                ：从中选择想要的车票， 并根据提示输入个人信息.
 *  Input(s)      : l 保存车票信息的链表
 *                : k 保存订票信息的链表
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void Bookticket(Link l, bookLink k)
{
	Node *r[10], *p;
	char ch[2], tnum[10], str[10], str1[10], str2[10];
	book *q, *h;
	int i=0, t=0, flag=0, dnum;
	q = k;
	while(q->next != NULL)
		q = q->next;
	printf("Input the city you want to go: ");
	scanf("%s", &str);    /* 输入要到达的城市 */
	p = l->next;    /* p指向传入的参数指针l的下一个节点 */
	/* 遍历链表 */
	while(p != NULL)
	{
		if(strcmp(p->data.reachcity, str) == 0)
		{
			r[i]=p;    /* 满足条件的保存到数组r中 */
			i++;
		}
		p = p->next;
	}
	printf("\n\nthe number of record have %d\n", i);
	printheader();
	for(t = 0; t < i; t++)
		printdata(r[t]);
	if(i == 0)
		printf("\nSorry!Can't find the train for you!\n");
	else
	{
		printf("\ndo you want to book it?<y/n>\n");
		scanf("%s", ch);
		if(strcmp(ch, "Y") == 0 || strcmp(ch, "y") == 0)    /* 判断是否订票 */
		{
			h = (book*)malloc(sizeof(book));
			printf("Input your name: ");
			scanf("%s", &str1);
			strcpy(h->data.name, str1);
			printf("Input your id: ");
			scanf("%s", &str2);
			strcpy(h->data.num, str2);
			printf("please input the number of the train:");    /* 输入要预定的车次 */
			scanf("%s", tnum);
			/* 遍历检查是否存在该车次 */
			for(t=0; t<i; t++)
			{
				if(strcmp(r[t]->data.num, tnum) == 0)
				{
					if(r[t]->data.ticketnum < 1)    /* 检查可预定的车票数是否为0 */
					{
						printf("sorry, no ticket!");
						sleep(2);
						return;
					}
					printf("remain %d tickets\n", r[t]->data.ticketnum);
					flag=1;
					break;
				}
			}
			if(flag == 0)
			{
				printf("input error");
				sleep(2);
				return;
			}
			printf("Input your bookNum: ");
			scanf("%d", &dnum);    /* 输入要预定的票数 */
			r[t]->data.ticketnum = r[t]->data.ticketnum - dnum;    /* 剩余票数减少 */
			h->data.bookNum = dnum;    /* 将订票数赋值给订票人信息节点 */
			h->next = NULL;
			q->next = h;    /* 尾插法插入订票信息链表 */
			q = h;
			printf("\nLucky!you have booked the ticket!");
			getch();
			saveflag = 1;
		}
	}
}

/****************************************************************************
 *  Function Name : Modify
 *  Description   : The Function 修改模块，用于修改已存在的车票信息.
 *  Input(s)      : l 保存车票信息的链表
 *                :
 *  Output(s)     : NULL
 *  Returns       : 0
 ****************************************************************************/
void Modify(Link l)
{
	Node *p;
	char tnum[10], ch;
	p = l->next;
	if(!p)
	{
		printf("\nthere isn't record for you to modify!\n");
		return;
	}
	else
	{
		printf("\nDo you want to modify it?(y/n)\n");
		getchar();
		scanf("%c", &ch);
		if(ch=='y'|| ch=='Y')
		{
			printf("\nInput the number of the train:");
			scanf("%s", tnum);
			/* 按输入的车号查找车次信息节点 */
			while(p != NULL)
			{
				if(strcmp(p->data.num, tnum) == 0)    /* 查找到存在，则跳出查找 */
					break;
				else
					p = p->next;
			}
			if(p)
			{
				printf("Input new number of train:");
				scanf("%s", &p->data.num);
				printf("Input new city the train will start:");
				scanf("%s", &p->data.startcity);
				printf("Input new city the train will reach:");
				scanf("%s", &p->data.reachcity);
				printf("Input new time the train take off");
				scanf("%s", &p->data.takeofftime);
				printf("Input new time the train reach:");
				scanf("%s", &p->data.receivetime);
				printf("Input new price of the ticket::");
				scanf("%d", &p->data.price);
				printf("Input new number of people who have booked ticket:");
				scanf("%d", &p->data.ticketnum);
				printf("\nmodifying record is sucessful!\n");
				saveflag = 1 ;
			}
			else
				printf("\tcan't find the record! \n");
		}
	}
}

/****************************************************************************
 *  Function Name : showtrain
 *  Description   : The Function 显示所有可预定的列车信息.
 *  Input(s)      : l 保存车票信息的链表
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void showtrain(Link l)
{
	Node *p;
	p = l->next;
	printheader();    /* 打印表格抬头格式 */
	if(l->next == NULL)
		printf("no records!");
	else
	{
		/* 循环打印所有车票信息 */
		while(p != NULL)
		{
			printdata(p);
			p = p->next;
		}
	}
}

/****************************************************************************
 *  Function Name : SaveTrainInfo
 *  Description   : The Function 保存模块1，保存车票信息.
 *  Input(s)      : l 保存车票信息的链表
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void SaveTrainInfo(Link l)
{
	FILE * fp;
	Node * p;
	int count = 0, flag = 1;
	fp = fopen("train.txt", "wb");
	if (fp == NULL)
	{
		printf("the file can't be opened!");
		return;
	}
	p = l->next;
	while(p)
	{
		if (fwrite(p, sizeof(Node), 1, fp) == 1)
		{
			p = p->next;
			count++;
		}
		else
		{
			flag = 0;
			break;
		}
	}
	if(flag)
	{
		printf(" saved %d train records\n", count);
		saveflag = 0;
	}
	fclose(fp);
}

/****************************************************************************
 *  Function Name : SaveBookInfo
 *  Description   : The Function 保存模块2, 保存订票信息.
 *  Input(s)      : k 保存订票信息的链表
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void SaveBookInfo(bookLink k)
{
	FILE * fp;
	book * p;
	int count = 0, flag = 1;
	fp = fopen("man.txt", "wb");
	if(fp == NULL)
	{
		printf("the file can't be opened!");
		return;
	}
	p = k->next;
	while(p)
	{
		if(fwrite(p, sizeof(book),1,fp) == 1)
		{
			p = p->next;
			count++;
		}
		else
		{
			flag = 0;
			break;
		}
	}
	if(flag)
	{
		printf(" saved %d booking records\n", count);
		saveflag = 0;
	}
	fclose(fp);
}

