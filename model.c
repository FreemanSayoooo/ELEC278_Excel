#include "model.h"
#include "interface.h"
#include "string.h"
#include <stddef.h>
#include <stdlib.h>
#include "stdio.h"

// Define a structure for the linked list node
typedef struct cell_node {
    int len;//tracks the length of the data
    ROW row;//tracks the row
    COL col;//tracks the column
    char *data;//points to the data
    char *eq;//stores equations
    struct cell_node *next;//points to the next node
} node;


//has pointers initially point to NULL for initial operation
node *head = NULL;
node *current = NULL;
node *traverse = NULL;


//changes the character for the columns stored in nodes to COL_A ... COL_G so they can be compared to the current COL col
COL char_to_col(char c) {
    switch (c) {
        case 'A':
            return COL_A;
        case 'B':
            return COL_B;
        case 'C':
            return COL_C;
        case 'D':
            return COL_D;
        case 'E':
            return COL_E;
        case 'F':
            return COL_F;
        case 'G':
            return COL_G;

        default:
            // Handle the case for an invalid column
            return 0;
    }
}

//same as the function above but for the numbers
ROW char_to_row(char c) {
    switch (c) {
        case '1':
            return ROW_1;
        case '2':
            return ROW_2;
        case '3':
            return ROW_3;
        case '4':
            return ROW_4;
        case '5':
            return ROW_5;
        case '6':
            return ROW_6;
        case '7':
            return ROW_7;
        case '8':
            return ROW_8;
        case '9':
            return ROW_9;
        case '0':
            return ROW_10;


        default:
            // Handle the case for an invalid row
            return 0;
    }
}

//calculates the equation for update cell diplay
char *results(char *text, ROW row, COL col){
    //pointer for token
    char *token;
    //character value for stdtod
    char *character;
    //stores the tokenized string as doule
    double temp;
    //used to hold the total sum of the expression
    double counter = 0;
    // create a copy of the input string for tokenization when using strtok
    char *text_copy = strdup(text);
    //removes the = sign from the string
    token = strtok(text_copy, "=");
    //get the first part of the expression as a string
    token = strtok(token, "+");
    //iterates through tokens
    while(token != NULL){
        //adds up final results
        traverse = head;
        //if the first element is the character A-G to tell if were editing a cell
        if(token[0] == 'A'||token[0] == 'B'||token[0] == 'C'||token[0] == 'D'||token[0] == 'E'||token[0] == 'F'||token[0] == 'G') {
            //traverses the list to check if the cell contains content
            while (traverse != NULL) {
                //checks to see if the requested cell is part of the linked list
                if (char_to_col(token[0]) == traverse->col && char_to_row(token[1]) == traverse->row) {
                    //changes the string value of the cell from a character to a double
                    temp = strtod(traverse->data, &character);
                    //breaks after cell is found
                    break;
                    //if no cell is found sets it to zero
                } else {
                    temp = 0;
                }
                //used to traverse the list
                traverse = traverse->next;
            }
        }
        //if the first element of the cell is a number
        if(token[0] == '1'||token[0] == '2'||token[0] == '3'||token[0] == '4'||token[0] == '5'||token[0] == '6'||token[0] == '7'||token[0] == '8'||token[0] == '9'||token[0] == '0'){

            //changes the number to a double
            temp = strtod(token, &character);
        }
            //sums up counter to find total sum
            counter = counter + temp;
        //adds null to the token, so it can iterate through the rest of the string
        token = strtok(NULL,"+");
    }
    //frees memory
    free(text_copy);

    // allocate memory for the result string
    char *result_str = (char *)malloc(current->len);

    // convert the result to a string with one decimal place
    sprintf(result_str, "%.1lf", counter);

    //sets the results to data for other purposes
    current->data = result_str;


    //returns the string
    return result_str;
}

void model_init() {
    // TODO: implement this.
//No static memory used did not need to initialize any memory
}



void set_cell_value(ROW row, COL col, char *text) {
    // TODO: implement this.



    //pointer to point to head
    node* ptr = head;

    //int to evaluate if a node for a cell already exists. 1 if it exists 0 if it doesn't
    int true = 0;

    //iterates through the linked list
    while(ptr != NULL){
        //used to find if the node already exists to change the existing data
        //if the row and the col of a node is equal to the current cell
        if(ptr->row == row && ptr->col ==col) {
            //frees the memory of the existing data
            free(ptr -> data);
            //stores the length of text string to the struct +1 for null
            ptr->len =(char) strlen(text)+1;
            //allocates memory for the string
            ptr->data = (char *) malloc(ptr->len);
            //copies text to ptr->data using string copy
            strcpy(ptr->data, text);
            //if data is an equation
            if(ptr->data[0] == '=') {
                //the data is duplicated into eq
                ptr->eq = strdup(ptr->data);
            }
            //the node exists so true = 1
            true = 1;
            //sets current to be the node we just changed
            current = ptr;
            //breaks when we find the existing node to change the data
            break;
        }
        //changes ptr to iterate through the list
        ptr = ptr->next;
    }

    //if node does not exist true = 0, run this function
    if(true == 0) {
        //creates a new node and allocates the memory
        node *new = (node *) malloc(sizeof(node));

        //sets the current row and col values for the new node
        new->row = row;
        new->col = col;

        //sets next to NULL
        new->next = NULL;


        //sets the nodes length to the text length +1 for null terminator
        new->len = (char) strlen(text) + 1;

        //allocates memory for the string
        new->data = (char *) malloc(new->len);

        //copies the text to ptr->data using string copy
        strcpy(new->data, text);

        //if data is an equation then stores in eq
        if(new->data[0] == '=') {
            new->eq = strdup(new->data);

        }



        //if the list is empty head = new node
        if (head == NULL){
            //sets the head for the list
            head = new;

            //sets current node to first node of the list
            current = head;

            //if data is an equation stores in eq
            if(head->data[0] == '=') {
                head->eq = strdup(head->data);

            }
    }
        else {//if the list is not empty
            //resets the ptr to be head to iterate through the list
            ptr = head;

            //iterates to the end of the list
            while (ptr->next != NULL) {
                //sets ptr to the next node
                ptr = ptr->next;
            }
            //when it is at the end of the list adds the new node
            ptr->next = new;

            //makes current the node we just added to the list
            current = new;
        }

    }

    //if there is an equation
    if(current->data[0] == '=') {
        //stores equation in eq
        current->eq = strdup(current->data);

        //updates cell with the results of the equation
        update_cell_display(row, col, results(current->eq, row, col));
    }
    else {//updates the cell if there is no equation
        update_cell_display(row, col, text);
    }

    free(text);



}

void clear_cell(ROW row, COL col) {
    // TODO: implement this.

    // This just clears the display without updating any data structure. You will need to change this.
    update_cell_display(row, col, "");
}


char *get_textual_value(ROW row, COL col) {
    // TODO: implement this.
    //issues where I have to traverse the list to get the textual value everytime for each cell
    //seems like it would be inefficient

    //make pointer null
    char *textValue = NULL;

    //initially when head is null return null
    if(head == NULL) {
        return NULL;
    }else {//else return last input
        textValue = (char *) malloc(current->len);
        strcpy(textValue, current->data);

        return textValue;
        //text stays up because array isn't freed, so it only displays what the user last inputted
    }
}

