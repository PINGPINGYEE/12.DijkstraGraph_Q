#include <stdio.h>
#include <stdlib.h>

#define INF 1000L // 무한대를 나타내는 상수 정의
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
    int number_of_vertex; // 정점 수
} GraphTypeList;

// error 생성
void error(char* comment) {
    fprintf(stderr, "%s\n", comment);
}

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
    printf("1번 방법 사용\n");
    GraphTypeMatrix* g;
    g = make_graph_matrix();
    shortest_path_matrix(g, 0);
    free(g);

    printf("\n2번 방법 힙을 이용해 사용\n");
    GraphTypeList* graph_list = makeGraph_list();
    shortest_path_list(graph_list, 0);
    free(graph_list);

    return 0;
}

// 인접리스트 그래프 생성
GraphTypeList* create_list() {
    return (GraphTypeList*)malloc(sizeof(GraphTypeList));
}

// 인접리스트 그래프 초기화
void init_graph_list(GraphTypeList* gl) {
    gl->number_of_vertex = 0;
    for (int i = 0; i < MAX; i++) {
        gl->adj_list[i] = NULL;
    }
}

// 힙 생성
HeapType* create() {
    return(HeapType*)malloc(sizeof(HeapType));
}

// 힙 초기화
void init_heap(HeapType* h) {
    h->heapsize = 0;
    for (int i = 0; i < MAX; i++) {
        h->heap[i] = 0;
    }
}

// 인접리스트 그래프 만들기
GraphTypeList* makeGraph_list() {
    GraphTypeList* graph_list = create_list();
    init_graph_list(graph_list);

    for (int i = 0; i < 10; i++) {
        insert_vertex(graph_list, i);
    }

    // 1
    insert_edge(graph_list, 0, 1, 3);
    insert_edge(graph_list, 0, 5, 11);
    insert_edge(graph_list, 0, 6, 12);

    // 2
    insert_edge(graph_list, 1, 0, 3);
    insert_edge(graph_list, 1, 2, 5);
    insert_edge(graph_list, 1, 3, 4);
    insert_edge(graph_list, 1, 4, 1);
    insert_edge(graph_list, 1, 5, 7);
    insert_edge(graph_list, 1, 6, 8);
    // 3
    insert_edge(graph_list, 2, 1, 5);
    insert_edge(graph_list, 2, 3, 2);
    insert_edge(graph_list, 2, 6, 6);
    insert_edge(graph_list, 2, 7, 5);
    // 4
    insert_edge(graph_list, 3, 1, 4);
    insert_edge(graph_list, 3, 2, 2);
    insert_edge(graph_list, 3, 4, 13);
    insert_edge(graph_list, 3, 7, 14);
    insert_edge(graph_list, 3, 9, 16);
    // 5
    insert_edge(graph_list, 4, 1, 1);
    insert_edge(graph_list, 4, 3, 13);
    insert_edge(graph_list, 4, 5, 9);
    insert_edge(graph_list, 4, 8, 18);
    insert_edge(graph_list, 4, 9, 17);
    // 6
    insert_edge(graph_list, 5, 0, 11);
    insert_edge(graph_list, 5, 1, 7);
    insert_edge(graph_list, 5, 4, 9);
    // 7
    insert_edge(graph_list, 6, 0, 12);
    insert_edge(graph_list, 6, 1, 8);
    insert_edge(graph_list, 6, 2, 6);
    insert_edge(graph_list, 6, 7, 13);
    // 8
    insert_edge(graph_list, 7, 2, 5);
    insert_edge(graph_list, 7, 3, 14);
    insert_edge(graph_list, 7, 6, 13);
    insert_edge(graph_list, 7, 9, 15);
    // 9
    insert_edge(graph_list, 8, 4, 18);
    insert_edge(graph_list, 8, 9, 10);
    // 10
    insert_edge(graph_list, 9, 3, 16);
    insert_edge(graph_list, 9, 4, 17);
    insert_edge(graph_list, 9, 7, 15);
    insert_edge(graph_list, 9, 8, 10);

    return graph_list;
}

// 정점 추가
void insert_vertex(GraphTypeList* graph_list, int vertex) {
    if (vertex > graph_list->number_of_vertex || graph_list->number_of_vertex > MAX - 1) {
        error("정점 초과");
        return;
    }
    graph_list->number_of_vertex++;
}

// 간선 추가
void insert_edge(GraphTypeList* graph_line, int s, int e, int weight) {
    if (s >= graph_line->number_of_vertex || e >= graph_line->number_of_vertex) {
        error("잘못된 정점 번호");
        return;
    }
    GraphNode* newNode = (GraphNode*)malloc(sizeof(GraphNode));
    newNode->vertex = e;
    newNode->weight = weight;
    newNode->link = NULL;

    if (graph_line->adj_list[s] == NULL) {
        graph_line->adj_list[s] = newNode;
    }
    else {
        GraphNode* current = graph_line->adj_list[s];
        while (current->link != NULL) {
            current = current->link;
        }
        current->link = newNode;
    }
}

