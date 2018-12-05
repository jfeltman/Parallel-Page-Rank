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

    int *vertexCount;
    vertexCount = malloc(sizeof(int) * nodes);
    memset(vertexCount, 0, sizeof(vertexCount));

    Graph *graph = createGraph(nodes);
    read_file(fileName, graph);

    double avgTime = 0.0;
    int i;
    for(i = 0; i < 10; i++) {
        double time = omp_get_wtime();
        page_rank_estimator(graph, K, D, vertexCount);
        time = omp_get_wtime() - time;

        avgTime += time;
    }
    printf("Avg time = %f seconds\n", avgTime/10.0);
    
    //page_rank_estimator(graph, K, D, vertexCount);
    //print_top_five(vertexCount, nodes);
    
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
//    printf("Rand Double = %f\n", flip);

    if (flip <= D) 
        return 1;
    else 
        return 0;
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
        cur = cur->next;

    return cur->dest;
}

void page_rank_estimator(Graph *graph, int K, double D, int visit[]) {
    int i;
    int n = graph->numNodes;

    int curNodeNumber;
    Node *cur;

    omp_set_num_threads(p);

    #pragma omp parallel for schedule(static) shared(visit, graph) private(cur, curNodeNumber)
    for(i = 0; i < n; i++) 
    {
        int j;
        cur = graph->list[i].head;
        curNodeNumber = i;
        for(j = 0; j < K; j++) 
        {
            #pragma omp atomic
            visit[curNodeNumber] += 1;

            int toss = toss_coin(D, i);
            if(toss == 1) { // heads
                int next = get_random_node(n, i);
                cur = graph->list[next].head;
                curNodeNumber = next;
            } 
            else { // tails
                if(graph->list[curNodeNumber].numLinks != 0) {
                    int next = get_random_neighbor(cur, graph->list[curNodeNumber].numLinks, i);
                    cur = graph->list[next].head;
                    curNodeNumber = next;
                }
            }
        }
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

void print_top_five(int pageRanks [], int n) 
{ 
    int i;
    int first, second, third, fourth, fifth; 
    int firstIndex, secondIndex, thirdIndex, fourthIndex, fifthIndex;
                
    fifth = fourth = third = first = second = 0.0; 
    firstIndex = secondIndex = thirdIndex = fourthIndex = fifthIndex = 0;                       
    for (i = 0; i < n ; i ++) 
    { 
        if (pageRanks[i] > first) { 
            fifth = fourth;
            fifthIndex = fourthIndex;
            fourth = third;
            fourthIndex = thirdIndex;
            third = second;
            thirdIndex = secondIndex;                                                                               
            second = first;
            secondIndex = firstIndex;
            first = pageRanks[i];
            firstIndex = i;
        } else if (pageRanks[i] > second) { 
            fifth = fourth;
            fifthIndex = fourthIndex;
            fourth = third;
            fourthIndex = thirdIndex;
            third = second; 
            thirdIndex = secondIndex;
            second = pageRanks[i]; 
            secondIndex = i;
        } else if (pageRanks[i] > third) {
            fifth = fourth;
            fifthIndex = fourthIndex;
            fourth = third;
            fourthIndex = thirdIndex;
            third = pageRanks[i];
            thirdIndex = i;
        } else if (pageRanks[i] > fourth) { 
            fifth = fourth;
            fifthIndex = fourthIndex;
            fourth = pageRanks[i]; 
            fourthIndex = i;
        } else if (pageRanks[i] > fifth) {
            fifth = pageRanks[i];
            fifthIndex = i;
        }
    }
                            
    printf("Node %d\t| Page Rank: %d\n", firstIndex, first);
    printf("Node %d\t| Page Rank: %d\n", secondIndex, second);
    printf("Node %d\t| Page Rank: %d\n", thirdIndex, third);
    printf("Node %d\t| Page Rank: %d\n", fourthIndex, fourth);
    printf("Node %d\t| Page Rank: %d\n", fifthIndex, fifth);
} 
