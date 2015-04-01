#include "ihash.h"

/*----------------------------------------------------------------------------*/
/* ihashpair */
/*----------------------------------------------------------------------------*/

/*------------------------------------*/
/* new */
/*------------------------------------*/
ihashpair *ihashpair_new(const char *_key, void *_val)
{
  ihashpair *ihp;

  ihp = (ihashpair *)malloc(sizeof(ihashpair));
  ihp->key = strcln(_key);
  ihp->val = _val;

  return ihp;
}

/*------------------------------------*/
/* free */
/*------------------------------------*/
void ihashpair_free(void *_vihp)
{
  ihashpair *_ihp = (ihashpair *)_vihp;
  ihashpair_free_func(_ihp, NULL);
}

/*------------------------------------*/
/* free with inner free */
/*------------------------------------*/
void ihashpair_free_func(void *_vihp, void (*free_func)(void *))
{
  ihashpair *_ihp = (ihashpair *)_vihp;

  if(_ihp != NULL){
    free(_ihp->key);
    if(free_func != NULL)  free_func(_ihp->val);
    free(_ihp);
  }
}

/*----------------------------------------------------------------------------*/
/* compare */
/*----------------------------------------------------------------------------*/

/*------------------------------------*/
/* compare keys of ihashpair */
/*------------------------------------*/
int ihashpair_comp_key(void *_va, void *_vb)
{
  ihashpair *_a = (ihashpair *)_va;
  ihashpair *_b = (ihashpair *)_vb;
  char *p, *q;

  p = _a->key;
  q = _b->key;

  while(p!='\0' && q!='\0'){
    if(*p != *q) return *p < *q;
    p++;
    q++;
  }
  return strcmp(_a->key, _b->key);
}

/*------------------------------------*/
/* compare val of ihashpair (as int) */
/*------------------------------------*/
int ihashpair_comp_val_int(void *_va, void *_vb)
{
  ihashpair *_a = (ihashpair *)_va;
  ihashpair *_b = (ihashpair *)_vb;
  int a, b;

  a = *(int *)(_a->val);
  b = *(int *)(_b->val);
  if(a == b) return ihashpair_comp_key(_a, _b);
  else       return a < b;
}

/*------------------------------------*/
/* compare val of ihashpair (as double) */
/*------------------------------------*/
int ihashpair_comp_val_double(void *_va, void *_vb)
{
  ihashpair *_a = (ihashpair *)_va;
  ihashpair *_b = (ihashpair *)_vb;
  double a, b;

  a = *(double *)(_a->val);
  b = *(double *)(_b->val);
  if(a == b) return ihashpair_comp_key(_a, _b);
  else       return a < b;
}

/*------------------------------------*/
/* compare val of ihashpair (as string) */
/*------------------------------------*/
int ihashpair_comp_val_str(void *_va, void *_vb)
{
  ihashpair *_a = (ihashpair *)_va;
  ihashpair *_b = (ihashpair *)_vb;
  char *p, *q;
  p = _a->val;
  q = _b->val;

  while(p!='\0' && q!='\0'){
    if(*p != *q) return *p < *q;
    p++;
    q++;
  }
  return strcmp(_a->val, _b->val);
}


/*----------------------------------------------------------------------------*/
/* ihash */
/*----------------------------------------------------------------------------*/

/*------------------------------------*/
/* new */
/*------------------------------------*/
ihash *ihash_new(size_t _size)
{
  int i;
  ihash *ih;

  ih = (ihash *)malloc(sizeof(ihash));
  ih->size = _size;
  ih->item = 0;
  ih->list = (ilist **)malloc(ih->size * sizeof(ilist *));
  for(i=0; i<ih->size; ih->list[i]=ilist_new(), i++);

  return ih;
}

/*------------------------------------*/
/* free */
/*------------------------------------*/
void ihash_free(ihash *_ih)
{
  ihash_free_func(_ih, NULL);
}

