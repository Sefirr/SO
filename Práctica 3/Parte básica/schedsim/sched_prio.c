#include <sched.h>

void sched_init_prio(void) {}

void sched_destroy_prio(void) {}

static void task_new_prio(task_t* t) { }

static task_t* pick_next_task_prio(runqueue_t* rq,int cpu) {
    task_t* t=head_slist(&rq->tasks); //List sorted by CPU burst lenght (just pick the first one)
    
    if (t) {
        /* Current is not on the rq*/
        remove_slist(&rq->tasks,t);
        t->on_rq=FALSE;
        rq->cur_task=t;
    }
    
    return t;
}

static int compare_tasks_cpu_burst(void *t1,void *t2) {
	task_t* tsk1=(task_t*)t1;
	task_t* tsk2=(task_t*)t2;
	return tsk1->prio-tsk2->prio;
}

static void enqueue_task_prio(task_t* t,int cpu, int runnable) {
    runqueue_t* rq=get_runqueue_cpu(cpu);
    
    if (t->on_rq || is_idle_task(t))
        return;
    
    if (t->flags & TF_INSERT_FRONT){
        //Clear flag
        t->flags&=~TF_INSERT_FRONT;
        sorted_insert_slist_front(&rq->tasks, t, 1, compare_tasks_cpu_burst);  //Push task    
    }
    else
        sorted_insert_slist(&rq->tasks, t, 1, compare_tasks_cpu_burst);  //Push task
    
    t->on_rq=TRUE;
            
    /* If the task was not runnable before, update the number of runnable tasks in the rq*/
    if (!runnable){
        task_t* current=rq->cur_task;
        rq->nr_runnable++;
        t->last_cpu=cpu;	
        
        /* Trigger a preemption if this task has a shorter CPU burst than current */
        if (preemptive_scheduler && !is_idle_task(current) && t->prio<current->prio) {
            rq->need_resched=TRUE;
            current->flags|=TF_INSERT_FRONT; /* To avoid unfair situations in the event
                                                another task with the same prio wakes up as well*/
        }
    }     
}


static void task_tick_prio(runqueue_t* rq,int cpu){
    
    task_t* current=rq->cur_task;
    
    if (is_idle_task(current))
        return;
    
    if (current->runnable_ticks_left==1) 
        rq->nr_runnable--; // The task is either exiting or going to sleep right now    
}

static task_t* steal_task_prio(runqueue_t* rq,int cpu){
    task_t* t=tail_slist(&rq->tasks);
    
    if (t) {
        remove_slist(&rq->tasks,t);
        t->on_rq=FALSE;
        rq->nr_runnable--;
    }
    return t;    
}

sched_class_t prio_sched = {
	.sched_init=sched_init_prio,
	.sched_destroy=sched_destroy_prio,
	.task_new=task_new_prio,
	.pick_next_task=pick_next_task_prio,
	.enqueue_task=enqueue_task_prio,
	.task_tick=task_tick_prio,
	.steal_task=steal_task_prio
};