// Dijkstra Algorithm
void shortest_path_list(GraphTypeList* graph_list, int start) {
    HeapType* h = create();
    init_heap(h);

    int visited_of_order[MAX]; // 노드 방문 순서를 저장하는 배열
    int order_count = 0; // 방문한 노드의 수를 카운트
    GraphNode* node;

    for (int i = 0; i < graph_list->number_of_vertex; i++) {
        distance[i] = INF;
        found[i] = 0;
        visited_of_order[i] = -1;
    }

    distance[start] = 0;

    node = (GraphNode*)malloc(sizeof(GraphNode));
    node->vertex = start;
    node->weight = 0;
    insert_heap(h, node);

    while (h->heapsize > 0) {
        node = delete_heap(h);
        int current = node->vertex;

        if (!found[current]) {
            found[current] = 1;
            visited_of_order[order_count++] = current;
            GraphNode* adjNode = graph_list->adj_list[current];

            while (adjNode != NULL) {
                int v = adjNode->vertex;
                if (found[v] == 0 && distance[v] > distance[current] + adjNode->weight) {
                    distance[v] = distance[current] + adjNode->weight;
                    GraphNode* newNode = (GraphNode*)malloc(sizeof(GraphNode));
                    newNode->vertex = v;
                    newNode->weight = distance[v];
                    insert_heap(h, newNode);
                }
                adjNode = adjNode->link;
                print_status(distance, found, graph_list->number_of_vertex);
            }
        }
        free(node);
    }

    for (int i = 0; i < order_count; i++) {
        printf("%d ", visited_of_order[i] + 1);
    }
    printf("\n");

    free(h);
}


// 힙 삽입
void insert_heap(HeapType* h, GraphNode* node) {
    int i = ++h->heapsize;

    while (i != 1 && node->weight <= h->heap[i / 2]->weight) {
        h->heap[i] = h->heap[i / 2];
        i = i / 2;
    }
    h->heap[i] = node;
}


// 힙 삭제
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

// 인접행렬 그래프 생성
GraphTypeMatrix* make_graph_matrix() {
    GraphTypeMatrix* g = create_matrix();
    init_graph_matrix(g);
    g->n = 10;
    int weight[10][10] = {
        {0, 3, INF, INF, INF, 11, 12, INF, INF, INF}, //1
        {3, 0, 5, 4, 1, 7, 8, INF, INF, INF}, // 2
        {INF, 5, 0, 2, INF, INF, 6, 5, INF, INF}, // 3
        {INF, 4, 2, 0, 13, INF, INF, 14, INF, 16}, // 4
        {INF, 1, INF, 13, 0, 9, INF, INF, 18, 17}, // 5
        {11, 7, INF, INF, 9, 0, INF, INF, INF, INF}, // 6
        {12, 8, 6, INF, INF, INF, 0, 13, INF, INF}, // 7
        {INF, INF, 5, 14, INF, INF, 13, 0, INF, 15}, // 8
        {INF, INF, INF, INF, 18, INF, INF, INF, 0, 10}, // 9
        {INF, INF, INF, 16, INF, INF, INF, 15, 10, 0} // 10
    };

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            g->weight[i][j] = weight[i][j];
        }
    }
    return g;
}

// Dijkstra Algorithm
void shortest_path_matrix(GraphTypeMatrix* g, int start) {
    int u, w;
    int visited_order[MAX];
    int order_count = 0;

    for (int i = 0; i < g->n; i++) {
        distance[i] = g->weight[start][i];
        found[i] = 0;
    }
    found[start] = 1;
    distance[start] = 0;
    visited_order[order_count++] = start;

    for (int j = 0; j < g->n - 1; j++) {
        print_status(distance, found, g->n);
        u = choose_short_path(distance, g->n, found); // 최단 거리의 정점 선택
        found[u] = 1;
        visited_order[order_count++] = u;

        for (w = 0; w < g->n; w++) {
            if (!found[w] && g->weight[u][w] != INF) {
                if (distance[u] + g->weight[u][w] < distance[w]) {
                    distance[w] = distance[u] + g->weight[u][w];
                }
            }
        }
    }

    for (int i = 0; i < order_count; i++) {
        printf("%d ", visited_order[i] + 1);
    }
    printf("\n");
}

// 최단 거리 선택
int choose_short_path(int distance[], int n, int found[]) {
    int min = INF;
    int minpos = -1;
    for (int i = 0; i < n; i++) {
        if (distance[i] < min && found[i] == 0) {
            min = distance[i];
            minpos = i;
        }
    }
    return minpos;
}

// 인접행렬 초기화
void init_graph_matrix(GraphTypeMatrix* g) {
    g->n = 0;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            g->weight[i][j] = 0;
        }
    }
}

// 인접행렬 생성
GraphTypeMatrix* create_matrix() {
    return (GraphTypeMatrix*)malloc(sizeof(GraphTypeMatrix));
}

// 상태 출력
void print_status(int distance[], int found[], int n) {
    printf("Distance: ");
    for (int i = 0; i < n; i++) {
        if (distance[i] != INF) {
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
