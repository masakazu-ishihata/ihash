#include "ihash.h"

/*----------------------------------------------------------------------------*/
/* ihashpair */
/*----------------------------------------------------------------------------*/

/*------------------------------------*/
/* new */
/*------------------------------------*/
ihashpair *ihashpair_new(void)
{
  ihashpair *ihp = (ihashpair *)malloc(sizeof(ihashpair));
  ihp->key = NULL;
  ihp->val = NULL;
  return ihp;
}

/*------------------------------------*/
/* clear */
/*------------------------------------*/
void ihashpair_free(void *_p)
{
  ihashpair_free_func(_p, NULL);
}

/*------------------------------------*/
/* free with inner free */
/*------------------------------------*/
void ihashpair_free_func(void *_p, void (*free_func)(void *))
{
  ihashpair *_ihp = (ihashpair *)_p;
  if(_ihp != NULL){
    free(_ihp->key);
    if(free_func != NULL)  free_func(_ihp->val);
    free(_ihp);
  }
}


/*----------------------------------------------------------------------------*/
/* ihash */
/*----------------------------------------------------------------------------*/

/*------------------------------------*/
/* new */
/*------------------------------------*/
ihash *ihash_new(size_t _size)
{
  return ihash_alloc(_size);
}
ihash *ihash_alloc(size_t _size)
{
  int i;
  ihash *ih;

  ih = (ihash *)malloc(sizeof(ihash));
  ih->size = _size;
  ih->item = 0;
  ih->used = 0;
  ih->list = (ilist **)malloc(ih->size * sizeof(ilist *));
  for(i=0; i<ih->size; ih->list[i]=ilist_new(), i++);

  return ih;
}

/*------------------------------------*/
/* clear */
/*------------------------------------*/
void ihash_clear(ihash *_ih)
{
  ihash_clear_func(_ih, NULL);
}

/*------------------------------------*/
/* clear with inner free */
/*------------------------------------*/
void ihash_clear_func(ihash *_ih, void (*free_func)(void *))
{
  int i;
  ihashpair *ihp;

  /* reset table */
  if(_ih != NULL)
    for(i=0; i<_ih->size; i++)
      ILIST_WHILE(_ih->list[i], ihp)
        ihashpair_free_func(ihp, free_func);

  /* reset counters */
  _ih->item = 0;
  _ih->used = 0;
}

/*------------------------------------*/
/* free */
/*------------------------------------*/
void ihash_free(void *_p)
{
  ihash_free_func(_p, NULL);
}

/*------------------------------------*/
/* free with inner free */
/*------------------------------------*/
void ihash_free_func(void *_p, void (*free_func)(void *))
{
  int i;
  ihash *ih = (ihash *)_p;

  if(ih != NULL){
    /* clear elements */
    ihash_clear_func(ih, free_func);

    /* free itself */
    for(i=0; i<ih->size; i++)
      ilist_free(ih->list[i]);
    free(ih->list);
    free(ih);
  }
}

/*------------------------------------*/
/* show */
/*------------------------------------*/
void ihash_show(FILE *_fp, ihash *_ih)
{
  int hv;
  ihashpair *ihp;

  fprintf(_fp, "HASH SIZE = %d\n", (int)_ih->size);
  fprintf(_fp, "# ITMES   = %d\n", (int)_ih->item);
  fprintf(_fp, "# USED HV = %d\n", (int)_ih->used);
  fprintf(_fp, "   HV,        KEY,        VAL\n");
  for(hv=0; hv<_ih->size; hv++)
    ILIST_FOR(_ih->list[hv], ihp)
      fprintf(_fp, "%5d, %10s, %10p\n", hv, ihp->key, ihp->val);
}


/*----------------------------------------------------------------------------*/
/* fundumental accessor */
/*----------------------------------------------------------------------------*/

/*------------------------------------*/
/* size */
/*------------------------------------*/
size_t ihash_size(ihash *_ih)
{
  return _ih->size;
}

/*------------------------------------*/
/* item */
/*------------------------------------*/
size_t ihash_item(ihash *_ih)
{
  return _ih->item;
}

/*------------------------------------*/
/* hit rate = #items / # used hvs */
/*------------------------------------*/
double ihash_hitrate(ihash *_ih)
{
  return _ih->used == 0 ? -1 : _ih->item / (double)_ih->used;
}

/*------------------------------------*/
/* occupancy */
/*------------------------------------*/
double ihash_occupancy(ihash *_ih)
{
  return _ih->used / (double)_ih->size;
}


