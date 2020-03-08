/**************************************************************************** **
 * Copyright (C) 2010-2011 Cameo Communications, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : staff.c
 *     FIRST CREATION DATE    : 2020/02/16
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : EExuke
 *     Last Change            : 2020/02/16
 *     FILE DESCRIPTION       :
** ************************************************************************** */

#include "../inc/staff.h"

/****************************************************************************
 *  Function Name : main
 *  Description   : The Main Functioni 完成各子函数模块的调用.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : 0
 ****************************************************************************/
int main()
{
	system("clear");
	system("setterm --background yellow");
	emp_first = emp_end = NULL;    //链表指针初始化
	gsave = gfirst = 0;            //判断标识赋值为0,表示数据为空

	checkfirst();                  //初始化检测函数
	login();                       //登录函数
	readdata();                    //从文件中读取数据初始化链表
	menu();                        //显示功能菜单
	return 0;
}

/****************************************************************************
 *  Function Name : addemp
 *  Description   : The Function 添加员工信息的函数.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void addemp(void)
{
	FILE *fp;               //声明文件型指针
	EMP *emp1;              //声明结构体指针
	int i = 0;
	char choice = 'y';

	if ((fp = fopen("employee.dat", "ab")) == NULL)       //判断信息文件中是否有信息
	{
		printf("打开文件 employee.dat 错误！\n");
		getchar();
		return;
	}

	do{
		i++;
		emp1 = (EMP *)malloc(sizeof(EMP));           //申请内存
		if(emp1 == NULL)                             //判断内存申请是否成功
		{
			printf("内存分配失败，按任意键退出！\n");
			getchar();
			return;
		}
		printf("请输入第%d个员工的信息 \n", i);
		bound('_', 30);
		printf("工号：");
		scanf("%d", &emp1->num);

		printf("职务：");
		scanf("%s", &emp1->duty);

		printf("姓名：");
		scanf("%s", &emp1->name);

		printf("性别：");
		scanf("%s", &emp1->sex);

		printf("年龄：");
		scanf("%d", &emp1->age);

		printf("文化程度：");
		scanf("%s", &emp1->edu);

		printf("薪资：");
		scanf("%d", &emp1->salary);

		printf("办公电话：");
		scanf("%s", &emp1->tel_office);

		printf("家庭电话：");
		scanf("%s", &emp1->tel_home);

		printf("移动电话：");
		scanf("%s", &emp1->mobile);

		printf("QQ：");
		scanf("%s", &emp1->qq);

		printf("地址：");
		scanf("%s", &emp1->address);

		emp1->next = NULL;
		/* 尾插法 */
		if(emp_first == NULL)            //判断链表头指针是否为空
		{
			emp_first = emp1;
			emp_end = emp1;
		}
		else
		{
			emp_end->next = emp1;
			emp_end = emp1;
		}

		fwrite(emp_end, sizeof(EMP), 1, fp);    //对数据流添加数据项，即写入到数据文件

		gfirst = 1;
		printf("\n");
		bound('_', 30);
		printf("\n是否继续输入?(y/n)");
		fflush(stdin);                     //清除缓冲区
		choice = getchar();

		if(toupper(choice) != 'Y')         //把小写转化为大写字母
		{
			fclose(fp);                    //关闭文件流
			printf("\n输入完毕，请按任意键返回\n");
			getchar();
			return;
		}
		system("clear");
	}while(1);
}

