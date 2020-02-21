/**************************************************************************** **
 * Copyright (C) 2010-2011 Cameo Communications, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : staff.h
 *     FIRST CREATION DATE    : 2020/02/16
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : EExuke
 *     Last Change            : 2020/02/16
 *     FILE DESCRIPTION       : API exported to external moduels
** ************************************************************************** */
#ifndef _STAFF_H_
#define _STAFF_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*------------------Macro Definition-------------------------*/
/* 存储员工信息的结构体 */
typedef struct employee
{
	int num;                   //员工编号
	char duty[10];             //员工职务
	char name[10];             //员工姓名
	char sex[5];               //员工性别
	unsigned char age;         //员工年龄
	char edu[20];              //教育水平
	int salary;                //员工薪资
	char tel_office[13];       //办公电话
	char tel_home[13];         //家庭电话
	char mobile[13];           //手机号码
	char qq[11];               //QQ号码
	char address[31];          //家庭住址
	struct employee *next;
}EMP;

/* 全局变量 */
static char password[9];           //系统密码
EMP *emp_first, *emp_end;   //头指针和尾指针
char gsave, gfirst;         //判断标识

/*------------------End of Macro Definition------------------*/

/*------------------API Definition---------------------------*/
void addemp(void);          //添加员工信息的函数
void findemp(void);         //查找员工信息的函数
void listemp(void);         //显示员工信息列表的函数
void modifyemp(void);       //修改员工信息的函数
void summaryemp(void);      //统计员工信息的函数
void delemp(void);          //删除员工信息的函数
void resetpwd(void);        //重置系统的函数
void readdata(void);        //读取文件数据
void savedata(void);        //保存数据
int modi_age(int s);        //修改员工年龄
int modi_salary(int s);     //修改员工薪资
char *modi_field(char *field, char *s, int n);       //修改员工其他信息
EMP *findname(char *name);                           //按名字查询员工信息
EMP *findnum(int num);                               //按工号查询员工信息
EMP *findtelephone(char *name);                      //按通讯号码查询员工信息
EMP *findqq(char *name);                             //按QQ号查询员工信息
void displayemp(EMP *emp, char *field, char *name);  //显示员工信息
void checkfirst(void);                               //初始化检测
void bound(char ch, int n);                          //画出分界线
void login();                                        //登录检测
void menu();                                         //主菜单列表

/*------------------End of API Definition--------------------*/

#endif /* End of _STAFF_H_ */

