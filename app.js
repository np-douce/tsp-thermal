"use strict";

const output = document.getElementById("output");

function write(text) {
  output.textContent = text;
}

function append(lines, text = "") {
  lines.push(text);
}

function formatNumber(value) {
  if (!Number.isFinite(value)) return String(value);
  if (Math.abs(value) >= 1e6 || (Math.abs(value) > 0 && Math.abs(value) < 1e-4)) {
    return value.toExponential(6);
  }
  return Number(value.toPrecision(10)).toString();
}

function lnGamma(z) {
  const coeff = [
    676.5203681218851,
    -1259.1392167224028,
    771.32342877765313,
    -176.61502916214059,
    12.507343278686905,
    -0.13857109526572012,
    9.9843695780195716e-6,
    1.5056327351493116e-7
  ];
  if (z < 0.5) {
    return Math.log(Math.PI) - Math.log(Math.sin(Math.PI * z)) - lnGamma(1 - z);
  }
  z -= 1;
  let x = 0.99999999999980993;
  for (let i = 0; i < coeff.length; i++) {
    x += coeff[i] / (z + i + 1);
  }
  const t = z + coeff.length - 0.5;
  return 0.5 * Math.log(2 * Math.PI) + (z + 0.5) * Math.log(t) - t + Math.log(x);
}

function makeMatrix(n, fill = 0) {
  return Array.from({ length: n + 1 }, () => Array(n + 1).fill(fill));
}

function buildSquaredEdgeMatrix(edge, n) {
  const edgeSquared = makeMatrix(n);
  for (let i = 0; i <= n; i++) {
    for (let j = 0; j <= n; j++) {
      edgeSquared[i][j] = edge[i][j] * edge[i][j];
    }
  }
  return edgeSquared;
}

function computeTheoryMoments(edge, edgeSquared, n) {
  const moments = {
    meanTourLength: 0,
    selfInteractionSum: 0,
    neighborInteractionSum: 0,
    disjointInteractionSum: 0,
    tourVariance: 0
  };
  const vertexCount = n;
  let edgeSum = 0;
  for (let i = 1; i < n; i++) {
    for (let j = i + 1; j <= n; j++) edgeSum += edge[i][j];
  }
  moments.meanTourLength = (2.0 / (vertexCount - 1.0)) * edgeSum;

  for (let i = 1; i <= n; i++) {
    for (let j = i + 1; j <= n; j++) moments.selfInteractionSum += edgeSquared[i][j];
  }

  const nonZeroEdges = [];
  const incidentEdges = Array.from({ length: n + 1 }, () => []);
  for (let i = 1; i <= n; i++) {
    for (let j = i + 1; j <= n; j++) {
      const weight = edge[i][j];
      if (weight === 0) continue;
      nonZeroEdges.push({ from: i, to: j, weight });
      incidentEdges[i].push({ from: i, to: j, weight });
      incidentEdges[j].push({ from: j, to: i, weight });
    }
  }

  for (let i = 1; i <= n; i++) {
    for (let first = 0; first < incidentEdges[i].length; first++) {
      for (let second = first + 1; second < incidentEdges[i].length; second++) {
        moments.neighborInteractionSum += incidentEdges[i][first].weight * incidentEdges[i][second].weight;
      }
    }
  }

  for (let first = 0; first < nonZeroEdges.length; first++) {
    const firstEdge = nonZeroEdges[first];
    for (let second = first + 1; second < nonZeroEdges.length; second++) {
      const secondEdge = nonZeroEdges[second];
      if (firstEdge.from === secondEdge.from || firstEdge.from === secondEdge.to ||
          firstEdge.to === secondEdge.from || firstEdge.to === secondEdge.to) {
        continue;
      }
      moments.disjointInteractionSum += firstEdge.weight * secondEdge.weight;
    }
  }

  moments.tourVariance =
    ((2.0 / (vertexCount - 1.0)) * moments.selfInteractionSum) +
    ((4.0 / ((vertexCount - 1.0) * (vertexCount - 2.0))) *
      (moments.neighborInteractionSum + (2.0 * moments.disjointInteractionSum))) -
    (moments.meanTourLength * moments.meanTourLength);

  return moments;
}

function collectAvailableVertices(endpointLink, n) {
  const values = [];
  for (let i = 1; i <= n; i++) if (endpointLink[i] !== -1) values.push(i);
  return values;
}

function accumulateRemainingEdgeBuckets(availableVertices, endpointLink, edge, edgeSquared) {
  const buckets = {
    activeFreeSum: 0,
    activeActiveOpenSum: 0,
    freeFreeSum: 0,
    activeFreeSquareSum: 0,
    activeActiveOpenSquareSum: 0,
    freeFreeSquareSum: 0,
    activeActiveOpenPairSum: 0,
    activeFreeTouchingPairSum: 0,
    activeFreeDisjointPairSum: 0,
    freeFreeTouchingPairSum: 0,
    freeFreeDisjointPairSum: 0,
    mixedOpenTouchingPairSum: 0,
    mixedOpenFreePairSum: 0,
    activeFreeFreeTouchingPairSum: 0,
    activeFreeFreeDisjointPairSum: 0
  };

  for (let a = 0; a < availableVertices.length; a++) {
    const y = availableVertices[a];
    for (let b = a + 1; b < availableVertices.length; b++) {
      const z = availableVertices[b];
      if ((endpointLink[y] === 0 && endpointLink[z] !== 0) ||
          (endpointLink[y] !== 0 && endpointLink[z] === 0)) {
        buckets.activeFreeSum += edge[y][z];
        buckets.activeFreeSquareSum += edgeSquared[y][z];
      }
      if (endpointLink[y] !== 0 && endpointLink[z] !== 0 &&
          endpointLink[y] !== z && endpointLink[z] !== y) {
        buckets.activeActiveOpenSum += edge[y][z];
        buckets.activeActiveOpenSquareSum += edgeSquared[y][z];
      }
      if (endpointLink[y] === 0 && endpointLink[z] === 0) {
        buckets.freeFreeSum += edge[y][z];
        buckets.freeFreeSquareSum += edgeSquared[y][z];
      }
    }
  }

  return buckets;
}

function accumulateVarianceBuckets(availableVertices, endpointLink, edge, buckets) {
  const remainingEdges = [];
  for (let fromIndex = 0; fromIndex < availableVertices.length; fromIndex++) {
    const from = availableVertices[fromIndex];
    const fromLink = endpointLink[from];
    for (let toIndex = fromIndex + 1; toIndex < availableVertices.length; toIndex++) {
      const to = availableVertices[toIndex];
      const toLink = endpointLink[to];
      if (fromLink === to || toLink === from) continue;
      const weight = edge[from][to];
      if (weight !== 0) remainingEdges.push({ from, to, fromLink, toLink, weight });
    }
  }

  for (let firstIndex = 0; firstIndex < remainingEdges.length; firstIndex++) {
    const first = remainingEdges[firstIndex];
    for (let secondIndex = firstIndex + 1; secondIndex < remainingEdges.length; secondIndex++) {
      const second = remainingEdges[secondIndex];
      const c = first.from, g = first.to, p = second.from, k = second.to;
      const cLink = first.fromLink, gLink = first.toLink;
      const pLink = second.fromLink, kLink = second.toLink;

      if ((cLink !== 0 && (k === c || p === c)) ||
          (gLink !== 0 && (k === g || p === g))) continue;
      if ((kLink !== 0 && (k === c || k === g)) ||
          (pLink !== 0 && (p === c || p === g))) continue;
      if ((cLink === p && g === k) || (cLink === k && g === p) ||
          (gLink === p && c === k) || (gLink === k && c === p)) continue;
      if ((kLink === c && g === p) || (kLink === g && c === p) ||
          (pLink === c && g === k) || (pLink === g && c === k)) continue;
      if ((cLink === p && gLink === k) || (cLink === k && gLink === p)) continue;

      const product = first.weight * second.weight;
      if (kLink !== 0 && pLink !== 0 && cLink !== 0 && gLink !== 0) {
        buckets.activeActiveOpenPairSum += product;
      }
      if ((cLink !== 0 && gLink === 0 && pLink !== 0 && kLink === 0 && g === k) ||
          (cLink !== 0 && gLink === 0 && pLink === 0 && kLink !== 0 && g === p) ||
          (cLink === 0 && gLink !== 0 && pLink !== 0 && kLink === 0 && c === k) ||
          (cLink === 0 && gLink !== 0 && pLink === 0 && kLink !== 0 && c === p)) {
        buckets.activeFreeTouchingPairSum += product;
      }
      if ((cLink !== 0 && gLink === 0 && pLink !== 0 && kLink === 0 && g !== k) ||
          (cLink !== 0 && gLink === 0 && pLink === 0 && kLink !== 0 && g !== p) ||
          (cLink === 0 && gLink !== 0 && pLink !== 0 && kLink === 0 && c !== k) ||
          (cLink === 0 && gLink !== 0 && pLink === 0 && kLink !== 0 && c !== p)) {
        buckets.activeFreeDisjointPairSum += product;
      }
      if (cLink === 0 && gLink === 0 && pLink === 0 && kLink === 0) {
        if (c === k || c === p || g === k || g === p) buckets.freeFreeTouchingPairSum += product;
        else buckets.freeFreeDisjointPairSum += product;
      }
      if ((cLink !== 0 && gLink !== 0 && pLink === 0 && kLink !== 0) ||
          (cLink !== 0 && gLink !== 0 && pLink !== 0 && kLink === 0) ||
          (cLink === 0 && gLink !== 0 && pLink !== 0 && kLink !== 0) ||
          (cLink !== 0 && gLink === 0 && pLink !== 0 && kLink !== 0)) {
        buckets.mixedOpenTouchingPairSum += product;
      }
      if ((cLink !== 0 && gLink !== 0 && pLink === 0 && kLink === 0) ||
          (cLink === 0 && gLink === 0 && pLink !== 0 && kLink !== 0)) {
        buckets.mixedOpenFreePairSum += product;
      }
      if ((cLink !== 0 && gLink === 0 && pLink === 0 && kLink === 0) ||
          (cLink === 0 && gLink !== 0 && pLink === 0 && kLink === 0) ||
          (cLink === 0 && gLink === 0 && pLink !== 0 && kLink === 0) ||
          (cLink === 0 && gLink === 0 && pLink === 0 && kLink !== 0)) {
        if (c === p || c === k || g === k || g === p) buckets.activeFreeFreeTouchingPairSum += product;
        else buckets.activeFreeFreeDisjointPairSum += product;
      }
    }
  }
}

