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

/**
 * @file event.c
 * 
 * @brief This file contains function used to work with the event data structure
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 *
 * @date 2012.12.02
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "event.h"
#include "cp_time.h"


extern int errno;

/**
 * @brief This function opens a file containing the name of the event, the date
 * and, the time. The file has 3 lines each no longer than 79 characters.
 * 
 * @param path Path to the file to read.
 * 
 * @return Returns 1 if successful, a negative integer otherwise.
 */
EVENT *read_event(char *path) {
    EVENT *event = NULL;
    FILE *file = fopen(path, "rb");
    
    event = (EVENT *) calloc(1, sizeof(EVENT));
    if(event == NULL) {
        fprintf(stderr, "[!] A call to calloc() failed.\n\n");
        
        exit(0);
    }
    
    if(file == NULL) {
        fprintf(stderr, "[!] Something went wrong with reading the file '%s'.\n", path);
        fprintf(stderr, "System error code is %d.\n\n", errno);
        
        return NULL;
    }
    
    //line counter.
    int line_count = 1;

    if(fgets(event->name, sizeof(event->name), file) == NULL) {
        fprintf(stderr, "[!] Something went wrong with reading the file '%s'.\n", path);
        fprintf(stderr, "System error code is %d.\n\n", errno);

        fclose(file);
        return NULL;
    }

    line_count += 1;
    if(fgets(event->date, sizeof(event->date), file) == NULL) {
        fprintf(stderr, "[!] Something went wrong with reading the file '%s'.\n", path);
        fprintf(stderr, "System error code is %d.\n\n", errno);
        
        fclose(file);
        return NULL;
    }

    line_count += 1;
    if(fgets(event->time, sizeof(event->time), file) == NULL) {
        fprintf(stderr, "[!] Something went wrong with reading the file '%s'.\n", path);
        fprintf(stderr, "System error code is %d.\n\n", errno);
        
        fclose(file);
        return NULL;
    }   
    
    fclose(file);
    
    return event;
}

/** 
 * @file entrant.c
 * 
 * @brief This file holds the functions used to work with the ENTRANT data
 * structure.
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * 
 * @date 2012.11.20
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "entrant.h"
#include "event.h"
#include "course.h"
#include "node.h"
#include "functions.h"

extern int errno;

/**
 * @brief This function opens a file containing the data about the entrants. 
 * The function does some simple error checking on the file, and will
 * return a negative integer on failure. The Data should include a positive
 * integer that represents a id. A single capital character which is the id of
 * the course the entrant has signed up for. Followed by a no longer than 80
 * characters long name.
 * 
 * @param path Path to the file you want to open.
 * 
 * @return Returns 1 if successful, a negative integer otherwise.
 */
ENTRANT_LIST *read_entrants(char *path) {
    FILE *file = fopen(path, "rb");
    ENTRANT_LIST *entrants = NULL;
    ENTRANT *current = NULL;

    entrants = (ENTRANT_LIST *) calloc(1, sizeof (ENTRANT_LIST));
    current = (ENTRANT *) calloc(1, sizeof (ENTRANT));
    if (entrants == NULL || current == NULL) {
        fprintf(stderr, "[!] A call to calloc() failed, cannot continue.\n\n");
        fprintf(stderr, "System error code is %d.\n\n", errno);

        exit(0);
    }

    if (file == NULL) {
        fprintf(stderr, "[!] Something went wrong with reading the file '%s'.\n", path);
        fprintf(stderr, "System error code is %d.\n\n", errno);

        return NULL;
    }

    //line counter.
    int line_count = 0;

    while (!feof(file)) {
        int ret;
        ENTRANT *entrant = calloc(1, sizeof (ENTRANT));
        if (entrant == NULL) {
            fprintf(stderr, "[!] A calloc() call failed\n\n");
            fprintf(stderr, "System error code is %d.\n\n", errno);

            exit(0);
        }


        /*File format is: competitor_number course_id entrant_name
        The second format option will make it take any characters for the
        rest of the line. */
        ret = fscanf(file, "%d %c %49[^\n]",
                &entrant->comp_number,
                &entrant->course_id,
                entrant->name);
        if (ret == -1) {
            break;
        } else if (ret != 3) {

            fprintf(stderr, "[!] File format error on line %d.\n\n", line_count);

            return NULL;
        }

        if (entrants->head == NULL) {
            entrants->head = entrant;
            entrants->tail = entrant;
            current = entrants->tail;
            entrants->size = 1;
        } else {
            current->next = (struct entrant *) entrant;
            current = entrant;
            entrants->tail = entrant;
            entrants->size += 1;
        }

        current->excluded = 0;

        line_count += 1;
    }


    fclose(file);

    return entrants;
}

