#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTEX 10
#define TRUE 1
#define FALSE 0

typedef struct graphNode {                     //인접 리스트의 노드를 구조체로 정의
    int vertex;
    struct graphNode* link;
} graphNode;

typedef struct graphType {                     //그래프를 인접 리스트로 표현하기 위해 구조체로 정의
    int n;
    graphNode* adjList_H[MAX_VERTEX];
    int visited[MAX_VERTEX];
} graphType;

void createGraph(graphType* graph);
void insertVertex(graphType* graph, int v);
void insertEdge(graphType* graph, int u, int v);
void DFS(graphType* graph, int v);
void BFS(graphType* graph, int v);
void print_AdjList(graphType* graph);
void freeGraph(graphType* graph);

int main() {
    graphType* graph = (graphType*)malloc(sizeof(graphType));    //graphType 구조체의 크기만큼 메모리를 동적 할당 및 graph포인터로 가리킴
    createGraph(graph);                                          //그래프 생성
    char command;
    int vertex, src, dest;
    while (1) {
        printf("----------------------------------------------------------------\n");
        printf("                         Graph Searches                         \n");
        printf("----------------------------------------------------------------\n");
        printf(" Initialize Graph = z                                           \n");
        printf(" Insert Vertex = v              Insert Edge = e                 \n");
        printf(" Depth First Search = d         Breath First Search = b         \n");
        printf(" Print Graph = p                Quit = q                        \n");
        printf("----------------------------------------------------------------\n");

        printf("Command: ");
        scanf(" %c", &command);

        switch (command) {
            case 'z':
                freeGraph(graph);
                createGraph(graph);
                printf("Graph is initialized.\n");
                break;
            case 'v':
                printf("Enter vertex (0-9): ");
                scanf("%d", &vertex);
                if (vertex >= 0 && vertex < MAX_VERTEX) {
                    insertVertex(graph, vertex);
                    printf("Vertex %d is inserted.\n", vertex);
                } else {
                    printf("It is Invalid vertex.\n");
                }
                break;
            case 'e':
                printf("Enter source and destination vertices (0-9): ");
                scanf("%d %d", &src, &dest);
                if (src >= 0 && src < MAX_VERTEX && dest >= 0 && dest < MAX_VERTEX) {
                    insertEdge(graph, src, dest);
                    printf("Edge (%d -> %d) inserted.\n", src, dest);
                } else {
                    printf("It is Invalid vertices.\n");
                }
                break;
            case 'd':
                printf("Enter starting vertex for DFS (0-9): ");
                scanf("%d", &vertex);
                if (vertex >= 0 && vertex < MAX_VERTEX) {
                    for (int i = 0; i < MAX_VERTEX; i++)
                        graph->visited[i] = 0;
                    DFS(graph, vertex);
                    printf("\n");
                } else {
                    printf("It is Invalid vertex.\n");
                }
                break;
            case 'b':
                printf("Enter starting vertex for BFS (0-9): ");
                scanf("%d", &vertex);
                if (vertex >= 0 && vertex < MAX_VERTEX) {
                    for (int i = 0; i < MAX_VERTEX; i++)
                        graph->visited[i] = 0;
                    BFS(graph, vertex);
                    printf("\n");
                } else {
                    printf("It is Invalid vertex.\n");
                }
                break;
            case 'p':
                print_AdjList(graph);
                break;
            case 'q':
                freeGraph(graph);
                free(graph);
                printf("Exiting program.\n");
                return 0;
            default:
                printf("It is Invalid command.\n");
                break;
        }
    }
}

void createGraph(graphType* graph)  {                    //그래프를 새로 생성해내는 함수
    int v;
    graph->n = 0;                                        //정점 수 = 0으로 초기화
    for (v = 0; v < MAX_VERTEX; v++) {                   
        graph->adjList_H[v] = NULL;                      //그래프의 인접리스트는 없다고 초기화
        graph->visited[v] = FALSE;                       //정점을 방문한 적이 없다고 하기 위해 FALSE로 초기화
    }
}

void insertVertex(graphType* graph, int v) {             //정점을 삽입하는 함수
    if (graph->n >= MAX_VERTEX) {                        //이미 꽉 찬 경우에는
        printf("\nVertex Repository is Full");           //더이상 정점을 삽입 할 수 없다는 메세지 출력 후 종료
        return; 
    }
    if (v >= MAX_VERTEX) {                               //삽입 하려는 정점이 MAX_VERTEX보다 크거나 같은경우(시작이 0부터이기 때문에)
        printf("\n It is invalid number");               //올바른 숫자가 아님을 출력하고 종료
        return;
    }
    graph->n++;                                          //정점 수 증가
}

