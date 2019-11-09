#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

struct Vertex;
struct VertexNode;
struct Edge;
struct EdgeNode;

// A vertex is a 2D point
typedef struct Vertex {
	int x; // x-coordinate
	int y; // y-coordinate

	int distance;
	int vis;
	struct Vertex *previous;
} Vertex;

// each edge is a pair of vertices (end-points)
typedef struct Edge {
	Vertex *p1; // first end point
	Vertex *p2; // second end point
} Edge;

typedef struct EdgeNode {
	Edge *e;
	struct EdgeNode *next;
} EdgeNode;

typedef struct VertexNode {
	Vertex *v;
	struct VertexNode *next;
	struct EdgeNode *head;
} VertexNode;

typedef struct GraphRep *Graph;
typedef struct GraphRep { // graph header
	VertexNode *vertices; // an array of vertices or a linked list of vertices
	int nV; // #vertices
	int nE; // #edges
} GraphRep;
// A vertex node stores a vertex and other information, and you need to expand this type

//The above types serve as a starting point only. You need to expand them and add more types.
// Watch the lecture video between 7:50pm-8:20 or so for my discussion about this assignment

// Add the time complexity analysis of CreateEmptyGraph() here
/*
O(1)
there are four operations whose time complexity are all O(1),
so the total complexity is O(1)
*/
Graph CreateEmptyGraph()
{
	Graph g = (Graph )malloc(sizeof(GraphRep));
	g->vertices = NULL;
	g->nV = 0;
	g->nE = 0;
	return g;
}

/*
O(1)
two equation are both O(1) and O(1) + O(1) = O(1)
*/
int isVertexEqual(Vertex *p, Vertex *q) {
	return p->x == q->x && p->y == q->y;
}

/*
O(1)
isVertexEqual is O(1) and there are two calls so O(1) + O(1) = O(1)
*/
int isEdgeEqual(Edge *a, Edge *b) {
	return isVertexEqual(a->p1, b->p1) && isVertexEqual(a->p2, b->p2);
}

/*
O(1)
the creation of a Vertx and five assignments are both O(1), so the total
time complexity is O(1)
*/
Vertex* newVertex(Vertex *v) {
	Vertex *u = (Vertex*)malloc(sizeof(Vertex));
	u->x = v->x;
	u->y = v->y;
	u->vis = 0;
	u->distance = 10000;
	u->previous = NULL;
	return u;
}

/*
O(V)
the function contains a vertexiteration of the graph, every Vertex will
be processed by one time so the complexity is O(V)
*/
VertexNode* getVertexNode(Graph g, Vertex *v, int deleted) {
	VertexNode *current_vertex = g->vertices, *previous = NULL;
	while (current_vertex != NULL) {
		if (isVertexEqual(current_vertex->v, v)) {
			if (deleted) {
				if (previous == NULL) {
					g->vertices = current_vertex->next;
				} else {
					previous->next = current_vertex->next;
				}
				free(current_vertex);
				return NULL;
			}
			return current_vertex;
		}

		previous = current_vertex;
		current_vertex = current_vertex->next;
	}

	return NULL;
}

/*
O(1)
the only phrase is printf, which is O(1)
*/
void printVertex(Vertex *v) {
	printf("(%d,%d)", v->x, v->y);
}

/*
O(1)
the only phrase is printf, which is O(1)
*/
void printEdge(Edge *e) {
	printf("(%d,%d),(%d,%d)", e->p1->x, e->p1->y, e->p2->x, e->p2->y);
}

/*
O(1)
the first phrase is to calcuate the (p->x-q->x)*(p->x-q->x) + (p->y-q->y)*(p->y-q->y),
which complexity is O(1),
then the sqrt function is called, which is also O(1)
so the total complexity is O(1)
*/
double edgeDistance(Vertex *p, Vertex *q) {
	return sqrt((p->x-q->x)*(p->x-q->x) + (p->y-q->y)*(p->y-q->y));
}

