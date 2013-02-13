/**
 * @file event.c
 * 
 * @brief This file contains function used to work with the event data structure
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 *
 * @date 2012.12.02
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "event.h"
#include "cp_time.h"


extern int errno;

/**
 * @brief This function opens a file containing the name of the event, the date
 * and, the time. The file has 3 lines each no longer than 79 characters.
 * 
 * @param path Path to the file to read.
 * 
 * @return Returns 1 if successful, a negative integer otherwise.
 */
EVENT *read_event(char *path) {
    EVENT *event = NULL;
    FILE *file = fopen(path, "rb");
    
    event = (EVENT *) calloc(1, sizeof(EVENT));
    if(event == NULL) {
        fprintf(stderr, "[!] A call to calloc() failed.\n\n");
        
        exit(0);
    }
    
    if(file == NULL) {
        fprintf(stderr, "[!] Something went wrong with reading the file '%s'.\n", path);
        fprintf(stderr, "System error code is %d.\n\n", errno);
        
        return NULL;
    }
    
    //line counter.
    int line_count = 1;

    if(fgets(event->name, sizeof(event->name), file) == NULL) {
        fprintf(stderr, "[!] Something went wrong with reading the file '%s'.\n", path);
        fprintf(stderr, "System error code is %d.\n\n", errno);

        fclose(file);
        return NULL;
    }

    line_count += 1;
    if(fgets(event->date, sizeof(event->date), file) == NULL) {
        fprintf(stderr, "[!] Something went wrong with reading the file '%s'.\n", path);
        fprintf(stderr, "System error code is %d.\n\n", errno);
        
        fclose(file);
        return NULL;
    }

    line_count += 1;
    if(fgets(event->time, sizeof(event->time), file) == NULL) {
        fprintf(stderr, "[!] Something went wrong with reading the file '%s'.\n", path);
        fprintf(stderr, "System error code is %d.\n\n", errno);
        
        fclose(file);
        return NULL;
    }   
    
    fclose(file);
    
    return event;
}

