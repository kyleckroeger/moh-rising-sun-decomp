#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// Member names follow what the code does with each word; the symbol table gives only the method names.
class CLinkedListElement {
public:
    CLinkedListElement *next;

    CLinkedListElement();
    ~CLinkedListElement();
    void AddElementAfterMe(CLinkedListElement *element);
    CLinkedListElement *GetNextElement();
};

class CLinkedList {
public:
    CLinkedListElement *head;
    CLinkedListElement *tail;
    int count;

    CLinkedList();
    ~CLinkedList();
    void AddElementAtBeginning(CLinkedListElement *element);
    void AddElementAtEnd(CLinkedListElement *element);
    void RemoveElement(CLinkedListElement *element);
    void RemoveFirstElement(CLinkedListElement *element);
    void RemoveElementGivenPrevious(CLinkedListElement *element, CLinkedListElement *previous);
    bool IsEmpty();
};

#endif
