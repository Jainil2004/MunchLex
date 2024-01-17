/// \file   threading.c
/// \author Gonçalo Marques
/// \date   2024-01
#include "include/threading.h"
#include "include/tree.h"

/// @private
/// @brief A utility function to instantiate a new tpool_work_t object, allocating memory for a new work object.
/// @param func A function pointer to the function that shall be called.
/// @param args The args that shall be passed to the function.
static tpool_work_t* work_create(work_func_t func, void* args){
    if (func == NULL)
        return NULL;

    tpool_work_t work = malloc(sizeof(tpool_work_t));
    if (work == NULL)
        return NULL;

    work->func = func;
    work->args = args;
    work->next = NULL;

    return work;
}

/// @private
/// @brief A Destructor function that simply frees the work unit from memory.
/// @param work Pointer to Work object.
static void work_destroy(tpool_work_t* work){
    if (work == NULL){
        return;
    }

    free(work);
}

/// @private
/// @brief A utility function that fetches a unit of work from the ThreadPool list, updating the work_first pointer to the next pointer of the previous work_first. Keeps integrity of list if it's the only item in list.
/// @param tp Pointer to ThreadPool Object.
static tpool_work_t* tpool_get_work(tpool_t* tp){
    if(tp == NULL){
        return NULL;
    }

    tpool_work_t workNow = tp->work_first;
    if(workNow == NULL){
        return NULL;
    }

    if(workNow->next == NULL){
        tp->work_first = NULL;
        tp->work_last = NULL;
    } else {
        tp->work_first = workNow->next;
    }
    
    return workNow;
}

tpool_t* tpool_create(size_t num){
    pthread_t  thread;
    size_t     i;

    if (num == 0) {
        num = 2;
    }

    tpool_t* tp = calloc(1, sizeof(tpool_t));
    if (tp == NULL)
        return NULL;

    tp->thread_cnt = num;

    pthread_mutex_init(&(tp->work_mutex), NULL);
    pthread_cond_init(&(tp->work_cond), NULL);
    pthread_cond_init(&(tp->working_cond), NULL);

    tp->work_first = NULL;
    tp->work_last  = NULL;

    for (i = 0; i < num; i++) {
        pthread_create(&thread, NULL, tpool_worker, tp);
        pthread_detach(thread);
    }

    return tp;
}

bool tpool_add_work(tpool_t* tp, thread_func_t func, void* arg){

}