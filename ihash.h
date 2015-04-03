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
ihashpair *ihashpair_new(void);
void ihashpair_free(void *_p);
void ihashpair_free_func(void *_p, void (*free_func)(void *));

/*----------------------------------------------------------------------------*/
/* ihash */
/*----------------------------------------------------------------------------*/
typedef struct IHASH ihash;
struct IHASH
{
  size_t size;  /* size of hash table */
  size_t item;  /* # items */
  size_t used;  /* # used hash value */
  ilist **list; /* hash table */
};

/* new, free, clear, show */
ihash *ihash_new(size_t _size);
ihash *ihash_alloc(size_t _size);
void ihash_clear(ihash *_ih);
void ihash_clear_func(ihash *_ih, void (*free_func)(void *));
void ihash_free(void *_p);
void ihash_free_func(void *_p, void (*free_func)(void *));
void ihash_show(FILE *_fp, ihash *_ih);

/* values */
size_t ihash_size(ihash *_ih);
size_t ihash_item(ihash *_ih);
double ihash_hitrate(ihash *_ih);
double ihash_occupancy(ihash *_ih);

/* accessors */
size_t ihash_hashval(ihash *_ih, const char *_key);
void *ihash_get(ihash *_ih, const char *_key);
int ihash_set(ihash *_ih, const char *_key, void *_val);
void *ihash_delete(ihash *_ih, const char *_key);

/* realloc */
void ihash_realloc(ihash *_ih, size_t _size);

/* hash -> list */
void ihash_pairs(ihash *_ih, ilist *_l);
void ihash_keys(ihash *_ih, ilist *_l);
void ihash_vals(ihash *_ih, ilist *_l);

#endif
