// AI-assisted reconstruction from GR8E69; see docs/Collision.md. dwi::list_iter is named
// by RemoveSubVolume's mangled symbol. The node and list members, their names and the
// header name are inferred from code that uses them.
#ifndef GAME_DWILIST_H
#define GAME_DWILIST_H
#include "PlacementNew.h"

extern "C" void *DWI_allocalign(const char *, unsigned int, int, int);
extern "C" void DWI_free(void *);

namespace dwi {

template <class T> inline void Exchange(T &a, T &b) {
    T t = a;
    a = b;
    b = t;
}

template <class T> struct list_node {
    list_node() : prev(this), next(this), data(0) {}
    void Reset() {
        list_node *h = this;
        next = h;
        prev = h;
    }
    list_node *prev;
    list_node *next;
    T data;
};

template <class T> class list_iter {
  public:
    list_iter() {}
    list_iter(list_node<T> *n) : node(n) {}
    list_iter(const list_iter &other) : node(other.node) {}
    T &operator*() const { return node->data; }
    list_iter &operator++() {
        node = node->next;
        return *this;
    }
    list_iter operator++(int) {
        list_iter old(*this);
        ++*this;
        return old;
    }
    bool operator!=(list_iter other) const { return node != other.node; }
    bool operator==(const list_iter &other) const { return node == other.node; }

    list_node<T> *node;
};

template <class T> class list {
  public:
    typedef list_iter<T> iterator;

    list() : count(0), capacity(0), pool(0), freeList(0) { owned = 1; }
    static list_node<T> *AllocateNodes(int size) {
        return (list_node<T> *)DWI_allocalign("source/tl/dwi_memory.h:56", size * sizeof(list_node<T>), 16, 1024);
    }
    template <class I> list(int size, I first, I last) : count(0), capacity(size), pool(AllocateNodes(size)), freeList(0) {
        owned = 1;
        list_node<T> *next = 0;
        list_node<T> *node;
        for (node = pool + (capacity - 1); node != pool; node--) {
            node->next = next;
            next = node;
        }
        node->next = next;
        freeList = node;
        insert(end(), first, last);
    }
    template <class I> void insert(iterator position, I first, I last) {
        list_node<T> *after = position.node;
        list_node<T> *tail = after->prev;
        for (I i = first; i != last; ++i) {
            const T &value = *i;
            list_node<T> *added = freeList;
            freeList = added->next;
            added->prev = 0;
            added->next = 0;
            added->data = value;
            tail->next = added;
            added->prev = tail;
            tail = added;
            count++;
        }
        tail->next = after;
        after->prev = tail;
    }
    void ResetHead() { head.Reset(); }
    void swap(list &other) {
        Exchange(count, other.count);
        Exchange(capacity, other.capacity);
        list_node<T> *p = pool;
        pool = other.pool;
        other.pool = p;
        Exchange(freeList, other.freeList);
        Exchange(owned, other.owned);
        if (count != 0) {
            if (other.count != 0) {
                Exchange(head.prev, other.head.prev);
                Exchange(head.next, other.head.next);
                head.prev->next = &head;
                other.head.prev->next = &other.head;
            } else {
                other.head.prev = head.prev;
                other.head.next = head.next;
                other.head.prev->next = &other.head;
                ResetHead();
            }
        } else if (other.count != 0) {
            head.prev = other.head.prev;
            head.next = other.head.next;
            head.prev->next = &head;
            other.ResetHead();
        }
    }
    void reserve(int size) {
        if (size > capacity) {
            list grown(size, begin(), end());
            swap(grown);
        }
    }

    iterator begin() const { return iterator(head.next); }
    iterator end() const { return iterator((list_node<T> *)&head); }

    iterator insert(iterator position, const T &value) {
        list_node<T> *after = position.node;
        list_node<T> *before = after->prev;
        list_node<T> *n = freeList;
        freeList = n->next;
        n->prev = 0;
        n->next = 0;
        n->data = value;
        count++;
        before->next = n;
        n->prev = before;
        n->next = after;
        after->prev = n;
        return iterator(n);
    }
    void push_back(const T &value) { insert(end(), value); }
    iterator erase(iterator position) {
        list_node<T> *prev = position.node->prev;
        list_node<T> *next = position.node->next;
        prev->next = next;
        next->prev = prev;
        Free(position.node);
        count--;
        return iterator(next);
    }
    void Free(list_node<T> *n) {
        n->prev = 0;
        n->next = freeList;
        freeList = n;
    }
    ~list() {
        list_node<T> *n = head.next;
        while (n != &head) {
            list_node<T> *next = n->next;
            n->prev = 0;
            n->next = freeList;
            freeList = n;
            n = next;
        }
        ResetHead();
        count = 0;
        if (owned)
            DWI_free(pool);
        pool = 0;
    }

    int count;
    int capacity;
    list_node<T> *pool;
    list_node<T> *freeList;
    list_node<T> head;
    int owned;
};

// A vector over caller storage; the destructor frees the storage only when owned.
template <class T> class fast_vec {
  public:
    fast_vec(T *storage, int size, int grow = 12) : data(storage), unknown_04(0), count(0), capacity(size), owned(0), growth(grow) {}
    ~fast_vec() {
        if (owned && data)
            DWI_free(data);
    }

    void push_back(const T &value) { data[count++] = value; }
    T &back() { return data[count - 1]; }
    void pop_back() { count--; }
    T *begin() const { return data; }
    T *end() const { return data + count; }

    T *data;
    int unknown_04;
    int count;
    int capacity;
    int owned;
    int growth;
};

} // namespace dwi
#endif