void insertEdge(graphType* graph, int u, int v) {                    //간선을 삽입하는 함수
    if (u < 0 || u >= MAX_VERTEX || v < 0 || v >= MAX_VERTEX) {      //정점 들이 0~9범위 사이에 있는지 여부 판단
        printf("\n Vertex isn't included in Graph");
        return;                                                      //제 범위에 없으면 종료
    } 
 
    graphNode* node = (graphNode*)malloc(sizeof(graphNode));         //새 노드인 node를 동적으로 할당 시킴
    if (node == NULL) {
        printf("\n Allocation is failed");
        return;
    }
    node->vertex = v;                                                //새 노드의 정점은 v로 설정
    node->link = NULL;                                               //그 새 노드는 link로 연결하는 것이 없음

    if (graph->adjList_H[u] == NULL) {                               //만약 u에 해당하는 인접리스트가 비어있으면
        graph->adjList_H[u] = node;                                  // 그 인접리스트의 첫 노드는 새 노드(node)로 설정
    } else {                                                         //반대로 u에 해당하는 인접리스트가 비어있지 않으면
        graphNode* temp = graph->adjList_H[u];                       //temp 노드 = 인접 리스트의 노드(시작점) 임시로 설정
        while (temp->link != NULL) {                                 //리스트의 끝에 도달할 때까지 반복
            temp = temp->link;
        }
        temp->link = node;                                           //리스트의 끝에 새 노드를 연결 시킴
    }
}

void DFS(graphType* graph, int v) {                                  //깊이 우선 탐색 함수
    graphNode* w;                                                    //노드 w 생성
    graph->visited[v] = TRUE;                                        //정점 v를 방문했다고 설정 = visited배열 값을 TRUE로 설정
    printf("%d ", v);                                                //방문한 정점 출력
    for (w = graph->adjList_H[v]; w; w = w->link) {                  //인접 리스트의 시작부터 w가 있을 때까지 , 다음 인접 노드로 이동하며 반복
        if (!graph->visited[w->vertex]) {                            //만약 해당 노드에 방문한 적이 없으면
            DFS(graph, w->vertex);                                   //DFS함수를 재귀적으로 호출
        }
    }
}

void BFS(graphType* graph, int v) {                                   //너비 우선 탐색 함수
    int queue[MAX_VERTEX];                                            //큐를 사용하여 구현하기 위해 큐 배열 선언
    int front = 0, rear = 0;                                          //초기에 front=rear=0으로 설정

    graph->visited[v] = TRUE;                                         //시작 정점 v를 방문했다고 설정 = visited배열 값을 TRUE로 설정
    printf("%d ", v);                                                 //방문한 정점 출력
    queue[rear++] = v;                                                //일단 큐에 v를 넣고 큐의 rear를 한 칸 뒤로 이동

    while (front != rear) {                                           //큐가 비어있지 않는 경우에 반복
        v = queue[front++];                                           //큐의 front의 요소를 v에 할당하고, front를 한 칸 뒤로 이동 즉, 큐에서 꺼냄
        for (graphNode* w = graph->adjList_H[v]; w; w = w->link) {    //인접 리스트의 시작부터 w가 있을 때까지 , 다음 인접 노드로 이동하며 반복
            if (!graph->visited[w->vertex]) {                         //만약 해당 노드에 방문한 적이 없으면
                graph->visited[w->vertex] = TRUE;                     //그 노드에 해당하는 정점을 방문했다고 설정 = visited배열 값을 TRUE로 설정
                printf("%d ", w->vertex);                             //해당 정점 출력
                queue[rear++] = w->vertex;                            //해당 정점을 큐에 넣고 큐의 rear를 한 칸 뒤로 이동
            }
        }
    }
}

void print_AdjList(graphType* graph) {                                 //인접 리스트를 출력하는 함수
    graphNode* p;                                                      //그래프 노드를 가리키는 포인터 p 선언
    for (int i = 0; i < graph->n; i++) {                               //정점 개수 만큼 인접리스트가 존재하므로 정점 개수 만큼 반복
        printf("\n\t\t정점 %d의 인접 리스트", i);    
        p = graph->adjList_H[i];                                       //p는 그래프의 해당 정점 인접리스트의 헤드를 가리킴
        while (p) {                                                    //해당 인접리스트의 끝에 도달할 때 까지 반복
            printf("-> %d", p->vertex);                                //리스트에 있는 정점들을 각각 출력
            p = p->link;                                               //다음 인접하는 정점으로 이동
        }
    }
    printf("\n");
}

void freeGraph(graphType* graph) {                                     //그래프의 메모리를 초기화 하는 함수
    for (int i = 0; i < MAX_VERTEX; i++) {                             //최대 정점 수만큼 반복
        graphNode* p = graph->adjList_H[i];                            //포인터 p를 선언, p는 그래프의 해당 정점 인접리스트의 헤드를 가리킴
        while (p) {                                                    //인접리스트의 끝에 도달할 때까지 반복
            graphNode* temp = p;                                       //현재 노드를  노드를 가리키는 임시 포인터 temp에 저장
            p = p->link;                                               //p를 다음 노드로 이동
            free(temp);                                                //temp포인터가 가리키는 노드의 메모리해제
        }
        graph->adjList_H[i] = NULL;                                    //해당 정점의 인접 리스트를 다 돌았으면 헤더를 NULL로 다시 초기화
    }
    graph->n = 0;                                                      //정점 수를 0으로 다시 초기화
}
