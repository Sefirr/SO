#ifndef SLIST_H
#define SLIST_H
#include "list_sol.h"

typedef struct
{
	list_sol_t t_list;	/* List of structures */
	size_t size;		/* Size of the list  */
}slist_t;


/* Operations on slist_t */
extern void init_slist (slist_t* slist, size_t node_size, size_t offset_node_field);
extern void insert_slist ( slist_t* slist,void* elem);
extern void insert_slist_head ( slist_t* slist,void* elem);
extern void remove_slist ( slist_t* slist,void* elem);
extern void* head_slist ( slist_t* slist);
extern void* tail_slist ( slist_t* slist);
extern void* next_slist ( slist_t* slist,void* elem);
extern void insert_after_slist(slist_t* slist, void *object, void *nobject);
extern void insert_before_slist(slist_t* slist, void *object, void *nobject);
extern void destroy_slist( slist_t* slist );
static inline int is_empty_slist(slist_t* slist) { return slist->size==0; }
static inline int size_slist(slist_t* slist){ return slist->size; }
void sorted_insert_slist(slist_t* slist, void* object, int ascending, int (*compare)(void*,void*));
void sorted_insert_slist_front(slist_t* slist, void* object, int ascending, int (*compare)(void*,void*));
void sort_slist(slist_t* slist, int ascending, int (*compare)(void*,void*));

#endif	/* _SLIST_H*/