function applyChosenEdge(from, to, edge, endpointLink, state) {
  if (endpointLink[from] === -1 || endpointLink[to] === -1 ||
      endpointLink[from] === to || endpointLink[to] === from) {
    return false;
  }
  state.chosenEdgeTotal += edge[from][to];

  if (endpointLink[from] !== 0 && endpointLink[to] !== 0) {
    const leftMate = endpointLink[from];
    const rightMate = endpointLink[to];
    endpointLink[leftMate] = rightMate;
    endpointLink[rightMate] = leftMate;
    endpointLink[from] = -1;
    endpointLink[to] = -1;
    state.closedChains -= 1;
    return true;
  }
  if (endpointLink[from] === 0 && endpointLink[to] !== 0) {
    const mate = endpointLink[to];
    endpointLink[from] = mate;
    endpointLink[mate] = from;
    endpointLink[to] = -1;
    state.usedVertices += 1;
    return true;
  }
  if (endpointLink[to] === 0 && endpointLink[from] !== 0) {
    const mate = endpointLink[from];
    endpointLink[to] = mate;
    endpointLink[mate] = to;
    endpointLink[from] = -1;
    state.usedVertices += 1;
    return true;
  }
  endpointLink[from] = to;
  endpointLink[to] = from;
  state.closedChains += 1;
  state.usedVertices += 2;
  return true;
}

function computeConditionedMeanTourLength(n, endpointLink, state, edge, buckets) {
  const remainingFactor = n - 1 + state.closedChains - state.usedVertices;
  if (remainingFactor === 0) {
    for (let i = 1; i <= n; i++) {
      if (endpointLink[i] === -1) continue;
      for (let j = i + 1; j <= n; j++) {
        if (endpointLink[j] !== -1 && endpointLink[j] === i) return state.chosenEdgeTotal + edge[i][j];
      }
    }
    return state.chosenEdgeTotal;
  }
  return state.chosenEdgeTotal +
    (0.5 / remainingFactor) * buckets.activeActiveOpenSum +
    (1.0 / remainingFactor) * buckets.activeFreeSum +
    (2.0 / remainingFactor) * buckets.freeFreeSum;
}

function computeConditionedVariance(n, state, conditionedMeanTourLength, buckets) {
  const remainingFactor = n - 1 + state.closedChains - state.usedVertices;
  const secondFactor = n - 2 + state.closedChains - state.usedVertices;
  if (remainingFactor <= 0 || secondFactor <= 0) return 0;
  return (0.5 / remainingFactor) * buckets.activeActiveOpenSquareSum +
    (1.0 / remainingFactor) * buckets.activeFreeSquareSum +
    (2.0 / remainingFactor) * buckets.freeFreeSquareSum +
    (1.0 / (remainingFactor * secondFactor)) *
      (0.5 * buckets.activeActiveOpenPairSum +
       buckets.activeFreeTouchingPairSum +
       2.0 * buckets.activeFreeDisjointPairSum +
       4.0 * buckets.freeFreeTouchingPairSum +
       8.0 * buckets.freeFreeDisjointPairSum +
       buckets.mixedOpenTouchingPairSum +
       2.0 * buckets.mixedOpenFreePairSum +
       2.0 * buckets.activeFreeFreeTouchingPairSum +
       4.0 * buckets.activeFreeFreeDisjointPairSum) -
    ((conditionedMeanTourLength - state.chosenEdgeTotal) *
     (conditionedMeanTourLength - state.chosenEdgeTotal));
}

function computeTheoryScore(n, edge, edgeSquared, endpointLink, state, beta) {
  const availableVertices = collectAvailableVertices(endpointLink, n);
  const buckets = accumulateRemainingEdgeBuckets(availableVertices, endpointLink, edge, edgeSquared);
  const mean = computeConditionedMeanTourLength(n, endpointLink, state, edge, buckets);
  accumulateVarianceBuckets(availableVertices, endpointLink, edge, buckets);
  const variance = computeConditionedVariance(n, state, mean, buckets);
  const remainingFactor = n - 1 + state.closedChains - state.usedVertices;
  const entropy = lnGamma(remainingFactor + 1.0) + (Math.log(2.0) * (state.closedChains - 1));
  return entropy - (beta * mean) + ((beta * beta) * 0.5 * variance);
}

function findBestScoringEdge(n, edge, edgeSquared, endpointLink, state, beta) {
  let best = { score: -Infinity, from: 0, to: 0 };
  const candidateEdges = [];
  for (let i = 1; i <= n - 1; i++) {
    if (endpointLink[i] === -1) continue;
    for (let j = i + 1; j <= n; j++) {
      if (endpointLink[j] === -1 || endpointLink[i] === j || endpointLink[j] === i) continue;
      candidateEdges.push({ from: i, to: j });
    }
  }
  for (const candidate of candidateEdges) {
    const trialLinks = endpointLink.slice();
    const trialState = { ...state };
    if (!applyChosenEdge(candidate.from, candidate.to, edge, trialLinks, trialState)) continue;
    const trialScore = computeTheoryScore(n, edge, edgeSquared, trialLinks, trialState, beta);
    if (trialScore >= best.score) best = { score: trialScore, from: candidate.from, to: candidate.to };
  }
  return best;
}

function findForcedFinalEdge(n, endpointLink, edge) {
  for (let i = 1; i <= n; i++) {
    if (endpointLink[i] === -1) continue;
    for (let j = i + 1; j <= n; j++) {
      if (endpointLink[j] !== -1 && endpointLink[i] === j && endpointLink[j] === i) {
        return { exists: true, from: i, to: j, weight: edge[i][j] };
      }
    }
  }
  return { exists: false, from: 0, to: 0, weight: 0 };
}

function applyDegreeTwoForcedEdges(edge, n, endpointLink, state) {
  const tried = new Set();
  let forcedVertexCount = 0;
  let forcedEdgeCount = 0;

  for (let vertex = 1; vertex <= n; vertex++) {
    const neighbors = [];
    for (let neighbor = 1; neighbor <= n; neighbor++) {
      if (vertex !== neighbor && edge[vertex][neighbor] !== 0) neighbors.push(neighbor);
    }
    if (neighbors.length !== 2) continue;

    let appliedForVertex = false;
    for (const neighbor of neighbors) {
      const a = Math.min(vertex, neighbor);
      const b = Math.max(vertex, neighbor);
      const key = `${a}:${b}`;
      if (tried.has(key)) continue;
      tried.add(key);
      if (applyChosenEdge(vertex, neighbor, edge, endpointLink, state)) {
        forcedEdgeCount += 1;
        appliedForVertex = true;
      }
    }
    if (appliedForVertex) forcedVertexCount += 1;
  }

  return { forcedVertexCount, forcedEdgeCount, forcedEdgeTotal: state.chosenEdgeTotal };
}

function runTrackingSolver(edge, n, beta, sourceLabel, options = {}) {
  if (n < 2) throw new Error("Need at least 2 vertices.");
  const lines = [];
  const edgeSquared = buildSquaredEdgeMatrix(edge, n);
  const moments = computeTheoryMoments(edge, edgeSquared, n);
  append(lines, `Source: ${sourceLabel}`);
  append(lines, `n = ${n}`);
  append(lines, `the average  = ${formatNumber(moments.meanTourLength)}`);
  append(lines, `S_self ${formatNumber(moments.selfInteractionSum)} S_neighbor ${formatNumber(moments.neighborInteractionSum)} S_non_neighbor ${formatNumber(moments.disjointInteractionSum)}`);
  append(lines, `the standard deviation = ${formatNumber(moments.tourVariance)} ${formatNumber(Math.sqrt(Math.max(0, moments.tourVariance)))}`);
  append(lines, `suggested beta value = ${formatNumber(1.0 / Math.sqrt(Math.max(Number.MIN_VALUE, moments.tourVariance)))}`);
  const entropy = lnGamma(n) - Math.log(2.0);
  const partition = entropy - (beta * moments.meanTourLength) + ((beta * beta) * 0.5 * moments.tourVariance);
  append(lines, `entropy ${formatNumber(entropy)} partition ${formatNumber(partition)}`);

  const endpointLink = Array(n + 1).fill(0);
  const state = { closedChains: 0, usedVertices: 0, chosenEdgeTotal: 0 };
  if (options.forceDegreeTwo) {
    const forced = applyDegreeTwoForcedEdges(edge, n, endpointLink, state);
    append(lines, "Degree-2 forced-edge precheck:");
    append(lines, `vertices with exactly two edges applied = ${forced.forcedVertexCount}`);
    append(lines, `forced edges applied = ${forced.forcedEdgeCount}`);
    append(lines, `forced edge total = ${formatNumber(forced.forcedEdgeTotal)}`);
  }
  let guard = 0;
  while (n - state.usedVertices + state.closedChains - 1 > 0) {
    const best = findBestScoringEdge(n, edge, edgeSquared, endpointLink, state, beta);
    append(lines, `The biggetst probability is ${formatNumber(best.score)} at Edge[${best.from}][${best.to}].`);
    if (!applyChosenEdge(best.from, best.to, edge, endpointLink, state)) break;
    guard++;
    if (guard > n + 2) throw new Error("Solver guard stopped a loop that exceeded n steps.");
  }
  const forced = findForcedFinalEdge(n, endpointLink, edge);
  let totalTourCost = state.chosenEdgeTotal;
  if (forced.exists) {
    append(lines, `The biggetst probability is 100% at Edge[${forced.from}][${forced.to}].`);
    totalTourCost += forced.weight;
  }
  append(lines, `Total tour cost = ${formatNumber(totalTourCost)}`);
  return lines.join("\n");
}

