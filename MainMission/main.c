/* 
 * File:   main.c
 * Author: sis13
 *
 * Created on November 20, 2012, 7:14 PM
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

/*
 * 
 *
 */

int main() {
    int sm_ret;
    EVENT *event = NULL;

    event = get_event();
    event->nodes = get_nodes();
    event->tracks = get_tracks(event->nodes);
    event->courses = get_courses(event->nodes, event->tracks);

    event->entrants = get_entrants();

/*
    event = read_event("data/name.txt");
    event->nodes = read_nodes("data/nodes.txt");
    event->tracks = read_tracks("data/tracks.txt", event->nodes);
    event->courses = read_courses("data/courses.txt", event->nodes, event->tracks);
    event->entrants = read_entrants("data/entrants.txt");

    read_event_data("data/cp_times_1.txt", event);*/


    do {
        sm_ret = show_menu();
        
        if(sm_ret == 1) {
            ENTRANT *ent = NULL;
            ent = query_entrant(event);

            if(ent == NULL) {
                continue;
            }
            
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
        
        else if(sm_ret == 0) {
            return EXIT_SUCCESS;
        }

    }while(1);
    
    return EXIT_SUCCESS;
}


/*
int main(int argc, char** argv) {
    test_calc_time();
    
    return 0;
}
*/
