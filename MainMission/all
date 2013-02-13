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

/** 
 * @file entrant.h
 * 
 * @brief This file holds structures and values needed to work with entrants.
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * 
 * @date 2012.11.20
 */


#include "cp_time.h"
#include "time_struct.h"


#ifndef ENTRANT_H
#define	ENTRANT_H

/**
 * @brief As stated in assignment, no name is longer than 50 chars including spaces.
 */
#define MAX_NAME_LENGTH 50

/**
 * @brief Minimum size of the entrant array.
 */
#define ENTRANTS_MIN_SIZE 100

/**
 * @brief The format string used when printing out data about a entrant.
 */
#define ENT_FORMAT "%-5d%-50s%-10c%-15s%-15s%-10s\n"

/**
 * @brief A typedef of a struct representing a entrant in the competition.
 */
typedef struct entrant {
    /*@{*/
    /** 
     * @brief The competitor number for the entrant. 
     */
    int comp_number;

    /** 
     * @brief The Single letter course code for the course the entrant is
     * participating in.
     */
    char course_id;

    /**
     * @brief Boolean used to denote if the entrant has been excluded from the
     * race.
     */
    int excluded;

    /**
     * @brief The name of the entrant.
     */
    char name[MAX_NAME_LENGTH];

    /**
     * @brief The node the entrant currently is at.
     */
    NODE *current_node;

    /**
     * @brief The time the entrant reached the first check point.
     */
    TIME_STRUCT *start_time;

    /**
     * @brief The time the entrant has used so far.
     */
    char total_time[20];

    /**
     * @brief The current status of the entrant. It can either be "Not Started", 
     * "Finished" or the id of the node the entrant last checked in at.
     */
    char status[20];

    /**
     * @brief Last check point the entrant was at.
     */
    CP_TIME *last_cp;

    /**
     * @brief A pointer to the next element in the linked list.
     */
    struct entrant *next;

    /**
     * @brief List of check points the entrant has been at so far.
     */
    CP_LIST *checkpoints;
    /*@}*/
} ENTRANT;

/**
 * @brief This is the entrant list. It is used to easily pass around the list
 * and information about it to make working with it easier. 
 */
typedef struct entrant_list {
    /**
     * @brief Pointer to the head of the linked list.
     */
    ENTRANT *head;

    /**
     * @brief Pointer to the tail of the linked list.
     */
    ENTRANT *tail;

    /**
     * @brief The size of the linked list.
     */
    int size;
} ENTRANT_LIST;


//Function definitions.
ENTRANT_LIST *read_entrants(char *path);
ENTRANT *entrant_id_search(int entrant_id, ENTRANT_LIST *entrants);
void print_entrant(ENTRANT *entrant);
void print_entrant_header();
ENTRANT *entrant_name_search(char *name, ENTRANT_LIST *entrants);


#endif	/* ENTRANT_H */

/**
 * @file cp_time.h
 * 
 * @brief This file contains the definition of the data structure used to 
 * represent a entrant checking in at a check point.
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * 
 * @date 2012.12.06
 */

#include "node.h"
#include "time_struct.h"

#ifndef CP_TIME_H
#define	CP_TIME_H

#ifdef	__cplusplus
extern "C" {
#endif
    

    /**
     * @brief This data structure represents a check-in at a checkpoint.
     */
    typedef struct cp_time {
        /*@{*/
        /**
         * @brief Pointer to the node that represents the checkpoint.
         */
        NODE *node;
        
        /**
         * @brief The status of the entrant when he reaches the check point.
         */
        char status;
        
        /**
         * @brief The string containing the time the entrant reached the CP.
         */
        TIME_STRUCT *time;;
        
        /**
         * @brief Pointer to the next element in the linked list.
         */
        struct cp_time *next;
        /*@}*/
    } CP_TIME;
    
    /**
     * @brief This structure is used to keep the list of CP_TIME elements.
     */
    typedef struct cp_time_list {
        /*@{*/
        /**
         * @brief Pointer to the head of the list.
         */
        CP_TIME *head;
        
        /**
         * @brief Pointer to the tail of the list.
         */
        CP_TIME *tail;
        
        /**
         * @brief Number of elements in the list.
         */
        int size;
        /*@}*/
    } CP_LIST;
    
    


#ifdef	__cplusplus
}
#endif

