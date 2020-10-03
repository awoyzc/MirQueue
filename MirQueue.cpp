

//#include <stdio.h>
//#include <stdlib.h>

#include "MirQueue.h" 





#ifndef PRINTLN
#define PRINTLN Serial.println
//#define PRINT //
#endif
#ifndef PRINT
#define PRINT Serial.print
//#define PRINT //
#endif
#ifndef TRACE
#define TRACE //
#endif



/////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
/////////////////////////////////////////////////////////////////////////

MirQueue::MirQueue(){
  internalHead=NULL; 
  internalIteratorPointer=NULL;
  queueSize=0;
  } 



MirQueue::~MirQueue(){
    // Empties queue....
    void* pointer;
    PRINT("QUEUE::Destructor...\n");
    while (getQueueSize()>0){ //actionQueue->pointer!=NULL){ // FIXME : >1 ou NULL 
        pointer=(void*)pop();       
        if (pointer!=NULL) free(pointer);
    }
}

/////////////////////////////////////////////////////////////////////////
// PUSH
/////////////////////////////////////////////////////////////////////////

void MirQueue::push( void *pointer) {
    queueElementType *new_node = (queueElementType*)malloc(sizeof(queueElementType));
    if (!new_node) {
		  PRINT("QUEUE::push Error within malloc()\n");
		  return;
		}

    new_node->pointer = pointer;
    new_node->next = internalHead;

    internalHead = new_node;

    queueSize++;
}




/////////////////////////////////////////////////////////////////////////
// GetQueueSize
/////////////////////////////////////////////////////////////////////////
// FIXME : Metre un compteur, ca va eviter de faire une boule pour connaitre la taille !!

// Non static methode
int MirQueue::getQueueSize() {
	if (internalHead == NULL) return 0;

  return queueSize;
  
	/*
	queueElementType *current = internalHead;
	int compteur=1;
	while (current->next != NULL) {
        //prev = current;
        current = current->next;
		compteur++;
    }
	
	return compteur;
  */
	
}



/////////////////////////////////////////////////////////////////////////
// POP
/////////////////////////////////////////////////////////////////////////

void* MirQueue::pop() {
  queueElementType *current, *prev = NULL;
  void* retPointer=NULL;
  
  if (internalHead == NULL) return NULL;
  
  current = internalHead;
  while (current->next != NULL) {
      prev = current;
      current = current->next;
  }
  
  retPointer = current->pointer;
  free(current);
  
  if (prev)
      prev->next = NULL;
  else
      internalHead = NULL;

  queueSize--;
  
  return retPointer;
}




/////////////////////////////////////////////////////////////////////////
// Peek
/////////////////////////////////////////////////////////////////////////

void* MirQueue::peek() {
    queueElementType *current, *prev = NULL;
    void* retPointer=NULL;

    if (internalHead == NULL) return NULL;
    if (internalIteratorPointer == NULL) return NULL;

    // Case 1 : le internalIteratorPointer==internalHead
    if (internalIteratorPointer==internalHead){
      current = internalIteratorPointer;
      retPointer = current->pointer;
      internalIteratorPointer=current->next;
      internalHead=current->next;
      free(current);
      queueSize--;
      return retPointer;
    }

    // Case 2 : internalIteratorPointer!=internalHead
    
    current = internalHead;
    while ((current->next != NULL) && (current!=internalIteratorPointer)) {
        prev = current;
        current = current->next;
    }

    retPointer = current->pointer;
    
    
    if (prev){
        prev->next = current->next;
        internalIteratorPointer= current->next;
    }else
        internalIteratorPointer = NULL;

    free(current);
    queueSize--;
    return retPointer;
}


/////////////////////////////////////////////////////////////////////////
// Iteration
/////////////////////////////////////////////////////////////////////////


MirQueue::queueElementType* MirQueue::initIterator(){
	internalIteratorPointer=internalHead;	
    return internalIteratorPointer ; 
}