function tokenizeNumbers(text) {
  return text.trim().split(/\s+/).filter(Boolean).map(Number);
}

function parsePairs(text) {
  const nums = tokenizeNumbers(text);
  if (nums.length % 2 !== 0) throw new Error("Pairs input needs an even count of numbers.");
  let n = 0;
  for (let i = 0; i < nums.length; i += 2) n = Math.max(n, nums[i], nums[i + 1]);
  const edge = makeMatrix(n);
  for (let i = 0; i < nums.length; i += 2) {
    const u = nums[i], v = nums[i + 1];
    edge[u][v] = -1;
    edge[v][u] = -1;
  }
  return { edge, n };
}

function parseMatrix(text) {
  const nums = tokenizeNumbers(text);
  const n = nums[0];
  if (!Number.isInteger(n) || n < 2) throw new Error("Matrix input first number must be n.");
  if (nums.length < 1 + n * n) throw new Error(`Matrix input needs ${n * n} weights after n.`);
  const edge = makeMatrix(n);
  let p = 1;
  for (let i = 1; i <= n; i++) {
    for (let j = 1; j <= n; j++) edge[i][j] = nums[p++];
  }
  return { edge, n };
}

function parseManual(text) {
  const nums = tokenizeNumbers(text);
  const n = nums[0];
  if (!Number.isInteger(n) || n < 2) throw new Error("Manual input first number must be n.");
  const expected = (n * (n - 1)) / 2;
  if (nums.length < 1 + expected) throw new Error(`Manual input needs ${expected} upper-triangle weights after n.`);
  const edge = makeMatrix(n);
  let p = 1;
  for (let i = 1; i <= n; i++) {
    for (let j = i + 1; j <= n; j++) {
      edge[i][j] = nums[p];
      edge[j][i] = nums[p];
      p++;
    }
  }
  return { edge, n };
}

function parsePoints(text) {
  const nums = tokenizeNumbers(text);
  const n = nums[0];
  if (!Number.isInteger(n) || n < 2) throw new Error("Points input first number must be n.");
  if (nums.length < 1 + n * 2) throw new Error(`Points input needs ${n} x y coordinate pairs.`);
  const points = [];
  let p = 1;
  for (let i = 0; i < n; i++) points.push({ x: nums[p++], y: nums[p++] });
  const edge = makeMatrix(n);
  for (let i = 1; i <= n; i++) {
    for (let j = i + 1; j <= n; j++) {
      const dx = points[i - 1].x - points[j - 1].x;
      const dy = points[i - 1].y - points[j - 1].y;
      edge[i][j] = Math.sqrt(dx * dx + dy * dy);
      edge[j][i] = edge[i][j];
    }
  }
  return { edge, n };
}

function literalIsTrue(literal, assignment) {
  const variable = Math.abs(literal);
  if (assignment[variable] === -1) return false;
  const value = assignment[variable] === 1;
  return literal > 0 ? value : !value;
}

function clauseIsSatisfied(clause, assignment) {
  return clause.some(literal => literalIsTrue(literal, assignment));
}

function formulaIsSatisfied(clauses, assignment) {
  return clauses.every(clause => clauseIsSatisfied(clause, assignment));
}

function partialFormulaCanStillBeSatisfied(clauses, assignment) {
  for (const clause of clauses) {
    let satisfied = false;
    let hasUnassigned = false;
    for (const literal of clause) {
      const variable = Math.abs(literal);
      if (assignment[variable] === -1) {
        hasUnassigned = true;
      } else if (literalIsTrue(literal, assignment)) {
        satisfied = true;
      }
    }
    if (!satisfied && !hasUnassigned) return false;
  }
  return true;
}

function findSatisfyingAssignment(variableCount, clauses, assignment) {
  if (!partialFormulaCanStillBeSatisfied(clauses, assignment)) return false;
  let variable = 0;
  for (let i = 1; i <= variableCount; i++) {
    if (assignment[i] === -1) {
      variable = i;
      break;
    }
  }
  if (variable === 0) return formulaIsSatisfied(clauses, assignment);
  assignment[variable] = 1;
  if (findSatisfyingAssignment(variableCount, clauses, assignment)) return true;
  assignment[variable] = 0;
  if (findSatisfyingAssignment(variableCount, clauses, assignment)) return true;
  assignment[variable] = -1;
  return false;
}

function parse3Sat(text) {
  const lines = text.split(/\r?\n/).map(line => line.trim()).filter(line => line && !line.startsWith("#"));
  if (!lines.length) throw new Error("3-SAT input is empty.");
  const first = lines[0].split(/\s+/).map(Number);
  const variableCount = first[0], clauseCount = first[1], padding = first[2] || 0;
  if (!Number.isInteger(variableCount) || variableCount <= 0) throw new Error("First line needs a positive variable count.");
  if (!Number.isInteger(clauseCount) || clauseCount <= 0) throw new Error("First line needs a positive clause count.");
  if (padding < 0) throw new Error("Padding cannot be negative.");
  const clauses = [];
  for (let i = 0; i < clauseCount; i++) {
    const parts = (lines[i + 1] || "").split(/\s+/).filter(Boolean).map(Number);
    if (parts.length !== 3) throw new Error(`Clause ${i + 1} must have exactly 3 literals.`);
    for (const literal of parts) {
      if (literal === 0 || Math.abs(literal) > variableCount) throw new Error(`Invalid literal ${literal}.`);
    }
    clauses.push(parts);
  }
  return { variableCount, clauseCount, padding, clauses };
}

function parseVertexCover(text) {
  const nums = tokenizeNumbers(text);
  if (nums.length < 2) throw new Error("Vertex cover input first line needs: vertices k optional_padding.");
  const n = nums[0];
  const k = nums[1];
  const padding = nums.length % 2 === 1 ? nums[2] : 0;
  const edgeStart = nums.length % 2 === 1 ? 3 : 2;
  if (!Number.isInteger(n) || n <= 0) throw new Error("Vertex count must be a positive integer.");
  if (!Number.isInteger(k) || k < 0) throw new Error("k must be a nonnegative integer.");
  if (!Number.isInteger(padding) || padding < 0) throw new Error("Optional padding must be a nonnegative integer.");
  if ((nums.length - edgeStart) % 2 !== 0) throw new Error("Vertex cover edges must be pairs of vertices.");

  const edges = [];
  for (let i = edgeStart; i < nums.length; i += 2) {
    const u = nums[i], v = nums[i + 1];
    if (!Number.isInteger(u) || !Number.isInteger(v) || u < 1 || v < 1 || u > n || v > n || u === v) {
      throw new Error(`Invalid edge ${u} ${v}.`);
    }
    edges.push([u, v]);
  }
  return { n, k, padding, edges };
}

function parseClique(text) {
  const nums = tokenizeNumbers(text);
  if (nums.length < 2) throw new Error("Clique input first line needs: vertices k optional_padding.");
  const n = nums[0];
  const k = nums[1];
  const padding = nums.length % 2 === 1 ? nums[2] : 0;
  const edgeStart = nums.length % 2 === 1 ? 3 : 2;
  if (!Number.isInteger(n) || n <= 0) throw new Error("Vertex count must be a positive integer.");
  if (!Number.isInteger(k) || k < 0) throw new Error("k must be a nonnegative integer.");
  if (!Number.isInteger(padding) || padding < 0) throw new Error("Optional padding must be a nonnegative integer.");
  if ((nums.length - edgeStart) % 2 !== 0) throw new Error("Clique edges must be pairs of vertices.");

  const seen = new Set();
  const edges = [];
  for (let i = edgeStart; i < nums.length; i += 2) {
    let u = nums[i], v = nums[i + 1];
    if (!Number.isInteger(u) || !Number.isInteger(v) || u < 1 || v < 1 || u > n || v > n || u === v) {
      throw new Error(`Invalid edge ${u} ${v}.`);
    }
    if (u > v) [u, v] = [v, u];
    const key = `${u}:${v}`;
    if (seen.has(key)) continue;
    seen.add(key);
    edges.push([u, v]);
  }
  return { n, k, padding, edges };
}

function parseIndependentSet(text) {
  const nums = tokenizeNumbers(text);
  if (nums.length < 2) throw new Error("Independent set input first line needs: vertices k optional_padding.");
  const n = nums[0];
  const k = nums[1];
  const padding = nums.length % 2 === 1 ? nums[2] : 0;
  const edgeStart = nums.length % 2 === 1 ? 3 : 2;
  if (!Number.isInteger(n) || n <= 0) throw new Error("Vertex count must be a positive integer.");
  if (!Number.isInteger(k) || k < 0) throw new Error("k must be a nonnegative integer.");
  if (!Number.isInteger(padding) || padding < 0) throw new Error("Optional padding must be a nonnegative integer.");
  if ((nums.length - edgeStart) % 2 !== 0) throw new Error("Independent set edges must be pairs of vertices.");

  const seen = new Set();
  const edges = [];
  for (let i = edgeStart; i < nums.length; i += 2) {
    let u = nums[i], v = nums[i + 1];
    if (!Number.isInteger(u) || !Number.isInteger(v) || u < 1 || v < 1 || u > n || v > n || u === v) {
      throw new Error(`Invalid edge ${u} ${v}.`);
    }
    if (u > v) [u, v] = [v, u];
    const key = `${u}:${v}`;
    if (seen.has(key)) continue;
    seen.add(key);
    edges.push([u, v]);
  }
  return { n, k, padding, edges };
}

