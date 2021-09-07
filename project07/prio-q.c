/* Nicholas Perry, nperry2, 116635288, 0104
   file for making a single pointer priority link list that is 
   dynamically allocated. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prio-q.h"

#define test 0

static int is_priority_present(Prio_que * prio_que, int priority);
static int get_occurrence(Prio_que * prio_que, char element[]);

/* creates a new priority queue that is empty unless the
pointer is null in which it does nothing */
void init(Prio_que *const prio_q) {
   if(prio_q != NULL) {
      prio_q->head = NULL;
   }
}

/* adds a new element with the priority from biggest to smallest. If the 
priority is already added then it returns 0 and does nothing.
If the prio_que is null it does nothing */
unsigned int enqueue(Prio_que *const prio_q, const char new_element[], 
                     unsigned int priority){
   Node *curr, *prev, *new_node;
   if(prio_q == NULL){
      return 0;
   }
   if(new_element == NULL){
      return 0;
   }

   /* reset curr position */
   curr = prio_q->head;
   
   /* make the new node */
   new_node = malloc(sizeof(Node));
   new_node->name = malloc(strlen(new_element) + 1);
   strcpy(new_node->name, new_element);
   new_node->priority = priority;

   /* set the helper pointers*/
   curr = prio_q->head;
   prev = prio_q->head;

   /* if there are no nodes to begin with */
   if(prio_q->head == NULL){
      prio_q->head = new_node;
      prio_q->head->next = NULL;
      return 1;
   }else{
      if(prio_q->head->priority < priority){
         new_node->next = prio_q->head;
         prio_q->head = new_node;
         return 1;
      }
      
      while(curr->next != NULL && curr->priority > priority){
         if(curr->priority == priority){
            return 0;
         }

         prev = curr;
         curr = curr->next;
      }

      if(curr->priority == priority){
         return 0;
      }

      if(curr->priority < priority){
         prev->next = new_node;
         new_node->next = curr;
      }else{
         curr->next = new_node;
         new_node->next = NULL;
      }

      return 1;
   }
}

/* returns 1 if it the queue is empty, else 0*/
unsigned int is_empty(const Prio_que prio_q){
   return (size(prio_q) != 0) ? 0 : 1;
}

/* returns the size of the array in an unsigned int */
unsigned int size(const Prio_que prio_q){
   Node *curr = prio_q.head;
   int counter = 0;

   while (curr != NULL){
      ++counter;
      curr = curr->next;
   }
   return counter;
}

/* gets the first element off of the queue but does not delete.
This is a deep copy */
char *peek(Prio_que prio_q){
   char * ans;
   if(is_empty(prio_q)){
      return NULL;
   }

   ans = malloc(strlen(prio_q.head->name) + 1);
   strcpy(ans, prio_q.head->name);
   return ans;
}

/* removes the first element off the array and returns the value. 
This is a deep copy of the string */
char *dequeue(Prio_que *const prio_q){
   char * ans;
   Node* temp = prio_q->head;
   if(prio_q == NULL || prio_q->head == NULL){
      return NULL;
   }

   ans = peek(*prio_q);
   prio_q->head = prio_q->head->next;
   
   free(temp->name);
   free(temp);
   return ans;
}

/* Returns all the string names in the queue as a dynamically allocated array
 of dynamically allocated strings. The last element is always NULL. 
 If it is empty then it does nothing */
char **get_all_elements(Prio_que prio_q){
   char ** arr;
   Node *curr;
   char * name;
   int counter = 0;

   if(is_empty(prio_q)){
      arr = malloc(sizeof(char *));
      arr[counter] = NULL;
      return arr;
   }

   arr = malloc((size(prio_q) + 1) * (sizeof(char *)));
   curr = prio_q.head;

   while(curr != NULL){
      name = malloc(strlen(curr->name) + 1);
      strcpy(name, curr->name);
      arr[counter] = name;
      ++counter;
      curr = curr->next;
   }

   arr[counter] = NULL;
   return arr;
}

void free_name_list(char *name_list[]){
   int i = 0;
   if(name_list != NULL){
      while(name_list[i] != NULL){
         free(name_list[i]);
         ++i;
      }
      free(name_list);
   }
}

void clear(Prio_que *const prio_q){
   if(prio_q != NULL){
      Node* curr = prio_q->head, *temp = prio_q->head;
      while(curr != NULL){
         temp = curr->next;
         free(curr->name);
         free(curr);
         curr = temp;
      }
   }
}

int get_priority(Prio_que prio_q, char element[]){
   Node* curr = prio_q.head;
   int compared, priority = -1;
   
   if(curr == NULL){
      return -1;
   }

   compared =  strcmp(curr->name, element);
   if(compared == 0){
      return curr->priority;
   }

   while(curr != NULL && compared != 0){
      compared =  strcmp(curr->name, element);
      priority = curr->priority;
      curr = curr->next;
   }

   if(compared == 0)
      return priority;
   return -1;
}

unsigned int remove_elements_between(Prio_que *const prio_q, unsigned int low,
                                     unsigned int high){
   Node * curr;
   Node * prev = NULL;
   Node * temp;
   int counter = 0;
   
   if(prio_q == NULL)
      return 0;
   
   curr = prio_q->head;
   prev = curr;
   while(curr != NULL && curr->priority >= low){
      temp = curr->next;
      if(curr->priority <= high){
         /* move head over if its set to be removed */
         if (curr == prio_q->head){
            prio_q->head = curr->next;
         }else{
            prev->next = curr->next;
         }
         /* remove the node */
         free(curr->name);
         free(curr);
         counter++;
      }else{
         prev = curr;
      }

      curr = temp;
   }

   return counter;
}

unsigned int change_priority(Prio_que *prio_q, char element[],
                             unsigned int new_priority){
   Node * selected = prio_q->head;
   if(prio_q == NULL)
      return 0;
   if(get_occurrence(prio_q, element) != 1)
      return 0;
   if(is_priority_present(prio_q, new_priority))
      return 0;
   
   /* find the node */
   while(strcmp(selected->name, element) != 0){
      selected = selected->next;
   }

   /* delete old node */
   remove_elements_between(prio_q, selected->priority, selected->priority);

   /* enqueue the node in the correct place */
   enqueue(prio_q, element, new_priority);
   return 1;
}

/* checks to see if the priority is in the queue yet. 
If it is in the queue : returns 1. NOT in the queue : returns 0
 */
static int is_priority_present(Prio_que * prio_que, int priority){
   Node * curr = prio_que->head;
   while(curr != NULL && curr->priority != priority){
      curr = curr->next;
   }

   if(curr == NULL)
      return 0;
   if(curr->priority == priority)
      return 1;
   return 0;
}

/* Returns the number of times an element with the same name has occurred in a
 priority queue. Output is the number of times */
static int get_occurrence(Prio_que * prio_que, char element[]){
   Node * curr = prio_que->head;
   short has_occurred = 0;
   while(curr != NULL){
      if(strcmp(curr->name, element) == 0){
         has_occurred++;
      }
      curr = curr->next;
   }

   return has_occurred;
}