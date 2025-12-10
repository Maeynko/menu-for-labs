// printtree.c
// Небольшой вариант "print tree" + определение struct Node с parent.
// Этот файл подключается в основной программе через #include "printtree.c"

#include <stdio.h>
#include <stdlib.h>

// Узел дерева — теперь с указателем на parent для подъёма вверх
struct Node {           /* узел дерева */
    int key;            // данные
    struct Node* left;  /* левый сын */
    struct Node* right; /* правый сын */
    struct Node* parent;/* родитель (можно NULL) */
};

//================================================================
// остальной код нужно вставить в свой проект
// Вызывать для отображения дерева надо так:
// printTree(root, NULL, 0);
//================================================================
#define true    1
#define false   0

struct Trunk
{
    struct Trunk* prev;
    char* str;
};

// Helper function to print branches of the binary tree
static void showTrunks(struct Trunk* p)
{
    if (p == NULL) {
        return;
    }

    showTrunks(p->prev);
    printf("%s", p->str);
}

static void printTree(struct Node* root, struct Trunk* prev, int isLeft)
{
    if (root == NULL) {
        return;
    }

    char* prev_str = "     ";

    struct Trunk* trunk = (struct Trunk*)malloc(sizeof(struct Trunk));
    trunk->prev = prev;
    trunk->str = prev_str;

    printTree(root->right, trunk, true);

    if (!prev) {
        trunk->str = "-----";
    }
    else if (isLeft)
    {
        trunk->str = ".----";
        prev_str = "    |";
    }
    else {
        trunk->str = "`----";
        prev->str = prev_str;
    }

    showTrunks(trunk);
    printf(" %d\n", root->key);

    if (prev) {
        prev->str = prev_str;
    }
    trunk->str = "    |";

    printTree(root->left, trunk, false);
    free(trunk);
}
