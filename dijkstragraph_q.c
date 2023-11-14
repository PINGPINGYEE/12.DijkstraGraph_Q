#include <stdio.h>
#include <stdlib.h>

#define INFINITY 1000L // 무한대를 나타내는 상수 정의
#define MAX 100 // 최대 정점 개수

// 전역 변수
int distance[MAX]; // 각 정점까지의 최소 거리 저장
int found[MAX]; // 최단 경로를 찾은 정점 표시

// 인접 행렬을 위한 그래프 구조체 정의
typedef struct graphTypeMatrix {
    int n; // 정점의 수
    int weight[MAX][MAX]; // 간선의 가중치 저장
} GraphTypeMatrix;

// 그래프 노드 구조체 정의
typedef struct graphNode {
    int vertex; // 정점
    int weight; // 가중치
    struct graphNode* link; // 다음 노드를 가리키는 포인터
} GraphNode;

// 힙 구조체 정의
typedef struct heapType {
    int heapsize; // 힙 크기
    GraphNode* heap[MAX]; // 힙 저장 배열
} HeapType;

// 인접 리스트를 위한 그래프 구조체 정의
typedef struct graphTypeList {
    GraphNode* adj_list[MAX]; // 인접 리스트
    int numOfvertex; // 정점 수
} GraphTypeList;

// 함수 선언

// 인접행렬 관련
GraphTypeMatrix* create_matrix();
void init_graph_matrix(GraphTypeMatrix* g);
int choose_short_path(int distance[], int n, int found[]);
void shortest_path_matrix(GraphTypeMatrix* g, int start);
GraphTypeMatrix* make_graph_matrix();

// 힙 관련
HeapType* create();
void init_heap(HeapType* h);
void insert_heap(HeapType* h, GraphNode* node);
GraphNode* delete_heap(HeapType* h);

// 인접리스트 관련
GraphTypeList* create_list();
void init_graph_list(GraphTypeList* gl);
void insert_vertex(GraphTypeList* gl, int vertex);
void insert_edge(GraphTypeList* gl, int s, int e, int weight);
void shortest_path_list(GraphTypeList* gl, int start);
GraphTypeList* makeGraph_list();

void print_status(int distance[], int found[], int n);

// 메인 함수
int main() {
    printf("Dijkstra Algorithm using matrix\n");
    GraphTypeMatrix* g;
    g = make_graph_matrix();
    shortest_path_matrix(g, 0);
    free(g);

    printf("\nDijkstra Algorithm using list\n");
    GraphTypeList* graph_list = makeGraph_list();
    shortest_path_list(graph_list, 0);
    free(graph_list);

    return 0;
}

GraphTypeList* create_list() {
    return (GraphTypeList*)malloc(sizeof(GraphTypeList));
}

void init_graph_list(GraphTypeList* gl) {
    gl->numOfvertex = 0;
    for (int i = 0; i < MAX; i++) {
        gl->adj_list[i] = NULL;
    }
}

HeapType* create() {
    return(HeapType*)malloc(sizeof(HeapType));
}

void init_heap(HeapType* h) {
    h->heapsize = 0;
    for (int i = 0; i < MAX; i++) {
        h->heap[i] = 0;
    }
}

