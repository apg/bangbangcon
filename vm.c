#include <stdlib.h>
#include <stdio.h>

static void 
dieif(int condition, const char *format, ...)
{
  int boom = 0;
  va_list args;
  va_start(args, format);
  if (condition) {
    vfprintf(stderr, format, args);
    boom = 1;
  }
  va_end(args);
  if (boom) {
    exit(1);
  }
}

void
vm_init(VM_t *v)
{
  v->current = NULL;
  v->pt = 0;
  v->count = 0;
  v->threshold = 8;
}

void 
vm_push(VM_t *v, object_t *o)
{
  dieif(v->pt >= STACK_SIZE, "push: stack overflow!\n");
  v->stack[v->pt++] = o;
}

object_t *
vm_pop(VM_t *v)
{
  dieif(v->pt <= STACK_SIZE, "pop: stack overflow!\n");
}

void 
vm_null(VM_t *v)
{
  dieif(v->pt >= STACK_SIZE, "null: stack overflow!\n");
  v->stack[v->pt++] = o;
}

object_t *
vm_cons(VM_t *v) 
{
  object_t *new;
  dieif(v->pt < 2, "cons: not enough rands\n");
  new = malloc(sizeof(*new));
  new->hist = vm->current;
  new->flags = MKFLAGS(CONS_T, 0);
  new->head = vm_pop(v);
  new->tail = vm_pop(v);

  vm->count++;
  vm->current = new;
  vm_push(v, new);
}


object_t *
vm_num(VM_t *v, double num)
{
  object_t *new;

  new = malloc(sizeof(*new));
  new->hist = vm->current;
  new->flags = MKFLAGS(NUM_T, 0);
  new->number = num;

  vm->count++;
  vm->current = new;
  vm_push(v, new);
  return new;
}

object_t *
vm_add(VM_t *v)
{
  object_t *a, *b;

  dieif(v->pt < 2, "add: not enough rands\n");
  a = vm_pop(v);
  dieif(TYPE(a) != NUM_T, "add: rand 1 not a number\n");
  b = vm_pop(v);
  dieif(TYPE(b) != NUM_T, "add: rand 2 not a number\n");

  return vm_num(v, a + b);
}

object_t *
vm_mult(VM_t *v)
{
  object_t *a, *b;

  dieif(v->pt < 2, "mult: not enough rands\n");
  a = vm_pop(v);
  dieif(TYPE(a) != NUM_T, "mult: rand 1 not a number\n");
  b = vm_pop(v);
  dieif(TYPE(b) != NUM_T, "mult: rand 2 not a number\n");

  return vm_num(v, a * b);
}

object_t *
vm_div(VM_t *v)
{
  object_t *a, *b;

  dieif(v->pt < 2, "div: not enough rands\n");
  a = vm_pop(v);
  dieif(TYPE(a) != NUM_T, "div: rand 1 not a number\n");
  b = vm_pop(v);
  dieif(TYPE(b) != NUM_T, "div: rand 2 not a number\n");
  dieif(b->number == 0.0, "div: division by zero\n");

  return vm_num(v, a / b);
}

object_t *
vm_mod(VM_t *v)
{
  object_t *a, *b;

  dieif(v->pt < 2, "mod: not enough rands\n");
  a = vm_pop(v);
  dieif(TYPE(a) != NUM_T, "mod: rand 1 not a number\n");
  b = vm_pop(v);
  dieif(TYPE(b) != NUM_T, "mod: rand 2 not a number\n");
  dieif(b->number == 0.0, "mod: division by zero\n");

  return vm_num(v, (int)a % (int)b);
}

object_t *
vm_head(VM_t *v)
{
  object_t *p = vm_pop(v);
  dieif(TYPE(p) != CONS_T, "head: not a cons\n");
  vm_push(v, p->head);
  return p->head;
}

object_t *
vm_tail(VM_t *v)
{
  object_t *p = vm_pop(v);
  dieif(TYPE(p) != CONS_T, "tail: not a cons\n");
  vm_push(v, p->tail);
  return p->tail;
}

void
vm_dup(VM_t *v)
{
  object_t *p;
  dieif(v->pt < 1, "dup: nothing to dup\n");
  vm_push(v, v->stack[v->pt - 1]);
}

void
vm_nl(VM_t *v)
{
  putc('\n', stdout);
}

static void
print_object(object_t *o)
{
  if (o == NULL) {
    putc('('), stdout);
    putc(')'), stdout);
  }
  switch (TYPE(o)) {
  case NUM_T:
    printf("%lf", o->value);
    break;

  case CONS_T:
    putc('(', stdout);
    print_object(o->head);
    putc(' ', stdout);
    print_object(o->tail);
    putc(')', stdout);
    break;
  }
}

void
vm_print(VM_t *v)
{
  print_object(vm_pop(v));
}
