#include "slist.h"
#ifndef NULL
#define NULL	0
#endif

void init_slist (slist_t* slist, size_t node_size, size_t offset_node_field)
{
	/* Init the list main structure */
	list_sol_create(&slist->t_list, node_size, offset_node_field);
	/* No elements */
	slist->size=0;
}

void insert_slist ( slist_t* slist,void* elem)
{
	list_sol_insert_tail(&slist->t_list, elem);
	slist->size++;
}

void insert_slist_head ( slist_t* slist,void* elem)
{
	list_sol_insert_head(&slist->t_list, elem);
	slist->size++;
}

void remove_slist ( slist_t* slist,void* elem)
{
	list_sol_remove(&slist->t_list, elem);
	slist->size--;
}

void* head_slist ( slist_t* slist)
{
	return list_sol_head(&slist->t_list);
}


void* tail_slist ( slist_t* slist)
{
	return list_sol_tail(&slist->t_list);
}

void* next_slist ( slist_t* slist,void* elem)
{
	return list_sol_next(&slist->t_list,elem);
}

void insert_after_slist(slist_t *slist, void *object, void *nobject)
{
	list_sol_insert_after(&slist->t_list,object,nobject);
	slist->size++;
}

void insert_before_slist(slist_t *slist, void *object, void *nobject)
{
	list_sol_insert_before(&slist->t_list,object,nobject);
	slist->size++;
}

void destroy_slist( slist_t* slist )
{
    list_sol_destroy(&slist->t_list);
    slist->size=0;
}

void sorted_insert_slist(slist_t* slist, void* object, int ascending, int (*compare)(void*,void*)){
	void *cur=NULL;
	
	cur=head_slist(slist);
	/* Search */
	if (ascending){
		// Find 
		while(cur!=NULL && compare(cur,object)<=0) {
			cur=next_slist(slist,cur);
		}
	}else{
		// Find 
		while(cur!=NULL && compare(cur,object)>=0) {
			cur=next_slist(slist,cur);
		}
	}		
		
	insert_before_slist(slist,cur,object);
}


void sorted_insert_slist_front(slist_t* slist, void* object, int ascending, int (*compare)(void*,void*)){
	void *cur=NULL;
	
	cur=head_slist(slist);
	/* Search */
	if (ascending){
		// Find 
		while(cur!=NULL && compare(cur,object)<0) {
			cur=next_slist(slist,cur);
		}
	}else{
		// Find 
		while(cur!=NULL && compare(cur,object)>0) {
			cur=next_slist(slist,cur);
		}
	}		
		
	insert_before_slist(slist,cur,object);
}

void sort_slist(slist_t* slist, int ascending, int (*compare)(void*,void*)){
	void *cur=NULL,*selected_node=NULL,*prev_selected=NULL;
	int i=0;
	
	/* Check if the list is already trivially sorted */
	if (slist->size<=1)
		return;
	
	cur=head_slist(slist);

	/* Insertion sort */		
	for (i=0;i<slist->size-1 && cur!=NULL;i++){
		
		/* Search */
		selected_node=cur;

		if (ascending){
			// Search for min
			while(cur!=NULL) {
				if (compare(cur,selected_node)<0)
					selected_node=cur;
				cur=next_slist(slist,cur);
			}
		}else{
			// Search for max
			while(cur!=NULL) {
				if (compare(cur,selected_node)>0)
					selected_node=cur;
				cur=next_slist(slist,cur);
			}
		}		
		
		remove_slist(slist,selected_node);
		insert_after_slist(slist,prev_selected,selected_node);
		prev_selected=selected_node;	
		cur=next_slist(slist,selected_node);
	}
}