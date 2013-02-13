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
 * @brief This function calculates the time the entrant has used so far. The
 * time is calculated by substracting the current time with the start time.
 * Error checking is then done on the result to counter in the cases where 
 * the time used is less than a whole hour.
 * 
 * @param ent Entrant structure used to get the start time, current time and
 * updating the total_time member.
 * 
 */
void calc_time(ENTRANT *ent) {
    CP_TIME *current = ent->checkpoints->head;
    int mc_min = 0, hour, min;

    while (current != NULL) {
        if (current->medical && current->departure != NULL) {
            int thour, tmin;

            thour = current->departure->hours - current->time->hours;
            tmin = current->departure->minutts - current->time->minutts;

            if (thour < 0) {
                thour = 24 + thour;
                if (tmin < 0) {
                    tmin = tmin * -1;
                }
            }

            if (tmin < 0) {
                tmin = 60 + tmin;
                tmin -= 1;
            }

            tmin += 60 * thour;
            mc_min += tmin;
        }

        current = current->next;
    }

    current = ent->checkpoints->tail;

    if (current->medical && current->departure != NULL) {
        hour = current->departure->hours - ent->start_time->hours;
        min = current->departure->minutts - ent->start_time->minutts;
    } else {
        hour = current->time->hours - ent->start_time->hours;
        min = current->time->minutts - ent->start_time->minutts;
    }

    if (mc_min > 60) {
        hour = hour - ((int) mc_min / 60);
        min = mc_min % 60;
    }
    else {
        min -= mc_min;

        if (min < 0) {
            min = 60 + min;
            hour -= 1;
        }
    }

    sprintf(ent->total_time, "%dH %dM", hour, min);
}

/**
 * @brief Extracts the time from a string, eg 07:30, and stores it in the 
 * TIME_STRUCT type.
 *  
 * @param time_str The string containing the time you want to extract.
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
 * ID Name Course Status Start time Total time
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
        calc_time(entrant);
        printf(ENT_FORMAT,
                entrant->comp_number,
                entrant->name,
                entrant->course_id,
                entrant->status,
                entrant->start_time->time_str,
                entrant->total_time);
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
    } else {
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
    } else {
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
    } else {
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
 * Prints out the a list of entrants that has been excluded from the race.
 * The mc parameter is used to determine if entrants that has been excluded
 * for medical reason should be printed out, or entrants who have been excluded
 * for taking the wrong turn somewhere. 
 *  
 * @param event Event structure containing the list of entrants.
 * 
 * @param mc If 1 entrants that has been excluded for medical reasons will be 
 * printed out. If 0 entrants which has been excluded for taking the wrong turn
 * will be printed out.
 */
