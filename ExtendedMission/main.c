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

