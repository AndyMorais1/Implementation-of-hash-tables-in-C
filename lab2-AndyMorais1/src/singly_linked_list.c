#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"
typedef struct Node_ *Node;
struct Node_
{
    void *element;
    Node next;
};
struct List_
{
    int size;
    Node head;
    Node tail;
};

Node create_node(void *element)
{
    Node node = malloc(sizeof(struct Node_));
    if (node != NULL)
    {
        node->next = NULL;
        node->element = element;
    }
    return node;
}

/**
 * @brief Creates a new list.
 *
 * @return List The new list.
 */
List list_create()
{
    List list = malloc(sizeof(struct List_));
    if (list != NULL)
    {
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
    }
    return list;
}

void destroy_node(Node node, void (*free_element)(void *))
{
    if (free_element != NULL)
    {
        free_element(node->element);
    }
    free(node);
}

/**
 * @brief Destroys a list.
 *
 * Frees all memory allocated for the list, and for all elements of the list.
 *
 * @param list The list to destroy.
 * @param free_element The function to free the elements of the list.
 */
void list_destroy(List list, void (*free_element)(void *))
{
    if (list_is_empty(list))
    {
        return;
    }
    Node current = list->head;
    while (current != NULL)
    {
        Node next = current->next;
        if (free_element != NULL && current->element != NULL)
        {
            free_element(current->element);
        }
        free(current);
        current = next;
    }
    free(list);
}

/**
 * @brief Returns true iff the list contains no elements.
 *
 * @param list The linked list.
 * @return true iff the list contains no elements.
 */
