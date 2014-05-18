#include <stdlib.h>
#include <stdio.h>

#include "vm.h"

static void
mark(VM_t *v, object_t *o)
{
  if (o == NULL) return;
  if (MARKED(o)) return;

  MARK(o);

  if (TYPE(o) == CONS_T) {
    mark(v, o->head);
    mark(v, o->tail);
  }
}

static void
mark_roots(VM_t *v)
{
  int i;
  for (i = 0; i < v->pt; i++) {
    if (v->stack[i] == NULL) continue;
    mark(v, v->stack[i]);
  }
}

static void
sweep(VM_t *v)
{
  object_t *this, *prev, *tmp;

  prev = v->current;
  this = v->current;

  while (this) {
    if (MARKED(this)) {
      UNMARK(this);

      /* we bump this only when we keep something */
      prev = this;
      this = this->hist;
    }
    else {
      /* save this */
      tmp = this; 
      /* set the previous's next to the next thing we're looking at. */
      prev->hist = this->hist;
      /* move forward */
      this = this->hist;
      /* tmp is safe to free now */
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
  sweep(v);

  collected = count - v->count;

  printf("guage#naive.threshold=%d "
         "guage#naive.before=%d guage#naive.collected=%d "
         "gauge#naive.after=%d timer#naive.ellapsed=%ldns\n", 
         v->threshold, count, collected, v->count, nanotime() - nstart);
}
