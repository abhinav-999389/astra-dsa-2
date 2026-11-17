#ifndef ROUTING_H
#define ROUTING_H

#define MAX_NODES 100

/* Stores the graph and the cost between every pair of nodes. */
typedef struct {
    int nodeCount;
    double cost[MAX_NODES][MAX_NODES];
} RouteGraph;

/* Stores the shortest path found by Dijkstra's algorithm. */
typedef struct {
    int reachable;
    double totalCost;
    int path[MAX_NODES];
    int pathLength;
} RouteResult;

void initializeGraph(RouteGraph *graph, int nodes);

int addPathway(RouteGraph *graph, int a, int b,
               double distance, double risk, double fuel);

RouteResult runDijkstra(RouteGraph *graph, int start, int target);

void buildSampleGraph(RouteGraph *graph);

#endif
