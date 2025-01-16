#include "list.h"
#include <stddef.h>
#include <stdio.h>

// static global variables
static int list_count = 0;
static int node_count = 0;
static List *nextFree = NULL;
static List *nextFreeNode = NULL;
static bool init = 0;

// General Error Handling:
// Client code is assumed never to call these functions with a NULL List pointer, or
// bad List pointer. If it does, any behaviour is permitted (such as crashing).
// HINT: Use assert(pList != NULL); just to add a nice check, but not required.

// Makes a new, empty list, and returns its reference on success.
// Returns a NULL pointer on failure.
List *List_create()
{
    // set array of lists
    static List lists[LIST_MAX_NUM_HEADS];

    // initialization flag for nodes
    if (init == false)
    {
        // iterate through all nodes
        for (int i = 0; i < LIST_MAX_NUM_NODES; i++)
        {
            // initialize nodes
            lists->nodes[i].data = NULL;
            lists->nodes[i].next = NULL;
            lists->nodes[i].prev = NULL;
        }
        // one time initialize only
        init = true;
    }

    // if the list count is more than the max and the next free list is nonexistent
    if (list_count >= LIST_MAX_NUM_HEADS && nextFree == NULL)
        return NULL;

    // if the next free list is nonexistent
    if (nextFree != NULL)
    {
        // set new list as the freed list
        List *newList = nextFree;

        // initialize a new list
        newList->head = NULL;
        newList->tail = NULL;
        newList->current = NULL;
        newList->count = 0;

        // remove the nextFree pointer
        if (nextFree->nextList != NULL)
        {
            nextFree = nextFree->nextList;
        }
        else
        {
            nextFree = NULL;
        }

        // increment list count
        list_count++;

        // return the new list
        return newList;
    }
    else if (list_count < LIST_MAX_NUM_HEADS)
    {
        // increment the list count by 1
        List *newList = &lists[list_count++];

        // initialize a new list
        newList->head = NULL;
        newList->tail = NULL;
        newList->current = NULL;
        newList->count = 0;

        // return the new list
        return newList;
    }
    return NULL;
}

// Returns the number of items in pList.
int List_count(List *pList)
{
    // return count variable
    return pList->count;
};

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void *List_first(List *pList)
{
    // if list is not empty
    if (pList->count > 0)
    {
        // make the first item the current item
        pList->current = pList->head;
        // return a pointer to the first item in pList
        return pList->current->data;
    }
    // set current item to NULL because list is empty
    pList->current = NULL;
    // return NULL
    return NULL;
};

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void *List_last(List *pList)
{
    // if list is not empty
    if (pList->count > 0)
    {
        // set current node to tail
        pList->current = pList->tail;
        // return current node item
        return pList->current->data;
    }

    // fail case
    pList->current = NULL;
    return NULL;
};

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer
// is returned and the current item is set to be beyond end of pList.
void *List_next(List *pList)
{
    // regular case
    if (pList->current != NULL && pList->current->next != NULL)
    {
        // set the current node to the next node
        pList->current = pList->current->next;
        return pList->current->data;
    }
    // beyond end of list case
    if (pList->current != NULL && pList->current->next == NULL)
    {
        // set the current node to the enum value out of bounds end
        pList->current = (Node *)LIST_OOB_END;
        return NULL;
    }
    return NULL;
};

// Backs up pList's current item by one, and returns a pointer to the new current item.
// If this operation backs up the current item beyond the start of the pList, a NULL pointer
// is returned and the current item is set to be before the start of pList.
void *List_prev(List *pList)
{
    // regular case
    if (pList->current != NULL && pList->current->prev != NULL)
    {
        // set the current node to the previous node
        pList->current = pList->current->prev;
        return pList->current->data;
    }
    // before the start of list case
    if (pList->current != NULL && pList->current->prev == NULL)
    {
        // set the current node to the enum value out of bounds start
        pList->current = (Node *)LIST_OOB_START;
        return NULL;
    }
    return NULL;
};

