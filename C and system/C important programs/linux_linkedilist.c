/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/

/* GPT link: https://chatgpt.com/share/67ec0f6f-ca40-8001-a6f5-ab007d12e5d0 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Kernel-style linked list implementation
struct list_head {
    struct list_head *next, *prev;
};

// Initialize a list head
#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

// Embedded structure
struct person {
    char name[50];
    int age;
    struct list_head list;
};

// Add a node to the list
void list_add(struct list_head *new, struct list_head *head) {
    head->next->prev = new;
    new->next = head->next;
    new->prev = head;
    head->next = new;
}

// Delete a node from the list
void list_del(struct list_head *entry) {
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
}

// Iterate over the list
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

// Get the struct from list_head
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

// Display the list contents
void display_list(struct list_head *head) {
    struct list_head *pos;
    list_for_each(pos, head) {
        struct person *p = list_entry(pos, struct person, list);
        printf("Name: %s, Age: %d\n", p->name, p->age);
    }
}

int main() {
    // Initialize the head
    struct list_head head;
    INIT_LIST_HEAD(&head);

    // Create some persons
    struct person *p1 = (struct person *)malloc(sizeof(struct person));
    strcpy(p1->name, "Alice");
    p1->age = 30;
    list_add(&p1->list, &head);

    struct person *p2 = (struct person *)malloc(sizeof(struct person));
    strcpy(p2->name, "Bob");
    p2->age = 25;
    list_add(&p2->list, &head);

    struct person *p3 = (struct person *)malloc(sizeof(struct person));
    strcpy(p3->name, "Charlie");
    p3->age = 35;
    list_add(&p3->list, &head);

    printf("Initial List:\n");
    display_list(&head);

    // Delete one node
    printf("\nDeleting Bob from the list...\n");
    list_del(&p2->list);
    free(p2);

    printf("Updated List:\n");
    display_list(&head);

    // Free remaining memory
    list_del(&p1->list);
    free(p1);

    list_del(&p3->list);
    free(p3);

    return 0;
}
