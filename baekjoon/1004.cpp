#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>
#include <utility>
#include <cassert>
using namespace std;

// Planetary System
struct Psystem {
  int centerX, centerY, radius;

  Psystem() {
  }

  Psystem(int cx, int cy, int r)
    : centerX(cx),
      centerY(cy),
      radius(r) {
  }
};

// Node in Tree
struct Node {
  int parent;
  vector<int> childs;
};

// Return distance between given two points.
double distanceBetweenPoints(int ax, int ay, int bx, int by) {
  int dx = ax - bx;
  int dy = ay - by;
  return sqrt((double)(dx * dx + dy * dy));
}

// Does system 'a' contain system 'b'? 
bool contains(const Psystem& a, const Psystem& b) {
  if (a.radius <= b.radius)
    return false;

  double dist = distanceBetweenPoints(a.centerX, a.centerY,
                                      b.centerX, b.centerY);
  if (dist < (double)a.radius)
    return true;
  return false;
}

// Does system 'a' contain point ('px', 'py')? 
bool contains(const Psystem& a, int px, int py) {
  double dist = distanceBetweenPoints(a.centerX, a.centerY, px, py);
  if (dist < (double)a.radius)
    return true;
  return false;
}

// DFS for topological sorting
void dfs(const vector<Psystem>& psystems, vector<bool>& visited, int here,
         vector<Psystem>& sorted) {
  visited[here] = true;
  for(int there = 0; there < (int)psystems.size(); there++) {
    if(!visited[there] && contains(psystems[here], psystems[there]))
      dfs(psystems, visited, there, sorted);
  }
  sorted.push_back(psystems[here]);
}

// Sort topologically 'psystems' 
void topologicalSort(vector<Psystem>& psystems) {
  vector<bool> visited;
  visited.resize(psystems.size(), false);
  vector<Psystem> sorted;
  dfs(psystems, visited, 0, sorted);
  reverse(sorted.begin(), sorted.end());
  psystems = sorted;
}

// Does node at index 'here' contain
// node at index 'nodeA' and node at index 'nodeB'?
pair<bool, bool> hasNodes(const vector<Node>& nodes,
                          vector< pair<bool, bool> >& cache,
                          vector<bool>& visited,
                          int here,
                          int nodeA,
                          int nodeB) {
  pair<bool, bool>& ret = cache[here];

  if (visited[here])
    return ret;
  visited[here] = true;
  ret = make_pair(false, false);

  if (here == nodeA)
    ret.first = true;
  if (here == nodeB)
    ret.second = true;

  for (int there : nodes[here].childs) {
    pair<bool, bool> childRet = hasNodes(nodes,
                                         cache,
                                         visited,
                                         there,
                                         nodeA,
                                         nodeB);
    ret.first |= childRet.first;
    ret.second |= childRet.second;
  }
  return ret;
}

// Before call this function, nodes must be topologically sorted.
// (Root node come first.)
// Find common ancestor of 'nodeA' and 'nodeB'.
int findAncestor(const vector<Node>& nodes, int nodeA, int nodeB) {
  vector< pair<bool, bool> > cache(nodes.size());
  vector<bool> visited(nodes.size(), false);
  
  int ancestor;
  for (int here = (int)nodes.size()-1; here >= 0; here--) {
    pair<bool, bool> ret = hasNodes(nodes, cache, visited, 
                    here, nodeA, nodeB);
    if (ret.first && ret.second) {
      ancestor = here;
      break;
    }
  }
  return ancestor;
}

// Calculate distance from a node to its ancestor
int distanceToAncestor(const vector<Node>& nodes, int here, int ancestor) {
  int dist = 0;
  while (here != ancestor) {
    here = nodes[here].parent;
    dist++;
  }
  return dist;
}

int main() {
  int numOfCases;
  cin >> numOfCases;
  while (numOfCases--) {
    int startX, startY, targetX, targetY;
    int numOfPsystems;
    vector<Psystem> psystems;
    vector<Node> nodes;
    int startIdx = -1, targetIdx = -1;

    // Input start point, destination point, and planetary systems.
    cin >> startX >> startY >> targetX >> targetY;
    cin >> numOfPsystems;

    // Add given systems and virtual system
    // which contains every other systems.
    psystems.resize(numOfPsystems+1);
    nodes.resize(numOfPsystems+1);
    psystems[0] = Psystem(0, 0, 3000);
    for (int i = 1; i <= numOfPsystems; i++) {
      cin >> psystems[i].centerX
        >> psystems[i].centerY
        >> psystems[i].radius;
    }

    // Sort topologically.
    topologicalSort(psystems);

    // Construct tree structure.
    for (int here = (int)psystems.size()-1; here >= 1; here--) {
      int parent;
      for (parent = here-1; parent >= 0; parent--) {
        if (contains(psystems[parent], psystems[here]))
          break;
      }
      nodes[here].parent = parent;
      nodes[parent].childs.push_back(here);
    }

    // Find two node of which each contains
    // start point or destination point.
    for (int here = (int)psystems.size()-1; here >= 0; here--) {
      if (startIdx == -1 && contains(psystems[here], startX, startY))
        startIdx = here;
      if (targetIdx == -1 && contains(psystems[here], targetX, targetY))
        targetIdx = here;
    }

    // Find common ancestor of them.
    int ancestorIdx = findAncestor(nodes, startIdx, targetIdx);

    // Calculate distance in tree.
    int dist = 0;
    if (startIdx == ancestorIdx) {
      dist += distanceToAncestor(nodes, targetIdx, ancestorIdx);
    } else if (targetIdx == ancestorIdx) {
      dist += distanceToAncestor(nodes, startIdx, ancestorIdx);
	  } else {
      dist += distanceToAncestor(nodes, targetIdx, ancestorIdx);
      dist += distanceToAncestor(nodes, startIdx, ancestorIdx);
    }

    cout << dist << endl;
  }
  return 0;
}