GraphTypeList* makeGraph_list() {
    GraphTypeList* graph_list = create_list();
    init_graph_list(graph_list);

    for (int i = 0; i < 10; i++) {
        insert_vertex(graph_list, i);
    }

    insert_edge(graph_list, 0, 1, 3);
    insert_edge(graph_list, 0, 5, 11);
    insert_edge(graph_list, 0, 6, 12);

    insert_edge(graph_list, 1, 0, 3);
    insert_edge(graph_list, 1, 2, 5);
    insert_edge(graph_list, 1, 3, 4);
    insert_edge(graph_list, 1, 4, 1);
    insert_edge(graph_list, 1, 5, 7);
    insert_edge(graph_list, 1, 6, 8);

    insert_edge(graph_list, 2, 1, 5);
    insert_edge(graph_list, 2, 3, 2);
    insert_edge(graph_list, 2, 6, 6);
    insert_edge(graph_list, 2, 7, 5);

    insert_edge(graph_list, 3, 1, 4);
    insert_edge(graph_list, 3, 2, 2);
    insert_edge(graph_list, 3, 4, 13);
    insert_edge(graph_list, 3, 7, 14);
    insert_edge(graph_list, 3, 9, 16);

    insert_edge(graph_list, 4, 1, 1);
    insert_edge(graph_list, 4, 3, 13);
    insert_edge(graph_list, 4, 5, 9);
    insert_edge(graph_list, 4, 8, 18);
    insert_edge(graph_list, 4, 9, 17);

    insert_edge(graph_list, 5, 0, 11);
    insert_edge(graph_list, 5, 1, 7);
    insert_edge(graph_list, 5, 4, 9);

    insert_edge(graph_list, 6, 0, 12);
    insert_edge(graph_list, 6, 1, 8);
    insert_edge(graph_list, 6, 2, 6);
    insert_edge(graph_list, 6, 7, 13);

    insert_edge(graph_list, 7, 2, 5);
    insert_edge(graph_list, 7, 3, 14);
    insert_edge(graph_list, 7, 6, 13);
    insert_edge(graph_list, 7, 9, 15);

    insert_edge(graph_list, 8, 4, 18);
    insert_edge(graph_list, 8, 9, 10);

    insert_edge(graph_list, 9, 3, 16);
    insert_edge(graph_list, 9, 4, 17);
    insert_edge(graph_list, 9, 7, 15);
    insert_edge(graph_list, 9, 8, 10);

    return graph_list;
}


void insert_vertex(GraphTypeList* gl, int vertex) {
    if (vertex > gl->numOfvertex || gl->numOfvertex > MAX - 1) {
        error("Over vertex's number");
        return;
    }
    gl->numOfvertex++;
}

void insert_edge(GraphTypeList* gl, int s, int e, int weight) {
    if (s >= gl->numOfvertex || e >= gl->numOfvertex) {
        error("Default vertex's number");
        return;
    }
    GraphNode* newNode = (GraphNode*)malloc(sizeof(GraphNode));
    newNode->vertex = e;
    newNode->weight = weight;
    newNode->link = NULL;

    if (gl->adj_list[s] == NULL) {
        gl->adj_list[s] = newNode;
    }
    else {
        GraphNode* current = gl->adj_list[s];
        while (current->link != NULL) {
            current = current->link;
        }
        current->link = newNode;
    }
}

void shortest_path_list(GraphTypeList *gl, int start) {
    HeapType *h = create();
    init_heap(h);

    int visitedOforder[MAX]; // 노드 방문 순서를 저장하는 배열
    int orderCount = 0; // 방문한 노드의 수를 카운트
    GraphNode *node;

    for(int i = 0; i < gl->numOfvertex; i++) {
        distance[i] = INFINITY;
        found[i] = 0;
        visitedOforder[i] = -1;
    }

    distance[start] = 0;

    node = (GraphNode*)malloc(sizeof(GraphNode));
    node->vertex = start;
    node->weight = 0;
    insert_heap(h, node);

    while(h->heapsize > 0) {
        node = delete_heap(h);
        int current = node->vertex;

        if (!found[current]) {
            found[current] = 1;
            visitedOforder[orderCount++] = current;
            GraphNode *adjNode = gl->adj_list[current];

            while(adjNode != NULL) {
                int v = adjNode->vertex;
                if (found[v] == 0 && distance[v] > distance[current] + adjNode->weight) {
                    distance[v] = distance[current] + adjNode->weight;
                    GraphNode *newNode = (GraphNode*)malloc(sizeof(GraphNode));
                    newNode->vertex = v;
                    newNode->weight = distance[v];
                    insert_heap(h, newNode);
                }
                adjNode = adjNode->link;
                print_status(distance, found, gl->numOfvertex);
            }
        }
        free(node);
    }

    for(int i=0; i<orderCount; i++) {
        printf("%d ", visitedOforder[i]+1);
    }
    printf("\n");

    free(h);
}



void insert_heap(HeapType* h, GraphNode* node) {
    int i = ++h->heapsize;

    while (i != 1 && node->weight <= h->heap[i / 2]->weight) {
        h->heap[i] = h->heap[i / 2];
        i = i / 2;
    }
    h->heap[i] = node;
}

