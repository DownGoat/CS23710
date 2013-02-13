/**
 * @file event.h
 * @brief This file contains the definition of the event structure and other
 * variables related to the event.
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * @date 2012.11.24
 */

#include "entrant.h"
#include "node.h"
#include "track.h"
#include "course.h"

#ifndef EVENT_H
#define	EVENT_H

#ifdef	__cplusplus
extern "C" {
#endif

    /** 
     * @brief The event structure holds information about the event such as the
     * name, date, and time. The structure also has a array of entrants and a 
     * linked list of the course nodes.
     */
    typedef struct event {
        /*@{*/
        /**
         * @brief The name of the event. Name can not be longer than 80
         * characters.
         */
        char name[80];
        
        /**
         * @brief The date of the event. The date is in free text format, and no
         * longer than 80 characters.
         */
        char date[80];
        
        /**
         * @brief The time of the event
         */
        // TODO add time format to comment.
        char time[80];
        
        /**
         * @brief An array of the entrants participating in the event.
         * The size of the array is dynamic.
         */
        ENTRANT_LIST *entrants;
        
        /**
         * @brief Linked list of all the courses in the event.
         */
        NODE_LIST *nodes;
        
        /**
         * @brief List of all the tracks in this event.
         */
        TRACK_LIST *tracks;
        
        /**
         * @brief List of all the courses in this event.
         */
        COURSE_LIST *courses;
        /*@}*/
    } EVENT;
    
    
    
    EVENT *read_event(char *path);


#ifdef	__cplusplus
}
#endif

#endif	/* EVENT_H */