/****************************************************************************
 *  Function Name : findemp
 *  Description   : The Function 查找员工信息的函数.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void findemp(void)
{
	int choice, ret = 0, num;
	char str[13];
	EMP *emp1;

	system("clear");

	getchar();
	do{
		printf("\t查询员工信息\n");
		bound('_', 30);
		printf("\t1.按姓名查询\n");
		printf("\t2.按工号查询\n");
		printf("\t3.按电话查询\n");
		printf("\t4.按QQ号查询\n");
		printf("\t0.返回主菜单\n");
		bound('_', 30);
		printf("\n请选择菜单：");

		do{
			choice = getchar();
			switch(choice)
			{
				case '1':
					printf("\n请输入要查询的员工姓名：");
					scanf("%s", str);

					emp1 = findname(str);
					displayemp(emp1, "姓名", str);
					getchar();
					getchar();
					break;

				case '2':
					printf("\n请输入要查询的员工的工号：");
					scanf("%d", &num);

					emp1 = findnum(num);
					sprintf(str, "%d", num);    //数字转化为字符串，num转化为str
					displayemp(emp1, "工号", str);
					getchar();
					getchar();
					break;

				case '3':
					printf("\n请输入要查询的员工的电话：");
					scanf("%s", str);

					emp1 = findtelephone(str);
					displayemp(emp1, "电话", str);
					getchar();
					getchar();
					break;

				case '4':
					printf("\n请输入要查询的员工的QQ号：");
					scanf("%s", str);

					emp1 = findqq(str);
					displayemp(emp1, "QQ号码", str);
					getchar();
					getchar();
					break;

				case '0':
					ret = 1;
					break;
			}
		}while(choice < '0' || choice > '4');

		system("clear");
		if(ret)
			break;
	}while(1);
}

/****************************************************************************
 *  Function Name : listemp
 *  Description   : The Function 显示员工信息列表的函数.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void listemp(void)
{
	EMP *emp1;
	printf("\n所有员工的信息列表如下：\n");
	bound('_', 40);
	emp1=emp_first;
	while(emp1)
	{
		printf("工号：%d\n", emp1->num);
		printf("职务：%s\n", emp1->duty);
		printf("姓名：%s\n", emp1->name);
		printf("性别：%s\n", emp1->sex);
		printf("年龄：%d\n", emp1->age);
		printf("文化程度：%s\n", emp1->edu);
		printf("薪资：%d\n", emp1->salary);
		printf("办公电话：%s\n", emp1->tel_office);
		printf("家庭电话：%s\n", emp1->tel_home);
		printf("移动电话：%s\n", emp1->mobile);
		printf("QQ号码：%s\n", emp1->qq);
		printf("住址：%s\n", emp1->address);
		bound('_', 40);
		emp1 = emp1->next;
	}
	printf("\n按任意键返回！\n");
	getchar();
	getchar();
	return;
}

/****************************************************************************
 *  Function Name : modifyemp
 *  Description   : The Function 修改员工信息的函数.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void modifyemp(void)
{
	EMP *emp1;
	char name[10], *newcontent;
	int choice;

	printf("\n请输入您要修改的员工的姓名：");
	scanf("%s", &name);

	emp1 = findname(name);
	displayemp(emp1, "姓名", name);

	if(emp1)
	{
		printf("\n 请输入你要修改的内容选项：\n");
		bound('_', 40);
		printf("1.修改职务                2.修改年龄\n");
		printf("3.修改文化程度            4.修改工资\n");
		printf("5.修改办公电话            6.修改家庭电话\n");
		printf("7.修改移动电话            8.修改QQ号码\n");
		printf("9.修改住址                0.返回\n  ");
		bound('_', 40);

		do{
			getchar();
			fflush(stdin);
			choice = getchar();
			switch(choice)         //根据操作数选择函数执行
			{
				case '1':
					newcontent = modi_field("职务", emp1->duty, 10);    //调用修改函数修改基本信息
					if(newcontent != NULL)
					{
						strcpy(emp1->duty, newcontent);
						free(newcontent);
					}
					break;
				case '2':
					emp1->age = modi_age(emp1->age);
					break;
				case '3':
					newcontent = modi_field("文化程度", emp1->edu, 10);
					if(newcontent != NULL)
					{
						strcpy(emp1->edu, newcontent);         //获取新内容
						free(newcontent);
					}
					break;
				case '4':
					emp1->salary = modi_salary(emp1->salary);
					break;
				case '5':
					newcontent = modi_field("办公电话", emp1->tel_office, 13);
					if(newcontent != NULL)
					{
						strcpy(emp1->tel_office, newcontent);
						free(newcontent);
					}
					break;
				case '6':
					newcontent = modi_field("家庭电话", emp1->tel_home, 13);
					if(newcontent != NULL)
					{
						strcpy(emp1->tel_home, newcontent);
						free(newcontent);
					}
					break;
				case '7':
					newcontent = modi_field("移动电话", emp1->mobile, 12);
					if(newcontent != NULL)
					{
						strcpy(emp1->mobile, newcontent);
						free(newcontent);
					}
					break;
				case '8':
					newcontent = modi_field("QQ号码", emp1->qq, 10);
					if(newcontent == NULL)
					{
						strcpy(emp1->qq, newcontent);
						free(newcontent);
					}
					break;
				case '9':
					newcontent = modi_field("住址", emp1->address, 30);
					if(newcontent != NULL)
					{
						strcpy(emp1->address, newcontent);
						free(newcontent);                 //释放内存空间
					}
					break;
				case '0':
					return;
			}
		}while(choice<'0' || choice>'9');

		gsave = 1;
		savedata();                                    //保存修改的数据信息
		printf("\n修改完毕，按任意键退出！\n");
		getchar();
		getchar();
	}
	return;
}

/****************************************************************************
 *  Function Name : summaryemp
 *  Description   : The Function 统计员工信息的函数.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : 统计结果
 *  Returns       : NULL
 ****************************************************************************/