GraphNode* delete_heap(HeapType* h) {
    GraphNode* item, * temp;
    int parent = 1, child = 2;
    item = h->heap[1];
    temp = h->heap[h->heapsize--];

    while (child < h->heapsize) {
        if (child < h->heapsize && h->heap[child]->weight > h->heap[child + 1]->weight) {
            child++;
        }

        if (temp->weight < h->heap[child]->weight) break;

        h->heap[parent] = h->heap[child];
        parent = child;
        child *= 2;
    }
    h->heap[parent] = temp;
    return item;
}

GraphTypeMatrix* make_graph_matrix() {
    GraphTypeMatrix* g = create_matrix();
    init_graph_matrix(g);
    g->n = 10;
    int weight[10][10] = {
        {0, 3, INFINITY, INFINITY, INFINITY, 11, 12, INFINITY, INFINITY, INFINITY}, //1
        {3, 0, 5, 4, 1, 7, 8, INFINITY, INFINITY, INFINITY}, // 2
        {INFINITY, 5, 0, 2, INFINITY, INFINITY, 6, 5, INFINITY, INFINITY}, // 3
        {INFINITY, 4, 2, 0, 13, INFINITY, INFINITY, 14, INFINITY, 16}, // 4
        {INFINITY, 1, INFINITY, 13, 0, 9, INFINITY, INFINITY, 18, 17}, // 5
        {11, 7, INFINITY, INFINITY, 9, 0, INFINITY, INFINITY, INFINITY, INFINITY}, // 6
        {12, 8, 6, INFINITY, INFINITY, INFINITY, 0, 13, INFINITY, INFINITY}, // 7
        {INFINITY, INFINITY, 5, 14, INFINITY, INFINITY, 13, 0, INFINITY, 15}, // 8
        {INFINITY, INFINITY, INFINITY, INFINITY, 18, INFINITY, INFINITY, INFINITY, 0, 10}, // 9
        {INFINITY, INFINITY, INFINITY, 16, INFINITY, INFINITY, INFINITY, 15, 10, 0} // 10
    };

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            g->weight[i][j] = weight[i][j];
        }
    }
    return g;
}

void shortest_path_matrix(GraphTypeMatrix* g, int start) {
    int u, w;
    int visitedOrder[MAX];
    int orderCount = 0;

    for (int i = 0; i < g->n; i++) {
        distance[i] = g->weight[start][i];
        found[i] = 0;
    }
    found[start] = 1;
    distance[start] = 0;
    visitedOrder[orderCount++] = start;

    for (int j = 0; j < g->n - 1; j++) {
        print_status(distance, found, g->n);
        u = choose_short_path(distance, g->n, found); // 최단 거리의 정점 선택
        found[u] = 1;
        visitedOrder[orderCount++] = u;

        for (w = 0; w < g->n; w++) {
            if (!found[w] && g->weight[u][w] != INFINITY) {
                if (distance[u] + g->weight[u][w] < distance[w]) {
                    distance[w] = distance[u] + g->weight[u][w];
                }
            }
        }
    }

    for (int i = 0; i < orderCount; i++) {
        printf("%d ", visitedOrder[i] + 1);
    }
    printf("\n");
}


int choose_short_path(int distance[], int n, int found[]) {
    int min = INFINITY;
    int minpos = -1;
    for (int i = 0; i < n; i++) {
        if (distance[i] < min && found[i] == 0) {
            min = distance[i];
            minpos = i;
        }
    }
    return minpos;
}


void init_graph_matrix(GraphTypeMatrix* g) {
    g->n = 0;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            g->weight[i][j] = 0;
        }
    }
}


GraphTypeMatrix* create_matrix() {
    return (GraphTypeMatrix*)malloc(sizeof(GraphTypeMatrix));
}


void print_status(int distance[], int found[], int n) {
    printf("Distance: ");
    for (int i = 0; i < n; i++) {
        if (distance[i] != INFINITY) {
            printf("%d ", distance[i]);
        }
        else {
            printf("* ");
        }
    }
    printf("\nFound: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", found[i]);
    }
    printf("\n\n");
}
