// LinkedList.cpp: an intrusive singly linked list with head, tail and count.
#include "LinkedList.h"

CLinkedListElement::CLinkedListElement() : next(0) {
}

CLinkedListElement::~CLinkedListElement() {
}

void CLinkedListElement::AddElementAfterMe(CLinkedListElement *element) {
    element->next = next;
    next = element;
}

CLinkedListElement *CLinkedListElement::GetNextElement() {
    return next;
}

CLinkedList::CLinkedList() : head(0), tail(0), count(0) {
}

CLinkedList::~CLinkedList() {
}

void CLinkedList::AddElementAtBeginning(CLinkedListElement *element) {
    if (head)
        element->next = head;
    else
        tail = element;
    head = element;
    count++;
}

void CLinkedList::AddElementAtEnd(CLinkedListElement *element) {
    if (tail)
        tail->AddElementAfterMe(element);
    else
        head = element;
    tail = element;
    count++;
}

void CLinkedList::RemoveElement(CLinkedListElement *element) {
    if (element == head) {
        RemoveFirstElement(element);
        return;
    }

    for (CLinkedListElement *previous = head; previous; previous = previous->next) {
        if (previous->next == element) {
            RemoveElementGivenPrevious(element, previous);
            return;
        }
    }
}

void CLinkedList::RemoveFirstElement(CLinkedListElement *element) {
    if (element->next) {
        head = element->next;
        element->next = 0;
    } else {
        head = 0;
        tail = 0;
    }
    count--;
}

void CLinkedList::RemoveElementGivenPrevious(CLinkedListElement *element, CLinkedListElement *previous) {
    if (element->next) {
        previous->next = element->next;
        element->next = 0;
    } else {
        previous->next = element->next;
        tail = previous;
    }
    count--;
}

bool CLinkedList::IsEmpty() {
    return head == 0;
}