void summaryemp(void)
{
	EMP *emp1;
	int sum=0, num=0, man=0, woman=0;
	char strw[2];

	emp1 = emp_first;
	while(emp1)
	{
		num++;
		sum += emp1->salary;
		strncpy(strw, emp1->sex, 2);
		if((strcmp(strw, "man")==0) || (strcmp(emp1->sex,"男") == 0))
			man++;
		else
			woman++;
		emp1 = emp1->next;
	}

	printf("\n下面是相关员工的统计信息：\n");
	bound('_', 40);
	printf("员工总数：%d\n", num);
	printf("员工的薪资总数：%d\n", sum);
	printf("男员工数：%d\n", man);
	printf("女员工数：%d\n", woman);
	bound('_', 40);
	printf("按任意键退出！\n");
	getchar();
	getchar();
}

/****************************************************************************
 *  Function Name : delemp
 *  Description   : The Function 删除员工信息的函数.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void delemp(void)
{
	int findok = 0;
	EMP *emp1, *emp2;
	char name[10], choice;
	system("clear");
	printf("\n输入要删除的员工姓名：");
	scanf("%s", name);

	emp1 = emp_first;
	emp2 = emp1;
	while(emp1)
	{
		if(strcmp(emp1->name, name) == 0)
		{
			findok = 1;
			system("clear");

			printf("员工：%s 的信息如下：\n", emp1->name);
			bound('_',40);
			printf("工号：%d\n", emp1->num);
			printf("职务：%s\n", emp1->duty);
			printf("姓名：%s\n", emp1->name);
			printf("性别：%s\n", emp1->sex);
			printf("年龄：%d\n", emp1->age);
			printf("文化程度：%s\n", emp1->edu);
			printf("薪资：%d\n", emp1->salary);
			printf("办公电话：%s\n", emp1->tel_office);
			printf("家庭电话：%s\n", emp1->tel_home);
			printf("移动电话：%s\n", emp1->mobile);
			printf("QQ号码：%s\n", emp1->qq);
			printf("住址：%s\n", emp1->address);
			bound('_', 40);
			printf("您确认要删除该员工信息吗？(y/n)");

			getchar();
			fflush(stdin);          //清除缓冲区
			choice = getchar();

			if(choice != 'y' && choice != 'Y')
			{
				return;
			}
			if(emp1 == emp_first)
			{
				emp_first = emp1->next;
			}
			else
			{
				emp2->next = emp1->next;
			}
			printf("员工：%s 已被删除 \n", emp1->name);
			getchar();
			getchar();
			free(emp1);
			gsave = 1;
			savedata();             //保存数据
			return;
		}
		else                        //继续遍历
		{
			emp2 = emp1;
			emp1 = emp1->next;
		}
	}
	if(!findok)
	{
		bound('_', 40);
		printf("\n未找到姓名是%s的信息！\n", name);
		getchar();
		getchar();
	}
}

/****************************************************************************
 *  Function Name : resetpwd
 *  Description   : The Function 重置系统的函数.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void resetpwd(void)
{
	char pwd[9], pwd1[9], ch;
	int i;
	FILE *fp1;

	system("clear");

	getchar();
	printf("\n请输入旧密码：\n");
	for(i=0; i<8; i++)
	{
		pwd[i] = getchar();
		if (pwd[i] == '\n')
			break;
	}
	pwd[i] = '\0';

	if(strcmp(password, pwd))
	{
		printf("\n密码错误，请按任意键退出！\n");       //比较旧密码，判断用户权限
		getchar();
		return;
	}
	fflush(stdin);
	do
	{
		getchar();
		printf("\n设置新密码，请不要超过8位：");
		for(i=0; i<8; i++)
		{
			pwd[i] = getchar();
			if (pwd[i] == '\n')
				break;
		}
		getchar();
		printf("\n再一次确认密码：");
		for(i=0; i<8; i++)
		{
			pwd1[i] = getchar();
			if (pwd1[i] == '\n')
				break;
		}
		pwd[i] = '\0';
		pwd1[i] = '\0';
		printf("pwd:%s \npwd1:%s\n", pwd, pwd1);
		/* 两次密码比较 */
		if (strcmp(pwd, pwd1) != 0)
			printf("\n两次密码输入不一致，请重新输入！\n");
		else
			break;
	}while(1);

	if((fp1 = fopen("config.bat", "wb")) == NULL)        //打开密码文件
	{
		printf("\n系统创建失败，请按任意键退出！");
		getchar();
		exit(1);
	}

	i = 0;
	while(pwd[i])
	{
		putw(pwd[i], fp1);
		i++;
	}

	fclose(fp1);                                    //关闭文件流
	printf("\n 密码修改成功，按任意键退出！\n");
	getchar();
}