// Add the time complexity analysis of InsertEdge() here
/*
O(V + E)
The first phrase is to find the VertexNode of e->p1 and e->p2, which is both O(V)
The second phrase is to iterate the Edge of e->p1 and e->p2, which is both O(E) for
the worst case.
so the total complexity is O(V+E)
*/
int InsertEdge(Graph g, Edge *e)
{
	VertexNode *first_node = getVertexNode(g, e->p1, 0);
	VertexNode *second_node = getVertexNode(g, e->p2, 0);

	if (first_node == NULL) {
		first_node = (VertexNode *)malloc(sizeof(VertexNode));
		first_node->v = newVertex(e->p1);
		first_node->next = g->vertices;
		first_node->head = NULL;
		g->vertices = first_node;
		g->nV ++;
	}

	if (second_node == NULL) {
		second_node = (VertexNode *)malloc(sizeof(VertexNode));
		second_node->v = newVertex(e->p2);
		second_node->next = g->vertices;
		second_node->head = NULL;
		g->vertices = second_node;
		g->nV ++;
	}

	EdgeNode *current_edge = first_node->head;
	while (current_edge != NULL) {
		if ( isEdgeEqual(current_edge->e, e) ) return 0;
		current_edge = current_edge->next;
	}

	EdgeNode* first_edge = (EdgeNode *)malloc(sizeof(EdgeNode));
	EdgeNode* second_edge= (EdgeNode *)malloc(sizeof(EdgeNode));

	Edge* e1 = (Edge*)malloc(sizeof(Edge));
	Edge* e2 = (Edge*)malloc(sizeof(Edge));
	e1->p1 = e2->p2 = first_node->v;
	e1->p2 = e2->p1 = second_node->v;

	first_edge->e = e1;
	second_edge->e = e2;
	first_edge->next = first_node->head;
	second_edge->next = second_node->head;
	first_node->head = first_edge;
	second_node->head = second_edge;

	g->nE ++;
	return 1;
}

// Add the time complexity analysis of DeleteEdge() here
/*
O(V + E)
The first phrase is to find the VertexNode of e->p1, and the
correspond edge, so the complexity is O(V+E)
The second phrase do the same thing for e->p2, which is also
O(V+E)
so the total complexity is O(V+E) + O(V+E) = O(V+E)
*/
void DeleteEdge(Graph g, Edge *e)
{
	VertexNode *first_node = getVertexNode(g, e->p1, 0);
	if (first_node == NULL) return;

	EdgeNode *current_edge, *previous;
	current_edge = first_node->head, previous = NULL;
	while (current_edge != NULL) {
		if ( isVertexEqual(current_edge->e->p2, e->p2) ) {
			if (previous == NULL) {
				first_node->head = current_edge->next;
			} else {
				previous->next = current_edge->next;
			}
			free(current_edge);
			break;
		}
		previous = current_edge;
		current_edge = current_edge->next;
	}

	VertexNode *second_node = getVertexNode(g, e->p2, 0);
	current_edge = second_node->head, previous = NULL;
	while (current_edge != NULL) {
		if ( isVertexEqual(current_edge->e->p2, e->p1) ) {
			if (previous == NULL) {
				second_node->head = current_edge->next;
			} else {
				previous->next = current_edge->next;
			}
			free(current_edge);
			break;
		}
		previous = current_edge;
		current_edge = current_edge->next;
	}

	g->nE --;
	if (first_node->head == NULL) {
		getVertexNode(g, e->p1, 1);
		g->nV --;
	}
	if (second_node->head == NULL) {
		getVertexNode(g, e->p2, 1);
		g->nV --;
	}
}