// Returns a pointer to the current item in pList.
void *List_curr(List *pList)
{
    // if the current node is not NULL
    if (pList->current != NULL)
    {
        // return the current node item
        return pList->current->data;
    }
    return NULL;
};

// Adds the new item to pList directly after the current item, and makes item the current item.
// If the current pointer is before the start of the pList, the item is added at the start. If
// the current pointer is beyond the end of the pList, the item is added at the end.
// Returns 0 on success, -1 on failure.
int List_insert_after(List *pList, void *pItem)
{
    // empty list case
    if (pList->count == 0)
    {
        // append item
        List_append(pList, pItem);
        return LIST_SUCCESS;
    }

    // before the start of list case
    if (pList->current == (Node *)LIST_OOB_START)
    {
        // use prepend
        List_prepend(pList, pItem);
        return LIST_SUCCESS;
    }
    // beyond the end of the list case
    if (pList->current == (Node *)LIST_OOB_END)
    {
        // use append
        List_append(pList, pItem);
        return LIST_SUCCESS;
    }
    // regular case
    if (pList->current != NULL && node_count < LIST_MAX_NUM_NODES)
    {
        // create new node
        Node *newNode = NULL;

        // increment the new node count and set it to one of the allocated nodes
        if (nextFreeNode == NULL)
        {
            newNode = &pList->nodes[node_count++];
        }
        else
        {
            newNode = (Node *)nextFreeNode;
            nextFreeNode = nextFreeNode->nextNode;
        }

        // set the node pointers
        newNode->data = pItem;
        newNode->next = pList->current->next;
        newNode->prev = pList->current;

        // if the current next node is not empty
        if (pList->current->next != NULL)
        {
            // update previous pointer of the current next node
            pList->current->next->prev = newNode;
        }
        // set the next node from the current node to the new node
        pList->current->next = newNode;

        // update tail if needed
        if (pList->tail == pList->current)
        {
            pList->tail = newNode;
        }

        // make the current node the new node
        pList->current = newNode;
        pList->count++;
        return LIST_SUCCESS;
    }

    return LIST_FAIL;
};

// Adds item to pList directly before the current item, and makes the new item the current one.
// If the current pointer is before the start of the pList, the item is added at the start.
// If the current pointer is beyond the end of the pList, the item is added at the end.
// Returns 0 on success, -1 on failure.
int List_insert_before(List *pList, void *pItem)
{
    // empty list case
    if (pList->count == 0)
    {
        // append item
        List_append(pList, pItem);
        return LIST_SUCCESS;
    }

    // before the start of list case
    if (pList->current == (Node *)LIST_OOB_START)
    {
        // use prepend
        List_prepend(pList, pItem);
        return LIST_SUCCESS;
    }
    // beyond the end of the list case
    if (pList->current == (Node *)LIST_OOB_END)
    {
        // use append
        List_append(pList, pItem);
        return LIST_SUCCESS;
    }
    // regular case
    if (pList->current != NULL && node_count < LIST_MAX_NUM_NODES)
    {
        // create new node
        Node *newNode = NULL;

        // increment the new node count and set it to one of the allocated nodes
        if (nextFreeNode == NULL)
        {
            newNode = &pList->nodes[node_count++];
        }
        else
        {
            newNode = (Node *)nextFreeNode;
            nextFreeNode = nextFreeNode->nextNode;
        }

        // set the node pointers
        newNode->data = pItem;
        newNode->prev = pList->current->prev->prev;
        newNode->next = pList->current;

        // if the current prev node is not empty
        if (pList->current->prev != NULL)
        {
            // update next pointer of the current prev node
            pList->current->prev->next = newNode;
        }
        // set the prev node from the current node to the new node
        pList->current->prev = newNode;

        // Update head if necessary
        if (pList->head == pList->current)
        {
            pList->head = newNode;
        }

        // make the current node the new node
        pList->current = newNode;
        pList->count++;
        return LIST_SUCCESS;
    }

    return LIST_FAIL;
};