function parseSetCover(text) {
  const lines = text.split(/\r?\n/).map(line => line.trim()).filter(line => line && !line.startsWith("#"));
  if (!lines.length) throw new Error("Set cover input is empty.");
  const first = lines[0].split(/\s+/).map(Number);
  if (first.length < 3 || first.length > 4) {
    throw new Error("Set cover first line needs: universe_size set_count k optional_padding.");
  }
  const universeSize = first[0];
  const setCount = first[1];
  const k = first[2];
  const padding = first[3] || 0;
  if (!Number.isInteger(universeSize) || universeSize <= 0) throw new Error("Universe size must be a positive integer.");
  if (!Number.isInteger(setCount) || setCount <= 0) throw new Error("Set count must be a positive integer.");
  if (!Number.isInteger(k) || k < 0) throw new Error("k must be a nonnegative integer.");
  if (!Number.isInteger(padding) || padding < 0) throw new Error("Optional padding must be a nonnegative integer.");
  if (lines.length - 1 < setCount) throw new Error(`Expected ${setCount} set lines.`);

  const sets = [];
  for (let setIndex = 0; setIndex < setCount; setIndex++) {
    const values = lines[setIndex + 1].split(/\s+/).filter(Boolean).map(Number);
    if (!values.length) throw new Error(`Set ${setIndex + 1} must contain at least one element.`);
    const seen = new Set();
    const elements = [];
    for (const element of values) {
      if (!Number.isInteger(element) || element < 1 || element > universeSize) {
        throw new Error(`Invalid element ${element} in set ${setIndex + 1}.`);
      }
      if (seen.has(element)) continue;
      seen.add(element);
      elements.push(element);
    }
    sets.push(elements);
  }
  return { universeSize, setCount, k, padding, sets };
}

function parseX3c(text) {
  const lines = text.split(/\r?\n/).map(line => line.trim()).filter(line => line && !line.startsWith("#"));
  if (!lines.length) throw new Error("X3C input is empty.");
  const first = lines[0].split(/\s+/).map(Number);
  if (first.length < 2 || first.length > 3) {
    throw new Error("X3C first line needs: universe_size set_count optional_padding.");
  }
  const universeSize = first[0];
  const setCount = first[1];
  const padding = first[2] || 0;
  if (!Number.isInteger(universeSize) || universeSize <= 0) throw new Error("Universe size must be a positive integer.");
  if (!Number.isInteger(setCount) || setCount <= 0) throw new Error("Set count must be a positive integer.");
  if (!Number.isInteger(padding) || padding < 0) throw new Error("Optional padding must be a nonnegative integer.");
  if (lines.length - 1 < setCount) throw new Error(`Expected ${setCount} 3-set lines.`);

  const sets = [];
  for (let setIndex = 0; setIndex < setCount; setIndex++) {
    const values = lines[setIndex + 1].split(/\s+/).filter(Boolean).map(Number);
    if (values.length !== 3) throw new Error(`Set ${setIndex + 1} must have exactly three elements.`);
    const seen = new Set();
    const elements = [];
    for (const element of values) {
      if (!Number.isInteger(element) || element < 1 || element > universeSize) {
        throw new Error(`Invalid element ${element} in set ${setIndex + 1}.`);
      }
      if (seen.has(element)) throw new Error(`Set ${setIndex + 1} must have three distinct elements.`);
      seen.add(element);
      elements.push(element);
    }
    sets.push(elements);
  }
  return { universeSize, setCount, padding, sets };
}

function parseGraphColoring(text) {
  const lines = text.split(/\r?\n/).map(line => line.trim()).filter(line => line && !line.startsWith("#"));
  if (!lines.length) throw new Error("Graph coloring input is empty.");
  const first = lines[0].split(/\s+/).map(Number);
  if (first.length < 3 || first.length > 4) {
    throw new Error("Graph coloring first line needs: vertices edges colors optional_padding.");
  }
  const n = first[0];
  const edgeCount = first[1];
  const colorCount = first[2];
  const padding = first[3] || 0;
  if (!Number.isInteger(n) || n <= 0) throw new Error("Vertex count must be a positive integer.");
  if (!Number.isInteger(edgeCount) || edgeCount < 0) throw new Error("Edge count must be a nonnegative integer.");
  if (!Number.isInteger(colorCount) || colorCount <= 0) throw new Error("Color count must be a positive integer.");
  if (!Number.isInteger(padding) || padding < 0) throw new Error("Optional padding must be a nonnegative integer.");
  if (lines.length - 1 < edgeCount) throw new Error(`Expected ${edgeCount} edge lines.`);

  const seen = new Set();
  const edges = [];
  for (let index = 0; index < edgeCount; index++) {
    const parts = lines[index + 1].split(/\s+/).filter(Boolean).map(Number);
    if (parts.length !== 2) throw new Error(`Edge ${index + 1} must have exactly two vertices.`);
    let u = parts[0], v = parts[1];
    if (!Number.isInteger(u) || !Number.isInteger(v) || u < 1 || v < 1 || u > n || v > n || u === v) {
      throw new Error(`Invalid edge ${u} ${v}.`);
    }
    if (u > v) [u, v] = [v, u];
    const key = `${u}:${v}`;
    if (seen.has(key)) continue;
    seen.add(key);
    edges.push([u, v]);
  }
  return { n, declaredEdgeCount: edgeCount, colorCount, padding, edges };
}

function findVertexCover(n, k, edges) {
  const chosen = Array(n + 1).fill(false);
  let best = null;

  function uncoveredEdge() {
    for (const [u, v] of edges) {
      if (!chosen[u] && !chosen[v]) return [u, v];
    }
    return null;
  }

  function chosenCount() {
    let count = 0;
    for (let i = 1; i <= n; i++) if (chosen[i]) count++;
    return count;
  }

  function search(count) {
    if (count > k) return false;
    const edge = uncoveredEdge();
    if (!edge) {
      best = [];
      for (let i = 1; i <= n; i++) if (chosen[i]) best.push(i);
      return true;
    }

    const [u, v] = edge;
    chosen[u] = true;
    if (search(count + 1)) return true;
    chosen[u] = false;

    chosen[v] = true;
    if (search(count + 1)) return true;
    chosen[v] = false;
    return false;
  }

  return search(chosenCount()) ? best : null;
}

function buildAdjacencyMatrix(n, edges) {
  const adjacency = Array.from({ length: n + 1 }, () => Array(n + 1).fill(false));
  for (const [u, v] of edges) {
    adjacency[u][v] = true;
    adjacency[v][u] = true;
  }
  return adjacency;
}

function buildComplementEdges(n, edges) {
  const adjacency = buildAdjacencyMatrix(n, edges);
  const complement = [];
  for (let u = 1; u <= n; u++) {
    for (let v = u + 1; v <= n; v++) {
      if (!adjacency[u][v]) complement.push([u, v]);
    }
  }
  return complement;
}

function findClique(n, k, edges) {
  if (k === 0) return [];
  if (k > n) return null;
  const adjacency = buildAdjacencyMatrix(n, edges);

  function search(clique, candidates) {
    if (clique.length === k) return clique.slice();
    while (candidates.length > 0) {
      if (clique.length + candidates.length < k) return null;
      const vertex = candidates.shift();
      const nextCandidates = candidates.filter(candidate => adjacency[vertex][candidate]);
      const found = search([...clique, vertex], nextCandidates);
      if (found) return found;
    }
    return null;
  }

  const candidates = [];
  for (let vertex = 1; vertex <= n; vertex++) candidates.push(vertex);
  return search([], candidates);
}

function findSetCover(universeSize, k, sets) {
  const covered = Array(universeSize + 1).fill(false);
  const chosen = [];

  function firstUncoveredElement() {
    for (let element = 1; element <= universeSize; element++) {
      if (!covered[element]) return element;
    }
    return 0;
  }

  function search() {
    const element = firstUncoveredElement();
    if (element === 0) return chosen.slice();
    if (chosen.length >= k) return null;

    for (let setIndex = 0; setIndex < sets.length; setIndex++) {
      if (!sets[setIndex].includes(element) || chosen.includes(setIndex + 1)) continue;
      const newlyCovered = [];
      for (const value of sets[setIndex]) {
        if (!covered[value]) {
          covered[value] = true;
          newlyCovered.push(value);
        }
      }
      chosen.push(setIndex + 1);
      const found = search();
      if (found) return found;
      chosen.pop();
      for (const value of newlyCovered) covered[value] = false;
    }
    return null;
  }

  return search();
}

function findX3c(universeSize, sets) {
  if (universeSize % 3 !== 0) return null;
  const targetSetCount = universeSize / 3;
  const covered = Array(universeSize + 1).fill(false);
  const chosen = [];

  function firstUncoveredElement() {
    for (let element = 1; element <= universeSize; element++) {
      if (!covered[element]) return element;
    }
    return 0;
  }

  function search() {
    const element = firstUncoveredElement();
    if (element === 0) return chosen.length === targetSetCount ? chosen.slice() : null;
    if (chosen.length >= targetSetCount) return null;

    for (let setIndex = 0; setIndex < sets.length; setIndex++) {
      if (!sets[setIndex].includes(element) || chosen.includes(setIndex + 1)) continue;
      let canUse = true;
      for (const value of sets[setIndex]) {
        if (covered[value]) {
          canUse = false;
          break;
        }
      }
      if (!canUse) continue;

      for (const value of sets[setIndex]) covered[value] = true;
      chosen.push(setIndex + 1);
      const found = search();
      if (found) return found;
      chosen.pop();
      for (const value of sets[setIndex]) covered[value] = false;
    }
    return null;
  }

  return search();
}

function findGraphColoring(n, colorCount, edges) {
  const adjacency = Array.from({ length: n + 1 }, () => []);
  for (const [u, v] of edges) {
    adjacency[u].push(v);
    adjacency[v].push(u);
  }
  const order = Array.from({ length: n }, (_, index) => index + 1)
    .sort((a, b) => adjacency[b].length - adjacency[a].length);
  const color = Array(n + 1).fill(0);

  function canUse(vertex, candidateColor) {
    for (const neighbor of adjacency[vertex]) {
      if (color[neighbor] === candidateColor) return false;
    }
    return true;
  }

  function search(position) {
    if (position === order.length) return true;
    const vertex = order[position];
    for (let candidateColor = 1; candidateColor <= colorCount; candidateColor++) {
      if (!canUse(vertex, candidateColor)) continue;
      color[vertex] = candidateColor;
      if (search(position + 1)) return true;
      color[vertex] = 0;
    }
    return false;
  }

  return search(0) ? color : null;
}

