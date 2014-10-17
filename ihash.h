#ifndef _INCLUDE_IHASH_H_
#define _INCLUDE_IHASH_H_

/*----------------------------------------------------------------------------*/
/* include */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <istring.h>
#include <ilist.h>

/*----------------------------------------------------------------------------*/
/* ihash_pair */
/*----------------------------------------------------------------------------*/
typedef struct IHASHPAIR ihashpair;
struct IHASHPAIR
{
  char *key;
  void *val;
};
/* new, free */
ihashpair *ihashpair_new(const char *_key, void *_val);
void ihashpair_free(void *_ihp);
void ihashpair_free_func(void *_ihp, void (*free_func)(void *));
/* comp */
int ihashpair_comp_key(void *_a, void *_b);
int ihashpair_comp_val_int(void *_a, void *_b);
int ihashpair_comp_val_double(void *_a, void *_b);
int ihashpair_comp_val_string(void *_a, void *_b);

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

/* new, free */
ihash *ihash_new(size_t _size);
void   ihash_free(ihash *_ih);
void   ihash_free_func(ihash *_ih, void (*free_func)(void *));

/* accessors */
void  *ihash_get(ihash *_ih, const char *_key);
int    ihash_set(ihash *_ih, const char *_key, void *_val);
void  *ihash_delete(ihash *_ih, const char *_key);
size_t ihash_size(ihash *_ih);
size_t ihash_item(ihash *_ih);
size_t ihash_hashval(ihash *_ih, const char *_key);
ilist *ihash_get_pairs(ihash *_ih);
ilist *ihash_get_keys(ihash *_ih);
ilist *ihash_get_vals(ihash *_ih);

/* show */
void ihash_show(FILE *_fp, ihash *_ih);

#endif
