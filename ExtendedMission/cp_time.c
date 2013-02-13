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
