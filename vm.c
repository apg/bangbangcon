#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "vm.h"

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
vm_init(VM_t *vm)
{
  vm->current = NULL;
  vm->pt = 0;
  vm->count = 0;
  vm->threshold = 8;
}

void 
vm_push(VM_t *vm, object_t *o)
{
  dieif(vm->pt >= STACK_SIZE, "push: stack overflow!\n");
  vm->stack[vm->pt++] = o;
}

object_t *
vm_pop(VM_t *vm)
{
  dieif(vm->pt <= 0, "pop: stack underflow!\n");
  return vm->stack[--vm->pt];
}

void 
vm_null(VM_t *vm)
{
  dieif(vm->pt >= STACK_SIZE, "null: stack overflow!\n");
  vm->stack[vm->pt++] = NULL;
}

object_t *
vm_cons(VM_t *vm) 
{
  object_t *new;
  dieif(vm->pt < 2, "cons: not enough rands\n");
  new = malloc(sizeof(*new));
  new->hist = vm->current;
  new->flags = MKFLAGS(CONS_T, 0);
  new->head = vm_pop(vm);
  new->tail = vm_pop(vm);

  vm->count++;
  vm->current = new;
  vm_push(vm, new);

  return new;
}


object_t *
vm_num(VM_t *vm, double num)
{
  object_t *new;

  new = malloc(sizeof(*new));
  new->hist = vm->current;
  new->flags = MKFLAGS(NUM_T, 0);
  new->number = num;

  vm->count++;
  vm->current = new;
  vm_push(vm, new);
  return new;
}

object_t *
vm_add(VM_t *vm)
{
  object_t *a, *b;

  dieif(vm->pt < 2, "add: not enough rands\n");
  a = vm_pop(vm);
  dieif(TYPE(a) != NUM_T, "add: rand 1 not a number\n");
  b = vm_pop(vm);
  dieif(TYPE(b) != NUM_T, "add: rand 2 not a number\n");

  return vm_num(vm, a->number + b->number);
}

object_t *
vm_sub(VM_t *vm)
{
  object_t *a, *b;

  dieif(vm->pt < 2, "sub: not enough rands\n");
  a = vm_pop(vm);
  dieif(TYPE(a) != NUM_T, "sub: rand 1 not a number\n");
  b = vm_pop(vm);
  dieif(TYPE(b) != NUM_T, "sub: rand 2 not a number\n");

  return vm_num(vm, a->number - b->number);
}

object_t *
vm_mult(VM_t *vm)
{
  object_t *a, *b;

  dieif(vm->pt < 2, "mult: not enough rands\n");
  a = vm_pop(vm);
  dieif(TYPE(a) != NUM_T, "mult: rand 1 not a number\n");
  b = vm_pop(vm);
  dieif(TYPE(b) != NUM_T, "mult: rand 2 not a number\n");

  return vm_num(vm, a->number * b->number);
}

object_t *
vm_div(VM_t *vm)
{
  object_t *a, *b;

  dieif(vm->pt < 2, "div: not enough rands\n");
  a = vm_pop(vm);
  dieif(TYPE(a) != NUM_T, "div: rand 1 not a number\n");
  b = vm_pop(vm);
  dieif(TYPE(b) != NUM_T, "div: rand 2 not a number\n");
  dieif(b->number == 0.0, "div: division by zero\n");

  return vm_num(vm, a->number / b->number);
}

object_t *
vm_mod(VM_t *vm)
{
  object_t *a, *b;

  dieif(vm->pt < 2, "mod: not enough rands\n");
  a = vm_pop(vm);
  dieif(TYPE(a) != NUM_T, "mod: rand 1 not a number\n");
  b = vm_pop(vm);
  dieif(TYPE(b) != NUM_T, "mod: rand 2 not a number\n");
  dieif(b->number == 0.0, "mod: division by zero\n");

  return vm_num(vm, (int)a->number % (int)b->number);
}

object_t *
vm_head(VM_t *vm)
{
  object_t *p = vm_pop(vm);
  dieif(TYPE(p) != CONS_T, "head: not a cons\n");
  vm_push(vm, p->head);
  return p->head;
}

object_t *
vm_tail(VM_t *vm)
{
  object_t *p = vm_pop(vm);
  dieif(TYPE(p) != CONS_T, "tail: not a cons\n");
  vm_push(vm, p->tail);
  return p->tail;
}

void
vm_dup(VM_t *vm)
{
  dieif(vm->pt < 1, "dup: nothing to dup\n");
  vm_push(vm, vm->stack[vm->pt - 1]);
}

void
vm_nl(VM_t *vm)
{
  putc('\n', stdout);
}

static void
print_object(object_t *o)
{
  if (o == NULL) {
    putc('(', stdout);
    putc(')', stdout);
    return;
  }

  switch (TYPE(o)) {
  case NUM_T:
    printf("%lf", o->number);
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
vm_print(VM_t *vm)
{
  print_object(vm_pop(vm));
}
