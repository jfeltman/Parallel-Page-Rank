#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <assert.h>
#include <string.h>

typedef struct drand48_data drandData;

int get_random_node(int numNodes, int i);
int toss_coin(double D);
int find_num_nodes(char *file);
