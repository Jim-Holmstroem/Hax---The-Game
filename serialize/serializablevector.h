#ifndef SERIALIZABLEVECTOR_H
#define SERIALIZABLEVECTOR_H

#include<iostream>
#include<vector>
#include<exception>
#include"parsehelper.h"
#include"iserializablecontainer.h"

namespace hax {

template < class T,
           class Allocator=std::allocator<T> >
    class SerializableVector : public ISerializableContainer<T>
{
  protected:
    std::vector<T,Allocator> content;

    //NOTE shouldn't inherit from STL-containers since they aren't made for it and you can end up with hardtofind errors and memory-leaks.
    //REF http://www.cplusplus.com/reference/stl/vector/

  public:
    typedef typename std::vector<T,Allocator>::value_type value_type;

    typedef typename std::vector<T,Allocator>::allocator_type allocator_type;

    typedef typename std::vector<T,Allocator>::size_type size_type;
    typedef typename std::vector<T,Allocator>::difference_type difference_type;

    typedef typename std::vector<T,Allocator>::reference reference;
    typedef typename std::vector<T,Allocator>::const_reference const_reference;

    typedef typename std::vector<T,Allocator>::pointer pointer;
    typedef typename std::vector<T,Allocator>::const_pointer const_pointer;

    typedef typename std::vector<T,Allocator>::iterator iterator;
    typedef typename std::vector<T,Allocator>::const_iterator const_iterator;
    typedef typename std::vector<T,Allocator>::reverse_iterator reverse_iterator;
    typedef typename std::vector<T,Allocator>::const_reverse_iterator const_reverse_iterator;

    explicit SerializableVector(const allocator_type& alloc = allocator_type()):content(alloc){};
    explicit SerializableVector(size_type n,const_reference value = value_type(),const allocator_type& alloc = allocator_type()):content(n,value){};
    template<class InputIterator> SerializableVector(InputIterator first,InputIterator last):content(first,last){};
    SerializableVector(const SerializableVector& x)
    {
        this->content = x.content;
    };
    SerializableVector& operator=(const SerializableVector& x)
    {
        this->content = x.content;
        return *this;
    };
    ~SerializableVector(){};

    /* Iterators */

          iterator begin()        {return content.begin();};
    const_iterator begin() const{return content.begin();};
          iterator end()      {return content.end();};
    const_iterator end() const{return content.end();};
          reverse_iterator rbegin()      {return content.rbegin();};
    const_reverse_iterator rbegin() const{return content.rbegin();};
          reverse_iterator rend()      {return content.rend();};
    const_reverse_iterator rend() const{return content.rend();};

    /* Capacity */
    size_type size() const{return content.size();};
    size_type max_size() const{return content.max_size();};
    void resize(size_type size,T c = T()){content.resize(size,c);};
    size_type capacity() const{return content.capacity();};
    bool empty() const{return content.empty();};
    void reserve(size_type n){content.reserve(n);};

    /* Element Access */
          reference operator[](size_type n)      {return content[n];};
    const_reference operator[](size_type n) const{return content[n];};
          reference at(size_type n)      {return content.at(n);};
    const_reference at(size_type n) const{return content.at(n);};
          reference front()      {return content.front();};
    const_reference front() const{return content.front();};
          reference back()      {return content.back();};
    const_reference back() const{return content.back();};

    /* Modifiers */
    template <class InputIterator> void assign(InputIterator first,InputIterator last){content.assign(first,last);};
    void assign(size_type n,const_reference u){content.assign(n,u);};
    void push_back(const_reference x){content.push_back(x);};
    void pop_back(){content.pop_back();};
    iterator insert(iterator pos,const_reference x){return content.insert(pos,x);};
        void insert(iterator pos,size_type n,const_reference x){content.insert(pos,n,x);};
    template <class InputIterator> void insert(iterator pos,InputIterator first,InputIterator last){content.insert(pos,first,last);};
    iterator erase(iterator pos){return content.erase(pos);};
    iterator erase(iterator first,iterator last){return content.erase(first,last);};
    void swap(SerializableVector& vec){
        content.swap(vec.content);
    };
    void clear(){content.clear();};

    /* Allocator */
    allocator_type get_allocator() const{return content.get_allocator();};

  protected:
    /* Pure virtual functions from ISerializableContainer */
    virtual void ElementToString(std::ostream& out, const value_type& elem) const
    {
        out << elem << ' ';
    };
    virtual void ElementFromString(std::istream& in)
    {
        T elem;
        in >> elem;
/*
        checkToken(in,"{");
	elem->FromString(in);
        checkToken(in,"}");
*/
        content.push_back(elem);
    };
    virtual void ToString(std::ostream& out) const
    {
        SerializableVector<T,Allocator>::const_iterator it = content.begin();
        for(; it!=content.end(); ++it)
        {
            out << "# ";
            ElementToString(out,*it);
        }
    };
    virtual void FromString(std::istream& in)
    {
        in.get();
        while(in.peek()=='#')
        {
            checkToken(in,"#");
            ElementFromString(in);
            in.get();
        }
    };
  
