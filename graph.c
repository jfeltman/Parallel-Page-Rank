#include "graph.h"

Node* createNode(int dest)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int numNodes)
{
    Graph *graph = malloc(sizeof(Graph));
    graph->numNodes = numNodes;

    graph->list = malloc(numNodes * sizeof(AdjList));

    int i;
    for(i = 0; i < numNodes; i++) {
        graph->list[i].head = NULL;
    }
    
    return graph;
}

void addEdge(Graph *graph, int source, int dest)
{
    Node *newNode = createNode(dest);
    newNode->next = graph->list[source].head;
    graph->list[source].head = newNode;
}

void printList(Node *head)
{
    Node *cur = head;
    while(cur != NULL) {
        printf("%d | ", cur->dest);
        cur = cur->next;
    }
    printf("NULL\n");
}

void printGraph(Graph *graph)
{
    int i;
    for(i = 0; i < graph->numNodes; i++) {
        Node *cur = graph->list[i].head;
        printf("Node %d -> ", i);
        printList(cur);
    }
}

void read_file(char *file, Graph *graph) {
    FILE *fp;
    char num[128];

    fp = fopen(file, "r");

    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    while(fgets(num, 128, fp) != NULL) {
        if (num[0] != '#') {
            char *temp = strtok(num, " \t");
            int x = atoi(temp);
            
            temp = strtok(NULL, " \t");
            int y = atoi(temp);
            // printf("x = %d, y = %d\n", x, y);
                      
            addEdge(graph, x, y);
        }
    }
    fclose(fp);
}