/****************************************************************************
 *  Function Name : readdata
 *  Description   : The Function 读取文件数据.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void readdata(void)
{
	FILE *fp;
	EMP *emp1;

	if((fp=fopen("employee.dat", "rb")) == NULL)
	{
		gfirst = 0;
		return;
	}

	while(!feof(fp))
	{
		emp1 = (EMP *)malloc(sizeof(EMP));
		if(emp1 == NULL)
		{
			printf("内存分配失败！\n");
			getchar();
			return;
		}

		fread(emp1, sizeof(EMP), 1, fp);
		if(feof(fp))
			break;    //读取完成，退出
		/* 尾插法建立链表 */
		if(emp_first == NULL)
		{
			emp_first = emp1;
			emp_end = emp1;
		}
		else
		{
			emp_end->next = emp1;
			emp_end = emp1;
		}
		emp_end->next = NULL;
	}
	if (emp_first != emp_end)
		gfirst = 1;
	else
		gfirst = 0;
	fclose(fp);
}

/****************************************************************************
 *  Function Name : savedata
 *  Description   : The Function 保存数据.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void savedata(void)
{
	FILE *fp;
	EMP *emp1;

	if(gsave == 0)
		return;

	if((fp = fopen("employee.dat", "wb")) == NULL)
	{
		printf("文件打开失败！\n");
		getchar();
		return;
	}

	emp1 = emp_first;
	while(emp1)
	{
		fwrite(emp1, sizeof(EMP), 1, fp);
		emp1 = emp1->next;
	}

	gsave = 0;
	fclose(fp);
}

/****************************************************************************
 *  Function Name : modi_age
 *  Description   : The Function 修改员工年龄.
 *  Input(s)      : age 原来的年龄
 *                :
 *  Output(s)     : NULL
 *  Returns       : newage 新的年龄
 ****************************************************************************/
