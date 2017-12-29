#ifndef __tcpmsg_h__
#define __tcpmsg_h__

typedef struct tcpmsg
{
	int type;
	char name[256];
	char data[256];
} MSG;


#define  R 1
#define  L 2
#define  Q 3
#define  H 4
#endif
