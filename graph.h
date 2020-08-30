#include "gluethread/glthread.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef __GRAPH__
#define __GRAPH__

#include <assert.h>

#define NODE_NAME_SIZE 32
#define MAX_INTERFACES_PER_NODE 10
#define INTERFACE_NAME_SIZE 32
#define TOPOLOGY_NAME_SIZE 32

typedef struct node_ node_t;
typedef struct link_ link_t;

typedef struct interface_ {
    char interface_name[INTERFACE_NAME_SIZE];
    struct node_ *att_node;
    struct link_ *link;
    // interface_network_properties_t interface_network_properties;
} interface_t;

struct link_ {
    interface_t interface1; /* WHY THIS IS NOT A POINTER?*/
    interface_t interface2; /* WHY THIS IS NOT A POINTER?*/
    unsigned int cost;
};

struct node_ {
    char node_name[NODE_NAME_SIZE];
    interface_t *interfaces[MAX_INTERFACES_PER_NODE];
    glthread_t graph_glue;
    // node_network_properties_t node_network_properties;
};

typedef struct graph_ {
    char topology_name[TOPOLOGY_NAME_SIZE];
    glthread_t node_list;
} graph_t;


/* node_t *graph_glue_to_node(glthread_t *glthread_ptr); */

GLTHREAD_TO_STRUCT(graph_glue_to_node, node_t, graph_glue);

graph_t *
create_new_graph(char *topology_name);

node_t *
create_graph_node(graph_t *graph, char *node_name);

void 
insert_link_between_two_nodes(node_t *node1, node_t *node2, 
    char *node1_interface_name, char *node2_interface_name, 
    unsigned int cost);


/* Helper functions */

static inline node_t *
get_nbr_node(interface_t *interface) {

    /* If the interface is not used, assert will 
     * send a message to stderr and terminates the 
     * program with calling abort. */

    assert(interface->att_node);
    assert(interface->link);

    /* Getting nbr node connected to local interface. */

    link_t *link = interface->link;
    if (&link->interface1 == interface)
        return link->interface2.att_node; 
    return link->interface1.att_node;

}

static inline int
get_node_interface_available_slot(node_t *node) {

    /* Searching for an available interface in the 
     * given node. */

    for (int interface = 0; interface < MAX_INTERFACES_PER_NODE; interface++) {
    
        /* If there is an available interface, 
         * return it. */
        
        if (node->interfaces[interface] == NULL)
            return interface;
    }

    /* If there are no more available slots, returns 1 */
    
    return -1;

}

static inline interface_t *
get_interface_by_name (node_t *node, char *interface_name) {
    for (int interface = 0; interface < MAX_INTERFACES_PER_NODE; interface++)
        if (strncmp(node->interfaces[interface]->interface_name, interface_name, INTERFACE_NAME_SIZE) == 0)
            return node->interfaces[interface]; 
    return NULL;
}

static inline node_t *
get_node_by_name (graph_t *topology, char *node_name) {
	node_t *node;
	glthread_t *glthread_ptr;
	
	ITERATE_GLTHREAD_BEGIN(&topology->node_list, glthread_ptr) {
		node = graph_glue_to_node(glthread_ptr);
		if (strncmp(node->node_name, node_name, NODE_NAME_SIZE) == 0)
			return node;
	} ITERATE_GLTHREAD_END(&topology->node_list, glthread_ptr);

	return NULL;
}

/* Display routines */

void
dump_graph(graph_t *graph);

void
dump_node(node_t *node);

void
dump_interface(interface_t *interface);

#endif /* __GRAPH__ */
