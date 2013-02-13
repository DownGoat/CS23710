/**
 * @file input.c
 * 
 * @brief This file holds functions related to getting input from the user.
 * 
 * @author Sindre Smistad <sis13@aber.ac.uk>
 *
 * @date 2012.12.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrant.h"
#include "node.h"
#include "track.h"
#include "course.h"
#include "functions.h"

/**
 * @brief This function asks the user a question and takes a single integer 
 * between a set range as input. The function will repeat the question until a
 * valid integer is entered as input.
 * 
 * @param question String with the question you want to ask the user.
 * 
 * @param min Minimum limit of the range.
 * 
 * @param max Maximum limit of the range.
 * 
 * @return The integer value the user inputted. 
 */
int ask_int(char *question, int min, int max) {
    do {
        int ret, response;
        printf("%s\n>> ", question);

        ret = scanf(" %d", &response);
        if (ret == 1) {
            if (response <= max && response >= min) {
                return response;
            }
        }
    } while (1);
}

/**
 * @brief This function ask the user a question and takes a line or up to 255
 * characters of input. The input is copied into a buffer which should not be
 * smaller than 256 characters.
 * 
 * @param question String with the question you want to ask the user.
 * 
 * @param response The buffer the response is copied into. This buffer should 
 * not be smaller than 256 characters.
 */
void ask_str(char *question, char *response) {
    do {
        char temp[256];
        int ret;

        printf("%s\n>> ", question);

        ret = scanf(" %255[^\t\n]", temp);
        if (ret == 1) {
            strcpy(response, temp);
            return;
        }
    } while (1);
}

/**
 * @brief This function prompts the user with a question asking for the path to
 * a file containing data on entrants. The Data should include a positive
 * integer that represents a id. A single capital character which is the id of
 * the course the entrant has signed up for. Followed by a no longer than 80
 * characters long name.
 * 
 * @return The function returns a pointer to a ENTRANT array if the function 
 * was successful. If it fails the function returns NULL.
 */
ENTRANT_LIST *get_entrants() {
    char response[255];

    do {
        ENTRANT_LIST *temp = NULL;

        printf("Please enter a path to a file containing entrant data:");
        scanf(" %[^\t\n]", response);

        temp = read_entrants(response);
        if (temp != NULL) {
            printf("[i] %d entrants loaded from '%s'.\n\n",
                    temp->size,
                    response);

            return temp;
        }

    } while (1);
}

/**
 * @brief This function prompts the user with a question asking for the path to
 * a file containing data on nodes. The data should contain a node ID and a 
 * short string used to identify what type of node the node is.
 * 
 * @return The function returns a pointer to a NODE_LIST, the NODE_LIST will be 
 * populated with data if successful. If it fails NULL is returned.
 */
NODE_LIST *get_nodes() {
    char response[255];

    do {
        NODE_LIST *temp = NULL;

        printf("Please enter a path to a file containing node data:\n>>> ");
        scanf(" %[^\t\n]", response);

        temp = read_nodes(response);
        if (temp != NULL) {
            printf("[i] %d nodes loaded from '%s'.\n\n", temp->size, response);

            return temp;
        }

    } while (1);
}

/**
 * @brief This function prompts the user with a question asking for the path to 
 * a file containing data on tracks. The file should have data about paths
 * between nodes and the maximum time a entrant should use between the nodes.
 * 
 * @param node_list A pointer to a NODE_LIST, it should be populated. The nodes
 * are needed to ensure that the tracks does not contain none existing nodes.
 * 
 * @return This function returns a pointer to a TRACK_LIST if successful. If the
 * function fails NULL is returned.
 */
TRACK_LIST *get_tracks(NODE_LIST *node_list) {
    char response[255];

    do {
        TRACK_LIST *temp = NULL;

        printf("Please enter a path to a file containing track data:\n>>> ");
        scanf(" %[^\t\n]", response);

        temp = read_tracks(response, node_list);
        if (temp != NULL) {
            printf("[i] %d tracks loaded from '%s'.\n\n", temp->size, response);

            return temp;
        }

    } while (1);
}

/**
 * @brief Prompts the user with a question about the path to the file containing
 * the date about the event. The file should have date about the name event, 
 * start time and date.
 * 
 * @return If the function is successful it will return a pointer to a event
 *  populated with the data from the file. If i fails NULL is returned. 
 */
EVENT *get_event() {
    char response[255];

    do {
        EVENT *temp = NULL;

        printf("Please enter a path to a file containing event name and date:\n>>> ");
        scanf(" %[^\t\n]", response);

        temp = read_event(response);
        if (temp != NULL) {
            printf("[i] Event loaded!\n\n");

            return temp;
        }

    } while (1);
}

/**
 * @brief Prompts the user about the path to a file containing data about
 * courses. The data should consist of a single capital character which will be
 * the course ID. Followed by a positive integer which will be the number of 
 * nodes for the course. The rest of the line should be the ID of each node 
 * separated by spaces. 
 * 
 * @param nodes Pointer to a populated linked list of nodes. The nodes are
 * needed to verify that the course is made of known nodes.
 * 
 * @param tracks Pointer to a populated linked list of tracks. The tracks are
 * needed to verify that there is a valid path from start to goal.
 * 
 * @return The function returns a pointer to a linked list of courses if
 * successful. If it fails NULL is returned.
 */
