/** @file */
#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"

/**
 * A linked list.
 *
 * The iterator iterates in the order of the elements being loaded into this list.
 */
template <class T>
class LinkedList
{
    struct Node{
        T data;
        Node *next;
        Node *pre;
        Node():next(this),pre(this){}
        Node(const T& parData,Node *parPre = NULL,Node *parNext = NULL)
        :data(parData),pre(parPre),next(parNext){
            if(parPre == NULL) pre = this;
            if(parNext == NULL) next = this;
        }
    };
private:
    Node *head;
    int iSize;
    Node* add(Node *parNode, const T& parData){
        ++iSize;
        Node *tmp = new Node(parData,parNode,parNode->next);
        tmp->next->pre = tmp;
        tmp->pre->next = tmp;
        return tmp;
    }
    Node* remove(Node *parNode){
        --iSize;
        parNode->next->pre = parNode->pre;
        parNode->pre->next = parNode->next;
        Node *tmp = parNode->next;
        delete parNode;
        return tmp;
    }
public:
    class Iterator
    {
    private:
        LinkedList* pLinkedList;
        Node *pNode;
    public:
        Iterator(LinkedList* const parLinkedList)
        :pLinkedList(parLinkedList){
            pNode = pLinkedList->head;
        }
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
            return (pNode->next != pLinkedList->head);
        }
        
        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
            if(!hasNext()) throw ElementNotExist();
            pNode = pNode->next;
            return pNode->data;
        }
        
        /**
         * TODO Removes from the underlying collection the last element
         * returned by the iterator
         * The behavior of an iterator is unspecified if the underlying
         * collection is modified while the iteration is in progress in
         * any way other than by calling this method.
         * @throw ElementNotExist
         */
        void remove() {
            if(pNode == pLinkedList->head)
                throw ElementNotExist();
            pNode = pLinkedList->remove(pNode);
            pNode = pNode->pre;
        }
    };
    
    /**
     * Constructs an empty linked list
     */
    LinkedList()
    :iSize(0) {
        head = new Node;
    }
    
    /**
     * TODO Copy constructor
     */
    LinkedList(const LinkedList<T> &c)
    :iSize(0) {
        head = new Node;
        for(Node *tmp = c.head->next; tmp != c.head; tmp = tmp->next){
            addLast(tmp->data);
        }
    }
    /**
     * TODO Assignment operator
     */
    LinkedList<T>& operator=(const LinkedList<T> &c) {
        if(this == &c) return *this;
        clear();
        for(Node *tmp = c.head->next; tmp != c.head; tmp = tmp->next){
            addLast(tmp->data);
        }
        return *this;
    }
    
    /**
     * TODO Desturctor
     */
    ~LinkedList() {
        clear();
        delete head;
    }
    
    /**
     * Appends the specified element to the end of this list.
     * Always returns true.
     */
    bool add(const T& elem) {
        add(head->pre,elem);
        return true;
    }
    
    /**
     * Inserts the specified element to the beginning of this list.
     */
    void addFirst(const T& elem) {
        add(head,elem);
    }
    
    /**
     * Insert the specified element to the end of this list.
     * Equivalent to add.
     */
    void addLast(const T &elem) {
        add(head->pre,elem);
    }
    
    /**
     *  Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, size], where index=0 means inserting to the head,
     * and index=size means appending to the end.
     * @throw IndexOutOfBound
     */
    void add(int index, const T& element) {
        if(index < 0 || index > iSize) throw IndexOutOfBound();
        Node  *tmp = head;
        while(index--) tmp = tmp->next;
        add(tmp,element);
    }
    
    /**
     * Removes all of the elements from this list.
     */
    void clear() {
        while(head->next != head)
            remove(head->next);
    }
    
    /**
     * Returns true if this list contains the specified element.
     */
    bool contains(const T& e) const {
        for(Node *tmp = head->next; tmp != head; tmp = tmp->next){
            if (tmp->data == e) return true;
        }
        return false;
    }
    
    /**
     *  Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const {
        if(index < 0 || index >= iSize) throw IndexOutOfBound();
        Node *tmp = head->next;
        while(index--){
            tmp = tmp->next;
        }
        return tmp->data;
    }
    
    /**
     *  Returns a const reference to the first element.
     * @throw ElementNotExist
     */
    const T& getFirst() const {
        if(!iSize) throw ElementNotExist();
        return head->next->data;
    }
    
    /**
     *  Returns a const reference to the last element.
     * @throw ElementNotExist
     */
    const T& getLast() const {
        if(!iSize) throw ElementNotExist();
        return head->pre->data;
    }
    
    /**
     *  Returns true if this list contains no elements.
     */
    bool isEmpty() const {
        return (!iSize);
    }
    
    /**
     *  Removes the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void removeIndex(int index) {
        if(index < 0 || index >= iSize) throw IndexOutOfBound();
        Node *tmp = head->next;
        while(index--){
            tmp = tmp->next;
        }
        remove(tmp);
    }
    
    /**
     *  Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it was present in the list, otherwise false.
     */
    bool remove(const T &e) {
        Node *tmp = head->next;
        while(tmp != head){
            if(tmp -> data == e){
                remove(tmp);
                return true;
            }
            tmp = tmp->next;
        }
        return false;
    }
    
    /**
     *  Removes the first element from this list.
     * @throw ElementNotExist
     */
    void removeFirst() {
        if(!iSize) throw ElementNotExist();
        remove(head->next);
    }
    
    /**
     *  Removes the last element from this list.
     * @throw ElementNotExist
     */
    void removeLast() {
        if(!iSize) throw ElementNotExist();
        remove(head->pre);
    }
    
    /**
     *  Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void set(int index, const T &element) {
        if(index < 0 || index >= iSize) throw IndexOutOfBound();
        Node *tmp = head->next;
        while(index--)
            tmp = tmp->next;
        tmp->data = element;
    }
    
    /**
     *  Returns the number of elements in this list.
     */
    int size() const {
        return iSize;
    }
    
    /**
     * TODO Returns an iterator over the elements in this list.
     */
    Iterator iterator() {
        return Iterator(this);
    }
};

#endif
