#include <stdlib.h>
#include <stdio.h>

#include "vm.h"

static void
mark_roots(VM_t *v)
{
  int i;
  for (i = 0; i < v->pt; i++) {
    mark(v, v->stack[i]);
  }
}

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
sweep(VM_t *v)
{
  object_t *this, *prev;

  prev = v->current;
  this = v->current;

  while (this) {
    if (MARKED(this)) {
      UNMARK(this);

      /* we bump this only when we keep something */
      prev = this;
      this = this->next;
    }
    else {
      /* save this */
      tmp = this; 
      /* set the previous's next to the next thing we're looking at. */
      prev->next = this->next;
      /* move forward */
      this = this->next;
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

  mark(v, v->current);
  sweep(v);

  collected = count - v->count;

  /* TODO: ellapsed time should be captured for each mark
     and sweep phase */
}
