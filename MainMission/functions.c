/** 
 * @file functions.c
 * 
 * @brief This file contains various functions for the program
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 *
 * @date 2012.12.04
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "time_struct.h"
#include "entrant.h"
#include "event.h"
#include "course.h"
#include "cp_time.h"
#include "node.h"
#include "track.h"

/**
 * @brief This function calculates the time the entrant has used so far.
 * 
 * @param start The time the entrant started
 * 
 * @param now The time now.
 * 
 * @param total_time String buffer to store the result.
 */
void calc_time(TIME_STRUCT *start, TIME_STRUCT *now, char *total_time) {
    int hour, min;

    hour = now->hours - start->hours;
    min = now->minutts - start->minutts;

    if (min < 0) {
        min = 60 + min;
    }

    sprintf(total_time, "%dH %dM", hour, min);

    return;
}

/**
 * @brief Extracts the time from a string, eg 07:30, and stores it in the 
 * TIME_STRUCT type.
 *  
 * @param time_str THe string containing the time you want to extract.
 * 
 * @return If successful the function returns a pointer to a TIME_STRUCT. 
 * NULL is returned if the function fails.
 */
TIME_STRUCT *format_time(char *time_str) {
    TIME_STRUCT *time = (TIME_STRUCT *) calloc(1, sizeof (TIME_STRUCT));
    if (time == NULL) {
        fprintf(stderr, "[!] A call to calloc() failed.\n\n");

        exit(0);
    }

    sscanf(time_str, "%d:%d", &time->hours, &time->minutts);
    strcpy(time->time_str, time_str);

    return time;
}

/**
 * @brief Prints out the header used showing what each colum is.
 */
void print_entrant_header() {
    int i;
    printf("%-5s%-50s%-10s%-15s%-15s%-10s\n",
            "ID",
            "Name",
            "Course",
            "Status",
            "Start time",
            "Total time");

    for (i = 0; i < 105; i++) {
        printf("=");
    }
    printf("\n");
}

/**
 * @brief Prints out nicely formated info about a entrant.
 * 
 * @param entrant The entrant to print out.
 */
void print_entrant(ENTRANT *entrant) {
    if (entrant->checkpoints == NULL) {
        printf(ENT_FORMAT,
                entrant->comp_number,
                entrant->name,
                entrant->course_id,
                "Not Started",
                "N/A",
                "N/A");

        //printf("This entrant has not started.\n");
    } else {
        char time[20];

        calc_time(entrant->start_time, entrant->last_cp->time, time);
        printf(ENT_FORMAT,
                entrant->comp_number,
                entrant->name,
                entrant->course_id,
                entrant->status,
                entrant->start_time->time_str,
                time);
    }
}

/**
 * @brief Prints out a nicely formated list of entrants that has not started yet.
 * 
 * @param event Event structure.
 */
void not_started(EVENT *event) {
    ENTRANT *current = event->entrants->head;
    int i, found = 0;

    for (i = 0; i < event->entrants->size; i++) {
        if (current->checkpoints == NULL) {
            found += 1;
        }

        current = current->next;
    }

    if (found) {
        print_entrant_header();
        current = event->entrants->head;
        for (i = 0; i < event->entrants->size; i++) {
            if (current->checkpoints == NULL) {
                print_entrant(current);
            }

            current = current->next;
        }
    }
    else {
        printf("\nAll entrants has started.\n");
    }
}

/**
 * @brief Prints out a nicely formated list of entrants that has finished the 
 * course.
 * 
 * @param event Event structure.
 */
void finished(EVENT *event) {
    ENTRANT *current = event->entrants->head;
    int i, found = 0;

    for (i = 0; i < event->entrants->size; i++) {
        if (strcmp("Finished", current->status) == 0) {
            found += 1;
        }

        current = current->next;
    }

    if (found) {
        print_entrant_header();
        current = event->entrants->head;
        for (i = 0; i < event->entrants->size; i++) {
            if (strcmp("Finished", current->status) == 0) {
                print_entrant(current);
            }

            current = current->next;
        }
    }
    else {
        printf("\nNo entrant has finished yet.\n");
    }
}

/**
 * @brief Prints out a nicely formated list of all entrants that are currently 
 * out on the track.
 * 
 * @param event Event structure.
 */