int modi_age(int age)
{
	int newage;
	printf("原来的年龄：%d", age);
	printf("输入新的年龄：");
	scanf("%d", &newage);
	return(newage);
}

/****************************************************************************
 *  Function Name : modi_salary
 *  Description   : The Function 修改员工薪资.
 *  Input(s)      : salary 原薪资
 *                :
 *  Output(s)     : NULL
 *  Returns       : newsalary 新薪资
 ****************************************************************************/
int modi_salary(int salary)
{
	int newsalary;
	printf("原来的工资为：%d", salary);
	printf("输入新的工资：");
	scanf("%d", &newsalary);
	return(newsalary);
}

/****************************************************************************
 *  Function Name : modi_field
 *  Description   : The Function 修改员工其他信息.
 *  Input(s)      : field 非数值型修改项
 *                : content 修改项的内容
 *                : len 内容长度
 *  Output(s)     : NULL
 *  Returns       : str 修改后的新内容
 ****************************************************************************/
char *modi_field(char *field, char *content, int len)
{
	char *str;
	str = malloc(sizeof(char)*len);
	if(str == NULL)
	{
		printf("内存分配失败，按任意键退出！");
		getchar();
		return NULL;
	}
	printf("原来的%s为%s\n", field, content);
	printf("要修改为(内容不要超过%d个字符)", len);
	scanf("%s", str);
	return str;
}

/****************************************************************************
 *  Function Name : findname
 *  Description   : The Function 按名字查询员工信息.
 *  Input(s)      : name
 *                :
 *  Output(s)     : NULL
 *  Returns       : emp1 查找到的信息节点
 ****************************************************************************/
EMP *findname(char *name)
{
	EMP *emp1;
	emp1 = emp_first;

	while(emp1)
	{
		if (strcmp(name, emp1->name) == 0)      //遍历链表，姓名配时返回节点信息
		{
			return emp1;
		}
		emp1 = emp_end->next;
	}
}

/****************************************************************************
 *  Function Name : findnum
 *  Description   : The Function 按工号查询员工信息.
 *  Input(s)      : num
 *                :
 *  Output(s)     : NULL
 *  Returns       : emp1 查找到的信息节点
 ****************************************************************************/
EMP *findnum(int num)
{
	EMP *emp1;

	emp1 = emp_first;
	while(emp1)
	{
		if (num == emp1->num)
			return emp1;
		emp1 = emp1->next;
	}
}

/****************************************************************************
 *  Function Name : findtelephone
 *  Description   : The Function 按通讯号码查询员工信息.
 *  Input(s)      : num 电话号码
 *                :
 *  Output(s)     : NULL
 *  Returns       : emp1 查找到的信息节点
 ****************************************************************************/
EMP *findtelephone(char *tel)
{
	EMP *emp1;

	emp1 = emp_first;    //指向头节点

	while(emp1)
	{
		if ((strcmp(tel, emp1->tel_office) == 0) ||
				(strcmp(tel, emp1->tel_home)==0) ||
				(strcmp(tel, emp1->mobile)==0))        //使用或逻辑同时判断3个电话号码
			return emp1;
		emp1 = emp1->next;
	}
}

/****************************************************************************
 *  Function Name : findqq
 *  Description   : The Function 按QQ号查询员工信息.
 *  Input(s)      : QQ 要查询的QQ号码
 *                :
 *  Output(s)     : NULL
 *  Returns       : emp1 查找到的信息节点
 ****************************************************************************/
EMP *findqq(char *QQ)
{
	EMP *emp1;

	emp1 = emp_first;
	while(emp1)
	{
		if(strcmp(QQ, emp1->qq)==0)
			return emp1;
		emp1 = emp1->next;
	}
}

