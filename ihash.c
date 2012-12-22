#include "ihash.h"

/*----------------------------------------------------------------------------*/
/* ihashpair */
/*----------------------------------------------------------------------------*/
ihashpair *ihashpair_new(const char *_key, void *_val)
{
  ihashpair *ihp;

  ihp = (ihashpair *)imalloc(sizeof(ihashpair), "ihashpair_new");
  ihp->key = istrcln(_key);
  ihp->val = _val;

  return ihp;
}
void ihashpair_free(ihashpair *_ihp)
{
  ihashpair_free_func(_ihp, NULL);
}
void ihashpair_free_func(ihashpair *_ihp, void (*free_func)(void *))
{
  if(_ihp != NULL){
    free(_ihp->key);
    if(free_func != NULL)  free_func(_ihp->val);
    free(_ihp);
  }
}

/*----------------------------------------------------------------------------*/
/* ihash */
/*----------------------------------------------------------------------------*/
/* new */
ihash *ihash_new(size_t _size)
{
  int i;
  ihash *ih;

  ih = (ihash *)imalloc(sizeof(ihash), "ihash_new");
  ih->size = _size;
  ih->item = 0;
  ih->list = (ilist **)imalloc(ih->size * sizeof(ilist *), "ihash_new");
  for(i=0; i<ih->size; ih->list[i]=ilist_new(), i++);

  return ih;
}

/* free */
void ihash_free(ihash *_ih)
{
  ihash_free_func(_ih, NULL);
}

/* free with inside free */
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

/* hashvalue */
size_t ihash_hashval(ihash *_ih, const char *_key)
{
  int i;
  size_t hv = 0;

  for(i=0; i<strlen(_key); i++){
    hv = ((i+1) * (hv+1) * _key[i]) % _ih->size;
  }

  return hv;
}
/* get val of key */
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
/* set [key, val] */
int ihash_set(ihash *_ih, const char *_key, void *_val)
{
  ilist *il;

  /* already member */
  if(ihash_get(_ih, _key) != NULL) return -1;

  /* new member */
  il = _ih->list[ ihash_hashval(_ih, _key) ];
  ilist_push(il, (void *)ihashpair_new(_key, _val));
  _ih->item++;

  return 0;
}
/* show hash keys */
void ihash_show(ihash *_ih, FILE *fp)
{
  int i, j;
  ilist *il;
  ihashpair *ihp;

  fprintf(fp, "HASH SIZE = %d\n", _ih->size);
  for(i=0; i<_ih->size; i++){
    il = _ih->list[i];
    for(j=0, ihp=ilist_head(il); ihp!=NULL; j++, ihp=ilist_succ(il)){
      fprintf(fp, "[%3d,%3d] %s\n", i+1, j+1, ihp->key);
    }
  }
}
