#ifndef VM_H_
#define VM_H_

#define STACK_SIZE 10000

typedef struct VM VM_t;
typedef struct object object_t;

typedef enum {
  NUM_T,
  CONS_T
} type_t;

struct VM {
  object_t *stack[STACK_SIZE];
  object_t *current, *first;
  int pt;

  int count;
  int threshold;
};

#define MKFLAGS(t, m) ((t << 1) | m)
#define TYPE(o) (o->flags >> 1)
#define MARK(o) o->flags ^= 1
#define UNMARK(o) o->flags &= ~1
#define MARKED(o) ((o->flags) & 1)

struct object {
  object_t *hist; /* previously allocated thing */
  union {
    double number;
    struct {
      object_t *head;
      object_t *tail;
    };
  };
  int flags;
};

void vm_init(VM_t *v);
void vm_push(VM_t *v, object_t *o);
object_t *vm_pop(VM_t *v);
object_t *vm_cons(VM_t *v);
object_t *vm_num(VM_t *v, double num);
object_t *vm_add(VM_t *v);
object_t *vm_mult(VM_t *v);
object_t *vm_div(VM_t *v);
object_t *vm_mod(VM_t *v);
object_t *vm_head(VM_t *v);
object_t *vm_tail(VM_t *v);
void vm_null(VM_t *v);
void vm_dup(VM_t *v);
void vm_nl(VM_t *v);
void vm_print(VM_t *v);

void vm_gc(VM_t *v);

long nanotime();

#endif
