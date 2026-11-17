/*
 * ASTRA safe-route demonstration using the simplified Dijkstra algorithm.
 * This file works with routing_simple.c and routing_simple.h.
 */

#include <stdio.h>
#include "routing_simple.h"

int main(void)
{
    RouteGraph graph;
    RouteResult result;
    int i;

    /* Create the same five-location sample graph. */
    buildSampleGraph(&graph);

    /* Find the lowest-cost route from location 0 to location 4. */
    result = runDijkstra(&graph, 0, 4);

    if (!result.reachable) {
        printf("Target is unreachable.\n");
        return 0;
    }

    printf("Total cost to target: %.2f\n", result.totalCost);
    printf("Recommended safe path: ");

    for (i = 0; i < result.pathLength; i++) {
        printf("%d", result.path[i]);
        if (i < result.pathLength - 1) {
            printf(" -> ");
        }
    }

    printf("\n");
    return 0;
}