void print_excluded(EVENT *event, int mc) {
    int found = 0;
    ENTRANT *current = event->entrants->head;

    print_entrant_header();

    while (current != NULL) {
        if (current->mc_excluded && mc) {
            print_entrant(current);
            found += 1;
        }
        else if (current->excluded && mc == 0) {
            print_entrant(current);
            found += 1;
        }

        current = current->next;
    }

    if (found == 0) {
        printf("No entrants has been excluded so far.\n");
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
 * finished 0 is returned. If the entrant has taken a wrong turn somewhere and
 * checked in at a wrong check point -1 is returned.
 */
int entrant_finished(EVENT *event, ENTRANT *entrant) {
    int i, elements = 0, *cp_nodes;
    CP_TIME *current = entrant->checkpoints->head;
    COURSE *course = search_course_id(event->courses, entrant->course_id);

    cp_nodes = (int *) calloc(course->elements, sizeof (int));
    if (cp_nodes == NULL) {
        fprintf(stderr, "Calloc() call failed.\n");

        exit(0);
    }

    //Extract all check points and medical points.
    for (i = 0; i < course->elements; i++) {
        NODE *node = node_id_search(course->nodes[i], event->nodes);

        if (strcmp("CP", node->node_type) == 0) {
            cp_nodes[elements] = course->nodes[i];
            elements += 1;
        }

        if (strcmp("MC", node->node_type) == 0) {
            cp_nodes[elements] = course->nodes[i];
            elements += 1;
        }
    }

    //Loop checks for a wrong turn somewhere.
    if (entrant->checkpoints->size < elements) {
        for (i = 0; i < entrant->checkpoints->size; i++) {
            if (cp_nodes[i] != current->node->node_id) {
                return -1;
            }

            current = current->next;
        }

        return 0;
    }

    
    for (i = 0; i < elements; i++) {
        if (cp_nodes[i] != current->node->node_id) {
            return -1;
        }

        current = current->next;
    }

    return 1;
}

/**
 * @brief This function is used to add a check point to a entrant. The function
 * will create a check point list in the entrant if this is the entrant's first
 * check point. If the check point status is 'I' the entrant will be marked as 
 * excluded by this function. Total time of the entrant will also be updated.
 * For adding medical checkpoints see add_mc.
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
    CP_TIME *temp = calloc(1, sizeof (CP_TIME));

    if (temp == NULL) {
        fprintf(stderr, "Call to calloc() failed.\n\n");

        exit(0);
    }

    //Need to create the list in the entrant if this is the first checkpoint.
    if (ent->checkpoints == NULL) {
        list = calloc(1, sizeof (CP_LIST));
        if (list == NULL) {
            fprintf(stderr, "Call to calloc() failed.\n\n");

            exit(0);
        }

        list->size = 1;
        ent->checkpoints = list;
        ent->start_time = format_time(time);
        list->head = temp;
    } else {
        TIME_STRUCT *ct = (TIME_STRUCT *) calloc(1, sizeof (TIME_STRUCT));
        if (ct == NULL) {
            fprintf(stderr, "Call to calloc() failed.\n\n");

            exit(0);
        }
        ent->checkpoints->size += 1;
        ct = format_time(time);
        temp->time = ct;
        calc_time(ent);
        ent->checkpoints->tail->next = temp;

    }

    temp->node = node;
    temp->status = status;
    temp->time = format_time(time);
    temp->medical = 0;
    //ent->start_time = format_time(time);
    ent->current_node = node;
    ent->last_cp = temp;
    ent->checkpoints->tail = temp;

    if (status == 'I') {
        ent->excluded = 1;
    }


    update_status(ent, event);
}

/**
 * @brief This function is used to add a medical check point to a entrant.
 * The function will create a check point list in the entrant if this is the
 * entrant's first check point. If the check point status is 'E' the entrant
 * will be marked as excluded by this function. Total time of the entrant will
 * also be updated. For adding normal checkpoints see add_cp.
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
void add_mc(EVENT *event, char status, NODE *node, ENTRANT *ent, char *time) {
    if (status == 'D') {
        CP_TIME *cp = ent->checkpoints->tail;
        cp->departure = format_time(time);
        calc_time(ent);
        update_status(ent, event);

        return;
    }

    CP_LIST *list = NULL;
    CP_TIME *temp = calloc(1, sizeof (CP_TIME));

    if (temp == NULL) {
        fprintf(stderr, "Call to calloc() failed.\n\n");

        exit(0);
    }

    //Need to create the list in the entrant if this is the first cp.
    if (ent->checkpoints == NULL) {
        list = calloc(1, sizeof (CP_LIST));
        if (list == NULL) {
            fprintf(stderr, "Call to calloc() failed.\n\n");

            exit(0);
        }

        list->size = 1;
        ent->checkpoints = list;
        ent->start_time = format_time(time);
        list->head = temp;
    } else {
        TIME_STRUCT *ct = (TIME_STRUCT *) calloc(1, sizeof (TIME_STRUCT));
        if (ct == NULL) {
            fprintf(stderr, "Call to calloc() failed.\n\n");

            exit(0);
        }
        ent->checkpoints->size += 1;
        ct = format_time(time);
        temp->time = ct;
        temp->medical = 1;
        ent->checkpoints->tail->next = temp;
        ent->checkpoints->tail = temp;
        calc_time(ent);
    }

    temp->node = node;
    temp->status = status;
    temp->time = format_time(time);

    ent->current_node = node;
    ent->last_cp = temp;
    ent->checkpoints->tail = temp;

    if (status == 'E') {
        ent->mc_excluded = 1;
    }
}