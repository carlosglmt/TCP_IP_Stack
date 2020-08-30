#include "graph.h"

graph_t *
create_new_graph(char *topology_name) {

    graph_t *new_graph = (graph_t *) calloc(1, sizeof(graph_t));
    strncpy(new_graph->topology_name, topology_name, TOPOLOGY_NAME_SIZE);
    new_graph->topology_name[TOPOLOGY_NAME_SIZE - 1] = '\0';
    init_glthread(&new_graph->node_list);
    return new_graph;

}

node_t *
create_graph_node(graph_t *graph, char *node_name) {

    node_t *new_node = (node_t *) calloc(1, sizeof(node_t));
    strncpy(new_node->node_name, node_name, NODE_NAME_SIZE);
    new_node->node_name[NODE_NAME_SIZE - 1] = '\0';
    
    init_glthread(&new_node->graph_glue);
    glthread_add_right(&graph->node_list, &new_node->graph_glue);
    return new_node;

}

void 
insert_link_between_two_nodes(node_t *node1, node_t *node2, 
    char *node1_interface_name, char *node2_interface_name, 
    unsigned int cost) {
    
    link_t *link = (link_t *) calloc(1, sizeof(link_t));
    
    strncpy(link->interface1.interface_name, node1_interface_name, INTERFACE_NAME_SIZE);
    link->interface1.interface_name[INTERFACE_NAME_SIZE - 1] = '\0';
    strncpy(link->interface2.interface_name, node2_interface_name, INTERFACE_NAME_SIZE);
    link->interface2.interface_name[INTERFACE_NAME_SIZE - 1] = '\0';    
    
    link->interface1.link = link;
    link->interface2.link = link;

    link->interface1.att_node = node1;
    link->interface2.att_node = node2;
    link->cost = cost;

    int available_slot;
    available_slot = get_node_interface_available_slot(node1);
    node1->interfaces[available_slot] = &link->interface1;

    available_slot = get_node_interface_available_slot(node2);
    node2->interfaces[available_slot] = &link->interface2;

}

void
dump_graph(graph_t *graph) {

    node_t *current_node = NULL;
    glthread_t *current_glthread = NULL;

    printf("Topology Name: %s \n", graph->topology_name);
    ITERATE_GLTHREAD_BEGIN(&graph->node_list, current_glthread) {
        current_node = graph_glue_to_node(current_glthread);
        dump_node(current_node);
    } ITERATE_GLTHREAD_END(&graph->node_list, current_glthread);

}

void
dump_node(node_t *node) {
    
    printf("Local Name: %s\n", node->node_name);
    for (unsigned int interface = 0; interface < MAX_INTERFACES_PER_NODE; interface++) {
        if (node->interfaces[interface] != NULL) 
            dump_interface(node->interfaces[interface]);
    }

}

void
dump_interface(interface_t *interface) {
    
    printf("\tInterface Name: %s, Nbr Node: %s, Cost: %d\n", 
            interface->interface_name, 
            get_nbr_node(interface)->node_name, 
            interface->link->cost);

}