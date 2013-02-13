/**
 * @file track.c
 * 
 * @brief This file contains functions used to work with the track data
 * structure.
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * 
 * @date 2012.11.30
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "track.h"
#include "node.h"


extern int errno;

/**
 * @brief This function reads in and parses a file containing the track data. 
 * It uses this data to create a linked list of the tracks. The file should have
 * data about paths between nodes and the maximum time a entrant should use
 * between the nodes.
 * 
 * @param path The path to the file containing the track data.
 * 
 * @param node_list Pointer to the list of nodes, the list is needed to look up
 * the nodes that are used in the track data to verify that the track is between
 * nodes that exist.
 * 
 * @return Upon success this function returns a pointer to the first element in 
 * the linked list. If it fails it will return NULL.
 */
TRACK_LIST *read_tracks(char *path, NODE_LIST *node_list) {
    TRACK_LIST *track_list = NULL;
    TRACK *current;
    FILE *file = fopen(path, "rb");
    
    track_list = (TRACK_LIST *) calloc(1, sizeof(NODE_LIST));
    if(track_list == NULL) {
        fprintf(stderr, "[!] A call to calloc() failed.\n\n");
        
        exit(0);
    }
    
    if(file == NULL) {
        fprintf(stderr, "[!] Something went wrong with reading the file '%s'.\n", path);
        fprintf(stderr, "System error code is %d.\n\n", errno);
        
        return NULL;
    }
    
    int line_count = 1;
    
    while(!feof(file)) {
        int start_id, end_id;
        
        TRACK *temp = (TRACK *) calloc(1, sizeof(TRACK));
        if(temp == NULL) {
            fprintf(stderr, "calloc() failed, no more memory?\n\n");
            
            return NULL;
        }
        
        int ret = fscanf(file, "%d %d %d %d",
                                            &temp->track_id,
                                            &start_id,
                                            &end_id,
                                            &temp->time);
        
        // return value of fscanf can be -1 if only a newline is read.
        if(ret == -1) {
            break;
        }
        
        //Return value should be 4, if it is something else something must have
        //went wrong.
        else if(ret != 4) {
            fprintf(stderr, "File format error on line %d.\n\n", line_count);
            
            return NULL;
        }
        
        //Check if the nodes is in the list of loaded nodes.
        NODE *start_node = node_id_search(start_id, node_list);
        if(start_node == NULL) {
            fprintf(stderr, "File format error on line %d.\n", line_count);
            fprintf(stderr, "Start node does not exist.\n\n");
            
            return NULL;
        }
        
        NODE *end_node = node_id_search(end_id, node_list);
        if(end_node == NULL) {
            fprintf(stderr, "File format error on line %d.\n", line_count);
            fprintf(stderr, "End node does not exist.\n\n");
            
            return NULL;
        }
        
        temp->start_id = start_id;
        temp->end_id = end_id;
        
        start_node->tracks[start_node->elements] = *temp;
        start_node->elements += 1;
        
        end_node->tracks[end_node->elements] = *temp;
        end_node->elements += 1;        
        
        line_count += 1;
        
        if(track_list->head == NULL) {
            track_list->head = temp;
            track_list->tail = temp;
            current = track_list->tail;
            track_list->size = 1;
        }
        
        else {
            current->next = (struct track *) temp;
            current = temp;
            track_list->tail = temp;
            track_list->size += 1;
        }
        
    }
    
    fclose(file);
    
    return track_list;
}

/**
 * @brief Checks if the given track is between the given nodes.
 * 
 * @param node_one ID of one of the NODEs.
 * 
 * @param node_two The ID of the other NODE.
 * 
 * @param track The TRACK you want to check.
 * 
 * @return Returns 1 if the TRACK is between NODEs. If fails 0 is returned.
 */
int validate_track(int node_one, int node_two, TRACK track) {
    
    if(track.start_id == node_one && track.end_id == node_two) {
        return 1;
    }
    
    else if(track.start_id == node_two && track.end_id == node_one) {
        return 1;
    }
    
    return 0;
}
