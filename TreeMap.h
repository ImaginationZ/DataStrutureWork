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
public:
    class Entry
    {
        K key;
        V value;
    public:
        Entry(const K& k, const V& v)
        {
            key = k;
            value = v;
        }
        
        void setValue(const V& v){
            value = v;
        }
        
        const K& getKey() const
        {
            return key;
        }
        
        const V& getValue() const
        {
            return value;
        }
    };
    
    struct TreapNode{
        TreapNode* lf;
        TreapNode* rt;
        Entry data;
        int fix;
        TreapNode(const K& k, const V& v)
        :data(k,v),fix(rand()),lf(NULL),rt(NULL){}
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
            return;
        }
        else if (root->data.getKey() == key) root->data.setValue(value);
        else if (key < root->data.getKey()){
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
        else if (key < root->data.getKey()) return remove(key, root->lf);
        else if (key > root->data.getKey()) return remove(key, root->rt);
        else{
            if(root->rt == NULL){
                TreapNode *tmp = root;
                root = root->lf;
                delete tmp;
                --iSize;
                return true;
            }
            else if(root->lf == NULL){
                TreapNode *tmp = root;
                root = root->rt;
                delete tmp;
                --iSize;
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
        else if(key < root->data.getKey()) return get(key, root->lf);
        else if(key > root->data.getKey()) return get(key, root->rt);
        else return root->data.getValue();
    }
    
    bool contain(const K& key, TreapNode *root) const{
        if(root == NULL) return false;
        if(key < root->data.getKey()) return contain(key,root->lf);
        if(key > root->data.getKey()) return contain(key,root->rt);
        return true;
    }
    
    void removeTree(TreapNode *&root){
        if(root == NULL) return;
        removeTree(root->lf);
        removeTree(root->rt);
        TreapNode *tmp = root;
        root = NULL;
        delete tmp;
    }
    
public:
    void copyTree(TreapNode *&destination, const TreapNode *source){
        if(source == NULL) return;
        destination = new TreapNode(source->data.getKey(),source->data.getValue());
        destination->fix = source->fix;
        copyTree(destination->lf,source->lf);
        copyTree(destination->rt,source->rt);
    }
    
    TreapNode* findNode(const K& key, TreapNode *root) const{
        if(root == NULL) return NULL;
        if(key >= root->data.getKey()) return findNode(key,root->rt);
        else{
            TreapNode *tmp = findNode(key,root->lf);
            if(tmp != NULL) return tmp;
            return root;
        }
    }

    TreapNode* findMin(TreapNode *root) const{
        if(root == NULL) return NULL;
        if(root->lf != NULL) return findMin(root->lf);
        return root;
    }
    
    class Iterator
    {
    private:
        const TreeMap *pTreeMap;
        bool flag;
        K key;
    public:
        Iterator(const TreeMap* parTreeMap)
        :pTreeMap(parTreeMap),flag(false){}
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
            if (pTreeMap->TreapRoot == NULL) return false;
            return (!flag || pTreeMap->findNode(key, pTreeMap->TreapRoot) != NULL);
            }
        
        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next() {
            if(!hasNext()) throw ElementNotExist();
            TreapNode *tmp;
            if(!flag) tmp = pTreeMap->findMin(pTreeMap->TreapRoot);
            else tmp = pTreeMap->findNode(key,pTreeMap->TreapRoot);
            key = tmp->data.getKey();
            flag = true;
            return tmp->data;
        }
    };
    
    /**
     * TODO Constructs an empty tree map.
     */
    TreeMap()
    :iSize(0),TreapRoot(NULL){}
    
    /**
     * TODO Destructor
     */
    ~TreeMap() {
        removeTree(TreapRoot);
    }
    
    /**
     * TODO Assignment operator
     */
    TreeMap &operator=(const TreeMap &x) {
        if(this == &x) return *this;
        removeTree(TreapRoot);
        iSize = x.iSize;
        copyTree(TreapRoot,x.TreapRoot);
        return *this;
    }
    
    /**
     * TODO Copy-constructor
     */
    TreeMap(const TreeMap &x)
    :iSize(x.iSize),TreapRoot(NULL) {
        copyTree(TreapRoot,x.TreapRoot);
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
        iSize = 0;
        removeTree(TreapRoot);
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
        return !iSize;
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
