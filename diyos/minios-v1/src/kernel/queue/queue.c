#define MAXQSIZE 100
typedef void ElemType;

typedef struct{
	ElemType *data;
	int front;
	int rear;
}SqQueue;

typedef struct {
	SqQueue  q;
	int key;
	int flag;//0 = can read or write   1 = can not other thread to operate msg_queue
}msg_queue;

void init_queue(SqQueue * Q)
{
	Q->front=-1;
	Q->rear=-1;
	Q->data = malloc(MAXQSIZE);
}

int is_queue_over(SqQueue * Q)
{
	if((Q->rear+1)%MAXQSIZE == Q->front)
		return 1;
	return 0;
}

int is_queue_empty(SqQueue * Q)
{
	if(Q->rear == Q->front)
		return 1;
	return 0;
}


int push_queue(SqQueue * Q,ElemType elem)
{
	if(is_queue_over(Q)
		return -1;
	
	Q->rear=(Q->rear+1)%MAXQSIZE;
	Q->data[Q->rear]=elem;
	
	return 0;
}

int pop_queue(SqQueue * Q,ElemType *elem)
{
	if(is_queue_empty(Q)
		return -1;
	
	Q->front=(Q->front+1)%MAXQSIZE;
	*elem = Q->data[Q->front];
	
	return 0;
}

int get_queue_front(SqQueue * Q,ElemType *elem)
{
	if(is_queue_empty(Q)
		return -1;
	
	*elem = Q->data[(Q->front+1)%MAXQSIZE];
	
	return 0;
}

int msg_queue_write(int key,msg_queue Q,void * data,int size)
{
	return 0;
}

int msg_queue_read(int key,msg_queue Q,void * data,int size)
{
	return 0;
}