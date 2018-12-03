#include "main.h"

int p=1;

int main(int argc, char *argv[])
{
    p = atoi(argv[1]);
    int K = atoi(argv[2]);
    
    double D;
    sscanf(argv[3], "%lf", &D);

    char *fileName = argv[4];

    printf("p = %d | K = %d | D = %lf | File = %s\n", p, K, D, fileName);
    
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

    printGraph(graph);
    
    double pageRanks[nodes];
    page_rank_estimator(graph, K, D, vertexCount, pageRanks);

//    sort_page_ranks(pageRanks, nodes);

//    int i;
//    for(i = 0; i < 5; i++) {
//    }
    return 0;
}

int get_random_node(int numNodes, int i) {
    int rank = omp_get_thread_num();
    int seed = rank + 1;

    seed = seed * i;

    return rand_r(&seed) % numNodes;
}

int toss_coin(double D, int i) {
    double flip;

    drandData state;

    int rank = omp_get_thread_num();
    int seed = rank + 1;
    seed = seed * i;

    srand48_r(time(NULL) + seed, &state);

    drand48_r(&state, &flip);

    printf("Rand Double = %f\n", flip);

    if (flip <= D) {
        printf("HEADs\n");
        return 1;
    } 
    else {
        printf("TAILS\n");
        return 0;
    }
}

int get_random_neighbor(Node *head, int numNeighbors, int index) {
    int rank = omp_get_thread_num();
    int seed = rank + 1;
    seed = seed * index;
   
    int nextNode = rand_r(&seed) % numNeighbors + 1;
    //printf("rand num = %d\n", nextNode);

    Node *cur = head;
    int i;
  
    for(i = 1; i < nextNode; i++)
    {
        cur = cur->next;
    }

    return cur->dest;
}

void page_rank_estimator(Graph *graph, int K, double D, int visit[], double pageRanks[]) {

    int i, j, next;
    int n = graph->numNodes;

    int curNodeNumber;
    Node *cur = malloc(sizeof(Node));

    for(i = 0; i < n; i++) {
        cur = graph->list[i].head;
        curNodeNumber = i;
        for(j = 0; j < K; j++) {
            visit[curNodeNumber]++;

            int toss = toss_coin(D, i);
            
            if(toss == 1) { // heads
                next = get_random_node(n, i);
                cur = graph->list[next].head;
                curNodeNumber = next;
            } 
            else { // tails
                // if node is by itself, keep hopping to itself
                if(graph->list[curNodeNumber].numLinks != 0) {
                    next = get_random_neighbor(cur, graph->list[curNodeNumber].numLinks, i);
                    cur = graph->list[next].head;
                    curNodeNumber = next;
                }
            }
        }
    }

    for(i = 0; i < n; i++) {
        pageRanks[i] = (double)visit[i] / (double)(n * K);
        printf("Node %d rank = %lf\n", i, pageRanks[i]);
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

void sort_page_ranks(double ranks[], int n) {
    int i, j;

    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(ranks[j] < ranks[i]) {
                double temp = ranks[i];
                ranks[i] = ranks[j];
                ranks[j] = temp;
            }
        }
    }
}
