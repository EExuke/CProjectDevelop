/**************************************************************************** **
 * Copyright (C) 2010-2011 Cameo Communications, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : addressBook.h
 *     FIRST CREATION DATE    : 2020/02/01
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : EExuke
 *     Last Change            : 2020/02/01
 *     FILE DESCRIPTION       : API exported to external moduels
** ************************************************************************** */
#ifndef _ADDRESSBOOK_H_
#define _ADDRESSBOOK_H_

#include <curses.h>
#include <string.h>

/*------------------Macro Definition-------------------------*/
/* 每个节点的数据成员的结构 */
typedef struct Info
{
	char name[15];        /* 姓名 */
	char city[10];        /* 城市 */
	char province[10];    /* 省份 */
	char state[10];       /* 国家 */
	char tel[15];         /* 电话 */
};

/* 定义通讯录链表的节点结构 */
typedef struct node
{
	struct Info data;
	struct node * next;
}Node,*link;

/*------------------End of Macro Definition------------------*/

/*------------------API Definition---------------------------*/
void stringinput(char * t, int lens, char * notice);
void enter(link l);
void del(link l);
void display(Node * p);
void search(link l);
void list(link l);
void save(link l);
void load(link l);
int menu_select();

/*------------------End of API Definition--------------------*/

#endif /* End of _ADDRESSBOOK_H_ */

