
#ifndef MIRQUEUE_H_INCLUDED
#define MIRQUEUE_H_INCLUDED

 
#include <Arduino.h>


#define USE_TEST_METHODE


class MirQueue{

public: 
 
	 typedef struct _queueElementType {
		void *pointer;
		struct _queueElementType *next;
	} queueElementType;


private :
  queueElementType* internalHead=NULL; 
  queueElementType* internalIteratorPointer=NULL; 
  int queueSize=0;

public: 


  MirQueue(); 
  ~MirQueue();
	
	void push( void *pointer); // Push element
	void* pop();  // get first element
	int getQueueSize();
	
	// Iterator tools
	queueElementType* initIterator();
	queueElementType* getNext();
	void *getCurrentPayload();// get current (iterator) element (without removing from queue) 
  void* peek(); // get current (iterator) element (and remove from queue) 


	// Test methode
	void testQueue();


	
};
	
#endif // MIRQUEUE_H_INCLUDED
