#include <stdio.h>
#include <stdlib.h>
#include "ihash.h"

int main(void)
{
  ilist *il;
  ihash *ih;
  ihashpair *ihp;
  char key[256];
  int i, *val;

  ih = ihash_new(100);

  /* add key & val */
  for(i=0; i<10; i++){
    sprintf(key, "key%d", i);
    val = (int *)malloc(sizeof(int));
    val[0] = i;

    printf("add %s as %d\n", key, *val);

    ihash_set(ih, key, val);
  }

  /* delete key */
  printf("delete key0\n");
  val = ihash_delete(ih, "key0");
  free(val);

  /* pairs */
  il = ihash_get_pairs(ih);
  ilist_sort(il, (void *)ihashpair_comp_key);
  for(ihp=ilist_head(il); ihp!=NULL; ihp=ilist_succ(il)){
    printf("h[%s] = %d\n", ihp->key, *((int *)ihp->val));
  }

  /* free */
  ihash_free_func(ih, free);

  return 0;
};