bool list_is_empty(List list)
{
    if (list_size(list) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Returns the number of elements in the list.
 *
 * @param list The linked list.
 * @return int The number of elements in the list.
 */
int list_size(List list)
{
    if (list==NULL)
    {
        return 0;
    }
    Node current=list->head;
    int size=0;
    while (current!=NULL)
    {
        size++;
        current=current->next;
    }
    return size;
}

/**
 * @brief Returns the first element of the list.
 *
 * @param list The linked list.
 * @return void* The first element of the list.
 */
void *list_get_first(List list)
{
    if (list_is_empty(list))
    {
        return NULL;
    }
    Node target = list->head;
    void *element = target->element;

    return element;
}

/**
 * @brief Returns the last element of the list.
 *
 * @param list The linked list.
 * @return void* The last element of the list.
 */
void *list_get_last(List list)
{
    if (list_is_empty(list))
    {
        return NULL;
    }
    Node target = list->tail;
    void *element = target->element;

    return element;
}

/**
 * @brief Returns the element at the specified position in the list.
 *
 * Range of valid positions: 0, ..., size()-1.
 *
 * @param list The linked list.
 * @param position The position of the element to return.
 * @return void* The element at the specified position in the list.
 */
void *list_get(List list, int position)
{
    if (list_is_empty(list) || position >= list_size(list) || position < 0)
    {
        return NULL;
    }
    if (position == 0)
    {
        return list_get_first(list);
    }
    if (position == (list_size(list) - 1))
    {
        return list_get_last(list);
    }
    Node node = list->head;
    int pos = 0;
    while (pos < position)
    {
        node = node->next;
        pos++;
    }
    return node->element;
}

/**
 * @brief Returns the position in the list of the first occurrence of the specified element.
 *
 * Returns -1 if the specified element does not occur in the list.
 *
 * @param list The linked list.
 * @param equal The function to compare two elements.
 * @param element The element to search for.
 * @return int The position in the list of the first occurrence of the specified element, or -1 if the specified element does not occur in the list.
 */
int list_find(List list, bool (*equal)(void *, void *), void *element)
{
    if (list_is_empty(list) || equal == NULL)
    {
        return -1;
    }
    Node node = list->head;
    int idx = 0;
    while (node != NULL)
    {
        if (equal(node->element, element))
        {
            return idx;
        }
        node = node->next;
        idx++;
    }
    return -1;
}

/**
 * @brief Inserts the specified element at the first position in the list.
 *
 * @param list The linked list.
 * @param element The element to insert.
 */
void list_insert_first(List list, void *element)
{
    Node node = create_node(element);
    if (list_is_empty(list))
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        node->next = list->head;
        list->head = node;
    }
    list->size++;
}

/**

@brief Inserts the specified element at the last position in the list.*
@param list The linked list.
@param element The element to insert.
*/
void list_insert_last(List list, void *element)
{
    Node node = create_node(element);
    if (list_is_empty(list))
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
}

/**
 * @brief Inserts the specified element at the specified position in the list.
 *
 * Range of valid positions: 0, ..., size().
 * If the specified position is 0, insert corresponds to insertFirst.
 * If the specified position is size(), insert corresponds to insertLast.
 *
 * @param list The linked list.
 * @param element The element to insert.
 * @param position The position at which to insert the specified element.
 */
void list_insert(List list, void *element, int position)
{
    if (position < 0 || position > list_size(list))
    {
        return;
    }
    Node node = create_node(element);

    if (position == 0)
    {
        list_insert_first(list, element);
    }
    else if (position == list_size(list))
    {
        list_insert_last(list, element);
    }
    else
    {
        Node current = list->head;
        for (int i = 0; i < position - 1; i++)
        {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
        list->size++;
    }
}

/**
 * @brief Removes and returns the element at the first position in the list.
 *
 * @param list The linked list.
 * @return void* The element at the first position in the list.
 */
void *list_remove_first(List list)
{
    if (list_is_empty(list))
    {
        return NULL;
    }
    Node target = list->head;
    void *element = target->element;
    list->head = list->head->next;
    free(target);
    list->size--;
    if (list_size(list) == 0)
    {
        list->tail = NULL;
    }

    return element;
}

/**
 * @brief Removes and returns the element at the last position in the list.
 *
 * @param list The linked list.
 * @return void* The element at the last position in the list.
 */
void *list_remove_last(List list)
{
    if (list_is_empty(list))
    {
        return NULL;
    }
    void *element = list->tail->element;

    if (list->size == 1)
    {
        return list_remove_first(list);
    }

    Node node = list->head;
    while (node->next->next != NULL)
    {
        node = node->next;
    }
    free(list->tail);
    node->next = NULL;
    list->tail = node;
    list->size--;
    return element;
}
/**
 * @brief Removes and returns the element at the specified position in the list.
 *
 * Range of valid positions: 0, ..., size()-1.
 *
 * @param list The linked list.
 * @param position The position of the element to remove.
 * @return void* The element at the specified position in the list.
 */
void *list_remove(List list, int position)
{
    if (position > list->size - 1 || position < 0 || list_is_empty(list))
    {
        return NULL;
    }
    if (list->size == 1 || position == 0)
    {
        return list_remove_first(list);
    }
    Node node = list->head->next;
    for (int i = 1; i < position - 1; i++)
    {
        node = node->next;
    }
    Node next = node->next;
    void *element = next->element;
    node->next = next->next;
    free(next);
    list->size--;
    return element;
}

/**
 * @brief Removes all elements from the list.
 *
 * @param list The linked list.
 * @param free_element The function to free the elements of the list.
 */
void list_make_empty(List list, void (*free_element)(void *))
{
    if (list_is_empty(list))
    {
        return;
    }
    Node current = list->head;
    while (current != NULL)
    {
        Node next = current->next;
        if (free_element != NULL)
        {
            free_element(current->element);
        }
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

/**
 * @brief Returns an array with the elements of the list.
 *
 * @param list The linked list.
 * @param out_array The array to fill with the elements of the list.
 */
void list_to_array(List list, void **out_array)
{
    for (int i = 0; i != list->size; i++)
    {
        out_array[i] = list_get(list, i);
    }
    return;
}

/**
 * @brief Returns the number of occurrences on an element.
 *
 * @param list The linked list.
 * @param equal The function to compare two elements.
 * @param element The element to search for.
 * @return int The number of occurrences on an element.
 */
int list_count_all(List list, bool (*equal)(void *, void *), void *element)
{
    Node node = create_node(element);
    int count = 0;
    int pos = 0;
    if (list_is_empty(list))
    {
        return 0;
    }
    else
    {
        if (list_size(list) == 1)
        {
            if (equal(element, list->head->element))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if (list_size(list) > 1)
            {
                while (pos < list_size(list))
                {
                    if (equal(list->head->element, element))
                    {
                        count++;
                    }
                    list->head = list->head->next;
                    pos++;
                }
                return count;
            }
        }
    }
}

/**
 * @brief Removes all occurrences of an element, and returns the number of occurrences.
 *
 * @param list The linked list.
 * @param equal_element The function to compare two elements.
 * @param free_element The function to free the elements of the list.
 * @param element The element to search for.
 * @return int The number of occurrences on an element.
 */
int list_remove_all(List list, bool (*equal_element)(void *, void *), void (*free_element)(void *), void *element)
{
    if (list_is_empty(list) || equal_element == NULL)
    {
        return 0;
    }

    int count = 0;
    Node prev = NULL;
    Node current = list->head;

    while (current != NULL)
    {
        if (equal_element(current->element, element))
        {
            Node temp = current;
            if (prev == NULL)
            {
                list->head = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            if (temp == list->tail)
            {
                list->tail = prev;
            }
            current = current->next;
            if (free_element != NULL)
            {
                free_element(temp->element);
            }
            free(temp);
            list->size--;
            count++;
            continue;
        }
        prev = current;
        current = current->next;
    }
    return count;
}

/**
 * @brief Removes all duplicate occurrences of an element.
 *
 * Keeps only the first occurrence, and returns the number of occurrences.
 *
 * @param list The linked list.
 * @param equal_element The function to compare two elements.
 * @param free_element The function to free the elements of the list.
 * @param element The element to search for.
 * @return int The number of occurrences on an element.
 */
int list_remove_duplicates(List list, bool (*equal_element)(void *, void *), void (*free_element)(void *), void *element)
{
    if (list_is_empty(list) || equal_element == NULL)
    {
        return 0;
    }
    int count = 0;
    Node current = list->head;
    Node prev = NULL;
    while (current != NULL)
    {
        if (equal_element(current->element, element))
        {

            if (prev == NULL)
            {
                list->head = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            Node temp = current;
            current = current->next;
            if (free_element != NULL)
            {
                free_element(temp->element);
            }
            free(temp);
            list->size--;
            count++;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
    return count;
}

/**
 * @brief Returns the result from the join of two lists.
 *
 * Preserves order.
 *
 * @param list1 The first linked list.
 * @param list2 The second linked list.
 * @return List The resulting from the join of two lists.
 */
List list_join(List list1, List list2)
{
    List newlist = list_create();
    Node node = list1->head;
    while (node != NULL)
    {
        list_insert_last(newlist, node->element);
        node = node->next;
    }
    node = list2->head;
    while (node != NULL)
    {
        list_insert_last(newlist, node->element);
        node = node->next;
    }
    return newlist;
}

/**
 * @brief Outputs a formatted representation of the list.
 *
 * @param list The linked list.
 * @param print_element The function to print the elements of the list.
 */
void list_print(List list, void (*print_element)(void *element))
{
    Node auxNode = list->head;
    while (auxNode != NULL)
    {
        print_element(auxNode->element);
        auxNode = auxNode->next;
    }
}

/**
 * @brief Returns a list with the elements from start_idx to end_idx.
 *
 * @param list The linked list.
 * @param start_idx The index of the first element to include.
 * @param end_idx The index of the last element to include.
 * @return List A list with the elements from start_idx to end_idx.
 */
List list_get_sublist_between(List list, int start_idx, int end_idx)
{
    List newList = list_create();
    if (list_is_empty(list) || start_idx < 0 || end_idx >= list_size(list) || start_idx > end_idx)
    {
        return NULL;
    }
    else
    {
        if (list_size(list) == 1)
        {
            list_insert_first(newList, list->head->element);
            return newList;
        }
        else
        {
            int pos = 0;
            Node current = list->head;
            while (pos < start_idx)
            {
                current = current->next;
                pos++;
            }
            while (pos <= end_idx)
            {
                list_insert_last(newList, current->element);
                current = current->next;
                pos++;
            }
            return newList;
        }
    }
}

/**
 * @brief Returns a list with the elements in the given array of unordered indexes.
 *
 * Returns the size of the array in the out parameter count.
 *
 * @param list The linked list.
 * @param indexes The array of unordered indexes.
 * @param count The size of the array.
 * @return List A list with the elements in the given array of unordered indexes.
 */
List list_get_sublist(List list, int indexes[], int count)
{
    if (list_is_empty(list) || indexes == NULL || count == 0)
    {
        return NULL;
    }
    List newList = list_create();
    if (newList == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < count; i++)
    {
        int idx = indexes[i];
        if (idx < 0 || idx >= list_size(list))
        {
            list_destroy(newList, NULL);
            return NULL;
        }
        void *aux = list_get(list, idx);
        if (aux == NULL)
        {
            list_destroy(newList, NULL);
            return NULL;
        }
        list_insert_last(newList, aux);
    }
    return newList;
}

/**
 * @brief Returns a list with the result of the execution of the function func with each element of the list as parameter.
 *
 * @param list The linked list.
 * @param func The function to apply to each element of the list.
 * @return List A list with the result of the execution of the function func with each element of the list as parameter.
 */
List list_map(List list, void *(*func)(void *))
{
    if (list_is_empty(list) || func == NULL)
    {
        return NULL;
    }

    List new_list = list_create();
    if (new_list == NULL)
    {
        return NULL;
    }

    Node current = list->head;
    while (current != NULL)
    {

        void *result = func(current->element);

        list_insert_last(new_list, result);

        current = current->next;
    }

    return new_list;
}

/**
 * @brief Returns a list with the elements that return true when applied with the given function.
 *
 * @param list The linked list.
 * @param func The boolean function to apply to each element of the list.
 * @return List A list with the elements that return true when applied with the given function.
 */
List list_filter(List list, bool (*func)(void *))
{
    if (list_is_empty(list) || func == NULL)
    {
        return NULL;
    }
    List newList = list_create();
    if (newList == NULL)
    {
        return NULL;
    }
    Node current = list->head;
    while (current != NULL)
    {
        bool result = func(current->element);
        if (result)
        {
            list_insert_last(newList, current->element);
        }
        current = current->next;
    }
    return newList;
}
