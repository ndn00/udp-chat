#ifndef _LIST_BUFFER_H_
#define _LIST_BUFFER_H_

typedef struct ListBuffer_s ListBuffer;
struct ListBuffer_s;

ListBuffer *ListBuffer_init();
void ListBuffer_signal(ListBuffer *plb);
void ListBuffer_free(ListBuffer *plb);
void ListBuffer_enqueue(ListBuffer *plb, char *pItem);
char *ListBuffer_dequeue(ListBuffer *plb);

#endif