// Add the time complexity analysis of ReachableVertices() here
/*
O(V*V+E)
ReachableVertices uses bfs to implement. The body of the while loop will
be processed V times, each time will iterate the vertices of the graph.
Every edge will also be processed by only one time, so the total complexity
is O(V*V) + O(E) = O(V*V+E)
*/
void ReachableVertices(Graph g, Vertex *v)
{
	VertexNode* current_vertex = g->vertices;
	while (current_vertex != NULL) {
		current_vertex->v->distance = 10000;
		current_vertex->v->vis = 0;

		if (isVertexEqual(current_vertex->v, v)) {
			current_vertex->v->distance = 0;
		}
		current_vertex = current_vertex->next;
	}

	while (1) {
		VertexNode *min = NULL;
		current_vertex = g->vertices;
		while (current_vertex != NULL) {
			if (!current_vertex->v->vis && (min == NULL || current_vertex->v->distance < min->v->distance))
				min = current_vertex;
			current_vertex = current_vertex->next;
		}
		if (min == NULL || min->v->distance >= 10000) break;
		min->v->vis = 1;
		EdgeNode *current_edge = min->head;
		while (current_edge != NULL) {
			int now = current_edge->e->p2->distance;
			int update = min->v->distance + edgeDistance(current_edge->e->p1, current_edge->e->p2);
			if (update < now) {
				current_edge->e->p2->distance = now > update ? update : now;
				current_edge->e->p2->previous = current_edge->e->p1;
			}
			current_edge = current_edge->next;
		}
	}

	current_vertex = g->vertices;
	while (current_vertex != NULL) {
		if (current_vertex->v->vis == 0) {
			current_vertex =current_vertex->next;
			continue;
		}
		if (!isVertexEqual(current_vertex->v, v))
			printVertex(current_vertex->v); printf(" ");
		current_vertex = current_vertex->next;
	}
	printf("\n");
}

// Add the time complexity analysis of ShortestPath() here
/*
O(V*V+E)
ShortestPath uses bfs to implement. The body of the while loop will
be processed V times, each time will iterate the vertices of the graph.
Every edge will also be processed by only one time, so the total complexity
is O(V*V) + O(E) = O(V*V+E)
*/
void ShortestPath(Graph g, Vertex *u, Vertex *v)
{
	VertexNode* current_vertex = g->vertices;
	while (current_vertex != NULL) {
		current_vertex->v->distance = 10000;
		current_vertex->v->vis = 0;

		if (isVertexEqual(current_vertex->v, u)) {
			current_vertex->v->distance = 0;
			current_vertex->v->previous = NULL;
		}
		current_vertex = current_vertex->next;
	}

	while (1) {
		VertexNode *min = NULL;
		current_vertex = g->vertices;
		while (current_vertex != NULL) {
			if (!current_vertex->v->vis && (min == NULL || current_vertex->v->distance < min->v->distance))
				min = current_vertex;
			current_vertex = current_vertex->next;
		}
		if (min == NULL || min->v->distance >= 10000) break;
		min->v->vis = 1;
		EdgeNode *current_edge = min->head;
		while (current_edge != NULL) {
			int now = current_edge->e->p2->distance;
			int update = min->v->distance + edgeDistance(current_edge->e->p1, current_edge->e->p2);
			if (update < now) {
				current_edge->e->p2->distance = now > update ? update : now;
				current_edge->e->p2->previous = current_edge->e->p1;
			}
			current_edge = current_edge->next;
		}
	}

	Vertex* nodes[g->nV];
	int num = 0;
	Vertex* cur = getVertexNode(g, v, 0)->v;
	if (!cur->vis) return;
	while (cur != NULL) {
		nodes[num ++] = cur;
		cur = cur->previous;
	}
	for (int i = num - 1; i >= 0; i--) {
		printVertex(nodes[i]); printf(" ");
	}
	printf("\n");
}

// Add the time complexity analysis of FreeGraph() here
/*
O(V + E)
every vertex will be processed and every edge will also be processed
by only one time, so the total complexity is O(V) + O(E) = O(V + E)
*/
void FreeGraph(Graph g)
{
	VertexNode* current_vertex = g->vertices, *previous_vertex = NULL;
	while (current_vertex != NULL) {
		EdgeNode *current_edge = current_vertex->head, *previous_edge = NULL;
		while (current_edge != NULL) {
			previous_edge = current_edge;
			current_edge = current_edge->next;
			free(previous_edge);
		}
		previous_vertex = current_vertex;
		current_vertex = current_vertex->next;
		free(previous_vertex);
	}
	free(g);
}

