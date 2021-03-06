#include "graph.h"

// Joshua Feltman
// 11494067

typedef struct drand48_data drandData;

int get_random_node(int numNodes, int i);
int toss_coin(double D, int i);
int get_random_neighbor(Node *head, int numNeighbors, int index);
void page_rank_estimator_critical(Graph *graph, int K, double D, int visit[]);
void page_rank_estimator_atomic(Graph *graph, int K, double D, int visit[]);
void page_rank_estimator_locks(Graph *graph, int K, double D, int visit[]);
int find_num_nodes(char *file);
void print_top_five(int pageRanks[], int n);
