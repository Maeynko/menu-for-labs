#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node* createNode(int data) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        perror("Ошибка выделения памяти");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void append(Node **head, int data) {
    Node *newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node *current = *head;
    while (current->next != NULL)
        current = current->next;
    current->next = newNode;
}

int contains(Node *head, int value) {
    while (head) {
        if (head->data == value)
            return 1;
        head = head->next;
    }
    return 0;
}

Node* difference(Node *L1, Node *L2) {
    Node *L = NULL;
    while (L1) {
        if (!contains(L2, L1->data))
            append(&L, L1->data);
        L1 = L1->next;
    }
    return L;
}

// Удаление списка
void deleteList(Node **head) {
    Node *current = *head;
    Node *next;
    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

void printList(Node *head) {
    if (!head) {
        printf("(пустой)\n");
        return;
    }
    while (head) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

// Ввод списка пользователем
void inputList(Node **head, const char *name) {
    int n, val;
    printf("Введите количество элементов в списке %s: ", name);
    scanf("%d", &n);
    printf("Введите элементы %s через пробел: ", name);
    for (int i = 0; i < n; i++) {
        scanf("%d", &val);
        append(head, val);
    }
}
int lab3() {
    Node *L1 = NULL, *L2 = NULL, *L = NULL;

    printf("=== Формирование списков L1 и L2 ===\n");
    inputList(&L1, "L1");
    inputList(&L2, "L2");

    printf("\nСписок L1: ");
    printList(L1);
    printf("Список L2: ");
    printList(L2);

    L = difference(L1, L2);

    printf("\nРезультирующий список L (L1 - L2): ");
    printList(L);

    // Очистка памяти
    deleteList(&L1);
    deleteList(&L2);
    deleteList(&L);

    return 0;
}
