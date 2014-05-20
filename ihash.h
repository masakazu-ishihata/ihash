#ifndef _INCLUDE_IHASH_H_
#define _INCLUDE_IHASH_H_

/*----------------------------------------------------------------------------*/
/* include */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <ilist.h>
#include <iutil.h>

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
void ihashpair_free(ihashpair *_ihp);
void ihashpair_free_func(ihashpair *_ihp, void (*free_func)(void *));
/* comp */
int ihashpair_comp_key(ihashpair *_a, ihashpair *_b);
int ihashpair_comp_val_int(ihashpair *_a, ihashpair *_b);
int ihashpair_comp_val_double(ihashpair *_a, ihashpair *_b);
int ihashpair_comp_val_str(ihashpair *_a, ihashpair *_b);

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

/* hash value */
size_t ihash_hashval(ihash *_ih, const char *_key);

/* accessor */
void  *ihash_get(ihash *_ih, const char *_key);
int    ihash_set(ihash *_ih, const char *_key, void *_val);
void  *ihash_delete(ihash *_ih, const char *_key);
size_t ihash_size(ihash *_ih);
size_t ihash_item(ihash *_ih);

/* get */
ilist *ihash_get_pairs(ihash *_ih);
ilist *ihash_get_keys(ihash *_ih);
ilist *ihash_get_vals(ihash *_ih);

#endif
