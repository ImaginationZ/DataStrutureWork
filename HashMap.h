/** @file */

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include "ElementNotExist.h"

/**
 * HashMap is a map implemented by hashing. Also, the 'capacity' here means the
 * number of buckets in your internal implemention, not the current number of the
 * elements.
 *
 * Template argument H are used to specify the hash function.
 * H should be a class with a static function named ``hashCode'',
 * which takes a parameter of type K and returns a value of type int.
 * For example, the following class
 * @code
 *      class Hashint {
 *      public:
 *          static int hashCode(int obj) {
 *              return obj;
 *          }
 *      };
 * @endcode
 * specifies an hash function for integers. Then we can define:
 * @code
 *      HashMap<int, int, Hashint> hash;
 * @endcode
 *
 * Hash function passed to this class should observe the following rule: if two keys
 * are equal (which means key1 == key2), then the hash code of them should be the
 * same. However, it is not generally required that the hash function should work in
 * the other direction: if the hash code of two keys are equal, the two keys could be
 * different.
 *
 * Note that the correctness of HashMap should not rely on the choice of hash function.
 * This is to say that, even the given hash function always returns the same hash code
 * for all keys (thus causing a serious collision), methods of HashMap should still
 * function correctly, though the performance will be poor in this case.
 *
 * The order of iteration could be arbitary in HashMap. But it should be guaranteed
 * that each (key, value) pair be iterated exactly once.
 */
template <class K, class V, class H>
class HashMap
{
public:
    class Entry
    {
        K key;
        V value;
    public:
        Entry(K k, V v)
        {
            key = k;
            value = v;
        }

        K getKey() const
        {
            return key;
        }

        V getValue() const
        {
            return value;
        }
    };
    private:
        struct Node{
            K key;
            V value;
            Node* next;
            Node* pre;
            Node(const K& k, const V& v, Node* p = NULL, Node* n = NULL)
            :key(k), value(v), pre(p), next(n){
            if(pre == NULL) pre = this;
            if(next == NULL) next = this;
            }
            Node():pre(this),next(this){}
        }**iHashTable;
        static const int iTableNum = 32;
        int iSize;
        Node* add(const K& k, const V& v, Node* where){
            ++iSize;
            Node *tmp = new Node(k, v, where, where->next);
            tmp->pre->next = tmp;
            tmp->next->pre = tmp;
            return tmp;
        }
        Node* remove(Node* where){
            --iSize;
            where->pre->next = where->next;
            where->next->pre = where->pre;
            Node *tmp = where->next;
            delete where;
            return tmp;
        }
        static int getTableNumber(const K& obj) {
            return H::hashCode(obj) % iTableNum;
        }
public:
    class Iterator
    {
    private:
        const HashMap* pHashMap;
        int iSize;
        int position;
        Node** pNode;
    public:
        Iterator(const HashMap* parHashMap)
        :pHashMap(parHashMap), position(-1), iSize(0){
            pNode = new Node*[parHashMap->iSize];
            for(int i=0; i<parHashMap->iTableNum; ++i){
                for(Node* tmp = (parHashMap->iHashTable[i])->next; tmp != parHashMap->iHashTable[i]; tmp = tmp->next){
                    pNode[iSize] = tmp;
                    ++iSize;
                }
            }
        }
        ~Iterator(){
            delete[] pNode;
        }
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
            return (position < iSize - 1);
        }

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next() {
            if(!hasNext()) throw ElementNotExist();
            ++position;
            return Entry(pNode[position]->key,pNode[position]->value);
        }
    };
  
    /**
     * TODO Constructs an empty hash map.
     */
    HashMap()
    :iSize(0){
        iHashTable = new Node*[iTableNum];
        for(int i=0; i<iTableNum; ++i){
            iHashTable[i] = new Node;
        }
    }

    /**
     * TODO Destructor
     */
    ~HashMap() {
        clear();
        delete[] iHashTable;
    }

    /**
     * TODO Assignment operator
     */
    HashMap &operator=(const HashMap &x) {
        if(this == &x) return *this;
        clear();
        for(Iterator itr = x.iterator(); itr.hasNext();){
            Entry tmp = itr.next();
            put(tmp.getKey(),tmp.getValue());
        }
        return *this;
    }

    /**
     * TODO Copy-constructor
     */
    HashMap(const HashMap &x)
    :iSize(0){
        iHashTable = new Node*[iTableNum];
        for(int i=0; i<iTableNum; ++i){
            iHashTable[i] = new Node;
        }
        for(Iterator itr = x.iterator(); itr.hasNext();){
            Entry tmp = itr.next();
            put(tmp.getKey(),tmp.getValue());
        }
    }

    /**
     * TODO Returns an iterator over the elements in this map.
     */
    Iterator iterator() const {
        return Iterator(this);
    }

    /**
     * TODO Removes all of the mappings from this map.
     */
    void clear() {
        for(int i=0; i<iTableNum; ++i){
            for(Node *tmp = iHashTable[i]->next; tmp != iHashTable[i];){
                tmp = remove(tmp);
            }
        }
    }

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    bool containsKey(const K &key) const {
        int iTable = getTableNumber(key);
        for(Node *tmp = iHashTable[iTable]->next; tmp != iHashTable[iTable]; tmp = tmp->next){
            if(tmp->key == key){
                return true;
            }
        }
        return false;
    }

    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) const {
        for(Iterator itr = iterator(); itr.hasNext();){
            if(itr.next().getValue() == value) return true;
        }
        return false;
    }

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key) const {
        int iTable = getTableNumber(key);
        for(Node *tmp = iHashTable[iTable]->next; tmp != iHashTable[iTable]; tmp = tmp->next){
            if(tmp->key == key){
                return tmp->value;
            }
        }
        throw ElementNotExist();
    }

    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const {
        return (iSize == 0);
    }

    /**
     * TODO Associates the specified value with the specified key in this map.
     */
    void put(const K &key, const V &value) {
        int iTable = getTableNumber(key);
        for(Node *tmp = iHashTable[iTable]->next; tmp != iHashTable[iTable]; tmp = tmp->next){
            if(tmp->key == key){
                tmp->value = value;
                return;
            }
        }
        add(key,value,iHashTable[iTable]);
    }

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) {
        int iTable = getTableNumber(key);
        for(Node *tmp = iHashTable[iTable]->next; tmp != iHashTable[iTable]; tmp = tmp->next){
            if(tmp->key == key){
                remove(tmp);
                return;
            }
        }
        throw ElementNotExist();
    }

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const {
        return iSize;
    }
};

#endif