/**
 * @brief Updates the status of entrant. Valid statues are "Not Started",
 * "Finished", or the id of the last node the entrant checked in at.
 * 
 * @param ent The entrant to update.
 * @param event Event structure.
 */
void update_status(ENTRANT *ent, EVENT *event) {
    int finished;
    COURSE *course = search_course_id(event->courses, ent->course_id);

    if (course == NULL) {
        strcpy(ent->status, "nil");

        return;
    }

    finished = entrant_finished(event, ent);

    if (ent->excluded) {
        strcpy(ent->status, "Excluded");

        return;
    } else if (ent->mc_excluded) {
        strcpy(ent->status, "MC Excluded");

        return;
    } else if (ent->checkpoints == NULL) {
        strcpy(ent->status, "Not Started");

        return;
    } else if (ent->checkpoints->size == 1) {
        strcpy(ent->status, "Started");

        return;
    } else if (finished == 1) {
        strcpy(ent->status, "Finished");

        return;
    } else if (finished == -1) {
        ent->excluded = 1;
        strcpy(ent->status, "Excluded");

        return;
    } else {
        sprintf(ent->status, "%d", ent->current_node->node_id);
        return;
    }
}

/**
 * @brief Searches the entrant list for a entrant that has the passed ID.
 * 
 * @param entrant_id The ID of the entrant to search for.
 * 
 * @param entrants List of entrants.
 * 
 * @return Returns a pointer to the entrant if found. If no entrant is found 
 * NULL is returned. 
 */
ENTRANT *entrant_id_search(int entrant_id, ENTRANT_LIST *entrants) {
    ENTRANT *current = entrants->head;

    do {

        if (current->comp_number == entrant_id) {
            return current;
        }

        current = current->next;
    } while (current->next != NULL);

    if (current->comp_number == entrant_id) {
        return current;
    }

    return NULL;
}

/**
 * @brief Search the entrant list by name. 
 * 
 * @param name Name of the entrant you are searching for.
 * 
 * @param entrants List of entrants.
 * 
 * @return Returns a pointer to the entrant if found. If no entrant is found 
 * NULL is returned. 
 */
ENTRANT *entrant_name_search(char *name, ENTRANT_LIST *entrants) {
    ENTRANT *current = entrants->head;

    do {
        if (strcmp(name, current->name) == 0) {
            return current;
        }

        current = current->next;
    } while (current->next != NULL);

    if (strcmp(name, current->name) == 0) {
        return current;
    }

    return NULL;
}

/**
 * @file cp_time.c
 * 
 * @brief This file contains functions used to work with the CP_TIME data
 * structure.
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * 
 * @date 2012.12.06
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "cp_time.h"
#include "node.h"
#include "event.h"
#include "functions.h"

/**
 * @brief This function reads the check point data from a file.
 * 
 * @param path Path to the file containing the checkpoint data.
 * 
 * @param event Event structure.
 * 
 * @return On success this function returns 1, 0 is returned on failure. 
 */
int read_event_data(char *path, EVENT *event) {
    int line_count = 1;
    FILE *file = fopen(path, "rb");

    if (file == NULL) {
        fprintf(stderr, "[!] Error when reading file '%s'.\n", path);
        fprintf(stderr, "System error code is %d.\n\n", errno);
    }

    while (!feof(file)) {
        int ret, node_id, entrant_id;
        NODE *node = NULL;
        ENTRANT *entrant = NULL;
        char time[20];
        char status;

        ret = fscanf(file, " %c %d %d %5s",
                &status,
                &node_id,
                &entrant_id,
                time);


        // Error checking on fscanf return value.
        if (ret == -1) { //The return value can be -1 when only a newline i read.
            break;
        }

        if (ret != 4) {
            fprintf(stderr, "[!] Format error on line '%d'\n", line_count);
            return 0;
        }

        node = node_id_search(node_id, event->nodes);
        if (node == NULL) {
            fprintf(stderr, "[!] Format error on line '%d'\n", line_count);
            fprintf(stderr, "Node with '%d' does not exist.\n\n", node_id);

            return 0;
        }

        entrant = entrant_id_search(entrant_id, event->entrants);
        if (entrant == NULL) {
            fprintf(stderr, "[!] Format error on line '%d'\n", line_count);
            fprintf(stderr, "Entrant with id '%d' does not exist.\n\n", entrant_id);

            return 0;
        }

        if (entrant->excluded == 0 && entrant->mc_excluded == 0) {
            if (status == 'A' || status == 'D' || status == 'E') {
                add_mc(event, status, node, entrant, time);
            } else {
                add_cp(event, status, node, entrant, time);
            }
        }

        update_status(entrant, event);

        line_count += 1;
        //printf("%c %d %d %s\n", temp->status, node_id, entrant_id, temp->time_str);
    }

    return 1;
}
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