function vertexCoverTo3Sat(n, k, edges) {
  let variableCount = n;
  const clauses = [];
  let rawClauseCount = 0;

  function addClauseAs3Sat(literals) {
    rawClauseCount += 1;
    if (literals.length === 1) {
      clauses.push([literals[0], literals[0], literals[0]]);
      return;
    }
    if (literals.length === 2) {
      clauses.push([literals[0], literals[1], literals[1]]);
      return;
    }
    if (literals.length === 3) {
      clauses.push([literals[0], literals[1], literals[2]]);
      return;
    }

    let previousAux = ++variableCount;
    clauses.push([literals[0], literals[1], previousAux]);
    for (let index = 2; index < literals.length - 2; index++) {
      const nextAux = ++variableCount;
      clauses.push([-previousAux, literals[index], nextAux]);
      previousAux = nextAux;
    }
    clauses.push([-previousAux, literals[literals.length - 2], literals[literals.length - 1]]);
  }

  for (const [u, v] of edges) {
    addClauseAs3Sat([u, v]);
  }

  if (k === 0) {
    for (let vertex = 1; vertex <= n; vertex++) {
      addClauseAs3Sat([-vertex]);
    }
  } else if (k < n) {
    const counter = Array.from({ length: n + 1 }, () => Array(k + 1).fill(0));
    for (let i = 1; i <= n; i++) {
      for (let j = 1; j <= k; j++) {
        counter[i][j] = ++variableCount;
      }
    }

    addClauseAs3Sat([-1, counter[1][1]]);
    for (let j = 2; j <= k; j++) {
      addClauseAs3Sat([-counter[1][j]]);
    }

    for (let i = 2; i <= n; i++) {
      addClauseAs3Sat([-i, counter[i][1]]);
      addClauseAs3Sat([-counter[i - 1][1], counter[i][1]]);

      for (let j = 2; j <= k; j++) {
        addClauseAs3Sat([-counter[i - 1][j], counter[i][j]]);
        addClauseAs3Sat([-i, -counter[i - 1][j - 1], counter[i][j]]);
      }

      addClauseAs3Sat([-i, -counter[i - 1][k]]);
    }
  }

  return { variableCount, clauses, rawClauseCount, encoding: "sequential counter at-most-k" };
}

function setCoverTo3Sat(universeSize, setCount, k, sets) {
  let variableCount = setCount;
  const clauses = [];
  let rawClauseCount = 0;

  function addClauseAs3Sat(literals) {
    rawClauseCount += 1;
    if (literals.length === 1) {
      clauses.push([literals[0], literals[0], literals[0]]);
      return;
    }
    if (literals.length === 2) {
      clauses.push([literals[0], literals[1], literals[1]]);
      return;
    }
    if (literals.length === 3) {
      clauses.push([literals[0], literals[1], literals[2]]);
      return;
    }

    let previousAux = ++variableCount;
    clauses.push([literals[0], literals[1], previousAux]);
    for (let index = 2; index < literals.length - 2; index++) {
      const nextAux = ++variableCount;
      clauses.push([-previousAux, literals[index], nextAux]);
      previousAux = nextAux;
    }
    clauses.push([-previousAux, literals[literals.length - 2], literals[literals.length - 1]]);
  }

  for (let element = 1; element <= universeSize; element++) {
    const coveringSets = [];
    for (let setIndex = 0; setIndex < setCount; setIndex++) {
      if (sets[setIndex].includes(element)) coveringSets.push(setIndex + 1);
    }
    if (coveringSets.length === 0) {
      const impossible = ++variableCount;
      addClauseAs3Sat([impossible]);
      addClauseAs3Sat([-impossible]);
    } else {
      addClauseAs3Sat(coveringSets);
    }
  }

  if (k === 0) {
    for (let setIndex = 1; setIndex <= setCount; setIndex++) {
      addClauseAs3Sat([-setIndex]);
    }
  } else if (k < setCount) {
    const counter = Array.from({ length: setCount + 1 }, () => Array(k + 1).fill(0));
    for (let i = 1; i <= setCount; i++) {
      for (let j = 1; j <= k; j++) {
        counter[i][j] = ++variableCount;
      }
    }

    addClauseAs3Sat([-1, counter[1][1]]);
    for (let j = 2; j <= k; j++) {
      addClauseAs3Sat([-counter[1][j]]);
    }

    for (let i = 2; i <= setCount; i++) {
      addClauseAs3Sat([-i, counter[i][1]]);
      addClauseAs3Sat([-counter[i - 1][1], counter[i][1]]);

      for (let j = 2; j <= k; j++) {
        addClauseAs3Sat([-counter[i - 1][j], counter[i][j]]);
        addClauseAs3Sat([-i, -counter[i - 1][j - 1], counter[i][j]]);
      }

      addClauseAs3Sat([-i, -counter[i - 1][k]]);
    }
  }

  return { variableCount, clauses, rawClauseCount, encoding: "coverage clauses plus sequential counter at-most-k" };
}

function x3cTo3Sat(universeSize, setCount, sets) {
  let variableCount = setCount;
  const clauses = [];
  let rawClauseCount = 0;

  function addClauseAs3Sat(literals) {
    rawClauseCount += 1;
    if (literals.length === 1) {
      clauses.push([literals[0], literals[0], literals[0]]);
      return;
    }
    if (literals.length === 2) {
      clauses.push([literals[0], literals[1], literals[1]]);
      return;
    }
    if (literals.length === 3) {
      clauses.push([literals[0], literals[1], literals[2]]);
      return;
    }

    let previousAux = ++variableCount;
    clauses.push([literals[0], literals[1], previousAux]);
    for (let index = 2; index < literals.length - 2; index++) {
      const nextAux = ++variableCount;
      clauses.push([-previousAux, literals[index], nextAux]);
      previousAux = nextAux;
    }
    clauses.push([-previousAux, literals[literals.length - 2], literals[literals.length - 1]]);
  }

  for (let element = 1; element <= universeSize; element++) {
    const coveringSets = [];
    for (let setIndex = 0; setIndex < setCount; setIndex++) {
      if (sets[setIndex].includes(element)) coveringSets.push(setIndex + 1);
    }

    if (coveringSets.length === 0) {
      const impossible = ++variableCount;
      addClauseAs3Sat([impossible]);
      addClauseAs3Sat([-impossible]);
      continue;
    }

    addClauseAs3Sat(coveringSets);
    for (let left = 0; left < coveringSets.length; left++) {
      for (let right = left + 1; right < coveringSets.length; right++) {
        addClauseAs3Sat([-coveringSets[left], -coveringSets[right]]);
      }
    }
  }

  if (universeSize % 3 !== 0) {
    const impossible = ++variableCount;
    addClauseAs3Sat([impossible]);
    addClauseAs3Sat([-impossible]);
  }

  return { variableCount, clauses, rawClauseCount, encoding: "exactly-once coverage clauses" };
}

function graphColoringTo3Sat(n, colorCount, edges) {
  let variableCount = colorCount * n;
  const clauses = [];
  let rawClauseCount = 0;
  const colorVar = (vertex, color) => colorCount * (vertex - 1) + color;

  function addClauseAs3Sat(literals) {
    rawClauseCount += 1;
    if (literals.length === 1) {
      clauses.push([literals[0], literals[0], literals[0]]);
      return;
    }
    if (literals.length === 2) {
      clauses.push([literals[0], literals[1], literals[1]]);
      return;
    }
    if (literals.length === 3) {
      clauses.push(literals);
      return;
    }

    let previousAux = ++variableCount;
    clauses.push([literals[0], literals[1], previousAux]);
    for (let index = 2; index < literals.length - 2; index++) {
      const nextAux = ++variableCount;
      clauses.push([-previousAux, literals[index], nextAux]);
      previousAux = nextAux;
    }
    clauses.push([-previousAux, literals[literals.length - 2], literals[literals.length - 1]]);
  }

  for (let vertex = 1; vertex <= n; vertex++) {
    const colorChoices = [];
    for (let color = 1; color <= colorCount; color++) {
      colorChoices.push(colorVar(vertex, color));
    }
    addClauseAs3Sat(colorChoices);
    for (let left = 1; left <= colorCount; left++) {
      for (let right = left + 1; right <= colorCount; right++) {
        addClauseAs3Sat([-colorVar(vertex, left), -colorVar(vertex, right)]);
      }
    }
  }

  for (const [u, v] of edges) {
    for (let color = 1; color <= colorCount; color++) {
      addClauseAs3Sat([-colorVar(u, color), -colorVar(v, color)]);
    }
  }

  return { variableCount, clauses, rawClauseCount, encoding: "one-of-k vertex colors plus edge color conflicts" };
}

function runVertexCover(text) {
  const { n, k, padding, edges } = parseVertexCover(text);
  const cover = findVertexCover(n, k, edges);
  const sat = vertexCoverTo3Sat(n, k, edges);
  const graph = buildCompressedReductionGraph(sat.variableCount, sat.clauses, padding);
  const forced = findDegreeTwoForcedEdges(graph.edge, graph.n);
  const edgeSquared = buildSquaredEdgeMatrix(graph.edge, graph.n);
  const moments = computeTheoryMoments(graph.edge, edgeSquared, graph.n);

  const lines = [];
  append(lines, "Vertex Cover instance:");
  append(lines, `vertices = ${n}`);
  append(lines, `edges = ${edges.length}`);
  append(lines, `k = ${k}`);
  append(lines, `optional padding directed nodes = ${padding}`);
  append(lines, `edge list = ${edges.map(([u, v]) => `(${u},${v})`).join(" ")}`);
  append(lines);
  append(lines, "3-SAT encoding:");
  append(lines, `cardinality encoding = ${sat.encoding}`);
  append(lines, `CNF clauses before 3-literal normalization = ${sat.rawClauseCount}`);
  append(lines, `variables including auxiliary = ${sat.variableCount}`);
  append(lines, `3-SAT clauses = ${sat.clauses.length}`);
  append(lines);
  append(lines, "Compressed HC reduction size:");
  append(lines, `base directed nodes = ${graph.baseDirected}`);
  append(lines, `total directed nodes = ${graph.directedCount}`);
  append(lines, `undirected HC nodes = ${graph.n}`);
  append(lines);
  append(lines, "Degree-2 forced-edge precheck:");
  append(lines, `vertices with exactly two HC edges = ${forced.forcedVertexCount}`);
  append(lines, `forced HC edges = ${forced.forcedEdgeCount}`);
  append(lines, `forced edge total = ${formatNumber(forced.forcedEdgeTotal)}`);
  append(lines);
  append(lines, "Hamiltonian calculation summary:");
  append(lines, `the average  = ${formatNumber(moments.meanTourLength)}`);
  append(lines, `S_self ${formatNumber(moments.selfInteractionSum)} S_neighbor ${formatNumber(moments.neighborInteractionSum)} S_non_neighbor ${formatNumber(moments.disjointInteractionSum)}`);
  append(lines, `the standard deviation = ${formatNumber(moments.tourVariance)} ${formatNumber(Math.sqrt(Math.max(0, moments.tourVariance)))}`);
  append(lines);
  if (cover) {
    append(lines, "Vertex Cover answer: YES");
    append(lines, `cover of size ${cover.length}: { ${cover.join(", ")} }`);
  } else {
    append(lines, "Vertex Cover answer: NO");
    append(lines, `no vertex cover of size <= ${k} was found`);
  }
  return lines.join("\n");
}

