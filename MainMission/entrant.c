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
    COURSE *course = search_course_id(event->courses, ent->course_id);

    if (course == NULL) {
        strcpy(ent->status, "nil");

        return;
    }

    if (ent->excluded) {
        strcpy(ent->status, "Excluded");

        return;
    }
    else if (ent->checkpoints == NULL) {
        strcpy(ent->status, "Not Started");

        return;
    } else if (ent->checkpoints->size == 1) {
        strcpy(ent->status, "Started");

        return;
    } else if (entrant_finished(event, ent)) {
        strcpy(ent->status, "Finished");

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
