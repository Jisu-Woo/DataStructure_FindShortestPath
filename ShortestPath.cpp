#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
   
struct Edge {
    int dst; //destination
    float weight;
};

struct Node {
    int data;
    std::vector<Edge> adjList;  //거리와 연결할 수 있는 곳 쓰여있음.
};

struct QueueNode {
    float distance;
    int node;

    bool operator<(const QueueNode& rhs) const {
        return distance > rhs.distance; // 더 작은 거리가 높은 우선순위를 가짐
    }
};

struct Graph {
    std::vector<Node> nodes;

    void addNode(int data) {
        nodes.push_back({ data });  //{}안에 정보를 넣어주면 알아서 struct를 만들어서 데이터를 저장하여 추가해줌
    }
    void addEdge(int from, int to, float w) {
        nodes[from].adjList.push_back({ to,w }); //from노드에서 to노드로 가는 edge추가
    }

    void print(int start, int end) const {
        std::vector<float> distances(nodes.size(), 1000000); //시작노드로부터의 최단거리를 저장하는 배열(무한대로 초기화)
        std::vector<int> parents(nodes.size(), -1); //최단경로에서 각 노드의 직전 노드를 저장하는 배열 (-1로 초기화)

        std::priority_queue<QueueNode> pq;

        distances[start] = 0;  //시작 노드 거리 = 0
        pq.push({ 0, start });  // 우선순위 큐에 시작노드와 거리(0) 삽입

        while (!pq.empty()) {  //큐가 비어있지 않는 동안
            int currentNode = pq.top().node;  //현재 노드를 우선순위 큐의 top에서 가져옴
            float currentDistance = pq.top().distance;  //현재 노드까지의 거리 가져옴
            pq.pop();  //우선순위 큐의 top 제거

            // 만약 현재 계산한 거리가 이미 테이블에 업데이트된 거리보다 크면 무시
            if (currentDistance > distances[currentNode]) continue;

            //최단 거리를 가진 노드와 연결된 노드들의 최단거리 갱신
            for (const Edge& edge : nodes[currentNode].adjList) {// 현재 노드와 연결된 모든 노드를 순회
                int adjacentNode = edge.dst;  //연결된 노드의 인덱스
                float weight = edge.weight;  //edge의 가중치

                float nextDistance = currentDistance + weight; // 현재 노드를 거치는 거리를 계산

                if (nextDistance < distances[adjacentNode]) {
                    distances[adjacentNode] = nextDistance; //최단거리 갱신
                    parents[adjacentNode] = currentNode; //부모노드 정보 갱신
                    pq.push({ nextDistance, adjacentNode }); // 계산한 거리와 노드를 우선순위 큐에 삽입
                }
            }
        }

        // 최단 경로 출력
        std::vector<int> path;
        int current = end;

        while (current != -1) {
            path.push_back(current);  //경로에 현재 노드 추가
            current = parents[current];  //부모 노드로 이동
        }

        std::cout << "최단 경로: ";

        for (int i = path.size() - 1; i >= 0; --i) {
            std::cout << path[i] + 1 << " ";  //역순으로 최단경로 출력 / 1부터 시작하는 노드 번호로 출력
        }
        std::cout << std::endl;
    }
};

int main() {
    std::ifstream file("C:/Users/gemge/OneDrive/바탕 화면/자료구조과제text.txt");
    if (!file.is_open()) {
        std::cout << "파일이 존재하지 않습니다." << std::endl;
        return 1;
    }

    int nNodes;
    int nEdges;

    file >> nNodes >> nEdges;

    Graph g;

    //nNodes만큼 g에 노드를 생성
    for (int i = 0; i < nNodes; i++) {
        g.addNode(i);
    }

    int node1, node2;
    float weight;
    char command;


    //존재하는 edge들 모두 g에 추가
    while (file >> command >> node1 >> node2 >> weight) {
        if (command == 'a') {
            g.addEdge(node1 - 1, node2 - 1, weight);
        }
    }

    file.close();

    int start, end;

    std::cout << "시작 노드 번호 입력: ";
    std::cin >> start;
    std::cout << "끝 노드 번호 입력: ";
    std::cin >> end;

    g.print(start - 1, end - 1);  //0부터 시작해야하므로 -1
    std::cout << "출력 완료" << std::endl;

    return 0;
}
