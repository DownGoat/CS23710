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

#ifdef	__cplusplus
}
#endif

#endif	/* TRACK_H */

