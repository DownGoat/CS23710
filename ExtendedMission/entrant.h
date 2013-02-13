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
     * race. This is set if the entrant has been excluded for taking the wrong
     * turn.
     */
    int excluded;
    
    /**
     * @brief Boolean used to denote if the entrant has been excluded from the
     * race. This is set if the entrant has been excluded for failing a medical
     * check point.
     */
    int mc_excluded;

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

