

#include "list.h"


int main(int argc, const char *argv[])
{
	list_t *p;
	list_t *p1;
	
	data_t buf;
	int i;

	p = creat_list(15);

	for(i = 0;i < 20;i ++){
		ins_index_list(p,0,&i);
	}

	print_list(p);

	del_index_list(p,4);

	print_list(p);

	buf = 250;
	ins_index_list(p,6,&buf);
	print_list(p);

	buf = 250;
	i = locate_list(p,&buf);
	
	buf = 251;
	change_index_list(p,i,&buf);
	print_list(p);


	//p->data[5];
	get_index_list(p,5,&buf);

	clean_list(p);
	
	print_list(p);

	dis_list(p);



	return 0;
}





