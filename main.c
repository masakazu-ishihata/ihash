#include <stdio.h>
#include <stdlib.h>
#include "ihash.h"

int main(void)
{
  ihash *ih;
  char key[256];
  int i, *val;

  ih = ihash_new(10);

  /* add key & val */
  for(i=0; i<10; i++){
    sprintf(key, "key%d", i);
    val  = int_new(i);

    printf("add %s as %d\n", key, *val);
    ihash_set(ih, key, val);
  }

  /* delete key */
  printf("delete key0\n");
  val = ihash_delete(ih, "key0");
  free(val);

  /* show */
  ihash_show(stdout, ih);

  /* realloc */
  ihash_realloc(ih, 20);
  ihash_show(stdout, ih);

  /* free */
  ihash_free_func(ih, free);

  return 0;
};
