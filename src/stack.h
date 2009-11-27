#define MAX 100
typedef char* ItemType;

typedef struct {
    int top;                      /* topo = -1, se a pilha está vazia */ 
    ItemType items[MAX];
} Stack;

void stack_clean(Stack *s);
void stack_push(ItemType item, Stack *s);
ItemType stack_pop(Stack *s);