// Adds item to the end of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_append(List *pList, void *pItem)
{
    Node *newNode = NULL;

    if (node_count < LIST_MAX_NUM_NODES)
    {
        if (pList->count == 0)
        {
            // If the list is empty, set both head and tail to the new item.
            // increment the new node count and set it to one of the allocated nodes
            if (nextFreeNode == NULL)
            {
                newNode = &pList->nodes[node_count++];
            }
            else
            {
                newNode = (Node *)nextFreeNode;
                nextFreeNode = nextFreeNode->nextNode;
            }
            newNode->data = pItem;
            newNode->next = NULL;
            newNode->prev = NULL;
            pList->head = newNode;
            pList->tail = newNode;
            pList->current = newNode;
            pList->count++;
            return LIST_SUCCESS;
        }
        else
        {
            // Otherwise, add the item at the end.
            // increment the new node count and set it to one of the allocated nodes
            if (nextFreeNode == NULL)
            {
                newNode = &pList->nodes[node_count++];
            }
            else
            {
                newNode = (Node *)nextFreeNode;
                if (nextFreeNode->nextNode != NULL)
                {
                    nextFreeNode = nextFreeNode->nextNode;
                }
                else
                {
                    nextFreeNode = NULL;
                }
            }
            newNode->data = pItem;
            newNode->next = NULL;
            newNode->prev = pList->tail;
            pList->tail->next = newNode;
            pList->tail = newNode;
            pList->current = newNode;
            pList->count++;
            return LIST_SUCCESS;
        }
    }

    return LIST_FAIL;
};

