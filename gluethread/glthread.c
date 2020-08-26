#include "glthread.h"
#include <stdlib.h>

void
init_glthread(glthread_t *glthread) {
   
    glthread->left = NULL;
    glthread->right = NULL;

}

void
glthread_add_left(glthread_t *base_glthread, glthread_t *new_glthread) {
    
    if (!base_glthread->left) {
        new_glthread->left = NULL;
        base_glthread->left = new_glthread;
        new_glthread->right = base_glthread;
        return;
    }

    new_glthread->left = base_glthread->left;
    new_glthread->left->right = new_glthread;
    base_glthread->left = new_glthread;
    new_glthread->right = base_glthread;

}

void
glthread_add_right(glthread_t *base_glthread, glthread_t *new_glthread) {
    
    if (!base_glthread->right) {
        new_glthread->right = NULL;
        base_glthread->right = new_glthread;
        new_glthread->left = base_glthread;
        return;
    }

    new_glthread->right = base_glthread->right;
    new_glthread->right->left = new_glthread;
    base_glthread->right = new_glthread;
    new_glthread->left = base_glthread;

}

void
remove_glthread(glthread_t *glthread) {
    
    if (!glthread->left) {
        if (glthread->right) {
            glthread->right->left = NULL;
            glthread->right = NULL;
            return;
        }
        return;
    }
    
    if (!glthread->right) {
        glthread->left->right = NULL;
        glthread->left = NULL;
        return;
    }
    
    glthread->left->right = glthread->right;
    glthread->right->left = glthread->left;
    glthread->left = NULL;
    glthread->right = NULL;

}

void
glthread_append(glthread_t *base_glthread, glthread_t *new_glthread) {
   
    glthread_t *glthread_ptr = NULL, *prev_glthread = NULL;
    
    ITERATE_GLTHREAD_BEGIN(base_glthread, glthread_ptr) {
        prev_glthread = glthread_ptr;
    } ITERATE_GLTHREAD_END(base_glthread, glthread_ptr);
    
    if (prev_glthread) 
        glthread_add_right(prev_glthread, new_glthread);
    else
        glthread_add_right(base_glthread, new_glthread);
}

 void
 delele_glthread_list(glthread_t *base_glthread) {
     
     glthread_t *glthread_ptr = NULL;
     
     ITERATE_GLTHREAD_BEGIN (base_glthread, glthread_ptr) {
         remove_glthread(glthread_ptr);
     } ITERATE_GLTHREAD_END(base_glthread, glthread_ptr);
 
 }

 unsigned int
 get_glthread_list_count(glthread_t *base_glthread) {
    
    unsigned int counter = 0;
    glthread_t *glthread_ptr = NULL;
    
    ITERATE_GLTHREAD_BEGIN(base_glthread, glthread_ptr) {
        counter++;
    } ITERATE_GLTHREAD_END(base_glthread, glthread_ptr);
    
    return counter;

}

void
glthread_priority_insert(glthread_t *base_glthread, glthread_t *new_glthread, int (*comp_fn)(void *, void *), int offset) {
    
    glthread_t *curr_glthread = NULL, *prev_glthread = NULL;
    
    if (IS_GLTHREAD_EMPTY(base_glthread)) {
        glthread_add_right(base_glthread, new_glthread);
        return;
    }

    ITERATE_GLTHREAD_BEGIN(base_glthread, curr_glthread){
        if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(new_glthread, offset), 
                GLTHREAD_GET_USER_DATA_FROM_OFFSET(curr_glthread, offset)) == -1) {
            glthread_add_right(curr_glthread, new_glthread);
            return;
        }
        prev_glthread = curr_glthread;
    } ITERATE_GLTHREAD_END(base_glthread, curr_glthread);

    glthread_add_right(prev_glthread, new_glthread);

}

#if 0
void *
glthread_search(glthread_t *base_glthread, void * (*thread_to_struct_fn)(glthread_t *), void *key, int (*comparison_fn)(void *, void)) {
    return NULL;
}
#endif