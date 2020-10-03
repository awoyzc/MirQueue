# MirQueue
FIFO (Queue) Class for Arduino / ESP32 / ESP8266


This class implements FIFO for Arduino / ESP32 / ESP8266....


How to test :

Serial.print("\n\nQUEUE::testQueue() Initial Heap : ");Serial.println(ESP.getFreeHeap());

MirQueue *q=new MirQueue();
q->testQueue();
delete q;

Serial.print("\nQUEUE::testQueue() Final Heap : ");Serial.println(ESP.getFreeHeap());Serial.println();


How to use :

// Define a content type (to store in the FIFO) 
typedef struct _queuePayloadType{
    char key[64];
    char value[64];
} queuePayloadType; 

// Create the FIFO :
MirQueue *q=new MirQueue();

// Create an element
queuePayloadType  *testPayload;
testPayload=(queuePayloadType*)malloc(sizeof(queuePayloadType));
// set key & value
sprintf(testPayload->key,"key%d",i);
sprintf(testPayload->value,"Value %d",i); 

// Push on FIFO
push(testPayload); 

// and so on.....

// To get back :
testPayload=(queuePayloadType*)pop();
sprintf(buffer,"QUEUE::testQueue() : pop from object [%s,%s]\n",testPayload->key,testPayload->value);

// Check methode testQueue() in src for a complet example


Methodes :

  // Constructor/Destructor
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


