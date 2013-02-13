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
         * @brief Is set if this check point is a medical check point.
         */
        int medical;
        
        /**
         * @brief The string containing the time the entrant reached the CP.
         */
        TIME_STRUCT *time;;
        
        /**
         * @brief The departure time from the medical check point.
         */
        TIME_STRUCT *departure;
        
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

