#include<iostream>
using namespace std;
#ifndef __DLIST_IMPL_H__
#define __DLIST_IMPL_H__
template <class T>
bool Dlist<T>::isEmpty()const{
	return (first==NULL && last==NULL);
}

template <class T>
void Dlist<T>::insertFront(T *op){
	node *n = new node;
	T* temp = op;
	n->op = temp;
	n->prev = NULL;
	if(first == NULL){ 
		n->next = NULL;
		last = n;
	}
	else{
		n->next = first;
		this->first->prev = n;		
	}
	first = n;
}

template <class T>
void Dlist<T>::insertBack(T *op){
	node *n = new node;
	T* temp = op;
	n->op = temp;
	n->next = NULL;
	if(first == NULL){
		n->prev = NULL;
		first = n;
	}
	else{
		n->prev = last;
		this->last->next = n;
	}
	last = n;
}

template <class T>
T* Dlist<T>::removeFront(){
	if(this->isEmpty()){
		emptyList err;
		throw err;
	}
	node* temp = first;
	T* result = temp->op;
	if(!first->next){
		first = NULL;
		last = NULL;	
	}
	else{
		first->next->prev = NULL;
		first = temp->next;
	}
	delete temp;
	return result;
}

template <class T>
T* Dlist<T>::removeBack(){
	if(this->isEmpty()){
		emptyList err;
		throw err;
	}
	node* temp = last;
	T* result = temp->op;
	if(!last->prev){
		first = NULL;
		last = NULL;
	}
	else{
		last->prev->next = NULL;
		last = temp->prev;
	}
	delete temp;
	return result;
}

template <class T>
void Dlist<T>::removeAll(){
	while(!this->isEmpty()){
		T* target = removeFront();
		delete target;
	}
}

template <class T>
void Dlist<T>::copyAll(const Dlist &l){
	first = NULL;
	last = NULL;
	node* temp = l.last;
	while(temp){
		T* copy = new T;
		*copy = *temp->op;
		insertFront(copy);
		temp = temp->prev;
	}
}

template <class T>
Dlist<T>::Dlist(){
	first = NULL;
	last = NULL;
}

template <class T>
Dlist<T>::Dlist(const Dlist &l){
	first = NULL;
	last = NULL;	
	copyAll(l);
}

template <class T>
Dlist<T>& Dlist<T>::operator=(const Dlist &l){
	if(this != &l){
		this->removeAll();
		this->copyAll(l);
	}
	return *this;
}

template <class T>
Dlist<T>::~Dlist(){
	removeAll();
} 
#endif