void out_track(EVENT *event) {
    ENTRANT *current = event->entrants->head;
    int i, found = 0;

    for (i = 0; i < event->entrants->size; i++) {
        if ((current->checkpoints != NULL) &&
                (strcmp("Finished", current->status) != 0)) {
            found += 1;
        }

        current = current->next;
    }

    if (found) {
        print_entrant_header();
        current = event->entrants->head;
        for (i = 0; i < event->entrants->size; i++) {
            if ((current->checkpoints != NULL) &&
                    (strcmp("Finished", current->status) != 0)) {
                print_entrant(current);
            }

            current = current->next;
        }
    }
    else {
        printf("\nNo entrants are out on the track.\n");
    }
}

/**
 * @brief Prints out a nicely formated list of all entrants.
 * 
 * @param event Event structure.
 */
void print_all(EVENT *event) {
    ENTRANT *current = event->entrants->head;
    int i;

    print_entrant_header();

    for (i = 0; i < event->entrants->size; i++) {
        print_entrant(current);

        current = current->next;
    }
}

/**
 * This function checks if a user has finished the course the entrant is
 * attending. The function creates a array of all check points in the course
 * where time is recorded. It then compares the contents of the array with the
 * checkpoints the entrant has been to. If they are in the same order and equal
 * the entrant is finished.
 * 
 * @param event Event structure containing data the function needs.
 * 
 * @param entrant The entrant you want to check.
 * 
 * @return If the entrant is finished 1 is returned, if the entrant is not 
 * finished 0 is returned.
 */
int entrant_finished(EVENT *event, ENTRANT *entrant) {
    int i, elements = 0, *cp_nodes;
    CP_TIME *current = entrant->checkpoints->head;
    COURSE *course = search_course_id(event->courses, entrant->course_id);

    cp_nodes = (int *) calloc(course->elements, sizeof(int));
    if(cp_nodes == NULL) {
        fprintf(stderr, "Calloc() call failed.\n");
        
        exit(0);
    }
    
    for(i=0 ; i < course->elements; i++) {
        NODE *node = node_id_search(course->nodes[i], event->nodes);
        
        if(strcmp("CP", node->node_type) == 0) {
            cp_nodes[elements] = course->nodes[i];
            elements += 1;
        }
    }
    
    if(entrant->checkpoints->size < elements) {
        return 0;
    }
    
    for(i=0; i < elements; i++) {
        if(cp_nodes[i] != current->node->node_id) {
            return 0;
        }
        
        current = current->next;
    }
    
    return 1;
}

/**
 * @brief This function is used to add a check point to a entrant.
 * 
 * @param event Event structure with data about the event.
 *  
 * @param status The status read from file.
 * 
 * @param node Pointer to the node that is the checkpoint.
 * 
 * @param ent The entrant that checked in at the check point
 * 
 * @param time Time string of the time.
 */
void add_cp(EVENT *event, char status, NODE *node, ENTRANT *ent, char *time) {
    CP_LIST *list = NULL;
    CP_TIME *temp = calloc(1, sizeof(CP_TIME));
        
    if(temp == NULL) {
        fprintf(stderr, "Call to calloc() failed.\n\n");

        exit(0);
    }
    
    if(ent->checkpoints == NULL) {
        list = calloc(1, sizeof(CP_LIST));
        if(list == NULL) {
            fprintf(stderr, "Call to calloc() failed.\n\n");

            exit(0);
        }
        
        list->size = 1;
        ent->checkpoints = list;
        ent->start_time = format_time(time);
        list->head = temp;
    }
    
    else {
        TIME_STRUCT *ct = (TIME_STRUCT *) calloc(1, sizeof(TIME_STRUCT));
        if(ct == NULL) {
            fprintf(stderr, "Call to calloc() failed.\n\n");

            exit(0);
        }
        ent->checkpoints->size += 1;
        ct = format_time(time);
        temp->time = ct;
        calc_time(ent->start_time, ct, ent->total_time);
        ent->checkpoints->tail->next = temp;
        
    }
    
    temp->node = node;
    temp->status = status;
    temp->time = format_time(time);
    //ent->start_time = format_time(time);
    ent->current_node = node;
    ent->last_cp = temp;
    ent->checkpoints->tail = temp;
    
    if(status == 'I') {
        ent->excluded = 1;
    }
    
    update_status(ent, event);
}