function runClique(text) {
  const { n, k, padding, edges } = parseClique(text);
  const clique = findClique(n, k, edges);
  const complementEdges = buildComplementEdges(n, edges);
  const vertexCoverK = n - k;
  const cover = vertexCoverK >= 0 ? findVertexCover(n, vertexCoverK, complementEdges) : null;
  const sat = vertexCoverK >= 0 ? vertexCoverTo3Sat(n, vertexCoverK, complementEdges) : null;
  const graph = sat ? buildCompressedReductionGraph(sat.variableCount, sat.clauses, padding) : null;
  const forced = graph ? findDegreeTwoForcedEdges(graph.edge, graph.n) : null;
  const edgeSquared = graph ? buildSquaredEdgeMatrix(graph.edge, graph.n) : null;
  const moments = graph ? computeTheoryMoments(graph.edge, edgeSquared, graph.n) : null;

  const lines = [];
  append(lines, "Clique instance:");
  append(lines, `vertices = ${n}`);
  append(lines, `edges = ${edges.length}`);
  append(lines, `k = ${k}`);
  append(lines, `optional padding directed nodes = ${padding}`);
  append(lines, `edge list = ${edges.map(([u, v]) => `(${u},${v})`).join(" ") || "(none)"}`);
  append(lines);
  append(lines, "Reduction used:");
  append(lines, "Clique(G, k) -> Vertex Cover(complement(G), vertices - k) -> 3-SAT -> compressed Hamiltonian Cycle");
  append(lines, `complement graph edges = ${complementEdges.length}`);
  append(lines, `vertex cover target on complement = ${vertexCoverK}`);

  if (!sat || !graph || !forced || !moments) {
    append(lines);
    append(lines, "Clique answer: NO");
    append(lines, `k = ${k} is larger than the vertex count ${n}`);
    return lines.join("\n");
  }

  append(lines);
  append(lines, "3-SAT encoding:");
  append(lines, `cardinality encoding = ${sat.encoding}`);
  append(lines, `CNF clauses before 3-literal normalization = ${sat.rawClauseCount}`);
  append(lines, `variables including auxiliary = ${sat.variableCount}`);
  append(lines, `3-SAT clauses = ${sat.clauses.length}`);
  append(lines);
  append(lines, "Compressed HC reduction size:");
  append(lines, `base directed nodes = ${graph.baseDirected}`);
  append(lines, `total directed nodes = ${graph.directedCount}`);
  append(lines, `undirected HC nodes = ${graph.n}`);
  append(lines);
  append(lines, "Degree-2 forced-edge precheck:");
  append(lines, `vertices with exactly two HC edges = ${forced.forcedVertexCount}`);
  append(lines, `forced HC edges = ${forced.forcedEdgeCount}`);
  append(lines, `forced edge total = ${formatNumber(forced.forcedEdgeTotal)}`);
  append(lines);
  append(lines, "Hamiltonian calculation summary:");
  append(lines, `the average  = ${formatNumber(moments.meanTourLength)}`);
  append(lines, `S_self ${formatNumber(moments.selfInteractionSum)} S_neighbor ${formatNumber(moments.neighborInteractionSum)} S_non_neighbor ${formatNumber(moments.disjointInteractionSum)}`);
  append(lines, `the standard deviation = ${formatNumber(moments.tourVariance)} ${formatNumber(Math.sqrt(Math.max(0, moments.tourVariance)))}`);
  append(lines);
  if (clique) {
    append(lines, "Clique answer: YES");
    append(lines, `clique of size ${clique.length}: { ${clique.join(", ")} }`);
    if (cover) append(lines, `equivalent complement vertex cover size ${cover.length}: { ${cover.join(", ")} }`);
  } else {
    append(lines, "Clique answer: NO");
    append(lines, `no clique of size ${k} was found`);
  }
  return lines.join("\n");
}

function runIndependentSet(text) {
  const { n, k, padding, edges } = parseIndependentSet(text);
  const vertexCoverK = n - k;
  const cover = vertexCoverK >= 0 ? findVertexCover(n, vertexCoverK, edges) : null;
  const sat = vertexCoverK >= 0 ? vertexCoverTo3Sat(n, vertexCoverK, edges) : null;
  const graph = sat ? buildCompressedReductionGraph(sat.variableCount, sat.clauses, padding) : null;
  const forced = graph ? findDegreeTwoForcedEdges(graph.edge, graph.n) : null;
  const edgeSquared = graph ? buildSquaredEdgeMatrix(graph.edge, graph.n) : null;
  const moments = graph ? computeTheoryMoments(graph.edge, edgeSquared, graph.n) : null;
  const independentSet = cover ? Array.from({ length: n }, (_, index) => index + 1).filter(vertex => !cover.includes(vertex)) : null;

  const lines = [];
  append(lines, "Independent Set instance:");
  append(lines, `vertices = ${n}`);
  append(lines, `edges = ${edges.length}`);
  append(lines, `k = ${k}`);
  append(lines, `optional padding directed nodes = ${padding}`);
  append(lines, `edge list = ${edges.map(([u, v]) => `(${u},${v})`).join(" ") || "(none)"}`);
  append(lines);
  append(lines, "Reduction used:");
  append(lines, "Independent Set(G, k) -> Vertex Cover(G, vertices - k) -> 3-SAT -> compressed Hamiltonian Cycle");
  append(lines, `vertex cover target = ${vertexCoverK}`);

  if (!sat || !graph || !forced || !moments) {
    append(lines);
    append(lines, "Independent Set answer: NO");
    append(lines, `k = ${k} is larger than the vertex count ${n}`);
    return lines.join("\n");
  }

  append(lines);
  append(lines, "3-SAT encoding:");
  append(lines, `cardinality encoding = ${sat.encoding}`);
  append(lines, `CNF clauses before 3-literal normalization = ${sat.rawClauseCount}`);
  append(lines, `variables including auxiliary = ${sat.variableCount}`);
  append(lines, `3-SAT clauses = ${sat.clauses.length}`);
  append(lines);
  append(lines, "Compressed HC reduction size:");
  append(lines, `base directed nodes = ${graph.baseDirected}`);
  append(lines, `total directed nodes = ${graph.directedCount}`);
  append(lines, `undirected HC nodes = ${graph.n}`);
  append(lines);
  append(lines, "Degree-2 forced-edge precheck:");
  append(lines, `vertices with exactly two HC edges = ${forced.forcedVertexCount}`);
  append(lines, `forced HC edges = ${forced.forcedEdgeCount}`);
  append(lines, `forced edge total = ${formatNumber(forced.forcedEdgeTotal)}`);
  append(lines);
  append(lines, "Hamiltonian calculation summary:");
  append(lines, `the average  = ${formatNumber(moments.meanTourLength)}`);
  append(lines, `S_self ${formatNumber(moments.selfInteractionSum)} S_neighbor ${formatNumber(moments.neighborInteractionSum)} S_non_neighbor ${formatNumber(moments.disjointInteractionSum)}`);
  append(lines, `the standard deviation = ${formatNumber(moments.tourVariance)} ${formatNumber(Math.sqrt(Math.max(0, moments.tourVariance)))}`);
  append(lines);
  if (independentSet && independentSet.length >= k) {
    append(lines, "Independent Set answer: YES");
    append(lines, `independent set of size ${independentSet.length}: { ${independentSet.join(", ")} }`);
    append(lines, `equivalent vertex cover size ${cover.length}: { ${cover.join(", ")} }`);
  } else {
    append(lines, "Independent Set answer: NO");
    append(lines, `no independent set of size ${k} was found`);
  }
  return lines.join("\n");
}