// Adds item to the front of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_prepend(List *pList, void *pItem)
{
    Node *newNode = NULL;

    if (node_count < LIST_MAX_NUM_NODES)
    {
        if (pList->count == 0)
        {
            // If the list is empty, set both head and tail to the new item.
            // increment the new node count and set it to one of the allocated nodes
            if (nextFreeNode == NULL)
            {
                newNode = &pList->nodes[node_count++];
            }
            else
            {
                newNode = (Node *)nextFreeNode;
                nextFreeNode = nextFreeNode->nextNode;
            }
            newNode->data = pItem;
            newNode->next = NULL;
            newNode->prev = NULL;
            pList->head = newNode;
            pList->tail = newNode;
            pList->current = newNode;
            pList->count++;
            return LIST_SUCCESS;
        }
        else
        {
            // Otherwise, add the item at the front.
            // increment the new node count and set it to one of the allocated nodes
            if (nextFreeNode == NULL)
            {
                newNode = &pList->nodes[node_count++];
            }
            else
            {
                newNode = (Node *)nextFreeNode;
                nextFreeNode = nextFreeNode->nextNode;
            }
            newNode->data = pItem;
            newNode->prev = NULL;
            newNode->next = pList->head;
            pList->head->prev = newNode;
            pList->head = newNode;
            pList->current = newNode;
            pList->count++;
            return LIST_SUCCESS;
        }
    }

    return LIST_FAIL;
};

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void *List_remove(List *pList)
{
    if (pList->current == NULL || pList->current == (Node *)LIST_OOB_START || pList->current == (Node *)LIST_OOB_END)
    {
        // Current pointer is invalid.
        return NULL;
    }

    // set the removed node to the current node
    Node *removedNode = pList->current;

    // update head
    if (removedNode == pList->head)
    {
        pList->current = removedNode->next;
        pList->head = pList->current;
    }
    // update tail
    else if (removedNode == pList->tail)
    {
        pList->current = (Node *)LIST_OOB_END;
        pList->tail = removedNode->prev;
        removedNode->prev->next = NULL;
    }
    // regular case
    else
    {
        pList->current = removedNode->next;
        removedNode->prev->next = removedNode->next;
        removedNode->next->prev = removedNode->prev;
    }

    // set the nextFreeNode pointer to the freed node
    if (nextFreeNode != NULL)
    {
        List *tempNode = NULL;
        tempNode = nextFreeNode;
        nextFreeNode = (List *)removedNode;
        nextFreeNode->nextNode = tempNode;
    }
    else
    {
        nextFreeNode = (List *)removedNode;
    }
    // decrement the count
    pList->count--;
    node_count--;
    return removedNode->data;
};

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void *List_trim(List *pList)
{
    // if list is empty
    if (pList->count == 0)
    {
        return NULL;
    }

    // set the last node to the tail
    Node *lastNode = pList->tail;

    // if list has only one item
    if (pList->count == 1)
    {
        pList->head = NULL;
        pList->tail = NULL;
        pList->current = NULL;
    }
    else
    // update the tail and the current pointer
    {
        pList->tail = lastNode->prev;
        pList->tail->next = NULL;
        pList->current = pList->tail;
    }

    // set the nextFreeNode pointer to the freed node
    if (nextFreeNode != NULL)
    {
        List *tempNode = NULL;
        tempNode = nextFreeNode;
        nextFreeNode = (List *)lastNode;
        nextFreeNode->nextNode = tempNode;
    }
    else
    {
        nextFreeNode = (List *)lastNode;
    }
    // decrement count
    pList->count--;
    node_count--;
    return lastNode->data;
};

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1.
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List *pList1, List *pList2)
{
    // pList2 is empty
    if (pList2->count == 0)
    {
        return;
    }
    // pList1 is empty
    if (pList1->count == 0)
    {
        pList1->head = pList2->head;
        pList1->tail = pList2->tail;
    }
    // update list pointers
    else
    {
        pList1->tail->next = pList2->head;
        pList2->head->prev = pList1->tail;
        pList1->tail = pList2->tail;
    }

    // update the count of pList1
    pList1->count += pList2->count;

    // set pList2's head and tail to NULL to indicate that it's empty
    pList2->head = NULL;
    pList2->tail = NULL;
    pList2->current = NULL;
    pList2->count = 0;
};

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item.
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are
// available for future operations.
typedef void (*FREE_FN)(void *pItem);
void List_free(List *pList, FREE_FN pItemFreeFn)
{
    // set current node to the head
    Node *currentNode = pList->head;

    // iterate through the list to free all the items
    while (currentNode != NULL)
    {
        if (currentNode->data != NULL)
        {
            (*pItemFreeFn)(currentNode->data);
        }

        // node die
        currentNode->next = NULL;
        currentNode->prev = NULL;

        // set the nextFreeNode pointer to the freed node
        if (nextFreeNode != NULL)
        {
            List *tempNode = NULL;
            tempNode = nextFreeNode;
            nextFreeNode = (List *)currentNode;
            nextFreeNode->nextNode = tempNode;
        }
        else
        {
            nextFreeNode = (List *)currentNode;
        }
        node_count--;
        currentNode = currentNode->next;
    }

    // reset the list variables
    pList->head = NULL;
    pList->tail = NULL;
    pList->current = NULL;
    pList->count = 0;

    nextFreeNode = NULL;

    // set the nextFree pointer to the freed list
    if (nextFree != NULL)
    {
        List *tempList = NULL;
        tempList = nextFree;
        nextFree = pList;
        nextFree->nextList = tempList;
    }
    else
    {
        nextFree = pList;
    }

    // decrement the list count
    list_count--;
};

// Search pList, starting at the current item, until the end is reached or a match is found.
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match,
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator.
//
// If a match is found, the current pointer is left at the matched item and the pointer to
// that item is returned. If no match is found, the current pointer is left beyond the end of
// the list and a NULL pointer is returned.
//
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void *pItem, void *pComparisonArg);
void *List_search(List *pList, COMPARATOR_FN pComparator, void *pComparisonArg)
{
    // set the current node to the current node of the list
    Node *currentNode = pList->current;

    // start at the head if OOB start
    if (currentNode == (Node *)LIST_OOB_START)
    {
        currentNode = pList->head;
    }
    // fail case
    else if (currentNode == (Node *)LIST_OOB_END || currentNode == NULL)
    {
        return NULL;
    }

    // iterate through the list
    while (currentNode != NULL)
    {
        if (pComparator(currentNode->data, pComparisonArg))
        {
            // match found, return the item
            pList->current = currentNode;
            return currentNode->data;
        }
        currentNode = currentNode->next;
    }

    // no match found
    pList->current = (Node *)LIST_OOB_END;
    return NULL;
}
