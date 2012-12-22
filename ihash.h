#ifndef _INCLUDE_IHASH_H_
#define _INCLUDE_IHASH_H_

/*----------------------------------------------------------------------------*/
/* include */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "iutil.h"
#include "ilist.h"

/*----------------------------------------------------------------------------*/
/* ihash_pair */
/*----------------------------------------------------------------------------*/
typedef struct IHASHPAIR ihashpair;
struct IHASHPAIR
{
  char *key;
  void *val;
};
ihashpair *ihashpair_new(const char *_key, void *_val);
void ihashpair_free(ihashpair *_ihp);
void ihashpair_free_func(ihashpair *_ihp, void (*free_func)(void *));

/*----------------------------------------------------------------------------*/
/* ihash */
/*----------------------------------------------------------------------------*/
typedef struct IHASH ihash;
struct IHASH
{
  size_t size;
  size_t item;
  ilist **list;
};
/* new/free */
ihash *ihash_new(size_t _size);
void ihash_free(ihash *_ih);
void ihash_free_func(ihash *_ih, void (*free_func)(void *));
/* getter/setter */
size_t ihash_hashval(ihash *_ih, const char *_key);
void *ihash_get(ihash *_ih, const char *_key);
int ihash_set(ihash *_ih, const char *_key, void *_val);
/* show */
void ihash_show(ihash *_ih, FILE *fp);

#endif