COURSE_LIST *get_courses(NODE_LIST *nodes, TRACK_LIST *tracks) {
    char response[255];

    do {
        COURSE_LIST *temp = NULL;

        printf("Please enter a path to a file containing courses:\n>>> ");
        scanf(" %[^\t\n]", response);

        temp = read_courses(response, nodes, tracks);
        if (temp != NULL) {
            printf("[i] %d courses loaded from '%s'.\n\n", temp->size, response);

            return temp;
        }

    } while (1);
}

/**
 * @brief Prompts the user with the question to enter the path to the filename
 * containing the checkpoint data.
 * 
 * @param event The result are stored in this variable.
 */
void get_cp_data(EVENT *event) {
    do {
        char response[256];
        int ret;

        printf("\nPlease enter a path to a file containing check point data:");
        printf("\n0. Back.\n>>> ");

        scanf(" %[^\t\n]", response);

        if (strcmp(response, "0") == 0) {
            return;
        }

        ret = read_event_data(response, event);
        if (ret) {
            printf("[i] Checkpoint data loaded.\n\n");

            return;
        }
    } while (1);
}

/**
 * @brief Prompts the user with the choice of loading the check point data from
 * file or entering it manually.
 * 
 * @param event Event data structure. The result will be stored in this 
 * variable.
 */
void load_cp_data(EVENT *event) {
    int i_response;
    do {
        i_response = ask_int("\n1. From file.\n2. Enter manually:\n0. Back", 0, 2);
        if (i_response == 0) {
            return;
        } else if (i_response == 1) {
            get_cp_data(event);
            return;
        } else if (i_response == 2) {

            do {
                int ret, node_id, entrant_id;
                NODE *node = NULL;
                ENTRANT *entrant = NULL;
                char time[20];
                char status;

                printf("Enter the cp data:\n>> ");
                ret = scanf(" %c %d %d %5s",
                        &status,
                        &node_id,
                        &entrant_id,
                        time);

                if (ret != 4) {
                    fprintf(stderr, "[!] Format error.\n");
                    continue;
                }

                node = node_id_search(node_id, event->nodes);
                if (node == NULL) {
                    fprintf(stderr, "[!] Format error.\n");
                    fprintf(stderr, "Node with '%d' does not exist.\n\n", node_id);

                    continue;
                }

                entrant = entrant_id_search(entrant_id, event->entrants);
                if (entrant == NULL) {
                    fprintf(stderr, "[!] Format error.\n");
                    fprintf(stderr, "Entrant with id '%d' does not exist.\n\n", entrant_id);

                    continue;
                }

                if (entrant->excluded == 0 && entrant->mc_excluded == 0) {
                    if (status == 'A' || status == 'D' || status == 'E') {
                        add_mc(event, status, node, entrant, time);
                    } else {
                        add_cp(event, status, node, entrant, time);
                    }
                }

                //add_cp(event, status, node, entrant, time);
                return;
            } while (1);
        }
    } while (1);
}

/**
 * @brief This function prints out the main user menu the user will use to
 * interact with the different parts of the program . The user is given the
 * choice to go to several different sub menus. The function takes a integer
 * as input and returns the input.
 * 
 * @return User inputed integer.
 */
int show_menu() {
    do {
        int response, ret;

        printf("\nWhat do you wish to do?\n");
        printf("1. Current status of a entrant.\n");
        printf("2. Load checkpoint data.\n");
        printf("3. Show entrants list.\n");
        printf("4. Who have not started?\n");
        printf("5. Who have finished?\n");
        printf("6. Who is out on the track?\n");
        printf("7. Print entrants excluded for taking the wrong track.\n");
        printf("8. Print entrants excluded for failing a medical check point\n");
        printf("0. Exit.\n");

        printf(">> ");
        ret = scanf(" %d", &response);

        if (ret == 1) {
            return response;
        }
    } while (1);
}

/**
 * @brief Searches for a entrant either by name or id given by the user.
 * The function prompts the user with the choice of searching by name or by
 * the entrant id.
 * 
 * @param event Event structure. The result will be stored in this variable.
 * 
 * @return If successful the function returns a pointer to the entrant. If it
 * fails NULL is returned.
 */
ENTRANT *query_entrant(EVENT *event) {
    int response, ret;

    do {
        printf("\nSearch by name or entrant number?\n");
        printf("1. Number.\n2. Name.\n\n0. Back\n>> ");
        ret = scanf(" %d", &response);

        if (ret != 1) {
            continue;
        }

        if (response == 1) {
            response = ask_int("\nPlease enter a entrant ID:\n0. Back.",
                    0, event->entrants->size);
            if (response == 0) {
                return NULL;
            }
            return entrant_id_search(response, event->entrants);
        } else if (response == 2) {
            char response_str[256];

            ask_str("\nPlease enter entrant name:\n0. Back", response_str);
            if (strcmp(response_str, "0") == 0) {
                return NULL;
            }

            return entrant_name_search(response_str, event->entrants);
        } else if (response == 0) {
            return NULL;
        }

    } while (1);
}