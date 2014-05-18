#include <getopt.h>
#include <stdio.h>

#include "vm.h"

int
main(int argc, char **argv)
{
  int i, j;
  long nstart;
  VM_t vm, *v;
  v = &vm;

  vm_init(v);

  nstart = nanotime();

  vm_null(v);
  for (i = 0; i < 1000; i++) {
    for (j = 0; j < 101; j++) {
      vm_num(v, (double) j);
    }

    for (j = 0; j < 100; j++) {
      vm_cons(v);
    }

    vm_pop(v);
  }
  
  vm_print(v);
  vm_nl(v);

  for (i = 0; i < 20; i++) {
    vm_num(v, i);
  }

  printf("timer#program.ellapsed=%ldns\n", nanotime()- nstart);
  return 0;
}
