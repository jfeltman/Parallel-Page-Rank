#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <assert.h>
#include <string.h>

typedef struct node
{
    int dest;
    struct node *next;
}Node;

typedef struct adjlist
{
    int numLinks;
    Node *head;
}AdjList;

typedef struct graph
{
    int numNodes;
    AdjList *list;
}Graph;

Node* createNode(int dest);
Graph* createGraph(int numNodes);
void addEdge(Graph *graph, int source, int dest);
void printList(Node *head);
void printGraph(Graph *graph);
void read_file(char *file, Graph *graph);
