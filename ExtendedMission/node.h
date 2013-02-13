/**
 * @file node.h
 * 
 * @brief This file contains the definition of the node data structure and 
 * definitions of functions used to work with the data structure.
 *
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * 
 * @date 2012.11.25
 */

#include "track.h"


#ifndef COURSE_NODE_H
#define	COURSE_NODE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
//#define NORMAL_CP "CP"
//#define MEDICAL_CP "MC"
//#define JUNCTION "JN"


    /**
     * @brief The node data structure represents a checkpoint in the course.
     * There are several different checkpoints. Each node is identified by a
     * positive integer. The data structure is also a linked list and has a
     * pointer to the next element in the list.
     */
    typedef struct node {
        /*@{*/
        /**
         * @brief The node id used to identify the node. The id is a positive integer.
         */
        int node_id;
        
        /**
         * @brief The node type.
         */
        char node_type[10];
        
        /**
         * @brief Pointer to the next node element.
         */
        struct node *next;
        
        /**
         * @brief Number of track elements currently in the tracks array.
         */
        int elements;
        
        /**
         * @brief Array of tracks for this node.
         */
        TRACK tracks[100];
        /*@}*/
    } NODE;
    
    /**
     * @brief This is the node list. It is used to easily pass around the list
     * and information about it to make working with it easier. 
     */
    typedef struct node_list {
        /*@{*/
        /**
         * @brief The head of the linked list.
         */
        NODE *head;
        
        /**
         * @brief The tail of the linked list.
         */
        NODE *tail;
        
        /**
         * @brief The size of the linked list.
         */
        int size;
        /*@}*/
    } NODE_LIST;
    
    //Function definitions.
    NODE *node_id_search(int id, NODE_LIST *list);
    NODE_LIST *read_nodes(char *path);
    
    

#ifdef	__cplusplus
}
#endif

#endif	/* COURSE_NODE_H */

