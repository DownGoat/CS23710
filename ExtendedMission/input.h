/**
 * @file input.h
 * 
 * @brief This file contains function definitions for input.c
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * 
 * @date 2012.11.30
 */

#include "entrant.h"

#ifndef INPUT_H
#define	INPUT_H

#ifdef	__cplusplus
extern "C" {
#endif


ENTRANT_LIST *get_entrants();
NODE_LIST *get_nodes();
TRACK_LIST *get_tracks(NODE_LIST *node_list);
EVENT *get_event();
COURSE_LIST *get_courses(NODE_LIST *nodes, TRACK_LIST *tracks);
int show_menu();
ENTRANT *query_entrant(EVENT *event);
int ask_int(char *question, int min, int max);
void ask_str(char *question, char *response);
void get_cp_data(EVENT *event);
void load_cp_data(EVENT *event);

#ifdef	__cplusplus
}
#endif

#endif	/* INPUT_H */

