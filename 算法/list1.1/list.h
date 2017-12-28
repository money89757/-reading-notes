#ifndef __LIST_H__
#define __LIST_H__

typedef int data_t;

#define list_t struct list

extern list_t *creat_list(int size);
extern int ins_index_list(list_t *p,int index,const data_t *data);
extern int del_index_list(list_t *p,int index);
extern int change_index_list(list_t *p,int index,const data_t *newdata);
extern int get_index_list(list_t *p,int index,data_t *databuf);
extern int is_full_list(list_t *p);
extern int is_empty_list(list_t *p);
extern int locate_list(list_t *p,const data_t *data);
extern int clean_list(list_t *p);
extern int dis_list(list_t *p);
extern void print_list(list_t *p);

#endif