MirQueue::queueElementType* MirQueue::getNext(){
	if (internalIteratorPointer == NULL) return NULL;
    return internalIteratorPointer= internalIteratorPointer->next ; 
}

void * MirQueue::getCurrentPayload(){
	if (internalIteratorPointer == NULL) return NULL;
	return internalIteratorPointer->pointer;
}




/////////////////////////////////////////////////////////////////////////
// Test
/////////////////////////////////////////////////////////////////////////


#ifdef USE_TEST_METHODE

typedef struct _queuePayloadType{
    char key[64];
    char value[64];
} queuePayloadType; 



void MirQueue::testQueue(){
	char buffer[256];
  int i;
	
	queuePayloadType  *testPayload;

  PRINT("\n\nQUEUE::testQueue() Initial Heap : ");PRINTLN(ESP.getFreeHeap());
  
  // Push
  for (i=0;i<5;i++){
    testPayload=(queuePayloadType*)malloc(sizeof(queuePayloadType));
  
    sprintf(testPayload->key,"key%d",i);
  	sprintf(testPayload->value,"Value %d",i); 
  
  	sprintf(buffer,"QUEUE::testQueue() : push in object [%s,%s]\n",testPayload->key,testPayload->value);
  	PRINT(buffer);
  	push(testPayload); 
  }

  PRINT("QUEUE::testQueue()  Heap after PUSH : ");PRINTLN(ESP.getFreeHeap());
  PRINTLN("QUEUE::testQueue() : Test Iterator 1 : ");
  
  // Iteration
  initIterator();
  
  do{
      testPayload=(queuePayloadType*)getCurrentPayload();       
      sprintf(buffer,"QUEUE::testQueue() : enum from object [%s,%s]\n",testPayload->key,testPayload->value);
      PRINT(buffer);
      
  } while (getNext()!=NULL);
  
  PRINT("QUEUE::testQueue()  Heap after ITERATOR : ");PRINTLN(ESP.getFreeHeap());
  
  
  // Peek
  PRINT("QUEUE::testQueue() : Test queue.... peek actionItem:\n");
  initIterator();
  do{
      testPayload=(queuePayloadType*)getCurrentPayload();  
      if (!strcmp(testPayload->key,"key4")) break;
  }while (getNext()!=NULL);    
  
  if (!strcmp(testPayload->key,"key4")) {
    testPayload=(queuePayloadType*)peek();
    sprintf(buffer,"QUEUE::testQueue() : peek from object [%s,%s]\n",testPayload->key,testPayload->value);
    PRINT(buffer);
    free(testPayload);
  } else PRINTLN("NOT FOUND");
  
  PRINT("QUEUE::testQueue()  Heap after PEEK : ");PRINTLN(ESP.getFreeHeap());
  PRINT("QUEUE::testQueue() : Test Iterator 3\n");
  // Iterator
  initIterator();
  
  do{
      testPayload=(queuePayloadType*)getCurrentPayload();       
      sprintf(buffer,"QUEUE::testQueue() : enum from object [%s,%s]\n",testPayload->key,testPayload->value);
      PRINT(buffer);
      
  }while (getNext()!=NULL);
  
  PRINT("QUEUE::testQueue()  Heap after ITERATOR 3 : ");PRINTLN(ESP.getFreeHeap());
  
  // Pop
  PRINT("QUEUE::testQueue() : Test queue.... pop actionItem:\n");
  while (getQueueSize()>0){ //actionQueue->pointer!=NULL){
      testPayload=(queuePayloadType*)pop();

      sprintf(buffer,"QUEUE::testQueue() : pop from object [%s,%s]\n",testPayload->key,testPayload->value);
      PRINT(buffer);
      free(testPayload);
  }
  PRINT("QUEUE::testQueue()  Heap after POP : ");PRINTLN(ESP.getFreeHeap());
  PRINT("QUEUE::testQueue() : Test ended\n"); 
	
	
}

#endif