function runSetCover(text) {
  const { universeSize, setCount, k, padding, sets } = parseSetCover(text);
  const cover = findSetCover(universeSize, k, sets);
  const sat = setCoverTo3Sat(universeSize, setCount, k, sets);
  const graph = buildCompressedReductionGraph(sat.variableCount, sat.clauses, padding);
  const forced = findDegreeTwoForcedEdges(graph.edge, graph.n);
  const edgeSquared = buildSquaredEdgeMatrix(graph.edge, graph.n);
  const moments = computeTheoryMoments(graph.edge, edgeSquared, graph.n);

  const lines = [];
  append(lines, "Set Cover instance:");
  append(lines, `universe elements = ${universeSize}`);
  append(lines, `sets = ${setCount}`);
  append(lines, `k = ${k}`);
  append(lines, `optional padding directed nodes = ${padding}`);
  append(lines, "sets:");
  sets.forEach((set, index) => append(lines, `S${index + 1} = { ${set.join(", ")} }`));
  append(lines);
  append(lines, "Reduction used:");
  append(lines, "Set Cover -> 3-SAT coverage clauses plus at-most-k -> compressed Hamiltonian Cycle");
  append(lines, `one Boolean variable per set before auxiliary variables = ${setCount}`);
  append(lines);
  append(lines, "3-SAT encoding:");
  append(lines, `cardinality encoding = ${sat.encoding}`);
  append(lines, `CNF clauses before 3-literal normalization = ${sat.rawClauseCount}`);
  append(lines, `variables including auxiliary = ${sat.variableCount}`);
  append(lines, `3-SAT clauses = ${sat.clauses.length}`);
  append(lines);
  append(lines, "Compressed HC reduction size:");
  append(lines, `base directed nodes = ${graph.baseDirected}`);
  append(lines, `total directed nodes = ${graph.directedCount}`);
  append(lines, `undirected HC nodes = ${graph.n}`);
  append(lines);
  append(lines, "Degree-2 forced-edge precheck:");
  append(lines, `vertices with exactly two HC edges = ${forced.forcedVertexCount}`);
  append(lines, `forced HC edges = ${forced.forcedEdgeCount}`);
  append(lines, `forced edge total = ${formatNumber(forced.forcedEdgeTotal)}`);
  append(lines);
  append(lines, "Hamiltonian calculation summary:");
  append(lines, `the average  = ${formatNumber(moments.meanTourLength)}`);
  append(lines, `S_self ${formatNumber(moments.selfInteractionSum)} S_neighbor ${formatNumber(moments.neighborInteractionSum)} S_non_neighbor ${formatNumber(moments.disjointInteractionSum)}`);
  append(lines, `the standard deviation = ${formatNumber(moments.tourVariance)} ${formatNumber(Math.sqrt(Math.max(0, moments.tourVariance)))}`);
  append(lines);
  if (cover) {
    append(lines, "Set Cover answer: YES");
    append(lines, `selected sets of size ${cover.length}: { ${cover.map(index => `S${index}`).join(", ")} }`);
  } else {
    append(lines, "Set Cover answer: NO");
    append(lines, `no set cover of size <= ${k} was found`);
  }
  return lines.join("\n");
}

function runX3c(text) {
  const { universeSize, setCount, padding, sets } = parseX3c(text);
  const exactCover = findX3c(universeSize, sets);
  const sat = x3cTo3Sat(universeSize, setCount, sets);
  const graph = buildCompressedReductionGraph(sat.variableCount, sat.clauses, padding);
  const forced = findDegreeTwoForcedEdges(graph.edge, graph.n);
  const edgeSquared = buildSquaredEdgeMatrix(graph.edge, graph.n);
  const moments = computeTheoryMoments(graph.edge, edgeSquared, graph.n);
  const targetSetCount = universeSize % 3 === 0 ? universeSize / 3 : "not integral";

  const lines = [];
  append(lines, "X3C instance:");
  append(lines, `universe elements = ${universeSize}`);
  append(lines, `3-sets = ${setCount}`);
  append(lines, `target selected sets = ${targetSetCount}`);
  append(lines, `optional padding directed nodes = ${padding}`);
  append(lines, "sets:");
  sets.forEach((set, index) => append(lines, `S${index + 1} = { ${set.join(", ")} }`));
  append(lines);
  append(lines, "Reduction used:");
  append(lines, "X3C -> 3-SAT exactly-once coverage clauses -> compressed Hamiltonian Cycle");
  append(lines, `one Boolean variable per 3-set before auxiliary variables = ${setCount}`);
  append(lines);
  append(lines, "3-SAT encoding:");
  append(lines, `encoding = ${sat.encoding}`);
  append(lines, `CNF clauses before 3-literal normalization = ${sat.rawClauseCount}`);
  append(lines, `variables including auxiliary = ${sat.variableCount}`);
  append(lines, `3-SAT clauses = ${sat.clauses.length}`);
  append(lines);
  append(lines, "Compressed HC reduction size:");
  append(lines, `base directed nodes = ${graph.baseDirected}`);
  append(lines, `total directed nodes = ${graph.directedCount}`);
  append(lines, `undirected HC nodes = ${graph.n}`);
  append(lines);
  append(lines, "Degree-2 forced-edge precheck:");
  append(lines, `vertices with exactly two HC edges = ${forced.forcedVertexCount}`);
  append(lines, `forced HC edges = ${forced.forcedEdgeCount}`);
  append(lines, `forced edge total = ${formatNumber(forced.forcedEdgeTotal)}`);
  append(lines);
  append(lines, "Hamiltonian calculation summary:");
  append(lines, `the average  = ${formatNumber(moments.meanTourLength)}`);
  append(lines, `S_self ${formatNumber(moments.selfInteractionSum)} S_neighbor ${formatNumber(moments.neighborInteractionSum)} S_non_neighbor ${formatNumber(moments.disjointInteractionSum)}`);
  append(lines, `the standard deviation = ${formatNumber(moments.tourVariance)} ${formatNumber(Math.sqrt(Math.max(0, moments.tourVariance)))}`);
  append(lines);
  if (exactCover) {
    append(lines, "X3C answer: YES");
    append(lines, `exact cover: { ${exactCover.map(index => `S${index}`).join(", ")} }`);
  } else {
    append(lines, "X3C answer: NO");
    append(lines, "no exact cover by 3-sets was found");
  }
  return lines.join("\n");
}

function graphColorName(index) {
  const names = ["", "red", "green", "blue", "yellow", "purple", "orange", "cyan", "magenta", "gray"];
  return names[index] || `color${index}`;
}

function runGraphColoring(text) {
  const { n, declaredEdgeCount, colorCount, padding, edges } = parseGraphColoring(text);
  const coloring = findGraphColoring(n, colorCount, edges);
  const sat = graphColoringTo3Sat(n, colorCount, edges);
  const graph = buildCompressedReductionGraph(sat.variableCount, sat.clauses, padding);
  const forced = findDegreeTwoForcedEdges(graph.edge, graph.n);
  const edgeSquared = buildSquaredEdgeMatrix(graph.edge, graph.n);
  const moments = computeTheoryMoments(graph.edge, edgeSquared, graph.n);

  const lines = [];
  append(lines, "Graph Coloring instance:");
  append(lines, `vertices = ${n}`);
  append(lines, `declared edges = ${declaredEdgeCount}`);
  append(lines, `unique edges used = ${edges.length}`);
  append(lines, `colors requested = ${colorCount}`);
  append(lines, `optional padding directed nodes = ${padding}`);
  append(lines, `edge list = ${edges.map(([u, v]) => `(${u},${v})`).join(" ") || "(none)"}`);
  append(lines);
  append(lines, "Reduction used:");
  append(lines, "Graph Coloring -> 3-SAT color clauses -> compressed Hamiltonian Cycle");
  append(lines, `${colorCount} Boolean color variables per vertex`);
  append(lines);
  append(lines, "3-SAT encoding:");
  append(lines, `encoding = ${sat.encoding}`);
  append(lines, `CNF clauses before 3-literal normalization = ${sat.rawClauseCount}`);
  append(lines, `variables = ${sat.variableCount}`);
  append(lines, `3-SAT clauses = ${sat.clauses.length}`);
  append(lines);
  append(lines, "Compressed HC reduction size:");
  append(lines, `base directed nodes = ${graph.baseDirected}`);
  append(lines, `total directed nodes = ${graph.directedCount}`);
  append(lines, `undirected HC nodes = ${graph.n}`);
  append(lines);
  append(lines, "Degree-2 forced-edge precheck:");
  append(lines, `vertices with exactly two HC edges = ${forced.forcedVertexCount}`);
  append(lines, `forced HC edges = ${forced.forcedEdgeCount}`);
  append(lines, `forced edge total = ${formatNumber(forced.forcedEdgeTotal)}`);
  append(lines);
  append(lines, "Hamiltonian calculation summary:");
  append(lines, `the average  = ${formatNumber(moments.meanTourLength)}`);
  append(lines, `S_self ${formatNumber(moments.selfInteractionSum)} S_neighbor ${formatNumber(moments.neighborInteractionSum)} S_non_neighbor ${formatNumber(moments.disjointInteractionSum)}`);
  append(lines, `the standard deviation = ${formatNumber(moments.tourVariance)} ${formatNumber(Math.sqrt(Math.max(0, moments.tourVariance)))}`);
  append(lines);
  if (coloring) {
    append(lines, "Graph Coloring answer: YES");
    append(lines, `coloring: { ${Array.from({ length: n }, (_, index) => `${index + 1}=${graphColorName(coloring[index + 1])}`).join(", ")} }`);
  } else {
    append(lines, "Graph Coloring answer: NO");
    append(lines, `no valid ${colorCount}-coloring was found`);
  }
  return lines.join("\n");
}

function literalText(literal) {
  return literal < 0 ? `~x${-literal}` : `x${literal}`;
}

function formatFormula(clauses) {
  return clauses.map(clause => `(${clause.map(literalText).join(" OR ")})`).join(" AND ");
}

function run3SatCompressed(text) {
  const { variableCount, clauseCount, padding, clauses } = parse3Sat(text);
  const graph = buildCompressedReductionGraph(variableCount, clauses, padding);
  const lines = [];
  append(lines, "3-SAT instance:");
  append(lines, `variables = ${variableCount}`);
  append(lines, `clauses = ${clauseCount}`);
  append(lines, `Formula: ${formatFormula(clauses)}`);
  append(lines);
  append(lines, "Compressed polynomial reduction size:");
  append(lines, `base directed nodes = ${graph.baseDirected}`);
  append(lines, `padding directed nodes = ${padding}`);
  append(lines, `total directed nodes = ${graph.directedCount}`);
  append(lines, `undirected HC nodes = ${graph.n}`);

  const forced = findDegreeTwoForcedEdges(graph.edge, graph.n);
  append(lines);
  append(lines, "Degree-2 forced-edge precheck:");
  append(lines, `vertices with exactly two HC edges = ${forced.forcedVertexCount}`);
  append(lines, `forced HC edges = ${forced.forcedEdgeCount}`);
  append(lines, `forced edge total = ${formatNumber(forced.forcedEdgeTotal)}`);

  const edgeSquared = buildSquaredEdgeMatrix(graph.edge, graph.n);
  const moments = computeTheoryMoments(graph.edge, edgeSquared, graph.n);
  append(lines);
  append(lines, "Hamiltonian calculation summary:");
  append(lines, `the average  = ${formatNumber(moments.meanTourLength)}`);
  append(lines, `S_self ${formatNumber(moments.selfInteractionSum)} S_neighbor ${formatNumber(moments.neighborInteractionSum)} S_non_neighbor ${formatNumber(moments.disjointInteractionSum)}`);
  append(lines, `the standard deviation = ${formatNumber(moments.tourVariance)} ${formatNumber(Math.sqrt(Math.max(0, moments.tourVariance)))}`);

  const assignment = Array(variableCount + 1).fill(-1);
  append(lines);
  if (findSatisfyingAssignment(variableCount, clauses, assignment)) {
    append(lines, "Original 3-SAT answer: SATISFIABLE");
    append(lines, `Witness assignment: { ${assignment.slice(1).map((v, i) => `x${i + 1}=${v === 1 ? "T" : "F"}`).join(", ")} }`);
  } else {
    append(lines, "Original 3-SAT answer: UNSATISFIABLE");
  }
  return lines.join("\n");
}

