# NP-douce

Open `index.html` in a browser to run the app.

The app is static: it uses only local HTML, CSS, and JavaScript. No server is required for desktop use.

## Included tools

- 3-SAT polynomial Hamiltonian-cycle reduction
- Vertex Cover to 3-SAT to compressed Hamiltonian-cycle reduction
- Clique to Vertex Cover complement to 3-SAT to compressed Hamiltonian-cycle reduction
- Independent Set to Vertex Cover to 3-SAT to compressed Hamiltonian-cycle reduction
- Set Cover to 3-SAT to compressed Hamiltonian-cycle reduction
- X3C to 3-SAT to compressed Hamiltonian-cycle reduction
- Graph Coloring to 3-SAT to compressed Hamiltonian-cycle reduction
- Hamiltonian pairs input
- Matrix input
- Euclidean points input
- Manual upper-triangle edge input

## Math note

The Hamiltonian/TSP theory calculations are ported from the C++ formulas using JavaScript `Number`, which is IEEE-754 double precision like C++ `double`.

Large Hamiltonian graphs can still be expensive because the original theory includes nested pair/edge calculations. Matrix, points, manual, and pair inputs do not have a fixed app maximum; their practical size depends on the input and the device running the browser.

## 3-SAT input format

The first line is:

```text
variables clauses optional_padding_directed_nodes
```

Each following line is one clause with three integer literals:

```text
1 -2 3
```

This means:

```text
(x1 OR ~x2 OR x3)
```

Node count for the polynomial reduction:

```text
undirected nodes = 3 * (variables * (clauses + 1) + clauses + 2 + padding)
```

## Vertex Cover input format

The first line is:

```text
vertices k optional_padding_directed_nodes
```

Each following line is one undirected edge:

```text
1 2
2 3
```

The app reports whether a vertex cover of size at most `k` exists, shows a witness cover when one is found, then builds an equivalent 3-SAT encoding and compressed Hamiltonian-cycle graph with the degree-2 precheck.

The at-most-`k` part uses a sequential counter encoding, so it grows closer to `O(vertices * k)` instead of enumerating every `k + 1` subset.

## Clique input format

The first line is:

```text
vertices k optional_padding_directed_nodes
```

Each following line is one undirected edge:

```text
1 2
1 3
2 3
```

This asks whether the graph has a clique of size at least `k`. The app uses the standard practical reduction:

```text
Clique(G, k) -> Vertex Cover(complement(G), vertices - k) -> 3-SAT -> compressed Hamiltonian Cycle
```

Example:

```text
5 3
1 2
1 3
2 3
3 4
4 5
```

This example has a clique `{1, 2, 3}`. The Hamiltonian-cycle side still uses the compressed reduction and degree-2 forced-edge precheck.

## Independent Set input format

The first line is:

```text
vertices k optional_padding_directed_nodes
```

Each following line is one undirected edge:

```text
1 2
2 3
3 4
4 5
```

This asks whether the graph has an independent set of size at least `k`. The app uses the shorter standard reduction:

```text
Independent Set(G, k) -> Vertex Cover(G, vertices - k) -> 3-SAT -> compressed Hamiltonian Cycle
```

Example:

```text
5 3
1 2
2 3
3 4
4 5
```

This example has an independent set `{1, 3, 5}`. Because this route goes directly to Vertex Cover on the same graph, it avoids an extra complement graph step before the Hamiltonian-cycle calculation.

## Set Cover input format

The first line is:

```text
universe_size set_count k optional_padding_directed_nodes
```

Each following line is one set, written as element numbers from `1` through `universe_size`:

```text
1 2
2 3 4
4 5
1 5
```

This asks whether at most `k` sets cover every universe element. The app uses:

```text
Set Cover -> 3-SAT coverage clauses plus at-most-k -> compressed Hamiltonian Cycle
```

Example:

```text
5 4 2
1 2
2 3 4
4 5
1 5
```

This example has a set cover `{S2, S4}`. The 3-SAT encoding uses one Boolean variable per set, one coverage clause per universe element, and the sequential counter for the at-most-`k` rule.

## X3C input format

The first line is:

```text
universe_size set_count optional_padding_directed_nodes
```

Each following line is one 3-element set, written as element numbers from `1` through `universe_size`:

```text
1 2 3
4 5 6
1 4 5
2 3 6
```

This asks whether the universe can be covered exactly once by disjoint 3-sets. The app uses:

```text
X3C -> 3-SAT exactly-once coverage clauses -> compressed Hamiltonian Cycle
```

Example:

```text
6 4
1 2 3
4 5 6
1 4 5
2 3 6
```

This example has an exact cover `{S1, S2}`. The 3-SAT encoding uses one Boolean variable per 3-set, one at-least-one coverage clause for each universe element, and pairwise not-both clauses for every pair of sets that share an element.

## Graph Coloring input format

The first line is:

```text
vertices edges colors optional_padding_directed_nodes
```

Each following line is one undirected edge:

```text
1 2
2 3
3 4
4 1
```

This asks whether every vertex can be colored with the requested number of colors so that connected vertices have different colors. The app uses:

```text
Graph Coloring -> 3-SAT color clauses -> compressed Hamiltonian Cycle
```

Example 3-colorable instance:

```text
4 4 3
1 2
2 3
3 4
4 1
```

Example 4-colorable instance:

```text
4 6 4
1 2
1 3
1 4
2 3
2 4
3 4
```

That second graph is `K4`: it is not 3-colorable, but it is 4-colorable. The 3-SAT encoding uses `colors * vertices` base variables, one at-least-one-color clause per vertex, pairwise not-both color clauses per vertex, and one conflict clause per edge per color before 3-literal normalization.
