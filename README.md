# TSP Thermal: NP-Complete Problem Solver

**An interactive, browser-based tool exploring the landscape of NP-complete problems through polynomial reductions to the Traveling Salesman Problem.**

## Why This Matters

- 📚 **Educational tool** for computer science students learning complexity theory
- 🔍 **Hands-on visualization** of NP-completeness proofs and polynomial reductions
- 🧩 **Practical solver** for small combinatorial optimization instances
- ✨ **No installation required** — runs entirely in your browser
- ⚡ **Static app** using only local HTML, CSS, and JavaScript

## Problems Supported

Explore how different NP-complete problems reduce to each other:

- **3-SAT** → Hamiltonian-cycle reduction
- **Vertex Cover** → 3-SAT → Hamiltonian-cycle
- **Clique** → Vertex Cover → 3-SAT → Hamiltonian-cycle
- **Independent Set** → Vertex Cover → 3-SAT → Hamiltonian-cycle
- **Set Cover** → 3-SAT → Hamiltonian-cycle
- **Exact Cover (X3C)** → 3-SAT → Hamiltonian-cycle
- **Graph Coloring** → 3-SAT → Hamiltonian-cycle
- **Direct Hamiltonian input** (pairs, matrix, Euclidean points, manual edges)

## Get Started

1. Clone or download this repo
2. Open `index.html` in your browser
3. Try an example or input your own problem

```bash
git clone https://github.com/np-douce/tsp-thermal.git
cd tsp-thermal
# Open index.html in your browser
```

## Who Should Use This?

- 🎓 CS students studying NP-completeness and computational complexity
- 🔬 Researchers exploring reduction chains and hardness proofs
- 💡 Anyone curious about why certain problems are hard to solve
- 📊 Educators teaching combinatorial optimization

## Technical Details

### Math Foundation

The Hamiltonian/TSP theory calculations are ported from C++ formulas using JavaScript `Number` (IEEE-754 double precision). 

Large Hamiltonian graphs can still be expensive because the original theory includes nested pair/edge calculations. Matrix, points, manual, and pair inputs do not have a fixed app maximum.

### Input Formats

Detailed documentation for each problem type is available in the full [Input Format Reference](#input-formats-reference) below.

## Contributing

We'd love your help! Whether it's:
- Adding visualizations for reductions
- Optimizing algorithms for larger instances
- Expanding problem types
- Building test suites
- Improving the UI/UX

See [CONTRIBUTING.md](CONTRIBUTING.md) for details.

## License

Apache License 2.0

---

## Input Formats Reference

### 3-SAT Input Format

The first line is:
```
variables clauses optional_padding_directed_nodes
```

Each following line is one clause with three integer literals:
```
1 -2 3
```

This means: `(x1 OR ~x2 OR x3)`

Node count for the polynomial reduction:
```
undirected nodes = 3 * (variables * (clauses + 1) + clauses + 2 + padding)
```

### Vertex Cover Input Format

The first line is:
```
vertices k optional_padding_directed_nodes
```

Each following line is one undirected edge:
```
1 2
2 3
```

The app reports whether a vertex cover of size at most `k` exists, shows a witness cover when one is found, then builds an equivalent 3-SAT encoding and compressed Hamiltonian-cycle graph.

The at-most-`k` part uses a sequential counter encoding, so it grows closer to `O(vertices * k)` instead of enumerating every `k + 1` subset.

### Clique Input Format

The first line is:
```
vertices k optional_padding_directed_nodes
```

Each following line is one undirected edge:
```
1 2
1 3
2 3
```

This asks whether the graph has a clique of size at least `k`. The app uses the standard practical reduction:

```
Clique(G, k) -> Vertex Cover(complement(G), vertices - k) -> 3-SAT -> compressed Hamiltonian Cycle
```

Example:
```
5 3
1 2
1 3
2 3
3 4
4 5
```

This example has a clique `{1, 2, 3}`. The Hamiltonian-cycle side still uses the compressed reduction and degree-2 forced-edge precheck.

### Independent Set Input Format

The first line is:
```
vertices k optional_padding_directed_nodes
```

Each following line is one undirected edge:
```
1 2
2 3
3 4
4 5
```

This asks whether the graph has an independent set of size at least `k`. The app uses the shorter standard reduction:

```
Independent Set(G, k) -> Vertex Cover(G, vertices - k) -> 3-SAT -> compressed Hamiltonian Cycle
```

Example:
```
5 3
1 2
2 3
3 4
4 5
```

This example has an independent set `{1, 3, 5}`. Because this route goes directly to Vertex Cover on the same graph, it avoids an extra complement graph step before the Hamiltonian-cycle calculation.

### Set Cover Input Format

The first line is:
```
universe_size set_count k optional_padding_directed_nodes
```

Each following line is one set, written as element numbers from `1` through `universe_size`:
```
1 2
2 3 4
4 5
1 5
```

This asks whether at most `k` sets cover every universe element. The app uses:

```
Set Cover -> 3-SAT coverage clauses plus at-most-k -> compressed Hamiltonian Cycle
```

Example:
```
5 4 2
1 2
2 3 4
4 5
1 5
```

This example has a set cover `{S2, S4}`. The 3-SAT encoding uses one Boolean variable per set, one coverage clause per universe element, and the sequential counter for the at-most-`k` rule.

### X3C Input Format

The first line is:
```
universe_size set_count optional_padding_directed_nodes
```

Each following line is one 3-element set, written as element numbers from `1` through `universe_size`:
```
1 2 3
4 5 6
1 4 5
2 3 6
```

This asks whether the universe can be covered exactly once by disjoint 3-sets. The app uses:

```
X3C -> 3-SAT exactly-once coverage clauses -> compressed Hamiltonian Cycle
```

Example:
```
6 4
1 2 3
4 5 6
1 4 5
2 3 6
```

This example has an exact cover `{S1, S2}`. The 3-SAT encoding uses one Boolean variable per 3-set, one at-least-one coverage clause for each universe element, and pairwise not-both clauses for exclusivity.

### Graph Coloring Input Format

The first line is:
```
vertices edges colors optional_padding_directed_nodes
```

Each following line is one undirected edge:
```
1 2
2 3
3 4
4 1
```

This asks whether every vertex can be colored with the requested number of colors so that connected vertices have different colors. The app uses:

```
Graph Coloring -> 3-SAT color clauses -> compressed Hamiltonian Cycle
```

Example 3-colorable instance:
```
4 4 3
1 2
2 3
3 4
4 1
```

Example 4-colorable instance:
```
4 6 4
1 2
1 3
1 4
2 3
2 4
3 4
```

That second graph is `K4`: it is not 3-colorable, but it is 4-colorable. The 3-SAT encoding uses `colors * vertices` base variables, one at-least-one-color clause per vertex, and pairwise not-both clauses for each edge.
