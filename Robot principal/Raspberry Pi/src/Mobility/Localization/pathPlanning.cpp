#include "../../../include/Robot.h"

void Robot::loadNodes(const std::string& filename, void *game) {
    GAME * mygame = (GAME *)game;
    Queen * queen = mygame->queen;
    MAP * mymap = mygame->map;

    std::ifstream file(filename);

    if (!file.is_open()) {
        fprintf(stderr, "Error opening the node file!\n");
        return ;
    }

    int nodeNumber;
    double x, y;

    // Read node data from the file
    while (file >> nodeNumber >> x >> y) {
        //fprintf(stderr, "nodeNumber: %d, x: %f, y: %f\n", nodeNumber, x, y);
        Node node;
        node.x = x;
        node.y = y;
        mymap->nodes[nodeNumber].x = node.x;
        mymap->nodes[nodeNumber].y = node.y;
    }
    fprintf(stderr, "Nodes loaded\n");

    // print the nodes :
    for (int i = 0; i < 19; i++){
        fprintf(stderr, "node %d : x: %f, y: %f\n", i, mymap->nodes[i].x, mymap->nodes[i].y);
    }

    file.close();
}

void Robot::loadEdges(const std::string& filename, void *game) {
    GAME * mygame = (GAME *)game;
    Queen * queen = mygame->queen;
    MAP * mymap = mygame->map;

    std::ifstream file(filename);
    if (!file.is_open()) {
        fprintf(stderr, "Error opening the edge file!\n");
        return ;
    }
    
    int linkNumber, startNode, endNode;
    double cost;

    // Read edge data from the file
    while (file >> linkNumber >> startNode >> endNode >> cost) {
        Edge edge;
        edge.node1 = startNode;
        edge.node2 = endNode;
        edge.cost = cost;
        mymap->edges[linkNumber] = edge;
    }
    
    file.close();
    fprintf(stderr, "Edges loaded\n");
}

double Robot::heuristic(const Node& a, const Node& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void Robot::aStar(int start, int goal, void *game) {
    GAME * mygame = (GAME *)game;
    Queen * queen = mygame->queen;
    MAP * mymap = mygame->map;

    // reset path to empty list
    path.clear();

    std::unordered_map<int, double> gCost, fCost;
    std::unordered_map<int, int> cameFrom;
    auto cmp = [&](int left, int right) { return fCost[left] > fCost[right]; };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> openSet(cmp);
    
    gCost[start] = 0;
    fCost[start] = heuristic(mymap->nodes[start], mymap->nodes[goal]);
    openSet.push(start);
    
    while (!openSet.empty()) {
        int current = openSet.top();
        openSet.pop();
        
        if (current == goal) {
            while (current != start) {
                (path).push_back(current);
                current = cameFrom[current];
            }
            (path).push_back(start);
            std::reverse((path).begin(), (path).end());
            // path is now complete
            return;
        }
        
        for (const auto& edge : mymap->edges) {
            if (edge.node1 == current || edge.node2 == current) {
                int neighbor = (edge.node1 == current) ? edge.node2 : edge.node1;
                double tentative_gCost = gCost[current] + edge.cost;
                if (gCost.find(neighbor) == gCost.end() || tentative_gCost < gCost[neighbor]) {
                    cameFrom[neighbor] = current;
                    gCost[neighbor] = tentative_gCost;
                    fCost[neighbor] = tentative_gCost + heuristic(mymap->nodes[neighbor], mymap->nodes[goal]);
                    openSet.push(neighbor);
                }
            }
        }
    }
    return;
}

void Robot::printPath() {
    fprintf(stderr, "Path: ");
    for (int i = 0; i < path.size(); i++) {
        fprintf(stderr, "%d ", path[i]);
    }
    fprintf(stderr, "\n");
}
// Example initialization function
/*NodeMap path_planning_init(CtrlStruct *cvs) {

    NodeMap nodes;    
    // Load data
    nodes = loadNodes("../../userFiles/ctrl/groups_ctrl/gr1/nodes.txt", cvs);

    //fprintf(stdout, "Nodes: %lu\n", mymap->nodes.size());
    //loadEdges("../../userFiles/ctrl/groups_ctrl/gr1/links.txt", cvs);
    
    //fprintf(stdout, "Nodes: %lu\n", mymap->nodes.size());
    //fprintf(stdout, "Edges: %lu\n", cvs->edges.size());
    
    std::cerr << "Path planning initialized!\n";

    return nodes;
}*/