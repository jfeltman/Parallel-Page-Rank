#include "graph.h"

typedef struct drand48_data drandData;

int get_random_node(int numNodes, int i);
int toss_coin(double D, int i);
int get_random_neighbor(Node *head, int numNeighbors, int index);
void page_rank_estimator(Graph *graph, int K, double D, int visit[], double pageRanks[]);
int find_num_nodes(char *file);
