/**************************************************************************** **
 * Copyright (C) 2010-2011 Cameo Communications, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : trian.h
 *     FIRST CREATION DATE    : 2020/01/31
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : EExuke
 *     Last Change            : 2020/01/31
 *     FILE DESCRIPTION       : API exported to external moduels
** ************************************************************************** */
#ifndef _TRIAN_H_
#define _TRIAN_H_

/*------------------Macro Definition-------------------------*/
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HEADER1 " -------------------------------BOOK TICKET----------------------------------\n"
#define HEADER2 " |  number  |start city|reach city|takeofftime|receivetime|price|ticketnumber|\n"
#define HEADER3 " |----------|----------|----------|-----------|-----------|-----|------------|\n"
#define FORMAT  " |%-10s|%-10s|%-10s|%-10s |%-10s |%5d|  %5d     |\n"
#define DATA p->data.num, p->data.startcity, p->data.reachcity, p->data.takeofftime, \
             p->data.receivetime, p->data.price, p->data.ticketnum

int saveflag = 0;

/* 定义存储火车票信息的结构体 */
struct train
{
	char num[10];            /* 列车号 */
	char startcity[10];      /* 出发城市 */
	char reachcity[10];      /* 目的城市 */
	char takeofftime[10];    /* 发车时间 */
	char receivetime[10];    /* 到达时间 */
	int  price;              /* 票价 */
	int  ticketnum;         /* 票数 */
};

/* 订票人的信息 */
struct man
{
	char num[10];    /* ID */
	char name[10];   /* 姓名 */
	int  bookNum;   /* 定的票数 */
};

/* 定义火车票信息链表的节点结构 */
typedef struct node
{
	struct train data;    /* 声明train结构体类型的变量data */
	struct node * next;
}Node, *Link;

/* 定义订票人链表的节点结构 */
typedef struct Man
{
	struct man data;
	struct Man *next;
}book, *bookLink;

/*------------------End of Macro Definition------------------*/

/*------------------API Definition---------------------------*/
void menu();
void Traininfo(Link linkhead);
void printheader();
void printdata(Node *q);
void searchtrain(Link l);
void Bookticket(Link l, bookLink k);
void Modify(Link l);
void showtrain(Link l);
void SaveTrainInfo(Link l);
void SaveBookInfo(bookLink k);

/*------------------End of API Definition--------------------*/

#endif /* End of _TRIAN_H_ */

