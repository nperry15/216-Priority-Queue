/* Nicholas Perry, nperry2, 116635288, 0104
   file for making a structure for a single pointer priority link list that is 
   dynamically allocated. */
#if !defined(PRIO_QUE)
#define PRIO_QUE

typedef struct node {
    char *name;
    int priority;
    struct node *next;
} Node;

typedef struct prio_que {
    Node *head;
} Prio_que;

#endif