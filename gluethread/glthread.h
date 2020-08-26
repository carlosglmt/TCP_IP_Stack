#ifndef __GLUETHREAD__
#define __GLUETHREAD__

typedef struct glthread_ {
    struct glthread_ *left;
    struct glthread_ *right;
} glthread_t;

void
init_glthread(glthread_t *glthread);

void
glthread_add_left(glthread_t *base_glthread, glthread_t *new_glthread);

void 
glthread_add_right(glthread_t *base_glthread, glthread_t *new_glthread);

void
remove_glthread(glthread_t *glthread);

void
glthread_append(glthread_t *base_glthread, glthread_t *new_glthread);

#define IS_GLTHREAD_EMPTY(glthread_ptr) \
    ((glthread_ptr)->left == NULL && (glthread_ptr)->right == NULL)

#define GLTHREAD_TO_STRUCT(fn_name, struct_name, field_name)                                        \
    static inline struct_name *fn_name(glthread_t *glthread_ptr) {                                  \
        return (struct_name *)((char *)glthread_ptr - (char *)&(((struct_name *)0)->field_name));   \
    }

#define BASE(glthread_ptr) ((glthread_ptr)->right)

#define ITERATE_GLTHREAD_BEGIN(glthread_ptr_start, glthread_ptr)        \
    {                                                                   \
        glthread_t *_glthread_ptr = NULL;                               \
        glthread_ptr = BASE(glthread_ptr_start);                        \
        for (; glthread_ptr != NULL; glthread_ptr = _glthread_ptr) {    \
            _glthread_ptr = (glthread_ptr)->right; 
#define ITERATE_GLTHREAD_END(glthread_ptr_start, glthread_ptr)          \
    }}

#define GLTHREAD_GET_USER_DATA_FROM_OFFSET(glthread_ptr, offset) \
    (void *)((char*)(glthread_ptr) - offset)

void
delete_glthread_list(glthread_t *base_glthread);

unsigned int
get_glthread_list_count(glthread_t *base_glthread);

void
glthread_priority_insert(glthread_t *base_glthread, glthread_t *new_glthread, int (*comp_fn)(void *, void *), int offset);

#if 0   
void *
glthread_search(glthread_t *base_glthread, void * (*thread_to_struct_fn)(glthread_t *), void *key, int (*comparison_fn)(void *, void *));
#endif
#endif /* __GLUETHREAD__ */