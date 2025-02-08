# 2dFragmentedTree
the code for 2d Fragmented Tree where updates and queries are ranged, and query in a form of

$$f_{i=x0}^{x1}( g_{j=y0}^{y1} (A_{i,j}))$$

where f and g is an associative operation, but in my code I'm using f as $\sum$, and g as $min$

## Definitions
$n$ is the numbers of rows

$m$ is the numbers of columns

$q$ is the numbers of queries

$q_2$ is the numbers of range upd queries

## Time Complexity
upd: $O(q_2logm)$

query: $O(q_2logm)$

## Memory usage: $O(q_2logm)$
