/**
 * Group ID - 
 * Member 1 Name - 
 * Member 1 BITS ID - 
 * Member 2 Name - 
 * Member 2 BITS ID - 
 * Member 3 Name - 
 * Member 3 BITS ID - 
*/
#define INT_MAX 2147483647
// ---------------------------DO NOT MODIFY (Begin)------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


typedef struct Graph {
    int n; // Size of the graph
    int** adj; // Adjacency matrix
    char** station_names; // Array of station names
} Graph;

/**
 * This function has been pre-filled. It reads the input testcase 
 * and creates the Graph structure from it.
*/
Graph* create_graph(char input_file_path[]) {
    FILE* f = fopen(input_file_path, "r");
    Graph* g = (Graph*)malloc(sizeof(Graph));
    // Size of graph
    fscanf(f, "%d", &(g->n));
    // Names of stations
    g->station_names = (char**)malloc(g->n * sizeof(char*));
    for(int i = 0; i < g->n; i++) {
        g->station_names[i] = (char*)malloc(100 * sizeof(char));
        fscanf(f, "%s", g->station_names[i]);
    }
    // Adjacency matrix
    g->adj = (int**)malloc(g->n * sizeof(int*));
    for(int i = 0; i < g->n; i++) {
        g->adj[i] = calloc(g->n, sizeof(int));
    }
    int edges;
    fscanf(f, "%d", &edges);
    for(int i = 0; i < edges; i++) {
        int x, y;
        fscanf(f, "%d%d", &x, &y);
        g->adj[x][y] = 1;
        g->adj[y][x] = 1;
    }
    fclose(f);
    return g;
}
// ---------------------------DO NOT MODIFY (End)------------------------------

/**
 * Q.1 
 * Return the number of junctions.
*/
int find_junctions(Graph* g) {
	int** graph = g->adj;
	int n = g->n;
	
	int junction_count = 0;
	for(int i = 0; i < n; i++){
		int degree = 0;
		for(int j = 0; j < n; j++){
			if(graph[i][j]==1){
				degree++;
			}
		}
		if(degree >= 4){
			junction_count++;
		}
	}
	return junction_count;
}

/**
 * Q.2
 * Return true if the tour specified in the question is possible for this 
 * graph, else return false.
 * Parameter SAME_STATION - If it is true, then question 2(a) must be solved. 
 * If false, then question 2(b) must be solved.
*/
bool sheldons_tour(Graph* g, bool SAME_STATION) {
	int** graph = g->adj;
	int n = g->n;
	int even_degree = 0;
	int odd_degree = 0;
	for(int i = 0; i < n; i++){
		int degree = 0;
		for(int j = 0; j < n; j++){
			if(graph[i][j]==1){
				degree++;
			}
		}
		if(degree%2==0){
			even_degree++;
		}else{
			odd_degree++;
		}
	}
	if(SAME_STATION==true){
		return even_degree==n ? true : false;
	}else{
		return odd_degree==2 ? true : false;
	}
}

/**
 * Q.3
 * Return closure, an n * n matrix filled with 0s and 1s as required. 
*/
int** warshall(Graph* g) {
    // Do not modify or delete pre-filled code!
    int** closure = (int**)malloc(g->n * sizeof(int*));
    for(int i = 0; i < g->n; i++) {
        closure[i] = calloc(g->n, sizeof(int));
    }

    // Code goes here
	int** graph = g->adj;
	int n = g->n;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			closure[i][j] = graph[i][j];
		}
	}
	for(int k = 0; k < n; k++){
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				if(closure[i][j] || closure[i][k] && closure[k][j]){
					closure[i][j]=1;
				}
			}
		}
	}
    
    return closure; // Do not modify
}

/**
 * Q.3
 * Return the number of impossible pairs.
*/
int find_impossible_pairs(Graph* g) {
    int** closure = warshall(g); // Do not modify
	int** graph = g->adj;
	int n = g->n;
	int disconnected_pairs = 0;
	for(int i = 0; i < n; i++){
		for(int j = i; j < n; j++){
			if(closure[i][j]==0){
				disconnected_pairs++;
			}
		}
	}
	return disconnected_pairs;
    
}

/**
 * Q.4
 * Return the number of vital train tracks.
*/
int find_vital_train_tracks(Graph* g) {
	int** closure =  warshall(g);
	int** graph = g->adj;
	int n = g->n;
	// int** temp_graph = (int**)calloc(n,sizeof(int*));
	// for(int i = 0; i < n; i++){
	// 	temp_graph[i] = (int*)calloc(n,sizeof(int));
	// 	for(int j = 0; j < n; j++){
	// 		temp_graph[i][j] = graph[i][j];
	// 	}
	// }
	// struct Graph* TEMP;
	// TEMP->adj = temp_graph;
	// TEMP->n = n;
	// TEMP->station_names = NULL;
	
	int vital_edges = 0;
		
	for(int i = 0; i < n; i++){
		for(int j = i; j < n; j++){
			if(graph[i][j]==1){
				graph[i][j] = 0;
				graph[j][i] = 0;
				int** temp_closure = warshall(g);
				bool flag = true;
				for(int p = 0; p < n; p++){
					if(flag == false){
						break;
					}
					for(int q = p; q < n; q++){
						if(temp_closure[p][q]==0 && closure[p][q]==1){
							vital_edges++;
							flag = false;
							break;
						}
					}
				}
				graph[i][j] = 1;
				graph[j][i] = 1;
			}
		}
	}
	return vital_edges;
}

