from tryalgo.bipartite_matching import max_bipartite_matching

def alternate(u, bigraph, visitU, visitV, matchV):
    """extend alternating tree from free vertex u.
      visitU, visitV marks all vertices covered by the tree.
    """
    visitU[u] = True
    for v in bigraph[u]:
        if not visitV[v]:
            visitV[v] = True
            assert matchV[v] is not None   # otherwise match not maximum
            alternate(matchV[v], bigraph, visitU, visitV, matchV)


def koenig(bigraph):
    """Bipartie minimum vertex cover by Koenig's theorem

    :param bigraph: adjacency list, index = vertex in U,
                                    value = neighbor list in V
    :assumption: U = V = {0, 1, 2, ..., n - 1} for n = len(bigraph)
    :returns: boolean table for U, boolean table for V
    :comment: selected vertices form a minimum vertex cover,
              i.e. every edge is adjacent to at least one selected vertex
              and number of selected vertices is minimum
    :complexity: `O(|V|*|E|)`
    """
    V = range(len(bigraph))
    matchV = max_bipartite_matching(bigraph)
    matchU = [None for u in V]
    for v in V:                      # -- build the mapping from U to V
        if matchV[v] is not None:
            matchU[matchV[v]] = v
    visitU = [False for u in V]      # -- build max alternating forest
    visitV = [False for v in V]
    for u in V:
        if matchU[u] is None:        # -- starting with free vertices in U
            alternate(u, bigraph, visitU, visitV, matchV)
    inverse = [not b for b in visitU]

    return (inverse, visitV)