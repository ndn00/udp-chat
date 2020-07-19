#include "listbuffer.h"

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

#define MAX_LIST_BUFFER_SIZE LIST_MAX_NUM_NODES / 2
static void freeptr(void *pItem) { free(pItem); }

struct ListBuffer_s {
  List *pList;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
};

ListBuffer *ListBuffer_init() {
  ListBuffer *plb = (ListBuffer *)malloc(sizeof(ListBuffer));
  plb->pList = List_create();
  assert(plb->pList != NULL);
  memset(&plb->mutex, 0, sizeof(pthread_mutex_t));
  memset(&plb->cond, 0, sizeof(pthread_mutex_t));
  assert(pthread_mutex_init(&(plb->mutex), NULL) == 0);
  // pthread_mutex_unlock(&(plb->mutex));
  assert(pthread_cond_init(&(plb->cond), NULL) == 0);

  return plb;
}
void ListBuffer_free(ListBuffer *plb) {
  List_free(plb->pList, freeptr);
  free(plb);
}
void ListBuffer_enqueue(ListBuffer *plb, char *pItem) {
  pthread_mutex_lock(&(plb->mutex));
  while (List_count(plb->pList) == MAX_LIST_BUFFER_SIZE) {
    pthread_cond_wait(&(plb->cond), &(plb->mutex));
  }
  assert(List_prepend(plb->pList, pItem) == 0);
  pthread_cond_signal(&(plb->cond));
  pthread_mutex_unlock(&(plb->mutex));
}

char *ListBuffer_dequeue(ListBuffer *plb) {
  char *pItem;
  pthread_mutex_lock(&(plb->mutex));
  while (List_count(plb->pList) == 0) {
    pthread_cond_wait(&(plb->cond), &(plb->mutex));
  }
  pItem = (char *)List_trim(plb->pList);
  assert(pItem != NULL);
  pthread_cond_signal(&(plb->cond));
  pthread_mutex_unlock(&(plb->mutex));
  return pItem;
}