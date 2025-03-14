#include <stdio.h>
#include <stdlib.h>
#define MAX_STACK_SIZE 100
#define MAX_QUEUE_SIZE 100

struct node {
  char data;
  struct node *left_child, *right_child;
};
typedef struct node *tree_pointer;

tree_pointer stack[MAX_STACK_SIZE];
tree_pointer queue[MAX_QUEUE_SIZE];

tree_pointer create_tree_node(char data);
void recursive_inorder(tree_pointer ptr);
void iter_inorder(tree_pointer ptr);
void iter_preorder(tree_pointer ptr);
void iter_postorder(tree_pointer ptr);
void level_order(tree_pointer ptr);

/* TODO: add function prototype if necessary */
void add(int*, tree_pointer);  //if stack full, output error and exit
tree_pointer delete(int*);  //if stack empty, return NULL
void addq(int, int*, tree_pointer);  //if queue full, output error and exit
tree_pointer deleteq(int*, int);  //if queue empty, return NULL

void main() {

  /* create a tree that represents an arithmetic expression */
  tree_pointer ptr, ptr1, ptr2;
 
  ptr1 = create_tree_node('A');
  ptr2 = create_tree_node('B');
  ptr = create_tree_node('/');
  ptr->left_child = ptr1;
  ptr->right_child = ptr2;

  ptr1 = ptr;
  ptr2 = create_tree_node('C');
  ptr = create_tree_node('*');
  ptr->left_child = ptr1;
  ptr->right_child = ptr2;

  ptr1 = ptr;
  ptr2 = create_tree_node('D');
  ptr = create_tree_node('*');
  ptr->left_child = ptr1;
  ptr->right_child = ptr2;

  ptr1 = ptr;
  ptr2 = create_tree_node('E');
  ptr = create_tree_node('+');
  ptr->left_child = ptr1;
  ptr->right_child = ptr2;


  /* call traversal functions */
  recursive_inorder(ptr);
  printf("\n");

  iter_inorder(ptr);
  printf("\n");
 
  iter_preorder(ptr);
  printf("\n");
  
  iter_postorder(ptr);
  printf("\n");
  
  level_order(ptr);
  printf("\n");

  free(ptr);
  free(ptr1);
  free(ptr2);

}

tree_pointer create_tree_node(char data) {
  tree_pointer ptr = (tree_pointer)malloc(sizeof(struct node));
  ptr->data = data;
  ptr->left_child = NULL;
  ptr->right_child = NULL;
  return ptr;
}

void recursive_inorder(tree_pointer ptr) {
  if(ptr) {
    recursive_inorder(ptr->left_child);
    printf("%c ", ptr->data);
    recursive_inorder(ptr->right_child);
  }
}

void iter_inorder(tree_pointer ptr) {
  int top = -1;  /* initialize stack */
  for(;;) {
    for(; ptr; ptr = ptr->left_child)
        add(&top, ptr);  /* add to stack */
      ptr = delete (&top);  /* delete from stack */
      if(!ptr) break;  /* empty stack */
      printf("%c ", ptr->data);
      ptr = ptr->right_child;
  }
}

void iter_preorder(tree_pointer ptr) {
  // VLR
    int top = -1;
    for (;;) {
        for (; ptr; ptr = ptr->left_child) {
            printf("%c ", ptr->data);
            add(&top, ptr);
        }
        ptr = delete (&top);
        if (!ptr) break;
        ptr = ptr->right_child;
    }
}

void iter_postorder(tree_pointer ptr) {
    //LRV
    int top = -1;
    for (;;) {
        for (; ptr; ptr = ptr->left_child) {
            add(&top, ptr);
        }
        ptr = delete(&top);
        if (!ptr) break;
        while (ptr == stack[top]->right_child) { //현재노드가 이미 right노드인 경우를 처리
            printf("%c ", ptr->data);
            ptr = delete(&top);
            if (top == -1) break;
        }
        printf("%c ", ptr->data);
        if (top == -1) break;
        ptr = stack[top]->right_child;
    }
}

void level_order(tree_pointer ptr) {
    int front = 0, rear = 0;
    if (!ptr) return; /* empty tree */
    addq(front, &rear, ptr);
    for (; ; ) {
        ptr = deleteq(&front, rear);
        if (ptr) {
            printf("%c ", ptr->data);
            if (ptr->left_child) addq(front, &rear, ptr->left_child);
            if (ptr->right_child) addq(front, &rear, ptr->right_child);
        }
        else break;
    }
}

// TODO: add function implementations here 
void add(int* top, tree_pointer ptr) { //if stack full, output error and exit
    if (*top == MAX_STACK_SIZE - 1) {
        fprintf(stderr, "error\n");
        exit(1);
    }
    stack[++(*top)] = ptr;
}
tree_pointer delete(int* top) {  //if stack empty, return NULL
    if (*top == -1) {
        return NULL;
    }
    return stack[(*top)--];
}
void addq(int top, int* rear, tree_pointer ptr) { //if queue full, output error and exit
    if (*rear == MAX_QUEUE_SIZE - 1) {//top이 첫 원소 직전 인덱스를 가리키고 rear가 맨 마지막 인덱스를 가리키도록
        fprintf(stderr, "error\n");
        exit(1);
    }
    (*rear)++;
    queue[*rear] = ptr;
}
tree_pointer deleteq(int* top, int rear) { //if queue empty, return NULL
    if (*top == rear) {
        return NULL;
    }
    return queue[++(*top)];
}