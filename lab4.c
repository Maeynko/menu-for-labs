#include <stdio.h>
#include <stdlib.h>

// Подключаем файл печати дерева (содержит struct Node)
#include "printtree.c"

/* ============================================================
   АВЛ-ВСТАВКА
   ============================================================ */

// Получение высоты поддерева
int height(struct Node* n) {
    if (!n) return 0;
    int hl = height(n->left);
    int hr = height(n->right);
    return (hl > hr ? hl : hr) + 1;
}

// Баланс-фактор
int balance(struct Node* n) {
    return height(n->left) - height(n->right);
}

// Повороты AVL
static struct Node* rotateRight(struct Node* y) {
    struct Node* x = y->left;
    y->left = x->right;
    x->right = y;
    return x;
}

static struct Node* rotateLeft(struct Node* x) {
    struct Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// Создать узел
static struct Node* newNode(int key) {
    struct Node* n = malloc(sizeof(struct Node));
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

// Вставка в AVL-дерево
static struct Node* insertAVL(struct Node* root, int key) {
    if (!root) return newNode(key);

    if (key < root->key)
        root->left = insertAVL(root->left, key);
    else
        root->right = insertAVL(root->right, key);

    int b = balance(root);

    // LL
    if (b > 1 && key < root->left->key)
        return rotateRight(root);
    // RR
    if (b < -1 && key >= root->right->key)
        return rotateLeft(root);
    // LR
    if (b > 1 && key >= root->left->key) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    // RL
    if (b < -1 && key < root->right->key) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

/* ============================================================
   ПОИСК ЛИСТЬЕВ
   ============================================================ */

struct Node** leaves = NULL;
int leaves_count = 0;
int leaves_cap = 0;

void addLeaf(struct Node* n) {
    if (leaves_count == leaves_cap) {
        leaves_cap = leaves_cap ? leaves_cap * 2 : 8;
        leaves = realloc(leaves, leaves_cap * sizeof(struct Node*));
    }
    leaves[leaves_count++] = n;
}

void collectLeaves(struct Node* root) {
    if (!root) return;
    if (!root->left && !root->right) {
        addLeaf(root);
        return;
    }
    collectLeaves(root->left);
    collectLeaves(root->right);
}

/* ============================================================
   ПОИСК ГЛУБИНЫ, LCA и ПОСТРОЕНИЕ ПУТИ
   ============================================================ */

// Глубина узла (по ключу)
int depth(struct Node* root, int key) {
    int d = 0;
    while (root) {
        if (key == root->key) return d;
        root = (key < root->key) ? root->left : root->right;
        d++;
    }
    return -1;
}

// НОД/Lowest Common Ancestor (для BST)
struct Node* LCA(struct Node* root, int a, int b) {
    if (!root) return NULL;

    if (a < root->key && b < root->key)
        return LCA(root->left, a, b);

    if (a > root->key && b > root->key)
        return LCA(root->right, a, b);

    return root;
}

// Строит путь root → key (по ключам)
int buildPath(struct Node* root, int key, int** out) {
    *out = malloc(32 * sizeof(int));
    int size = 0, cap = 32;

    while (root) {
        if (size == cap) {
            cap *= 2;
            *out = realloc(*out, cap * sizeof(int));
        }
        (*out)[size++] = root->key;

        if (key == root->key) return size;
        root = (key < root->key) ? root->left : root->right;
    }

    free(*out);
    *out = NULL;
    return -1;
}

// Построить путь между двумя листьями
int buildLeafPath(struct Node* root, int a, int b,
                  int** out_path, long* out_sum)
{
    int *pa, *pb;
    int la = buildPath(root, a, &pa);
    int lb = buildPath(root, b, &pb);

    if (la < 0 || lb < 0) return -1;

    int i = 0, last = -1, min = (la < lb ? la : lb);

    for (i = 0; i < min; i++) {
        if (pa[i] == pb[i]) last = i;
        else break;
    }

    int len = (la - last - 1) + 1 + (lb - last - 1);
    int* p

=

malloc(len * sizeof(int));
    int pos = 0;

    // вверх от a до LCA
    for (i = la - 1; i > last; i--)
        p[pos++] = pa[i];

    // сам LCA
    p[pos++] = pa[last];

    // вниз к b
    for (i = last + 1; i < lb; i++)
        p[pos++] = pb[i];

    long sum = 0;
    for (i = 0; i < len; i++) sum += p[i];

    *out_path = p;
    *out_sum = sum;

    free(pa);
    free(pb);

    return len;
}

/* ============================================================
   ПОИСК МИНИМАЛЬНОГО И МАКСИМАЛЬНОГО ПУТИ
   ============================================================ */

void findPaths(struct Node* root) {
    collectLeaves(root);

    if (leaves_count < 2) {
        printf("Недостаточно листьев.\n");
        return;
    }

    int min_edges = 1e9, max_edges = -1;
    int* min_path = NULL, *max_path = NULL;
    int min_len = 0, max_len = 0;
    long min_sum = 0, max_sum = 0;

    for (int i = 0; i < leaves_count; i++) {
        for (int j = i + 1; j < leaves_count; j++) {

            int a = leaves[i]->key;
            int b = leaves[j]->key;

            struct Node* lca = LCA(root, a, b);
            int da = depth(root, a);
            int db = depth(root, b);
            int dl = depth(root, lca->key);

            int edges = da + db - 2 * dl;

            int* path = NULL;
            long sum = 0;
            int len = buildLeafPath(root, a, b, &path, &sum);

            // минимальный
            if (edges < min_edges) {
                if (min_path) free(min_path);
                min_edges = edges;
                min_sum = sum;
                min_len = len;
                min_path = path;
                path = NULL;
            } 

            // максимальный
            if (edges > max_edges) {
                if (max_path) free(max_path);
                max_edges = edges;
                max_sum = sum;
                max_len = len;
                max_path = path ? path : NULL; 
                path = NULL;
            }
            if (path)
                free(path);
        }
    }

    printf("\nМинимальный путь (рёбра = %d, сумма = %ld):\n", min_edges, min_sum);
    for (int i = 0; i < min_len; i++)
        printf("%d ", min_path[i]);
    printf("\n");

    printf("\nМаксимальный путь (рёбра = %d, сумма = %ld):\n", max_edges, max_sum);
    for (int i = 0; i < max_len; i++)
        printf("%d ", max_path[i]);
    printf("\n");

    free(min_path);
    free(max_path);
}

/* ============================================================
   ОЧИСТКА ДЕРЕВА
   ============================================================ */

static void freeTree(struct Node* r) {
    if (!r) return;
    freeTree(r->left);
    freeTree(r->right);
    free(r);
}

/* ============================================================
   MAIN
   ============================================================ */

int lab4(char* filename) {
    /*if (argc < 2) {
        printf("Использование: %s файл_входа\n", argv[0]);
        return 1;
    }*/

    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Не удалось открыть файл.\n");
        return 1;
    }

    struct Node* root = NULL;
    int x;
    while (fscanf(f, "%d", &x) == 1)
        root = insertAVL(root, x);

    fclose(f);

    printf("Структура дерева:\n");
    printTree(root, NULL, 0);

    findPaths(root);
    freeTree(root);

    free(leaves);

    return 0;
}
