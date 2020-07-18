#ifndef _LIST_BUFFER_H_
#define _LIST_BUFFER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

int Display_enqueue(const char* pMsg, int length);
void Display_dequeue();
void Display_free();

int Send_enqueue(const char* pMsg, int length);
void Send_dequeue();
void Send_free();
