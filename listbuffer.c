#include "listbuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

static List *buffer_display, *buffer_send;

static void freeptr(void* pItem) { free(pItem); }

int Display_enqueue(const char* pm) { return List_prepend(buffer_display, pm); }
void Display_dequeue() { return freeptr(List_trim(buffer_display)); }
void Display_free() { return List_free(buffer_display, freeptr) }

int Send_enqueue(const char* pm) { return List_prepend(buffer_send, pm); }
void Send_dequeue() { return freeptr(List_trim(buffer_send)); }
void Send_free() { return List_free(buffer_send, freeptr) }