#include <stdlib.h>
#include <stdio.h>

#include "vm.h"

static void
mark_roots(VM_t *v)
{
  int i;
  for (i = 0; i < v->pt; i++) {
    if (v->stack[i] == NULL) continue;
    MARK(v->stack[i]);
  }
}

static void
pass(VM_t *v)
{
  object_t *prev, *tmp, *this;

  prev = v->current;
  this = prev->hist;

  while (this != v->first) {
    if (MARKED(this)) {
      if (TYPE(this) == CONS_T) {
        MARK(this->head);
        MARK(this->tail);
      }

      UNMARK(this);
      prev = this;
      this = prev->hist;
    }
    else {
      tmp = this;
      this = this->hist;
      prev->hist = this;
      free(tmp);
      v->count--;
    }
  }
}

void
vm_gc(VM_t *v)
{
  int count = v->count;
  int collected;

  long nstart = nanotime();

  mark_roots(v);
  pass(v);

  collected = count - v->count;

  printf("guage#onepass.threshold=%d "
         "guage#onepass.before=%d onepass#naive.collected=%d "
         "gauge#onepass.after=%d onepass#naive.ellapsed=%ldns\n", 
         v->threshold, count, collected, v->count, nanotime() - nstart);
}



