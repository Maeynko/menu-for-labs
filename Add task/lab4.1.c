#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "printtree.c"  

// -------------------- AVL --------------------

int height_rec(struct Node* n) {
    if (!n) return 0;
    int hl = height_rec(n->left), hr = height_rec(n->right);
    return (hl > hr ? hl : hr) + 1;
}
int balanceFactor(struct Node* n) {
    if (!n) return 0;
    return height_rec(n->left) - height_rec(n->right);
}

struct Node* rotateRight(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T = x->right;
    x->right = y;
    y->left = T;
    return x;
}

struct Node* rotateLeft(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T = y->left;
    y->left = x;
    x->right = T;
    return y;
}

struct Node* newNode(int key) {
    struct Node* n = malloc(sizeof(struct Node));
    n->key = key; n->left = n->right = n->parent = NULL;
    return n;
}

struct Node* insertAVL(struct Node* r, int key) {
    if (!r) return newNode(key);
    if (key < r->key) r->left = insertAVL(r->left, key);
    else r->right = insertAVL(r->right, key);

    int b = balanceFactor(r);

    if (b > 1 && key < r->left->key) return rotateRight(r);
    if (b < -1 && key >= r->right->key) return rotateLeft(r);
    if (b > 1 && key >= r->left->key) {
        r->left = rotateLeft(r->left); return rotateRight(r);
    }
    if (b < -1 && key < r->right->key) {
        r->right = rotateRight(r->right); return rotateLeft(r);
    }
    return r;
}

void set_parents(struct Node* n, struct Node* p) {
    if (!n) return;
    n->parent = p;
    set_parents(n->left, n);
    set_parents(n->right, n);
}

void freeTree(struct Node* r) {
    if (!r) return;
    freeTree(r->left);
    freeTree(r->right);
    free(r);
}

// -------------------- пути и листья --------------------

int* collect_leaves_keys(struct Node* r, int *outn) {
    int cap = 16, cnt = 0;
    int *a = malloc(cap * sizeof(int));

    void dfs(struct Node* n) {
        if (!n) return;
        if (!n->left && !n->right) {
            if (cnt == cap) { cap*=2; a = realloc(a, cap*sizeof(int)); }
            a[cnt++] = n->key; return;
        }
        dfs(n->left); dfs(n->right);
    }
    dfs(r);
    *outn = cnt; return a;
}

// путь root→key
int buildPathToKey(struct Node* root, int key, int **out) {
    int cap = 32, len = 0;
    int *p = malloc(cap*sizeof(int));
    struct Node *cur = root;

    while (cur) {
        if (len == cap) { cap*=2; p = realloc(p, cap*sizeof(int)); }
        p[len++] = cur->key;
        if (cur->key == key) { *out = p; return len; }
        cur = (key < cur->key ? cur->left : cur->right);
    }
    free(p); *out = NULL; return -1;
}

// путь между ключами a и b
int buildPathBetweenKeys(struct Node* root, int a, int b, int **out) {
    int *pa=NULL, *pb=NULL;
    int la = buildPathToKey(root, a, &pa);
    int lb = buildPathToKey(root, b, &pb);
    if (la<0||lb<0) { free(pa); free(pb); *out=NULL; return -1; }

    int min = la<lb?la:lb, last = -1;
    for (int i=0;i<min;i++){ if(pa[i]==pb[i])last=i; else break; }

    int len = (la-last-1) + 1 + (lb-last-1);
    int *path = malloc(len*sizeof(int)); int pos=0;

    for (int i=la-1;i>last;i--) path[pos++] = pa[i];
    path[pos++] = pa[last];
    for (int i=last+1;i<lb;i++) path[pos++] = pb[i];

    free(pa); free(pb);
    *out = path; return len;
}

// Поисковик узла по ключу
struct Node* findNode(struct Node* r, int key) {
    while (r) {
        if (r->key == key) return r;
        r = (key < r->key) ? r->left : r->right;
    }
    return NULL;
}

int rnd(int a,int b){return a+rand()%(b-a+1);}

int* gen_unique(int N,int lo,int hi) {
    int R = hi-lo+1;
    if (N>R) N=R;

    int *pool = malloc(R*sizeof(int));
    for(int i=0;i<R;i++) pool[i]=lo+i;

    for(int i=R-1;i>0;i--){
        int j=rand()%(i+1);
        int t=pool[i]; pool[i]=pool[j]; pool[j]=t;
    }
    int *res = malloc(N*sizeof(int));
    for(int i=0;i<N;i++)res[i]=pool[i];
    free(pool);
    return res;
}

int in_array(int *a,int n,int k){
    for(int i=0;i<n;i++)if(a[i]==k)return 1;
    return 0;
}

int* intersect_ints(int *a,int na,int *b,int nb,int *outn){
    int cap=8,cnt=0;
    int *res=malloc(cap*sizeof(int));
    for(int i=0;i<na;i++){
        if(in_array(b,nb,a[i])){
            if(cnt==cap){cap*=2;res=realloc(res,cap*sizeof(int));}
            res[cnt++]=a[i];
        }
    }
    *outn=cnt; return res;
}

