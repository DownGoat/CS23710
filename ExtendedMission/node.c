/**
 * @file node.c
 * 
 * @brief This file holds function used to wok with the node data structure.
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * 
 * @date 2012.11.25
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "node.h"


extern int errno;

/**
 * @brief This function searches for a node with a given ID.
 * 
 * @param id The ID of the node you want to search for.
 * 
 * @param list Pointer to the list, this list is the list that will be searched.
 * 
 * @return If the node is found the function returns the a pointer to the node.
 * If the node is not found it will return NULL.
 */
NODE *node_id_search(int id, NODE_LIST *list) {
    NODE *current = list->head;

    do {

        if(current->node_id == id) {
            return current;
        }

        current = current->next;
    } while(current->next != NULL);

    if(current->node_id == id) {
            return current;
    }

    return NULL;
}

/**
 * @brief This functions read in nodes from file and creates a linked list of 
 * the nodes. The data should contain a node ID and a short string used to 
 * identify what type of node the node is.
 * 
 * @param path Path to the file where the node data is stored.
 * 
 * @return Returns a pointer to the first NODE in the linked list.
 * Returns NULL if something went wrong.
 */
NODE_LIST *read_nodes(char *path) {
    NODE_LIST *list = NULL;
    NODE *current = NULL;
    FILE *file = fopen(path, "rb");
    
    list = (NODE_LIST *) calloc(1, sizeof(NODE_LIST));
    if(list == NULL) {
        fprintf(stderr, "[!] calloc() failed, no more memory?\n\n");
            
        return NULL;
    }
    
    
    if(file == NULL) {
        fprintf(stderr, "[!] Something went wrong with reading the file '%s'.\n", path);
        fprintf(stderr, "System error code is %d.\n\n", errno);
        
        return NULL;
    }
    
    int line_count = 1;
    
    while (!feof(file)) {
        NODE *temp = (NODE *) calloc(1, sizeof(NODE));
        temp->elements = 0;
        //NODE *temp = init_node();
        if(temp == NULL) {
            printf("[!] calloc() failed, no more memory?\n\n");
            
            return NULL;
        }
        
        int ret = fscanf(file, "%d %2s", &temp->node_id, temp->node_type);
        
        // Error checking on fscanf return value.
        if(ret == -1) { //The return value can be -1 when only a newline i read.
            break;
        }
        
        //If the return value is not 2 something was wrong with the file format.
        else if(ret != 2) { 
            fprintf(stderr, "[!] File format error on line %d.\n\n", line_count);
            
            return NULL;
        }
        
        line_count += 1;
        
        
        ///Need to set a start if it hasn't been done yet.
        if(list->head == NULL) {
            list->head = temp;
            list->tail = temp;
            current = list->tail;
            list->size = 1;
        }
        
        else {
            current->next = (struct node *) temp;
            current = temp;
            list->tail = temp;
            list->size += 1;
        }
    }
    
    fclose(file);
    
    return list;
}