// Add the time complexity analysis of ShowGraph() here
/*
O(V*V+E)
ShowGraph uses bfs to implement. The body of the while loop will
be processed V times, each time will iterate the vertices of the graph.
Every edge will also be processed by only one time, so the total complexity
is O(V*V) + O(E) = O(V*V+E)
*/
void ShowGraph(Graph g)
{
	VertexNode* current_vertex = g->vertices;
	while (current_vertex != NULL) {
		current_vertex->v->distance = 10000;
		current_vertex->v->vis = 0;
		current_vertex = current_vertex->next;
	}

	current_vertex = g->vertices;
	while (current_vertex != NULL) {
		if (current_vertex->v->vis) {
			current_vertex = current_vertex->next;
			continue;
		}

		current_vertex->v->distance = 0;

		while (1) {
			VertexNode* min = NULL;
			VertexNode* u = g->vertices;
			while (u != NULL) {
				if (!u->v->vis && (min == NULL || u->v->distance < min->v->distance))
					min = u;
				u = u->next;
			}
			if (min == NULL || min->v->distance >= 10000) break;
			min->v->vis = 1;
			EdgeNode *current_edge = min->head;
			while (current_edge != NULL) {
				int now = current_edge->e->p2->distance;
				int update = min->v->distance + edgeDistance(current_edge->e->p1, current_edge->e->p2);
				if (update < now) {
					current_edge->e->p2->distance = now > update ? update : now;
					current_edge->e->p2->previous = current_edge->e->p1;
				}
				if (!current_edge->e->p2->vis) {
					printEdge(current_edge->e); printf(" ");
				}
				current_edge = current_edge->next;
			}
		}

		current_vertex = current_vertex->next;
	}
	printf("\n");
}

int main() //sample main for testing
{
 Graph g1;
 Edge *e_ptr;
 Vertex *v1, *v2;

 // Create an empty graph g1;
 g1=CreateEmptyGraph();

 // Create first connected component
 // Insert edge (0,0)-(0,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=0;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Insert edge (0,0)-(5,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Insert edge (0, 10)-(10, 10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=10;
 v2->x=10;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Insert edge (0,10)-(5,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=10;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Insert edge (0,0)-(5,4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Insert edge (5, 4)-(10, 4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=5;
 v1->y=4;
 v2->x=10;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Insert edge (5,6)-(10,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=5;
 v1->y=6;
 v2->x=10;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Insert edge (10,10)-(10,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=10;
 v1->y=10;
 v2->x=10;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Insert edge (10, 6)-(10, 4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=10;
 v1->y=6;
 v2->x=10;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Create second connected component
 // Insert edge (20,4)-(20,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=20;
 v1->y=4;
 v2->x=20;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Insert edge (20,10)-(30,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=20;
 v1->y=10;
 v2->x=30;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 // Insert edge (25,5)-(30,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=25;
 v1->y=5;
 v2->x=30;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");

 //Display graph g1
 ShowGraph(g1);

 // Find the shortest path between (0,0) and (10,6)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=10;
 v2->y=6;
 ShortestPath(g1, v1, v2);
 free(v1);
 free(v2);

 // Delete edge (0,0)-(5, 6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 DeleteEdge(g1, e_ptr);
 free(e_ptr);
 free(v1);
 free(v2);

 // Display graph g1
 ShowGraph(g1);

 // Find the shortest path between (0,0) and (10,6)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=10;
 v2->y=6;
 ShortestPath(g1, v1, v2);
 free(v1);
 free(v2);

 // Find the shortest path between (0,0) and (25,5)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=25;
 v2->y=5;
 ShortestPath(g1, v1, v2);
 free(v1);
 free(v2);

 // Find reachable vertices of (0,0)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v1->x=0;
 v1->y=0;
 ReachableVertices(g1, v1);
 free(v1);

 // Find reachable vertices of (20,4)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v1->x=20;
 v1->y=4;
 ReachableVertices(g1, v1);
 free(v1);

 // Free graph g1
 FreeGraph(g1);

 return 0;
}