#endif	/* CP_TIME_H */

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

/**
 * @file track.h
 * 
 * @brief This file contains the definition of the track structure.
 *
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * 
 * @date 2012.11.24
 */


#ifndef TRACK_H
#define	TRACK_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * @brief This structure holds the information about each track.
     * The structure also has a pinter to the another track to act as a
     * linked list.
     */
    typedef struct track {
        /*@{*/
        /**
         * @brief  The ID of the track.
         */
        int track_id;
        
        /**
         * @brief The ID of the start node.
         */
        int start_id;
        
        /**
         * @brief The ID of the end node.
         */
        int end_id;
        
        /**
         * @brief The estimated time a entrant should use to go from start to end.
         */
        int time;
        
        /**
         * @brief Pointer to the next track element in the linked list.
         */
        struct track *next;
        /*@}*/
    } TRACK;
    
    /**
     * @brief This is the track list. It is used to easily pass around the list
     * and information about it to make working with it easier. 
     */
    typedef struct track_list {
        /*@{*/
        /**
         * @brief Head of the list.
         */
        TRACK *head;
        
        /**
         * @brief Tail of the list.
         */
        TRACK *tail;
        
        /**
         * @brief Size of the list.
         */
        int size;
        /*@}*/
    } TRACK_LIST;
    
    
    //Function definitions.
    int validate_track(int node_one, int node_two, TRACK track);
    void free_track(TRACK_LIST *track_list);


#ifdef	__cplusplus
}
#endif

#endif	/* TRACK_H */

/**
 * @file time_struct.h
 * 
 * @brief This file contains the definition of the data structure used to
 * represent the time structure.
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 *
 * @date 2012.12.07
 */

#ifndef TIME_STRUCT_H
#define	TIME_STRUCT_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * @brief The structure used to represent time.
     */
    typedef struct time_struct {
        /*@{*/
        int hours;
        int minutts;
        /**
         * @brief The current time in string format: xH yM
         */
        char time_str[20];
        /*@}*/
    } TIME_STRUCT;


#ifdef	__cplusplus
}
#endif

#endif	/* TIME_STRUCT_H */

/**
 * @file node.h
 * 
 * @brief This file contains the definition of the node data structure and 
 * definitions of functions used to work with the data structure.
 *
 * @author Sindre Smistad <sis13@aber.ac.uk>
 * 
 * @date 2012.11.25
 */

#include "track.h"


#ifndef COURSE_NODE_H
#define	COURSE_NODE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
//#define NORMAL_CP "CP"
//#define MEDICAL_CP "MC"
//#define JUNCTION "JN"


    /**
     * @brief The node data structure represents a checkpoint in the course.
     * There are several different checkpoints. Each node is identified by a
     * positive integer. The data structure is also a linked list and has a
     * pointer to the next element in the list.
     */
    typedef struct node {
        /*@{*/
        /**
         * @brief The node id used to identify the node. The id is a positive integer.
         */
        int node_id;
        
        /**
         * @brief The node type.
         */
        char node_type[10];
        
        /**
         * @brief Pointer to the next node element.
         */
        struct node *next;
        
        /**
         * @brief Number of track elements currently in the tracks array.
         */
        int elements;
        
        /**
         * @brief Array of tracks for this node.
         */
        TRACK tracks[100];
        /*@}*/
    } NODE;
    
    /**
     * @brief This is the node list. It is used to easily pass around the list
     * and information about it to make working with it easier. 
     */
    typedef struct node_list {
        /*@{*/
        /**
         * @brief The head of the linked list.
         */
        NODE *head;
        
        /**
         * @brief The tail of the linked list.
         */
        NODE *tail;
        
        /**
         * @brief The size of the linked list.
         */
        int size;
        /*@}*/
    } NODE_LIST;
    
    //Function definitions.
    NODE *node_id_search(int id, NODE_LIST *list);
    NODE_LIST *read_nodes(char *path);
    
    

#ifdef	__cplusplus
}
#endif

#endif	/* COURSE_NODE_H */

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