/*
        fgets(node_line, 1024, file);
*/
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
            
            if(strcmp(node->node_type, "MC") == 0) {
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

/**
 * @file main.c
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * @date 2012.11.20
 */

#include <stdio.h>
#include <stdlib.h>
#include "entrant.h"
#include "functions.h"
#include "input.h"
#include "node.h"
#include "event.h"
#include "cp_time.h"
#include "track.h"
#include "time_struct.h"
#include "course.h"


int main() {
    int sm_ret;
    EVENT *event = NULL;

    event = get_event();
    event->nodes = get_nodes();
    event->tracks = get_tracks(event->nodes);
    event->courses = get_courses(event->nodes, event->tracks);
    event->entrants = get_entrants();

/*
    event = read_event("data/e2/name.txt");
    event->nodes = read_nodes("data/e2/nodes.txt");
    event->tracks = read_tracks("data/e2/tracks.txt", event->nodes);
    event->courses = read_courses("data/e2/courses.txt", event->nodes, event->tracks);
    event->entrants = read_entrants("data/e2/entrants.txt");
*/

    //Main program loop.
    do {
        sm_ret = show_menu();

        if (sm_ret == 1) {
            ENTRANT *ent = NULL;
            ent = query_entrant(event);

            if (ent == NULL) {
                continue;
            }

            update_status(ent, event);

            print_entrant_header();
            print_entrant(ent);
        }
        else if (sm_ret == 2) {
            load_cp_data(event);
        }
        else if (sm_ret == 3) {
            print_all(event);
        }
        else if (sm_ret == 4) {
            not_started(event);
        }
        else if (sm_ret == 5) {
            finished(event);
        }
        else if (sm_ret == 6) {
            out_track(event);
        }
        else if (sm_ret == 7) {
            print_excluded(event, 0);
        }
        else if (sm_ret == 8) {
            print_excluded(event, 1);

        }
        else if (sm_ret == 0) {
            return EXIT_SUCCESS;
        }

    } while (1);

    return EXIT_SUCCESS;
}

/**
 * @file input.c
 * 
 * @brief This file holds functions related to getting input from the user.
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 *
 * @date 2012.12.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrant.h"
#include "node.h"
#include "track.h"
#include "course.h"
#include "functions.h"

/**
 * @brief This function asks the user a question and takes a single integer 
 * between a set range as input. The function will repeat the question until a
 * valid integer is entered as input.
 * 
 * @param question String with the question you want to ask the user.
 * 
 * @param min Minimum limit of the range.
 * 
 * @param max Maximum limit of the range.
 * 
 * @return The integer value the user inputted. 
 */
int ask_int(char *question, int min, int max) {
    do {
        int ret, response;
        printf("%s\n>> ", question);

        ret = scanf(" %d", &response);
        if (ret == 1) {
            if (response <= max && response >= min) {
                return response;
            }
        }
    } while (1);
}

/**
 * @brief This function ask the user a question and takes a line or up to 255
 * characters of input. The input is copied into a buffer which should not be
 * smaller than 256 characters.
 * 
 * @param question String with the question you want to ask the user.
 * 
 * @param response The buffer the response is copied into. This buffer should 
 * not be smaller than 256 characters.
 */
void ask_str(char *question, char *response) {
    do {
        char temp[256];
        int ret;

        printf("%s\n>> ", question);

        ret = scanf(" %255[^\t\n]", temp);
        if (ret == 1) {
            strcpy(response, temp);
            return;
        }
    } while (1);
}

/**
 * @brief This function prompts the user with a question asking for the path to
 * a file containing data on entrants. The Data should include a positive
 * integer that represents a id. A single capital character which is the id of
 * the course the entrant has signed up for. Followed by a no longer than 80
 * characters long name.
 * 
 * @return The function returns a pointer to a ENTRANT array if the function 
 * was successful. If it fails the function returns NULL.
 */
ENTRANT_LIST *get_entrants() {
    char response[255];

    do {
        ENTRANT_LIST *temp = NULL;

        printf("Please enter a path to a file containing entrant data:");
        scanf(" %[^\t\n]", response);

        temp = read_entrants(response);
        if (temp != NULL) {
            printf("[i] %d entrants loaded from '%s'.\n\n",
                    temp->size,
                    response);

            return temp;
        }

    } while (1);
}

/**
 * @brief This function prompts the user with a question asking for the path to
 * a file containing data on nodes. The data should contain a node ID and a 
 * short string used to identify what type of node the node is.
 * 
 * @return The function returns a pointer to a NODE_LIST, the NODE_LIST will be 
 * populated with data if successful. If it fails NULL is returned.
 */
NODE_LIST *get_nodes() {
    char response[255];

    do {
        NODE_LIST *temp = NULL;

        printf("Please enter a path to a file containing node data:\n>>> ");
        scanf(" %[^\t\n]", response);

        temp = read_nodes(response);
        if (temp != NULL) {
            printf("[i] %d nodes loaded from '%s'.\n\n", temp->size, response);

            return temp;
        }

    } while (1);
}

/**
 * @brief This function prompts the user with a question asking for the path to 
 * a file containing data on tracks. The file should have data about paths
 * between nodes and the maximum time a entrant should use between the nodes.
 * 
 * @param node_list A pointer to a NODE_LIST, it should be populated. The nodes
 * are needed to ensure that the tracks does not contain none existing nodes.
 * 
 * @return This function returns a pointer to a TRACK_LIST if successful. If the
 * function fails NULL is returned.
 */
TRACK_LIST *get_tracks(NODE_LIST *node_list) {
    char response[255];

    do {
        TRACK_LIST *temp = NULL;

        printf("Please enter a path to a file containing track data:\n>>> ");
        scanf(" %[^\t\n]", response);

        temp = read_tracks(response, node_list);
        if (temp != NULL) {
            printf("[i] %d tracks loaded from '%s'.\n\n", temp->size, response);

            return temp;
        }

    } while (1);
}

/**
 * @brief Prompts the user with a question about the path to the file containing
 * the date about the event. The file should have date about the name event, 
 * start time and date.
 * 
 * @return If the function is successful it will return a pointer to a event
 *  populated with the data from the file. If i fails NULL is returned. 
 */
EVENT *get_event() {
    char response[255];

    do {
        EVENT *temp = NULL;

        printf("Please enter a path to a file containing event name and date:\n>>> ");
        scanf(" %[^\t\n]", response);

        temp = read_event(response);
        if (temp != NULL) {
            printf("[i] Event loaded!\n\n");

            return temp;
        }

    } while (1);
}

/**
 * @brief Prompts the user about the path to a file containing data about
 * courses. The data should consist of a single capital character which will be
 * the course ID. Followed by a positive integer which will be the number of 
 * nodes for the course. The rest of the line should be the ID of each node 
 * separated by spaces. 
 * 
 * @param nodes Pointer to a populated linked list of nodes. The nodes are
 * needed to verify that the course is made of known nodes.
 * 
 * @param tracks Pointer to a populated linked list of tracks. The tracks are
 * needed to verify that there is a valid path from start to goal.
 * 
 * @return The function returns a pointer to a linked list of courses if
 * successful. If it fails NULL is returned.
 */
COURSE_LIST *get_courses(NODE_LIST *nodes, TRACK_LIST *tracks) {
    char response[255];

    do {
        COURSE_LIST *temp = NULL;

        printf("Please enter a path to a file containing courses:\n>>> ");
        scanf(" %[^\t\n]", response);

        temp = read_courses(response, nodes, tracks);
        if (temp != NULL) {
            printf("[i] %d courses loaded from '%s'.\n\n", temp->size, response);

            return temp;
        }

    } while (1);
}

/**
 * @brief Prompts the user with the question to enter the path to the filename
 * containing the checkpoint data.
 * 
 * @param event The result are stored in this variable.
 */
void get_cp_data(EVENT *event) {
    do {
        char response[256];
        int ret;

        printf("\nPlease enter a path to a file containing check point data:");
        printf("\n0. Back.\n>>> ");

        scanf(" %[^\t\n]", response);

        if (strcmp(response, "0") == 0) {
            return;
        }

        ret = read_event_data(response, event);
        if (ret) {
            printf("[i] Checkpoint data loaded.\n\n");

            return;
        }
    } while (1);
}

/**
 * @brief Prompts the user with the choice of loading the check point data from
 * file or entering it manually.
 * 
 * @param event Event data structure. The result will be stored in this 
 * variable.
 */
void load_cp_data(EVENT *event) {
    int i_response;
    do {
        i_response = ask_int("\n1. From file.\n2. Enter manually:\n0. Back", 0, 2);
        if (i_response == 0) {
            return;
        } else if (i_response == 1) {
            get_cp_data(event);
            return;
        } else if (i_response == 2) {

            do {
                int ret, node_id, entrant_id;
                NODE *node = NULL;
                ENTRANT *entrant = NULL;
                char time[20];
                char status;

                printf("Enter the cp data:\n>> ");
                ret = scanf(" %c %d %d %5s",
                        &status,
                        &node_id,
                        &entrant_id,
                        time);

                if (ret != 4) {
                    fprintf(stderr, "[!] Format error.\n");
                    continue;
                }

                node = node_id_search(node_id, event->nodes);
                if (node == NULL) {
                    fprintf(stderr, "[!] Format error.\n");
                    fprintf(stderr, "Node with '%d' does not exist.\n\n", node_id);

                    continue;
                }

                entrant = entrant_id_search(entrant_id, event->entrants);
                if (entrant == NULL) {
                    fprintf(stderr, "[!] Format error.\n");
                    fprintf(stderr, "Entrant with id '%d' does not exist.\n\n", entrant_id);

                    continue;
                }

                if (entrant->excluded == 0 && entrant->mc_excluded == 0) {
                    if (status == 'A' || status == 'D' || status == 'E') {
                        add_mc(event, status, node, entrant, time);
                    } else {
                        add_cp(event, status, node, entrant, time);
                    }
                }

                //add_cp(event, status, node, entrant, time);
                return;
            } while (1);
        }
    } while (1);
}

/**
 * @brief This function prints out the main user menu the user will use to
 * interact with the different parts of the program . The user is given the
 * choice to go to several different sub menus. The function takes a integer
 * as input and returns the input.
 * 
 * @return User inputed integer.
 */
int show_menu() {
    do {
        int response, ret;

        printf("\nWhat do you wish to do?\n");
        printf("1. Current status of a entrant.\n");
        printf("2. Load checkpoint data.\n");
        printf("3. Show entrants list.\n");
        printf("4. Who have not started?\n");
        printf("5. Who have finished?\n");
        printf("6. Who is out on the track?\n");
        printf("7. Print entrants excluded for taking the wrong track.\n");
        printf("8. Print entrants excluded for failing a medical check point\n");
        printf("0. Exit.\n");

        printf(">> ");
        ret = scanf(" %d", &response);

        if (ret == 1) {
            return response;
        }
    } while (1);
}

/**
 * @brief Searches for a entrant either by name or id given by the user.
 * The function prompts the user with the choice of searching by name or by
 * the entrant id.
 * 
 * @param event Event structure. The result will be stored in this variable.
 * 
 * @return If successful the function returns a pointer to the entrant. If it
 * fails NULL is returned.
 */
ENTRANT *query_entrant(EVENT *event) {
    int response, ret;

    do {
        printf("\nSearch by name or entrant number?\n");
        printf("1. Number.\n2. Name.\n\n0. Back\n>> ");
        ret = scanf(" %d", &response);

        if (ret != 1) {
            continue;
        }

        if (response == 1) {
            response = ask_int("\nPlease enter a entrant ID:\n0. Back.",
                    0, event->entrants->size);
            if (response == 0) {
                return NULL;
            }
            return entrant_id_search(response, event->entrants);
        } else if (response == 2) {
            char response_str[256];

            ask_str("\nPlease enter entrant name:\n0. Back", response_str);
            if (strcmp(response_str, "0") == 0) {
                return NULL;
            }

            return entrant_name_search(response_str, event->entrants);
        } else if (response == 0) {
            return NULL;
        }

    } while (1);
}