/**
 * @file functions.h
 * 
 * @brief This file contains definitions of various functions.
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * 
 * @date 2012.11.30
 */

#include "node.h"
#include "track.h"
#include "event.h"
#include "entrant.h"
#include "time_struct.h"

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    
    TRACK_LIST *read_tracks(char *path, NODE_LIST *list);
    TIME_STRUCT *format_time(char *time_str);
    void calc_time(TIME_STRUCT *start, TIME_STRUCT *now, char *total_time);
    int read_event_data(char *path, EVENT *event);
    void update_status(ENTRANT *ent, EVENT *event);
    void not_started(EVENT *event);
    void finished(EVENT *event);
    void out_track(EVENT *event);
    void print_all(EVENT *event);
    int entrant_finished(EVENT *event, ENTRANT *entrant);
    void add_cp(EVENT *event, char status, NODE *node, ENTRANT *ent, char *time);
    
#ifdef	__cplusplus
}
#endif

#endif	/* FUNCTIONS_H */

