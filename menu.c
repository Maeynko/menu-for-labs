#include <stdio.h>
#include <stdlib.h>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

int main() {
    int choice;

    while (1) {
        printf(BLUE "======= МЕНЮ =======\n" RESET);
        printf(GREEN "1" RESET " - lab 1.c\n");
        printf(GREEN "2" RESET " - lab 2.c\n");
        printf(GREEN "3" RESET " - lab 3.c\n");
        printf(GREEN "4" RESET " - lab 4.c\n");
        printf(GREEN "5" RESET " - lab 4 extra(agent).c\n");
        printf(GREEN "6" RESET " - lab 5.c\n");
        printf(RED   "0" RESET " - Выход\n");

        printf(YELLOW "Ваш выбор: " RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1: { 
                    printf("Воспроизвожу lab 1...\n\n");                    
                    char filename[256];
                    printf("Введите имя файла для lab1: ");
                    scanf("%s", filename);
                    lab1(filename);
                    break;
                   }
//printf("Воспроизвожу lab 1...\n\n");system("./lab1"); break;
            case 2: printf("Воспроизвожу lab 2...\n\n");lab2(); break;
            case 3: printf("Воспроизвожу lab 3...\n\n");lab3();break;
            case 4:{
                     printf("Воспроизвожу lab 4...\n\n");                   
                    char filename[256];
                    printf("Введите имя файла для lab4: ");
                    scanf("%s", filename);

                    lab4(filename);
                    break;
                    }
            case 5: printf("Воспроизвожу lab 4 extra(agent)...\n\n"); lab4extra(); break;
            case 6: printf("Воспроизвожу lab 5...\n\n"); lab5(); break;

            case 0:
                printf("Выход...\n");
                return 0;

            default:
                printf(RED "Неверный выбор\n" RESET);
        }

        printf(GREEN "\nВоспроизведение завершено\n\n" RESET);
    }

    return 0;
}