// -------------------- генерация двух деревьев --------------------

void generate_two_trees(struct Node **A, struct Node **B, int **portals, int *nport) {
    int tries=0;
    while(tries++<50){
        int szA=10+rnd(0,4), szB=10+rnd(0,4);
        int common=rnd(1,4);

        int *commonKeys = gen_unique(common,50,400);
        int *extraA = gen_unique(szA-common,401,700);
        int *extraB = gen_unique(szB-common,701,1000);

        int *arrA = malloc(szA*sizeof(int));
        int *arrB = malloc(szB*sizeof(int));

        for(int i=0;i<common;i++) arrA[i]=commonKeys[i];
        for(int i=0;i<common;i++) arrB[i]=commonKeys[i];
        for(int i=0;i<szA-common;i++) arrA[common+i]=extraA[i];
        for(int i=0;i<szB-common;i++) arrB[common+i]=extraB[i];

        for(int i=szA-1;i>0;i--){int j=rand()%(i+1); int t=arrA[i];arrA[i]=arrA[j];arrA[j]=t;}
        for(int i=szB-1;i>0;i--){int j=rand()%(i+1); int t=arrB[i];arrB[i]=arrB[j];arrB[j]=t;}

        struct Node *rA=NULL,*rB=NULL;
        for(int i=0;i<szA;i++) rA=insertAVL(rA,arrA[i]);
        for(int i=0;i<szB;i++) rB=insertAVL(rB,arrB[i]);

        set_parents(rA,NULL);
        set_parents(rB,NULL);

        int na,nb;
        int *la = collect_leaves_keys(rA,&na);
        int *lb = collect_leaves_keys(rB,&nb);

        int pc;
        int *ports = intersect_ints(la,na,lb,nb,&pc);

        free(arrA); free(arrB);
        free(extraA); free(extraB);
        free(commonKeys);
        free(la); free(lb);

        if (pc>=1) {
            *A=rA; *B=rB; *portals=ports; *nport=pc;
            return;
        }
        free(ports);
        freeTree(rA); freeTree(rB);
    }
    printf("Не удалось создать два дерева с общими листьями\n");
    exit(1);
}

// -------------------- движение --------------------

void step_random(struct Node **p){
    struct Node* c=*p;
    int ops[3],cnt=0;
    if(c->parent) ops[cnt++]=0;
    if(c->left)   ops[cnt++]=1;
    if(c->right)  ops[cnt++]=2;
    if(cnt==0)return;
    int ch=ops[rnd(0,cnt-1)];
    if(ch==0)*p=c->parent;
    else if(ch==1)*p=c->left;
    else *p=c->right;
}

void step_chase(struct Node **pos, struct Node* root, int targetKey) {
    if((*pos)->key==targetKey) return;
    int *path=NULL;
    int len=buildPathBetweenKeys(root, (*pos)->key, targetKey, &path);
    if(len<=1){ if(path)free(path); return; }
    int nextK = path[1];
    free(path);
    struct Node* nxt = findNode(root, nextK);
    if(nxt)*pos=nxt;
}

int lab4extra() {
    srand(time(NULL));

    struct Node *rootA,*rootB;
    int *portals,nport;
    generate_two_trees(&rootA,&rootB,&portals,&nport);

    printf("=== Дерево A ===\n");
    printTree(rootA,NULL,0);
    printf("\n=== Дерево B ===\n");
    printTree(rootB,NULL,0);

    printf("\nПорталы: ");
    for(int i=0;i<nport;i++)printf("%d ",portals[i]);
    printf("\n\n");

    // -------------------- выбор портала для B --------------------
    int targetPortal = portals[rnd(0,nport-1)];
    printf("B выбрал портал: %d\n\n", targetPortal);

    struct Node *posA=rootA;
    struct Node *posB=rootB;

    int B_in_A = 0;
    int MAX=1200;

    for(int step=1;step<=MAX;step++){
        printf("--- Шаг %d ---\n", step);

        // A случайно
        struct Node* prevA=posA;
        step_random(&posA);
        printf("A: %d -> %d\n", prevA->key, posA->key);

        // B
        if(!B_in_A){
            // движется к выбранному порталу
            struct Node* prevB=posB;
            step_chase(&posB, rootB, targetPortal);
            printf("B(в B): %d -> %d\n", prevB->key, posB->key);

            if(posB->key == targetPortal){
                printf("B достиг портала %d и переходит в дерево A!\n", targetPortal);
                posB = findNode(rootA, posB->key);
                B_in_A = 1;
                printf("Новая позиция B в A: %d\n", posB->key);
            }
        }
        else {
            // B преследует A
            struct Node* prevB=posB;
            step_chase(&posB, rootA, posA->key);
            printf("B(в A): %d -> %d (преследует)\n", prevB->key, posB->key);
        }

        // встреча
        if(B_in_A && posA->key == posB->key){
            printf("\n>>> ВСТРЕЧА! A и B встретились в узле %d на шаге %d\n",
                   posA->key, step);
            break;
        }

        printf("\n");
    }

    free(portals);
    freeTree(rootA);
    freeTree(rootB);
    return 0;
}