/**
 * Q.5
 * Return upgrades, an array of size n.
 * upgrades[i] should be 0 if railway station i gets a restaurant
 * upgrades[i] should be 1 if railway station i gets a maintenance depot
 * If it is not possible to upgrade, then return "-1" in the entire array
*/
int* upgrade_railway_stations(Graph* g) {
    int* upgrades = calloc(g->n, sizeof(int)); // Do not modify
    
    // Code goes here
	int** graph = g->adj;
	int n = g->n;
	int queue_front = 0;
	int queue_back = 1;
	int queue_capacity = 100;
	int Q[queue_capacity];
	bool visited[n];
	for(int i = 0; i < n; i++){
		visited[i] = false;
	}
	for(int i = 0; i < queue_capacity; i++){
		Q[i] = -1;
	}
	Q[0] = 0;
	upgrades[0] = 1;
	bool flag = true;
	while(Q[queue_front]!=-1){
		int city = Q[queue_front];
		for(int i = 0; i < n; i++){
			if(graph[city][i]==1){
				if(visited[i]==true){
					if(upgrades[city]==upgrades[i]){
						flag = false;	
						break;
					}
				}else{
					if(upgrades[city]==1){
						upgrades[i]=0;
					}else{
						upgrades[i]=1;
					}
					Q[queue_back]=i;
					queue_back++;
					visited[i]=true;
				}
			}
		}
		if(flag==false){
			for(int i = 0; i < n; i++){
				upgrades[i]=-1;
			}
			break;
		}
		queue_front++;
	}
	

    return upgrades; // Do not modify
}

/**
 * Q.6
 * Return distance between X and Y
 * city_x is the index of X, city_y is the index of Y
*/
int distance(Graph* g, int city_x, int city_y) {
	int **graph = g->adj;
	int n = g->n;
	
	int queue_capacity = n + 1;
	int qf = 0;
	int qb = 1;
	int Q[queue_capacity][2];
	
	bool visited[n];
	bool inqueue[n];
	for(int i = 0; i < n; i++){
		visited[i] = false;
		inqueue[i] = false;
	}
	for(int i = 0; i < queue_capacity; i++){
		Q[i][0] = -1;
	}
	Q[0][0] = city_x;
	Q[0][1] = 0;
	inqueue[city_x] = true;
	while(Q[qf][0]!=-1){
		int city = Q[qf][0];
		int lvl = Q[qf][1] + 1;
		visited[city] = true;
		for(int i = 0; i < n; i++){
			if(!visited[i] && graph[city][i]==1){
				if(i==city_y){
					printf("city=%d city_y=%d\n",city,city_y);
					return lvl;
				}
				else if(!inqueue[i]){
					printf("i=%d qb=%d\n",i,qb);
					Q[qb][0] = i;
					Q[qb][1] = lvl;
					inqueue[i] = true;
					qb++;
				}
			}
		}
		qf++;
	}
	return -1;


}

/**
 * Q.7
 * Return the index of any one possible railway capital in the network
*/
int railway_capital(Graph* g) {
	int **graph = g->adj;
	int n = g->n;
	int min_dist = INT_MAX;
	int station_no = -1;
	for(int i = 0; i < n; i++){
		int tot_dist = 0;
		for(int j = 0; j < n; j++){
			tot_dist+=distance(g,i,j);
		}
		if(tot_dist < min_dist){
			min_dist = tot_dist;
			station_no = i;
		}
	}
	return station_no;

}

/**
 * Helper function for Q.8
*/
bool maharaja_express_tour(Graph* g, int source, int current_city, int previous_city, int* visited) {
    
}

/**
 * Q.8 
 * Return true if it is possible to have a Maharaja Express tour 
 * starting from source city, else return false. 
*/
bool maharaja_express(Graph* g, int source) {
    int* visited = (int*)malloc(g->n * sizeof(int)); 
    for(int i = 0; i < g->n; i++) {
        visited[i] = 0;
    }
    // Hint: Call the helper function and pass the visited array created here.
}

void printGraph(int** graph,int n){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			printf("%d ",graph[i][j]);
		}
		printf("\n");
	}
}
int main() {
    char input_file_path[100] = "testcase_2.txt"; // Can be modified
    Graph* g = create_graph(input_file_path); // Do not modify
	// int juncs = find_junctions(g);
	// bool p1 = sheldons_tour(g,true);
	// bool p2 = sheldons_tour(g,false);
	// int** closure = warshall(g);
	// int impossible_pairs = find_impossible_pairs(g);
	// int vital_tracks = find_vital_train_tracks(g);
	// int* upgrades = upgrade_railway_stations(g);
	int dist=distance(g,1,4);
	int capital_station = railway_capital(g);

	// printf("juncs=%d p1=%d p2=%d",juncs,p1,p2);
	printGraph(g->adj,g->n);
	// printf("ip=%d vt=%d",impossible_pairs,vital_tracks);
	printf("\n");
	// for(int i = 0; i < g->n; i++){
	// 	printf("%d ",upgrades[i]);
	// }
	printf("dist=%d\n",dist);
	printf("capital=%d\n",capital_station);
    
    // Code goes here

    return 0;
}
