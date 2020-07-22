#include "listbuffer.h"

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "condutils.h"
#include "list.h"
#include "shutdown.h"

#define FULL_LIST_MSG "\n[Err]: Full buffer reached, possible loss messages\n"
#define MAX_LIST_BUFFER_SIZE (LIST_MAX_NUM_NODES / 2)
#define MAX_BUFFER 4000

static void freeptr(void *pItem) { free(pItem); }

struct ListBuffer_s {
  List *pList;
  pthread_mutex_t mutex;  // Control access to data
  pthread_cond_t cond;    // Handle full/empty cases
};

ListBuffer *ListBuffer_init() {
  ListBuffer *plb = (ListBuffer *)malloc(sizeof(ListBuffer));
  plb->pList = List_create();
  assert(plb->pList != NULL);
  assert(pthread_mutex_init(&(plb->mutex), NULL) == 0);
  assert(pthread_cond_init(&(plb->cond), NULL) == 0);

  return plb;
}
void ListBuffer_free(ListBuffer *plb) {
  List_free(plb->pList, freeptr);
  assert(pthread_cond_destroy(&(plb->cond)) == 0);
  // cond_destroy(&(plb->cond), &(plb->mutex));
  free(plb);
}

void ListBuffer_enqueue(ListBuffer *plb, char *pItem) {
  assert(pthread_mutex_lock(&(plb->mutex)) == 0);
  while (List_count(plb->pList) == MAX_LIST_BUFFER_SIZE) {
    // Handle full buffer case
    fputs(FULL_LIST_MSG, stderr);
    assert(pthread_cond_wait(&(plb->cond), &(plb->mutex)) == 0);
  }
  char *pe = (char *)malloc(MAX_BUFFER * sizeof(char));
  strcpy(pe, pItem);
  assert(List_prepend(plb->pList, pe) == 0);
  // printf("Lc: %d\n", List_count(plb->pList));
  // fflush(stdout);
  assert(pthread_cond_signal(&(plb->cond)) == 0);
  assert(pthread_mutex_unlock(&(plb->mutex)) == 0);
}

char *ListBuffer_dequeue(ListBuffer *plb) {
  char *pItem = NULL;
  assert(pthread_mutex_lock(&(plb->mutex)) == 0);
  while (List_count(plb->pList) == 0) {
    assert(pthread_cond_wait(&(plb->cond), &(plb->mutex)) == 0);
  }
  pItem = (char *)List_trim(plb->pList);
  // printf("Lc: %d\n", List_count(plb->pList));
  // fflush(stdout);
  assert(pthread_cond_signal(&(plb->cond)) == 0);
  assert(pthread_mutex_unlock(&(plb->mutex)) == 0);
  return pItem;
}