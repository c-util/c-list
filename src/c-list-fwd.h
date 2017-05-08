#pragma once

/*
 * Circular Double Linked List Implementation in Standard ISO-C11
 *
 * Header to declare CList structure.
 */

#ifndef _C_LIST_DEFINED

#define _C_LIST_DEFINED

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CList CList;

struct CList {
        CList *next;
        CList *prev;
};

#ifdef __cplusplus
}
#endif

#endif
