/** @file */
#ifndef __TREEMAP_H
#define __TREEMAP_H
#include <cstdlib>
#include <ctime>

#include "ElementNotExist.h"

/**
 * TreeMap is the balanced-tree implementation of map. The iterators must
 * iterate through the map in the natural order (operator<) of the key.
 */
template<class K, class V>
class TreeMap
{
    struct TreapNode{
        TreapNode* lf;
        TreapNode* rt;
        K key;
        V value;
        int fix;
        TreapNode(const K& k, const V& v)
        :key(k),value(v),fix(rand()),lf(NULL),rt(NULL){}
    };
private:
    TreapNode *TreapRoot;
    int iSize;
    void rot_lf(TreapNode *&root){
        TreapNode *tmp = root->rt;
        root->rt = tmp->lf;
        tmp->lf = root;
        root = tmp;
    }
    void rot_rt(TreapNode *&root){
        TreapNode *tmp = root->lf;
        root->lf = tmp->rt;
        tmp->rt = root;
        root = tmp;
    }
    void insert(const K& key,const V& value,TreapNode *&root){
        if(root == NULL){
            root = new TreapNode(key,value);
            ++iSize;
        }
        else if (root->key == key) root->value = value;
        else if (key < root->key){
            insert(key,value,root->lf);
            if(root->lf->fix > root->fix) rot_rt(root);
        }
        else {
            insert(key,value,root->rt);
            if(root->rt->fix > root->fix) rot_lf(root);
        }
    }
    bool remove(const K& key,TreapNode *&root){
        if(root == NULL) return false;
        else if (key < root->key) return remove(key, root->lf);
        else if (key > root->key) return remove(key, root->rt);
        else{
            --iSize;
            if(root->rt == NULL){
                TreapNode *tmp = root;
                root = root->lf;
                delete tmp;
                return true;
            }
            else if(root->lf == NULL){
                TreapNode *tmp = root;
                root = root->rt;
                delete tmp;
                return true;
            }
            else if(root->lf->fix > root->rt->fix){
                rot_rt(root);
                return remove(key,root->rt);
            }
            else {
                rot_lf(root);
                return remove(key,root->lf);
            }
        }
    }
    const V& get(const K& key,TreapNode *root) const{
        if(root == NULL) throw ElementNotExist();
        else if(key < root->key) return get(key, root->lf);
        else if(key > root->key) return get(key, root->rt);
        else return root->value;
    }
    bool contain(const K& key, TreapNode *root) const{
        if(root == NULL) return false;
        if(key < root->key) return contain(key,root->lf);
        if(key > root->key) return contain(key,root->rt);
        return true;
    }
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
    
    class Iterator
    {
    private:
        const TreeMap<K,V> *pTreeMap;
        TreapNode **pNode;
        int iSize;
        int position;
        void makelist(int &num,TreapNode *root){
            if(root == NULL) return;
            pNode[iSize] = root;
            ++iSize;
            makelist(iSize,root->lf);
            makelist(iSize,root->rt);
        }
    public:
        Iterator(const TreeMap *in_TreeMap)
        :pTreeMap(in_TreeMap),iSize(0),position(-1){
            pNode = new TreapNode*[pTreeMap->iSize];
            makelist(iSize,pTreeMap->TreapRoot);
        }
        ~Iterator(){
            delete[] pNode;
        }
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
            if(position == iSize - 1) return false;
            return true;
        }
        
        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next() {
            if(!hasNext()) throw ElementNotExist();
            position++;
            return Entry(pNode[position]->key,pNode[position]->value);
        }
    };
    
    /**
     * TODO Constructs an empty tree map.
     */
    TreeMap()
    :iSize(0),TreapRoot(NULL){
        srand(time(NULL));
    }
    
    /**
     * TODO Destructor
     */
    ~TreeMap() {
        while (TreapRoot != NULL)
            remove(TreapRoot->key,TreapRoot);
    }
    
    /**
     * TODO Assignment operator
     */
    TreeMap &operator=(const TreeMap &x) {
        if(this == &x) return *this;
        while (TreapRoot != NULL)
            remove(TreapRoot->key,TreapRoot);
            for(Iterator itr = x.iterator(); itr.hasNext();){
                Entry tmp = itr.next();
                put(tmp.getKey(), tmp.getValue());
            }
        return *this;
    }
    
    /**
     * TODO Copy-constructor
     */
    TreeMap(const TreeMap &x)
    :iSize(0),TreapRoot(NULL) {
        for(Iterator itr = x.iterator(); itr.hasNext();){
            Entry tmp = itr.next();
            put(tmp.getKey(), tmp.getValue());
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
        while (!isEmpty())
            remove(TreapRoot->key,TreapRoot);
    }
    
    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    bool containsKey(const K &key) const {
        return contain(key,TreapRoot);
    }
    
    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) const {
        for(Iterator itr = iterator(); itr.hasNext();){
            Entry tmp = itr.next();
            if(tmp.getValue() == value) return true;
        }
        return false;
    }
    
    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key) const {
        return get(key,TreapRoot);
    }
    
    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const {
        return (TreapRoot==NULL);
    }
    
    /**
     * TODO Associates the specified value with the specified key in this map.
     */
    void put(const K &key, const V &value) {
        insert(key,value,TreapRoot);
    }
    
    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) {
        if(!remove(key,TreapRoot)) throw ElementNotExist();
    }
    
    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const {
        return iSize;
    }
};

#endif
