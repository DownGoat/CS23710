/**
 * @file course.h
 * 
 * @brief This file contains the definition of the course data structure and 
 * definitions of functions used to work with the data structure.
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * 
 * @date 2012.11.25
 */

#include "entrant.h"
#include "node.h"
#include "track.h"

#ifndef COURSE_H
#define	COURSE_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * @brief This data structure holds information about a course in the event.
     * A course consists of several nodes and a single capital letter to 
     * identify the course. This data structure is also used as a linked list,
     * and so has a pointer to the next element in the list.
     */
    typedef struct course {
        /*@{*/
        /**
         * @brief The course id. The id is a single capital letter character.
         */
        char course_id;
        
        /**
         * @brief The number of nodes in the course.
         */
        int nodes_size;
        
        /**
         * @brief Pointer to the next element in the list.
         */
        struct course *next;
        
        /**
         * @brief Number of check points where time is recorded in the course.
         */
        int time_cp;
        
        /**
         * @brief Number of node elements currently in the nodes array.
         */
        int elements;
        
        /**
         * @brief Array of nodes in the course.
         */
        int nodes[100];
        /*@}*/
    } COURSE;
    
    /**
     * @brief This is the course list. It is used to easily pass around the list
     * and information about it to make working with it easier. 
     */
    typedef struct course_list {
        /*@{*/
        /**
         * @brief The head of the list.
         */
        COURSE *head;
        
        /**
         * @brief The tail of the list.
         */
        COURSE *tail;
        
        /**
         * @brief A pointer to a NODE_LIST containing nodes that are used in
         * this course. The list may also contain nodes that are not used in 
         * this course.
         */
        NODE_LIST *node_list;
        
        /**
         * @brief A pointer to a TRACK_LIST containing tracks that are used in
         * this course. The list may also contain tracks that are not used in
         * this course.
         */
        TRACK_LIST *track_list;
        
        /**
         * @brief A pointer to a ENTRANT array containing entrants that are
         * participating in this course. The list may also contain entrants that
         * are not participating in this course.
         */
        ENTRANT *entrants;
        
        /**
         * @brief The size of the course list.
         */
        int size;
        /*@}*/
    } COURSE_LIST;

    
    //Function definitions
    int validate_course(COURSE course, NODE_LIST *node_list);
    COURSE_LIST *read_courses(char *path, NODE_LIST *node_list, TRACK_LIST *track_list);
    COURSE *search_course_id(COURSE_LIST *list, char id);

#ifdef	__cplusplus
}
#endif

#endif	/* COURSE_H */

