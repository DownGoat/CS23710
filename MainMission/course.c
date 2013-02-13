/**
 * @file course.c
 * 
 * @brief This file contains functions used to work with the course data
 * structure.
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * 
 * @date 2012.11.25
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "node.h"
#include "track.h"
#include "course.h"
#include "entrant.h"


extern int errno;

/**
 * This function reads and parses the file with the courses. The function will
 * check that the node specified exists, and that there is a valid track between
 * the nodes. The data should consist of a single capital character which will 
 * be the course ID. Followed by a positive integer which will be the number of 
 * nodes for the course. The rest of the line should be the ID of each node 
 * separated by spaces.
 * 
 * @param path Path to the file containing the course data.
 * 
 * @param node_list Pointer to the node list. It is needed to verify that the
 * nodes in the course exists.
 * 
 * @param track_list Pointer to the track list. It is needed to verify that
 * there is a valid path between the nodes of the course.
 * 
 * @return If the function is successful it will return a pointer to the first
 * element of the list. If it fails it will return NULL.
 */
COURSE_LIST *read_courses(char *path, NODE_LIST *node_list, TRACK_LIST *track_list) {
    COURSE_LIST *course_list = NULL;
    //COURSE *head = NULL, *current;
    FILE *file = fopen(path, "rb");

    course_list = (COURSE_LIST *) calloc(1, sizeof (COURSE_LIST));
    if (course_list == NULL) {
        fprintf(stderr, "[!] A call to calloc() failed.\n\n");

        exit(0);
    }

    if (file == NULL) {
        fprintf(stderr, "[!] Something went wrong with reading the file '%s'.\n", path);
        fprintf(stderr, "System error code is %d.\n\n", errno);

        return NULL;
    }

    int line_count = 1;

    while (!feof(file)) {
        char node_line[1024];
        int i;

        COURSE *temp = (COURSE *) calloc(1, sizeof (COURSE));
        if (temp == NULL) {
            fprintf(stderr, "[!] A call to calloc() failed.\n");

            exit(0);
        }

        //fgets(node_line, 1024, file);
/*
        int ret = sscanf(node_line, " %c %d", )
*/
        int ret = fscanf(file, " %c %d %1023[^\t\n]",
                &temp->course_id,
                &temp->nodes_size,
                node_line);

        if (ret == -1) {
            break;
        }
        else if (ret != 3) {
            fprintf(stderr, "File format error on line %d. Ret val: %d\n\n", line_count, ret);

            return NULL;
        }

        temp->time_cp = 0;
        
        for (i = 0; i < temp->nodes_size; i++) {
            char tmp_line[255];
            int node_id;
            NODE *node;

            ret = sscanf(node_line, "%d %1023[^\t\n]", &node_id, tmp_line);
            strcpy(node_line, tmp_line);

            node = node_id_search(node_id, node_list);
            if (node == NULL) {
                fprintf(stderr, "File format error on line %d.\n", line_count);
                fprintf(stderr, "No node with ID %d was found.\n\n", node_id);

                return NULL;
            }

            temp->nodes[temp->elements] = node_id;
            temp->elements += 1;
            
            if(strcmp(node->node_type, "CP") == 0) {
                temp->time_cp += 1;
            }
        }

        if (!validate_course(*temp, node_list)) {
            fprintf(stderr, "File format error on line %d.\n", line_count);
            fprintf(stderr, "Invalid track.\n\n");

            return NULL;
        }

        if (course_list->head == NULL) {
            course_list->head = temp;
            course_list->tail = temp;
            //current = course_list->tail;
            course_list->size = 1;
        }
        else {
            course_list->tail->next = temp;
            course_list->tail = temp;
            course_list->size += 1;
            //current->next = temp;
            //current = temp;
        }

        line_count += 1;
    }

    return course_list;
}

/**
 * @brief Validates if the COURSE is combined of nodes with that tracks that 
 * are valid.
 * 
 * @param course The course you want to validate.
 * 
 * @param node_list A list of nodes, this is needed to lookup the nodes.
 * 
 * @return Returns 1 if it is a valid COURSE. If the COURSE is invalid 0 is 
 * returned.
 */
int validate_course(COURSE course, NODE_LIST *node_list) {
    int i, x;
    
    for(i=0; i < course.elements-1; i++) {
        int found = 0;
        NODE *one, *two;
        
        one = node_id_search(course.nodes[i], node_list);
        two = node_id_search(course.nodes[i+1], node_list);
        
        for(x=0; x < one->elements; x++) {
            if(validate_track(one->node_id, two->node_id, one->tracks[x])) {
                found = 1;
            }
        }
        
        if(!found) {
            return 0;
        }
    }
    
    return 1;
}

/**
 * @brief This function searches for a COURSE with the given ID. 
 * 
 * @param list List of the courses.
 * 
 * @param id The ID of the COURSE you are looking for.
 * 
 * @return Returns a pointer to the COURSE if it is found, if it fails NULL is
 * returned.
 */
COURSE *search_course_id(COURSE_LIST *list, char id) {
    COURSE *current = list->head;

    do {
        if (current->course_id == id) {
            return current;
        }

        current = current->next;
    } while (current->next != NULL);

    if (current->course_id == id) {
        return current;
    }

    return NULL;
}