/****************************************************************************
 *  Function Name : displayemp
 *  Description   : The Function 显示员工信息.
 *  Input(s)      : emp
 *                : field
 *                : name
 *  Output(s)     : 输出员工信息
 *  Returns       : NULL
 ****************************************************************************/
void displayemp(EMP *emp, char *field, char *name)
{
	if(emp)
	{
		printf("\n%s：%s信息如下：\n", field, name);
		bound('_', 30);
		printf("工号：%d\n", emp->num);
		printf("职务：%s\n", emp->duty);
		printf("姓名：%s\n", emp->name);
		printf("性别：%s\n", emp->sex);
		printf("年龄：%d\n", emp->age);
		printf("文化程度：%s\n", emp->edu);
		printf("薪资：%d\n", emp->salary);
		printf("办公电话：%s\n", emp->tel_office);
		printf("家庭电话：%s\n", emp->tel_home);
		printf("移动电话：%s\n", emp->mobile);
		printf("QQ号码：%s\n", emp->qq);
		printf("住址：%s\n", emp->address);
		bound('_', 30);
	}
	else
	{
		bound('_', 40);
		printf("资料库中没有%s为：%s的员工，请重新确认！", field, name);
	}
}

/****************************************************************************
 *  Function Name : checkfirst
 *  Description   : The Function 初始化检测.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void checkfirst(void)
{
	FILE *fp, *fp1;                                  /* 声明文件指针 */
	char pwd[9], pwd1[9], pwd2[9], pwd3[9], ch;
	int i;
	char strt = '8';

	if ((fp = fopen("config.bat", "rb")) == NULL)     //判断系统密码文件是否为空
	{
		printf("\n欢迎进入新系统，请进行相应的初始化操作！ \n");
		bound('_', 50);
		getchar();

		do{
			printf("\n设置密码(8位)：");
			for(i=0; i<8; i++)
			{
				pwd[i] = getchar();
				if (pwd[i] == '\n')
					break;
			}
			printf("\n再次确认密码：");
			for(i=0; i<8; i++)
			{
				pwd1[i] = getchar();
				if (pwd[i] == '\n')
					break;
			}

			pwd[i] = '\0';
			pwd1[i] = '\0';

			if(strcmp(pwd, pwd1) != 0)             //判断两次新密码是否一致
			{
				printf("\n两次密码输入不一致，请重新输入\n\n");
			}
			else
				break;

		}while(1);

		if ((fp1 = fopen("config.bat", "wb")) == NULL)
		{
			printf("\n系统创建失败，请按任意键退出！");
			getchar();
			exit(1);
		}

		i = 0;
		while(pwd[i])
		{
			putw(pwd[i], fp1);                  //将数组元素送入文件流中
			i++;
		}

		fclose(fp1);                            //关闭文件流
		printf("\n\n系统初始化成功，按任意键退出后，再重新进入！\n");
		getchar();
		exit(1);

	}
	else
	{
		i = 0;
		while(!feof(fp) && i<8)                   //判断是否读完密码文件
			pwd[i++] = (getw(fp));           //从文件流中读出字符赋给数组

		pwd[i] = '\0';

		if(i >= 8)
			i--;
		while(pwd[i] != -1 && i>=0)
			i--;

		pwd[i] = '\0';                            //补尾
		strcpy(password, pwd);                    //复制pwd到password中
	}
}

/****************************************************************************
 *  Function Name : bound
 *  Description   : The Function 画出分界线.
 *  Input(s)      : ch
 *                : n
 *  Output(s)     : 画出分界线
 *  Returns       : NULL
 ****************************************************************************/
void bound(char ch, int n)
{
	while(n--)
		putchar(ch);
	printf("\n");
}

/****************************************************************************
 *  Function Name : login
 *  Description   : The Functioni 登录函数.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : NULL
 *  Returns       : NULL
 ****************************************************************************/
