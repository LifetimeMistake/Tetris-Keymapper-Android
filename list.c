#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// A linked list implementation for C

struct list_node
{
    int value;
    struct list_node *previous;
    struct list_node *next;
};

// Checks if a list contains at least one occurence of the specified value.
bool list_contains(struct list_node *head, int value)
{
    struct list_node *current_node = head;
    while (current_node != NULL)
    {
        if(current_node->value == value)
        {
            return true;
        }
        current_node = current_node->next;
    }

    return false;
}

// Traverses a list and returns the number of contained elements.
int list_count(struct list_node *head)
{
    struct list_node *current_node = head;
    int counter = 0;
    while (current_node != NULL)
    {
        counter++;
        current_node = current_node->next;
    }

    return counter;
}

// Creates a new linked list. Returns the pointer to the created list's head node
struct list_node* list_create(int value)
{
    struct list_node *head = malloc(sizeof(struct list_node));
    head->value = value;
    head->previous = NULL;
    head->next = NULL;
    return head;
}

// Gets the first element in a linked list. Returns the pointer to the first element in the list
struct list_node* list_get_first(struct list_node *element)
{
    struct list_node *current_node = element;
    while (current_node != NULL)
    {
        current_node = current_node->previous;
    }

    return current_node;
}

// Gets the last element in a linked list. Returns the pointer to the last element in the list
struct list_node* list_get_last(struct list_node *element)
{
    struct list_node *current_node = element;
    while (current_node->next != NULL)
    {
        current_node = current_node->next;
    }

    return current_node;
}

// Gets the element at the specified index. Returns the pointer to the element or NULL if the index was out of bounds.
struct list_node* list_get_index(struct list_node *head, unsigned int index)
{
    struct list_node *current_node = head;
    int current_index = 0;
    while (current_node != NULL)
    {
        if(current_index == index)
        {
            break;
        }
        current_index++;
        current_node = current_node->next;
    }

    if(current_index != index)
    {
        return NULL;
    }

    return current_node;
}

// Inserts a new element with the specified value at the end of the list. Returns a pointer to the head of the list.
struct list_node* list_add(struct list_node *head, int value)
{
    struct list_node *new_node = malloc(sizeof(struct list_node));
    struct list_node *last_node = list_get_last(head);
    new_node->value = value;
    new_node->previous = last_node;
    new_node->next = NULL;
    last_node->next = new_node;
    return head;
}

// Removes an element at the specified index or doesn't remove it if the index was out of bounds. Returns a pointer to the head of the list.
struct list_node* list_remove_index(struct list_node *head, unsigned int index)
{
    struct list_node *removed_node = list_get_index(head, index);
    if(removed_node == NULL)
    {
        return head;
    }

    if(removed_node->previous != NULL)
    {
        removed_node->previous->next = removed_node->next;

        if(removed_node->next != NULL)
        {
            removed_node->next->previous = removed_node->previous;
        }

        struct list_node *previous_node = removed_node->previous;
        free(removed_node);
        return previous_node;
    }

    if(removed_node->next != NULL)
    {
        removed_node->next->previous = NULL;
        struct list_node *next_node = removed_node->next;
        free(removed_node);
        return next_node;
    }

    return NULL;
}

// Removes all occurences of the specified value. Returns a pointer to the head of the list.
struct list_node* list_remove_values(struct list_node *head, int value)
{
    struct list_node *current_node = head;
    while(current_node != NULL)
    {
        struct list_node *next_node = current_node->next;
        if(current_node->value == value)
        {
            if(current_node->previous != NULL)
            {
                current_node->previous->next = current_node->next;
            }

            if(current_node->next != NULL)
            {
                current_node->next->previous = current_node->previous;
            }

            if(current_node == head)
            {
                head = current_node->next;
            }

            free(current_node);
        }
        
        current_node = next_node;
    }

    return head;
}