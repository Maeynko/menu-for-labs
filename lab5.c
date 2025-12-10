#include <stdio.h>
#include <limits.h>

#define INF 99999
#define N 9    // количество вершин

// список рёбер графа варианта 9
struct Edge {
    int start, end, weight;
} edges[] = {
    {1,2,5},{1,4,6},{1,8,6},{1,7,19}, 
    {2,3,5},{2,5,4},  
    {3,7,2}, 
    {4,9,6},{4,5,1},
    {5,7,6},{5,6,9},
    {7,8,4},{7,9,1},
    {8,5,4}
};

int E = sizeof(edges)/sizeof(edges[0]);

void floydWarshall() {

    int dist[N+1][N+1];

    // инициализация матрицы
    for(int i=1;i<=N;i++)
        for(int j=1;j<=N;j++)
            dist[i][j] = (i==j) ? 0 : INF;

    // заполнение ребрами
    for(int i=0;i<E;i++)
        dist[edges[i].start][edges[i].end] = edges[i].weight;

    // основной цикл Флойда
    for(int k=1;k<=N;k++)
        for(int i=1;i<=N;i++)
            for(int j=1;j<=N;j++)
                if(dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];

    // вывод результата
    printf("\nМатрица кратчайших расстояний (Флойд-Уоршелл):\n\n");
    
    for (int j=0;j<=N;j++){
    if(j ==0) printf("Nodes ");
    else printf(" X%-2d",j);
    }
    printf("\n");
    for(int i=1;i<=N;i++){
        printf("X%-3d", i);
        for(int j=1;j<=N;j++){
            
            
            if(dist[i][j] >= INF) printf(" INF");
            else printf("%4d", dist[i][j]);
        }
        printf("\n");
    }
}

int lab5(){
    floydWarshall();
    return 0;
}