void login()
{
	int i, n = 3;
	char pwd[9];
	do{
		printf("请输入密码：");
		for(i=0; i<8; i++)
		{
			pwd[i] = getchar();
			if (pwd[i] == '\n')
				break;
		}
		pwd[i] = '\0';

		if (strcmp(pwd, password) != 0)
		{
			printf("\n密码错误，请重新输入！\n");
			getchar();
			system("clear");
			n--;
		}
		else
			break;
	} while(n > 0);                       //密码输入三次机会
	if (!n)
	{
		printf("请退出，你已经输入三次错误密码！");
		getchar();
		exit(1);
	}
}

/****************************************************************************
 *  Function Name : menu
 *  Description   : The Function 主菜单列表.
 *  Input(s)      : NULL
 *                :
 *  Output(s)     : 输出主菜单列表
 *  Returns       : NULL
 ****************************************************************************/
void menu()
{
	char choice;
	system("clear");
	fflush(stdin);
	do{
		printf("\n\t\t\t\t 企业员工管理系统\n\n");
		printf("\t\t\t-------------------------------------\n");
		printf("\t\t\t|\t\t\t\t    |\n");
		printf("\t\t\t|  \t1、输入员工信息\t\t    |\n");
		printf("\t\t\t|\t\t\t\t    |\n");
		printf("\t\t\t|  \t2、查询员工信息\t\t    |\n");
		printf("\t\t\t|\t\t\t\t    |\n");
		printf("\t\t\t|  \t3、显示员工信息\t\t    |\n");
		printf("\t\t\t|\t\t\t\t    |\n");
		printf("\t\t\t|  \t4、修改员工信息\t\t    |\n");
		printf("\t\t\t|\t\t\t\t    |\n");
		printf("\t\t\t|  \t5、删除员工信息\t\t    |\n");
		printf("\t\t\t|\t\t\t\t    |\n");
		printf("\t\t\t|  \t6、统计员工信息\t\t    |\n");
		printf("\t\t\t|\t\t\t\t    |\n");
		printf("\t\t\t|  \t7、重置系统密码\t\t    |\n");
		printf("\t\t\t|\t\t\t\t    |\n");
		printf("\t\t\t|  \t0、退出系统\t\t    |\n");
		printf("\t\t\t|\t\t\t\t    |\n");
		printf("\t\t\t-------------------------------------\n");
		printf("\n\t\t\t请选择您需要的操作：");

		do{
			choice = getchar();
			switch(choice)
			{
				case '1':
					addemp();        //调用员工信息添加函数
					break;
				case '2':
					if (!gfirst)
					{
						printf("系统中无员工信息，请先添加员工信息！\n");
						getchar();
						getchar();
						break;
					}
					findemp();       //调用员工信息查找函数
					break;
				case '3':
					if (!gfirst)
					{
						printf("系统中无员工信息，请先添加员工信息！\n");
						getchar();
						getchar();
						break;
					}
					listemp();       //员工列表函数
					break;
				case '4':
					if (!gfirst)
					{
						printf("系统中无员工信息，请先添加员工信息！\n");
						getchar();
						getchar();
						break;
					}
					modifyemp();     //员工信息修改函数
					break;
				case '5':
					if (!gfirst)
					{
						printf("系统中无员工信息，请先添加员工信息！\n");
						getchar();
						getchar();
						break;
					}
					delemp();        //删除员工信息的函数
					break;
				case '6':
					if (!gfirst)
					{
						printf("系统中无员工信息，请先添加员工信息！\n");
						getchar();
						getchar();
						break;
					}
					summaryemp();     //统计员工信息的函数
					break;
				case '7':
					resetpwd();      //重置系统密码函数
					break;
				case '0':
					savedata();      //保存数据的函数
					system("clear");
					exit(0);
				default:
					printf("请输入数字0~7 !");
			}
		}while(choice<'0' || choice>'7');
		system("clear");
	}while(1);
}

