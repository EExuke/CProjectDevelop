/**************************************************************************** **
 * Copyright (C) 2010-2011 Cameo Communications, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : consume.h
 *     FIRST CREATION DATE    : 2020/02/06
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : EExuke
 *     Last Change            : 2020/02/06
 *     FILE DESCRIPTION       : API exported to external moduels
** ************************************************************************** */
#ifndef _CONSUME_H_
#define _CONSUME_H_

/*------------------Macro Definition-------------------------*/
#define LEN sizeof(struct scorenode)
#define DEBUG

struct scorenode
{
	int number;        /* 学号 */
	char name[10];     /* 姓名 */
	int xiaofei;       /* 消费额 */
	struct scorenode *next;
};
typedef struct scorenode score;
int n,k;               /* 定义为全局变量 */

/*------------------End of Macro Definition------------------*/

/*------------------API Definition---------------------------*/
score *creat();
score *load(score *head);
score *search(score *head);
score *del(score *head);
score *add(score *head, score *stu);
void print(score *head);
int save(score *p1);
void menu();

/*------------------End of API Definition--------------------*/

#endif /* End of _CONSUME_H_ */

