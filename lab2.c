#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void quickSort(int *arr, int left, int right) {
    if (left >= right) 
        return;
    int i = left, j = right;
    int center = arr[(left + right) / 2];
    while (i <= j) {
        while (arr[i] < center) 
            i++;
        while (arr[j] > center) 
            j--;
        if (i <= j) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++; j--;
        }
    }
    if (left < j)
        quickSort(arr, left, j);
    if (i < right) 
        quickSort(arr, i, right);
}

void selection_Sort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        int temp = arr[i];
        arr[i] = arr[min_index];
        arr[min_index] = temp;
    }
}

void print_array(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int read_file(const char *filename, int *arr, int max_size) {
    FILE *file = fopen(filename, "r");
    if (file == 0) {
        perror("Ошибка открытия файла");
        return -1;
    }
    int n = 0;
    while (n < max_size && fscanf(file, "%d", &arr[n]) == 1) {
        n++;
    }
    fclose(file);
    return n;
}

int lab2() {
    int choice_input, choice_sort;
    int n;
    int *arr;

    srand(time(NULL));

    printf("Выберите способ задания массива:\n");
    printf("1 - Ввод с клавиатуры\n");
    printf("2 - Генерация случайных чисел\n");
    printf("3 - Чтение из файла\n");
    scanf("%d", &choice_input);

    if (choice_input == 1) {
        printf("Введите размер массива: ");
        scanf("%d", &n);
        arr = (int*)malloc(n * sizeof(int));
        printf("Введите элементы массива:\n");
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }
    } else if (choice_input == 2) {
        printf("Введите размер массива: ");
        scanf("%d", &n);
        arr = (int*)malloc(n * sizeof(int));
        printf("Случайный массив:\n");
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 100; // случайные числа 0..99
            printf("%d ", arr[i]);
        }
        printf("\n");
    } else if (choice_input == 3) {
        char filename[100];
        printf("Введите имя файла: ");
        scanf("%s", filename);
        arr = (int*)malloc(1000 * sizeof(int)); // ограничение на размер
        n = read_file(filename, arr, 1000);
        if (n <= 0) {
            printf("Ошибка чтения из файла.\n");
            free(arr);
            return 1;
        }
        printf("Массив из файла:\n");
        print_array(arr, n);
    } else {
        printf("Неверный выбор!\n");
        return 1;
    }

    printf("Выберите метод сортировки:\n");
    printf("1 - Быстрая сортировка\n");
    printf("2 - Сортировка выбором\n");
    scanf("%d", &choice_sort);

    if (choice_sort == 1) {
        quickSort(arr, 0, n - 1);
    } else if (choice_sort == 2) {
        selection_Sort(arr, n);
    } else {
        printf("Неверный выбор сортировки!\n");
        free(arr);
        return 1;
    }

    print_array(arr, n);
    free(arr);
    return 0;
}