/*------------------------------------*/
/* free with inner free */
/*------------------------------------*/
void ihash_free_func(ihash *_ih, void (*free_func)(void *))
{
  int i;
  ihashpair *ihp;

  if(_ih != NULL){
    for(i=0; i<_ih->size; i++){
      while( (ihp=ilist_shift(_ih->list[i])) != NULL){
        ihashpair_free_func(ihp, free_func);
      }
      ilist_free(_ih->list[i]);
    }
    free(_ih->list);
    free(_ih);
  }
}


/*----------------------------------------------------------------------------*/
/* accessor */
/*----------------------------------------------------------------------------*/

/*------------------------------------*/
/* get val of key */
/*------------------------------------*/
void *ihash_get(ihash *_ih, const char *_key)
{
  ilist *il;
  ihashpair *ihp;

  il = _ih->list[ ihash_hashval(_ih, _key) ];
  for(ihp=ilist_head(il); ihp!=NULL; ihp=ilist_succ(il)){
    if(strcmp(_key, ihp->key) == 0){
      return ihp->val;
    }
  }
  return NULL;
}

/*------------------------------------*/
/* set [key, val] */
/*------------------------------------*/
int ihash_set(ihash *_ih, const char *_key, void *_val)
{
  ilist *il;
  ihashpair *ihp;

  /* already member */
  if(ihash_get(_ih, _key) != NULL) return -1;

  /* new member */
  il = _ih->list[ ihash_hashval(_ih, _key) ];
  ihp = ihashpair_new(_key, _val);
  ilist_push(il, ihp);

  return ++_ih->item;
}

/*------------------------------------*/
/* delete [key, val] */
/*------------------------------------*/
void *ihash_delete(ihash *_ih, const char *_key)
{
  ilist *il;
  ihashpair *ihp;
  void *val;

  il = _ih->list[ ihash_hashval(_ih, _key) ];
  for(ihp=ilist_head(il); ihp!=NULL; ihp=ilist_succ(il)){
    if(strcmp(_key, ihp->key) == 0){
      ilist_remove(il);
      val = ihp->val;
      ihashpair_free(ihp);
      return val;
    }
  }
  return NULL;
}

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
/* hash value */
/*------------------------------------*/
size_t ihash_hashval(ihash *_ih, const char *_key)
{
  int i;
  size_t hv = 0;

  for(i=0; i<strlen(_key); i++){
    hv = (i+1) * (hv+1) * (( hv + _ih->size + _key[i]) % _key[i]);
    hv %= _ih->size;
  }

  return hv;
}

/*------------------------------------*/
/* list of pairs */
/*------------------------------------*/
void ihash_get_pairs(ihash *_ih, ilist *_l)
{
  int i;
  ilist *il;
  ihashpair *ihp;

  for(i=0; i<_ih->size; i++){
    il = _ih->list[i];
    for(ihp=ilist_head(il); ihp!=NULL; ihp=ilist_succ(il))
      ilist_push(_l, ihp);
  }
}

/*------------------------------------*/
/* list of keys */
/*------------------------------------*/
void ihash_get_keys(ihash *_ih, ilist *_l)
{
  int i;
  ilist *il;
  ihashpair *ihp;

  for(i=0; i<_ih->size; i++){
    il = _ih->list[i];
    for(ihp=ilist_head(il); ihp!=NULL; ihp=ilist_succ(il))
      ilist_push(_l, ihp->key);
  }
}

/*------------------------------------*/
/* list of vals */
/*------------------------------------*/
void ihash_get_vals(ihash *_ih, ilist *_l)
{
  int i;
  ilist *il;
  ihashpair *ihp;

  for(i=0; i<_ih->size; i++){
    il = _ih->list[i];
    for(ihp=ilist_head(il); ihp!=NULL; ihp=ilist_succ(il))
      ilist_push(_l, ihp->val);
  }
}

/*------------------------------------*/
/* show */
/*------------------------------------*/
void ihash_show(FILE *_fp, ihash *_ih)
{
  ihashpair *ihp;
  ilist *l = ilist_new();

  fprintf(_fp, "HASH SIZE = %d\n", (int)_ih->size);
  ihash_get_pairs(_ih, l);
  while((ihp = ilist_shift(l)) != NULL){
    fprintf(_fp, "h[%s] = %d\n", ihp->key, *((int *)ihp->val));
  }
  ilist_free(l);
}
