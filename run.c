#include <stdio.h>

#include "vm.h"

int
main(int argc, char **argv)
{
  int i;
  VM_t vm, *v;
  v = &vm;

  vm_init(v);

  vm_null(v);
  vm_num(v, 1);
  vm_num(v, 2);

  printf("pt: %d\n", v->pt);
  
  vm_add(v);

  printf("pt: %d\n", v->pt);

  vm_cons(v);
  vm_print(v);
  vm_nl(v);

  for (i = 0; i < 20; i++) {
    vm_num(v, i);
  }

  return 0;
}
