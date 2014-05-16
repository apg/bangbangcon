#include <stdio.h>

#include "vm.h"

int
main(int argc, char **argv)
{
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

  return 0;
}
