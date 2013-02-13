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