  public:
    virtual std::string getType() const
    {
        return "SerializableVector<"+T().getType()+">";
    };

};



/**********************************************************
Template partial specialization for a vector with pointers
**********************************************************/
template < class T >
    class SerializableVector<T*> : public ISerializableContainer<T*>
{
  protected:
    std::vector<T*,std::allocator<T*> > content;

  public:
    typedef typename std::vector<T*,std::allocator<T*> >::value_type value_type;
    typedef typename std::vector<T*,std::allocator<T*> >::allocator_type allocator_type;

    typedef typename std::vector<T*,std::allocator<T*> >::size_type size_type;
    typedef typename std::vector<T*,std::allocator<T*> >::difference_type difference_type;

    typedef typename std::vector<T*,std::allocator<T*> >::reference reference;
    typedef typename std::vector<T*,std::allocator<T*> >::const_reference const_reference;

    typedef typename std::vector<T*,std::allocator<T*> >::pointer pointer;
    typedef typename std::vector<T*,std::allocator<T*> >::const_pointer const_pointer;

    typedef typename std::vector<T*,std::allocator<T*> >::iterator iterator;
    typedef typename std::vector<T*,std::allocator<T*> >::const_iterator const_iterator;
    typedef typename std::vector<T*,std::allocator<T*> >::reverse_iterator reverse_iterator;
    typedef typename std::vector<T*,std::allocator<T*> >::const_reverse_iterator const_reverse_iterator;

    explicit SerializableVector(const allocator_type& alloc = allocator_type()):content(alloc){};
    explicit SerializableVector(size_type n,const_reference value = value_type(),const allocator_type& alloc = allocator_type()):content(n,value){};
    template<class InputIterator> SerializableVector(InputIterator first,InputIterator last):content(first,last){};
    SerializableVector(const SerializableVector& x)
    {
        this->content = x.content;
    };
    SerializableVector& operator=(const SerializableVector& x)
    {
        this->content = x.content;
        return *this;
    };
    ~SerializableVector(){};

    /* Iterators */

          iterator begin()        {return content.begin();};
    const_iterator begin() const{return content.begin();};
          iterator end()      {return content.end();};
    const_iterator end() const{return content.end();};
          reverse_iterator rbegin()      {return content.rbegin();};
    const_reverse_iterator rbegin() const{return content.rbegin();};
          reverse_iterator rend()      {return content.rend();};
    const_reverse_iterator rend() const{return content.rend();};

    /* Capacity */
    size_type size() const{return content.size();};
    size_type max_size() const{return content.max_size();};
    void resize(size_type size,T c = T()){content.resize(size,c);};
    size_type capacity() const{return content.capacity();};
    bool empty() const{return content.empty();};
    void reserve(size_type n){content.reserve(n);};

    /* Element Access */
          reference operator[](size_type n)      {return content[n];};
    const_reference operator[](size_type n) const{return content[n];};
          reference at(size_type n)      {return content.at(n);};
    const_reference at(size_type n) const{return content.at(n);};
          reference front()      {return content.front();};
    const_reference front() const{return content.front();};
          reference back()      {return content.back();};
    const_reference back() const{return content.back();};

    /* Modifiers */
    template <class InputIterator> void assign(InputIterator first,InputIterator last){content.assign(first,last);};
    void assign(size_type n,const_reference u){content.assign(n,u);};
    void push_back(const_reference x){content.push_back(x);};
    void pop_back(){content.pop_back();};
    iterator insert(iterator pos,const_reference x){return content.insert(pos,x);};
        void insert(iterator pos,size_type n,const_reference x){content.insert(pos,n,x);};
    template <class InputIterator> void insert(iterator pos,InputIterator first,InputIterator last){content.insert(pos,first,last);};
    iterator erase(iterator pos){return content.erase(pos);};
    iterator erase(iterator first,iterator last){return content.erase(first,last);};
    void swap(SerializableVector& vec){
        content.swap(vec.content);
    };
    void clear(){content.clear();};

    /* Allocator */
    allocator_type get_allocator() const{return content.get_allocator();};

  protected:
    /* Pure virtual functions from ISerializableContainer */
    virtual void ElementToString(std::ostream& out, const value_type& elem) const
    {
        out << *elem << ' ';
    };

    //not defined here so that it can be specialized elsewhere when needed
    void ElementFromString(std::istream& in);

    virtual void ToString(std::ostream& out) const
    {
        std::cout << "using partial specialized SerializableVector<T*>::ToString..." << std::endl;
        SerializableVector<T*,std::allocator<T*> >::const_iterator it = content.begin();
        for(; it!=content.end(); ++it)
        {
            out << "# ";
            ElementToString(out,*it);
        }
    };
    virtual void FromString(std::istream& in)
    {
        in.get();
        while(in.peek()=='#')
        {
            checkToken(in,"#");
            ElementFromString(in);
            in.get();
        }
    };
  
  public:
    //not defined here so that it can be specialized elsewhere when needed
    virtual std::string getType() const;
};


template <class T>
void SerializableVector<T*>::ElementFromString(std::istream& in)
{
    std::cout << "using partial specialized SerializableVector<T*>::ElementFromString..." << std::endl;
    T* elem = new T();
    in >> *elem;
    content.push_back(elem);
};

template <class T>
std::string SerializableVector<T*>::getType() const
{
    return "SerializableVector<"+T().getType()+"*>";
};


}
#endif
