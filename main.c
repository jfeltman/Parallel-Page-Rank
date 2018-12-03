#include "main.h"
#include "graph.c"

int p=1;

int main(int argc, char *argv[])
{
    p = atoi(argv[1]);
    int K = atoi(argv[2]);
    
    double D;
    sscanf(argv[3], "%lf", &D);

    char *fileName = argv[4];

    printf("p = %d | K = %d | D =  %lf | File = %s\n", p, K, D, fileName);
    
    omp_set_num_threads(p);

    #pragma omp parallel
    {
        assert(p == omp_get_num_threads());
        int rank = omp_get_thread_num();
        printf("Rank %d: Thread = %d\n", rank, omp_get_num_threads());
    }

    // Add 1 for last array element
    int nodes = find_num_nodes(fileName) + 1;
    printf("Number of Nodes: %d\n", nodes);

    int vertexCount[nodes];
    memset(vertexCount, 0, sizeof(vertexCount));

    Graph *graph = createGraph(nodes);
    read_file(fileName, graph);

    int t = toss_coin(D);

    printf("COIN FLIP = %d\n", t);


    int r = get_random_node(nodes, time(NULL));
    printf("random node = %d\n", r);

    printList(graph->list[r].head);

//    printGraph(graph);
    return 0;
}

int get_random_node(int numNodes, int i) {
    int rank = omp_get_thread_num();
    int seed = rank + 1;

    seed = seed * i;

    return rand_r(&seed) % numNodes;
}

int toss_coin(double D) {
    double flip;

    drandData state;

    int rank = omp_get_thread_num();
    int seed = rank + 1;

    srand48_r(time(NULL) + seed, &state);

    drand48_r(&state, &flip);

    printf("Rand Double = %f\n", flip);

    if (flip <= D) {
        return 1;
    } else {
        return 0;
    }
}

int find_num_nodes(char *file) {
    FILE *fp;
    char num[128];

    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    int max = 0;
    while(fgets(num, 128, fp) != NULL) {
        if (num[0] != '#') {
            char *temp = strtok(num, " \t");
            int x = atoi(temp);
            temp = strtok(NULL, " \t");
            int y = atoi(temp);

            if (x > y) {
                if(x >= max)
                    max = x;
            } else {
                if(y >= max)
                    max = y;
            }
        }
    }

    fclose(fp);
    return max;
}
