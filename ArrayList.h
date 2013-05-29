/** @file */
#ifndef __ARRAYLIST_H
#define __ARRAYLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"

/**
 * The ArrayList is just like vector in C++.
 * You should know that "capacity" here doesn't mean how many elements are now in this list, where it means
 * the length of the array of your internal implemention
 *
 * The iterator iterates in the order of the elements being loaded into this list
 */
template <class T>
class ArrayList
{
private:
    T *iStorage;
    int iSize;
    int iCapacity;
    inline void autoSpace(){
        if (iCapacity<=iSize)
            doubleSpace();
    }
    void doubleSpace(){
        T *tmp = iStorage;
        iCapacity *= 2;
        iStorage = new T[iCapacity];
        for (int i=0;i<iSize;++i) iStorage[i] = tmp[i];
        delete[] tmp;
    }
public:
    class Iterator
    {
        friend class ArrayList;
    private:
        int position;
        ArrayList *pArray;
        bool ifPointed;
    public:
        Iterator(ArrayList *parArray)
        :ifPointed(false),position(0),pArray(parArray){}
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
            if(position < pArray->iSize - 1) return true;
            if((!ifPointed) && (position == pArray->iSize - 1)) return true;
            return false;
        }
        
        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
            if(!hasNext()) throw ElementNotExist();
            if(ifPointed) ++position;
            else ifPointed = true;
            return pArray->iStorage[position];
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
            if(!ifPointed) throw ElementNotExist();
            pArray->removeIndex(position);
            ifPointed = false;
        }
    };
    
    /**
     *  Constructs an empty array list.
     */
    ArrayList()
    :iSize(0){
        iCapacity = 32;
        iStorage = new T[iCapacity];
    }
    
    /**
     *  Destructor
     */
    ~ArrayList() {
        delete[] iStorage;
    }
    
    /**
     *  Assignment operator
     */
    ArrayList& operator=(const ArrayList& x) {
        if(this == &x) return *this;
        delete[] iStorage;
        iCapacity = x.capacity;
        iSize = x.current_size;
        iStorage = new T[iCapacity];
        for (int i=0;i<iSize;++i)
            iStorage[i] = x.storage[i];
        return *this;
    }
    
    /**
     *  Copy-constructor
     */
    ArrayList(const ArrayList& x) {
        iCapacity = x.iCapacity;
        iSize = x.iSize;
        iStorage = new T[iCapacity];
        for (int i=0;i<iSize;++i)
            iStorage[i] = x.iStorage[i];
    }
    
    /**
     *  Appends the specified element to the end of this list.
     * Always returns true.
     */
    bool add(const T& e) {
        autoSpace();
        iStorage[iSize++] = e;
        return  true;
    }
    
    /**
     *  Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, size], where index=0 means inserting to the head,
     * and index=size means appending to the end.
     * @throw IndexOutOfBound
     */
    void add(int index, const T& element) {
        autoSpace();
        for(int i=iSize;i>index;--i)
            iStorage[i] = iStorage[i-1];
        iStorage[index] = element;
        ++iSize;
    }
    
    /**
     *  Removes all of the elements from this list.
     */
    void clear() {
        delete[] iStorage;
        iSize = 0;
        iCapacity = 32;
        iStorage = new T[iCapacity];
    }
    
    /**
     *  Returns true if this list contains the specified element.
     */
    bool contains(const T& e) const {
        for(int i=0;i<iSize;++i)
            if(iStorage[i] == e)
                return true;
        return false;
    }
    
    /**
     *  Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const {
        if(index >= iSize) throw IndexOutOfBound();
        return iStorage[index];
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
        if(index >= iSize) throw IndexOutOfBound();
        for (int i=index;i<iSize-1;++i)
            iStorage[i] = iStorage[i+1];
        --iSize;
    }
    
    /**
     *  Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it was present in the list, otherwise false.
     */
    bool remove(const T &e) {
        int p;
        for(p=0;p<iSize;++p)
            if(iStorage[p] == e){
                removeIndex(p);
                return true;
            }
        return false;
    }
    
    /**
     *  Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void set(int index, const T &element) {
        if(index >= iSize) throw IndexOutOfBound();
        iStorage[index] = element;
    }
    
    /**
     *  Returns the number of elements in this list.
     */
    int size() const {
        return iSize;
    }
    
    /**
     *  Returns an iterator over the elements in this list.
     */
    Iterator iterator() {
        return Iterator(this);
    }
};

#endif