function buildCompressedReductionGraph(variableCount, clauses, padding) {
  const clauseCount = clauses.length;
  const occurrences = Array.from({ length: variableCount }, () => []);
  for (let clauseIndex = 0; clauseIndex < clauseCount; clauseIndex++) {
    for (const literal of clauses[clauseIndex]) {
      occurrences[Math.abs(literal) - 1].push({ clauseIndex, literal });
    }
  }

  const variableStart = Array(variableCount + 1).fill(0);
  const variableSlotCount = Array(variableCount).fill(0);
  let nextNode = 1;
  for (let variable = 0; variable < variableCount; variable++) {
    variableStart[variable] = nextNode;
    variableSlotCount[variable] = Math.max(2, occurrences[variable].length + 1);
    nextNode += variableSlotCount[variable];
  }
  variableStart[variableCount] = nextNode;
  nextNode += clauseCount;

  const source = nextNode;
  const target = nextNode + 1;
  const baseDirected = nextNode + 1;
  const directedCount = baseDirected + padding;
  const arcs = Array.from({ length: directedCount + 1 }, () => []);
  const addArc = (from, to) => arcs[from].push(to);
  const slotNode = (variableIndex, slotIndex) => variableStart[variableIndex] + slotIndex;
  const clauseNode = clauseIndex => variableStart[variableCount] + clauseIndex;

  addArc(source, slotNode(0, 0));
  addArc(source, slotNode(0, variableSlotCount[0] - 1));

  for (let variable = 0; variable < variableCount; variable++) {
    for (let slot = 1; slot < variableSlotCount[variable]; slot++) {
      addArc(slotNode(variable, slot - 1), slotNode(variable, slot));
      addArc(slotNode(variable, slot), slotNode(variable, slot - 1));
    }

    if (variable + 1 < variableCount) {
      const leftEnd = slotNode(variable, 0);
      const rightEnd = slotNode(variable, variableSlotCount[variable] - 1);
      const nextLeft = slotNode(variable + 1, 0);
      const nextRight = slotNode(variable + 1, variableSlotCount[variable + 1] - 1);
      addArc(leftEnd, nextLeft);
      addArc(leftEnd, nextRight);
      addArc(rightEnd, nextLeft);
      addArc(rightEnd, nextRight);
    } else {
      addArc(slotNode(variable, 0), target);
      addArc(slotNode(variable, variableSlotCount[variable] - 1), target);
    }
  }

  for (let variable = 0; variable < variableCount; variable++) {
    for (let occurrenceIndex = 0; occurrenceIndex < occurrences[variable].length; occurrenceIndex++) {
      const occurrence = occurrences[variable][occurrenceIndex];
      const before = slotNode(variable, occurrenceIndex);
      const after = slotNode(variable, occurrenceIndex + 1);
      const clauseVertex = clauseNode(occurrence.clauseIndex);
      if (occurrence.literal > 0) {
        addArc(before, clauseVertex);
        addArc(clauseVertex, after);
      } else {
        addArc(after, clauseVertex);
        addArc(clauseVertex, before);
      }
    }
  }

  if (padding === 0) {
    addArc(target, source);
  } else {
    const firstPadding = baseDirected + 1;
    addArc(target, firstPadding);
    for (let vertex = firstPadding; vertex < directedCount; vertex++) addArc(vertex, vertex + 1);
    addArc(directedCount, source);
  }

  const n = 3 * directedCount;
  const edge = makeMatrix(n);
  const gadgetIn = vertex => (3 * vertex) - 2;
  const gadgetMid = vertex => (3 * vertex) - 1;
  const gadgetOut = vertex => 3 * vertex;
  const addUndirected = (from, to) => {
    edge[from][to] = -1;
    edge[to][from] = -1;
  };
  for (let vertex = 1; vertex <= directedCount; vertex++) {
    addUndirected(gadgetIn(vertex), gadgetMid(vertex));
    addUndirected(gadgetMid(vertex), gadgetOut(vertex));
  }
  for (let from = 1; from <= directedCount; from++) {
    for (const to of arcs[from]) addUndirected(gadgetOut(from), gadgetIn(to));
  }
  return { edge, n, baseDirected, directedCount };
}

function findDegreeTwoForcedEdges(edge, n) {
  const forcedEdge = new Set();
  let forcedEdgeCount = 0;
  let forcedVertexCount = 0;
  let forcedEdgeTotal = 0;

  for (let vertex = 1; vertex <= n; vertex++) {
    const neighbors = [];
    for (let neighbor = 1; neighbor <= n; neighbor++) {
      if (vertex !== neighbor && edge[vertex][neighbor] !== 0) neighbors.push(neighbor);
    }
    if (neighbors.length !== 2) continue;

    forcedVertexCount += 1;
    for (const neighbor of neighbors) {
      const a = Math.min(vertex, neighbor);
      const b = Math.max(vertex, neighbor);
      const key = `${a}:${b}`;
      if (forcedEdge.has(key)) continue;
      forcedEdge.add(key);
      forcedEdgeCount += 1;
      forcedEdgeTotal += edge[vertex][neighbor];
    }
  }

  return { forcedVertexCount, forcedEdgeCount, forcedEdgeTotal };
}

async function loadFileInto(fileInput, textareaId) {
  const file = fileInput.files[0];
  if (!file) return;
  document.getElementById(textareaId).value = await file.text();
}

function runSafely(fn) {
  write("Running...");
  setTimeout(() => {
    try {
      write(fn());
    } catch (error) {
      write(`Error: ${error.message}`);
    }
  }, 30);
}

document.querySelectorAll(".tab").forEach(button => {
  button.addEventListener("click", () => {
    document.querySelectorAll(".tab").forEach(tab => tab.classList.remove("active"));
    document.querySelectorAll(".panel").forEach(panel => panel.classList.remove("active"));
    button.classList.add("active");
    document.getElementById(button.dataset.panel).classList.add("active");
  });
});

document.getElementById("clearOutput").addEventListener("click", () => write("Ready."));
document.getElementById("runSat").addEventListener("click", () => runSafely(() => run3SatCompressed(document.getElementById("satInput").value)));
document.getElementById("runVc").addEventListener("click", () => runSafely(() => runVertexCover(document.getElementById("vcInput").value)));
document.getElementById("runClique").addEventListener("click", () => runSafely(() => runClique(document.getElementById("cliqueInput").value)));
document.getElementById("runIs").addEventListener("click", () => runSafely(() => runIndependentSet(document.getElementById("isInput").value)));
document.getElementById("runSetCover").addEventListener("click", () => runSafely(() => runSetCover(document.getElementById("setCoverInput").value)));
document.getElementById("runX3c").addEventListener("click", () => runSafely(() => runX3c(document.getElementById("x3cInput").value)));
document.getElementById("runGraphColoring").addEventListener("click", () => runSafely(() => runGraphColoring(document.getElementById("graphColoringInput").value)));
document.getElementById("runPairs").addEventListener("click", () => runSafely(() => {
  const { edge, n } = parsePairs(document.getElementById("pairsInput").value);
  return runTrackingSolver(edge, n, Number(document.getElementById("pairsBeta").value), "browser pairs input", { forceDegreeTwo: true });
}));
document.getElementById("runMatrix").addEventListener("click", () => runSafely(() => {
  const { edge, n } = parseMatrix(document.getElementById("matrixInput").value);
  return runTrackingSolver(edge, n, Number(document.getElementById("matrixBeta").value), "browser matrix input");
}));
document.getElementById("runPoints").addEventListener("click", () => runSafely(() => {
  const { edge, n } = parsePoints(document.getElementById("pointsInput").value);
  return runTrackingSolver(edge, n, Number(document.getElementById("pointsBeta").value), "browser points input");
}));
document.getElementById("runManual").addEventListener("click", () => runSafely(() => {
  const { edge, n } = parseManual(document.getElementById("manualInput").value);
  return runTrackingSolver(edge, n, Number(document.getElementById("manualBeta").value), "browser manual input");
}));

document.getElementById("satFile").addEventListener("change", event => loadFileInto(event.target, "satInput"));
document.getElementById("vcFile").addEventListener("change", event => loadFileInto(event.target, "vcInput"));
document.getElementById("cliqueFile").addEventListener("change", event => loadFileInto(event.target, "cliqueInput"));
document.getElementById("isFile").addEventListener("change", event => loadFileInto(event.target, "isInput"));
document.getElementById("setCoverFile").addEventListener("change", event => loadFileInto(event.target, "setCoverInput"));
document.getElementById("x3cFile").addEventListener("change", event => loadFileInto(event.target, "x3cInput"));
document.getElementById("graphColoringFile").addEventListener("change", event => loadFileInto(event.target, "graphColoringInput"));
document.getElementById("pairsFile").addEventListener("change", event => loadFileInto(event.target, "pairsInput"));
document.getElementById("matrixFile").addEventListener("change", event => loadFileInto(event.target, "matrixInput"));
document.getElementById("pointsFile").addEventListener("change", event => loadFileInto(event.target, "pointsInput"));
document.getElementById("manualFile").addEventListener("change", event => loadFileInto(event.target, "manualInput"));