/*----------------------------------------------------------------------------*/
/* accessor */
/*----------------------------------------------------------------------------*/

/*------------------------------------*/
/* hash value */
/*------------------------------------*/
size_t ihash_hashval(ihash *_ih, const char *_key)
{
  int i;
  size_t hv = 0;

  for(i=0; i<strlen(_key); i++)
    hv = (hv * 137 + _key[i]) % _ih->size;

  return hv;
}

/*------------------------------------*/
/* get val of key */
/*------------------------------------*/
void *ihash_get(ihash *_ih, const char *_key)
{
  ihashpair *ihp;
  ilist *il = _ih->list[ ihash_hashval(_ih, _key) ];

  ILIST_FOR(il, ihp)
    if(strcmp(_key, ihp->key) == 0)
      return ihp->val;

  return NULL;
}

/*------------------------------------*/
/* set [key, val] */
/*------------------------------------*/
int ihash_set(ihash *_ih, const char *_key, void *_val)
{
  /* realloc if half of tables are filled */
  if(ihash_occupancy(_ih) > 2/3.0)
    ihash_realloc(_ih, (size_t)(3 * _ih->size / 2.0));

  /* set */
  ihashpair *ihp;
  ilist *il = _ih->list[ ihash_hashval(_ih, _key) ];

  /* break if _key is already in _ih */
  ILIST_FOR(il, ihp)
    if(strcmp(_key, ihp->key) == 0)
      return -1;

  /* new member */
  ihp = ihashpair_new();
  ihp->key = strcln(_key);
  ihp->val = _val;

  /* set the new member */
  ilist_push(il, ihp);

  /* first use of il */
  if(ilist_size(il) == 1)
    _ih->used++;

  return ++_ih->item;
}

/*------------------------------------*/
/* delete [key, val] */
/*------------------------------------*/
void *ihash_delete(ihash *_ih, const char *_key)
{
  ihashpair *ihp;
  ilist *il = _ih->list[ ihash_hashval(_ih, _key) ];;
  void *val;

  ILIST_FOR(il, ihp){
    if(strcmp(_key, ihp->key) == 0){
      ilist_remove(il);
      val = ihp->val;
      ihashpair_free(ihp);
      _ih->item--;
      if(ilist_size(il) == 0) _ih->used--;
      return val;
    }
  }
  return NULL;
}


/*----------------------------------------------------------------------------*/
/* realloc */
/*----------------------------------------------------------------------------*/
void ihash_realloc(ihash *_ih, size_t _size)
{
  size_t i, hv;
  ilist *il;
  ihashpair *ihp;
  size_t  old_size = _ih->size;
  ilist **old_list = _ih->list;

  /* remalloc */
  _ih->size = _size;
  _ih->used = 0;
  _ih->list = (ilist **)calloc(_ih->size, sizeof(ilist *));
  for(i=0; i<_ih->size; i++)
    _ih->list[i] = ilist_new();

  /* old -> new */
  for(i=0; i<old_size; i++){
    ILIST_WHILE(old_list[i], ihp){
      hv = ihash_hashval(_ih, ihp->key);
      il = _ih->list[hv];
      ilist_push(il, ihp);
      if(ilist_size(il) == 1) _ih->used++;
    }
    ilist_free( old_list[i] );
  }
  free(old_list);
}


/*----------------------------------------------------------------------------*/
/* hash -> list */
/*----------------------------------------------------------------------------*/

/*------------------------------------*/
/* list of pairs */
/*------------------------------------*/
void ihash_pairs(ihash *_ih, ilist *_l)
{
  int i;
  ihashpair *ihp;

  for(i=0; i<_ih->size; i++){
    ILIST_FOR(_ih->list[i], ihp){
      ilist_push(_l, ihp->key);
      ilist_push(_l, ihp->val);
    }
  }
}

/*------------------------------------*/
/* list of keys */
/*------------------------------------*/
void ihash_keys(ihash *_ih, ilist *_keys)
{
  int i;
  ihashpair *ihp;

  for(i=0; i<_ih->size; i++)
    ILIST_FOR(_ih->list[i], ihp)
      ilist_push(_keys, ihp->key);
}

/*------------------------------------*/
/* list of vals */
/*------------------------------------*/
void ihash_vals(ihash *_ih, ilist *_vals)
{
  int i;
  ihashpair *ihp;

  for(i=0; i<_ih->size; i++)
    ILIST_FOR(_ih->list[i], ihp)
      ilist_push(_vals, ihp->val);
}
