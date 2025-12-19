#pragma once
#include <iostream>
#include "movieAVL.h"
#include "customString.h"
using namespace std;

class QueueNode {
public:
    int data;
    QueueNode* next;
    
    QueueNode(int val) {
        data = val;
        next = nullptr;
    }
};

class Queue {
    QueueNode* frontPtr;
    QueueNode* rearPtr;

public:
    Queue() {
        frontPtr = nullptr;
        rearPtr = nullptr;
    }
    
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    
    void enqueue(int value) {
        QueueNode* newNode = new QueueNode(value);
        if (isEmpty()) {
            frontPtr = rearPtr = newNode;
        } else {
            rearPtr->next = newNode;
            rearPtr = newNode;
        }
    }
    
    int dequeue() {
        if (isEmpty()) return -1;
        
        QueueNode* temp = frontPtr;
        int value = temp->data;
        frontPtr = frontPtr->next;
        if (frontPtr == nullptr) rearPtr = nullptr;
        delete temp;
        return value;
    }
    
    bool isEmpty() {
        return frontPtr == nullptr;
    }
};

class StackNode {
public:
    int data;
    StackNode* next;
    
    StackNode(int val) {
        data = val;
        next = nullptr;
    }
};

class Stack {
    StackNode* topPtr;

public:
    Stack() {
        topPtr = nullptr;
    }
    
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    void push(int value) {
        StackNode* newNode = new StackNode(value);
        newNode->next = topPtr;
        topPtr = newNode;
    }
    
    int pop() {
        if (isEmpty()) return -1;
        
        StackNode* temp = topPtr;
        int value = temp->data;
        topPtr = topPtr->next;
        delete temp;
        return value;
    }
    
    bool isEmpty() {
        return topPtr == nullptr;
    }
};

class Edge {
public:
    int targetIndex;
    string connectionType;
    string connectionDetail;
    double weight;
    Edge* next;

    Edge(int target, string type, string detail, double w) {
        targetIndex = target;
        connectionType = type;
        connectionDetail = detail;
        weight = w;
        next = nullptr;
    }
};

class AdjList {
public:
    Edge* head;
    int edgeCount;

    AdjList() {
        head = nullptr;
        edgeCount = 0;
    }

    ~AdjList() {
        Edge* current = head;
        while (current) {
            Edge* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void addEdge(int target, string type, string detail, double weight) {
        Edge* current = head;
        while (current) {
            if (current->targetIndex == target && current->connectionType == type) {
                return;
            }
            current = current->next;
        }

        Edge* newEdge = new Edge(target, type, detail, weight);
        newEdge->next = head;
        head = newEdge;
        edgeCount++;
    }
};

class MovieGraph {
    movieNode** vertices;
    AdjList* adjList;
    int numVertices;
    int capacity;
    int totalEdges;

public:
    MovieGraph(int cap = 5000) {
        capacity = cap;
        numVertices = 0;
        totalEdges = 0;
        vertices = new movieNode*[capacity];
        adjList = new AdjList[capacity];
        
        for (int i = 0; i < capacity; i++) {
            vertices[i] = nullptr;
        }
    }

    ~MovieGraph() {
        delete[] vertices;
        delete[] adjList;
    }

    int addVertex(movieNode* movie) {
        if (!movie) return -1;
        
        int existingIndex = getMovieIndex(movie->movie_title);
        if (existingIndex != -1) return existingIndex;
        
        if (numVertices >= capacity) resize();
        
        vertices[numVertices] = movie;
        return numVertices++;
    }

    void addEdge(int idx1, int idx2, string type, string detail, double weight) {
        if (idx1 < 0 || idx1 >= numVertices || idx2 < 0 || idx2 >= numVertices || idx1 == idx2) {
            return;
        }

        adjList[idx1].addEdge(idx2, type, detail, weight);
        adjList[idx2].addEdge(idx1, type, detail, weight);
        totalEdges++;
    }

    void buildFromAVL(movieAVL* avlTree) {
        if (!avlTree) {
            cout << "Error: AVL tree is null.\n";
            return;
        }

        addAllVertices(avlTree->getRoot());
        
        createEdges();
    }

    void addNewMovie(movieNode* movie) {
        if (!movie) return;
        
        int newIdx = addVertex(movie);
        if (newIdx == -1) return;
        
        for (int i = 0; i < numVertices - 1; i++) {
            checkAndConnect(i, newIdx);
        }
    }

    void searchMovie(string movieTitle) {
        int idx = getMovieIndex(movieTitle);
        
        if (idx == -1) {
            cout << "Movie not found.\n";
            return;
        }

        movieNode* m = vertices[idx];
        cout << "\n===== MOVIE DETAILS =====\n";
        cout << "Title: " << m->movie_title << "\n";
        cout << "Year: " << m->title_year << "\n";
        cout << "Director: " << m->director_name << "\n";
        cout << "Rating: " << m->imdb_score << "\n";
        cout << "Duration: " << m->duration << " minutes\n";
        cout << "Actors: " << m->actor_1_name;
        if (!m->actor_2_name.empty()) cout << ", " << m->actor_2_name;
        if (!m->actor_3_name.empty()) cout << ", " << m->actor_3_name;
        cout << "\n";
        cout << "Genres: " << m->gen << "\n";
        cout << "Content Rating: " << m->content_rating << "\n";
        cout << "Budget: " << m->budget << "\n";
        cout << "Gross: " << m->gross << "\n";
    }

    void recommendGeneral(string movieTitle, int maxRec = 5) {
        int startIdx = getMovieIndex(movieTitle);
        
        if (startIdx == -1) {
            cout << "Movie not found.\n";
            return;
        }

        cout << "\n===== RECOMMENDATION OPTIONS =====\n";
        cout << "1. Breadth-First Search (BFS)\n";
        cout << "2. Depth-First Search (DFS)\n";
        cout << "3. By Director\n";
        cout << "4. By Genre\n";
        cout << "5. By Co-Star\n";
        cout << "Enter choice (1-5): ";
        
        int choice;
        cin >> choice;
        cin.ignore(10000, '\n');

        if (choice < 1 || choice > 5) {
            cout << "Invalid choice.\n";
            return;
        }

        switch (choice) {
            case 1:
                recommendBFS(movieTitle, maxRec);
                break;
            case 2:
                recommendDFS(movieTitle, maxRec);
                break;
            case 3:
                recommendByType(movieTitle, "Director", maxRec);
                break;
            case 4:
                recommendByType(movieTitle, "Genre", maxRec);
                break;
            case 5:
                recommendByType(movieTitle, "Co-Star", maxRec);
                break;
        }
    }

    void recommendByType(string movieTitle, string type, int maxRec = 10) {
        int startIdx = getMovieIndex(movieTitle);
        
        if (startIdx == -1) {
            cout << "Movie not found.\n";
            return;
        }

        cout << "\n===== RECOMMENDATIONS BY " << type << " =====\n";
        cout << "For: " << movieTitle << "\n\n";

        bool* visited = new bool[numVertices];
        for (int i = 0; i < numVertices; i++) visited[i] = false;

        Queue queue;
        queue.enqueue(startIdx);
        visited[startIdx] = true;

        int count = 0;

        while (!queue.isEmpty() && count < maxRec) {
            int current = queue.dequeue();

            Edge* edge = adjList[current].head;
            while (edge && count < maxRec) {
                int neighbor = edge->targetIndex;

                if (!visited[neighbor] && edge->connectionType == type) {
                    visited[neighbor] = true;
                    queue.enqueue(neighbor);

                    count++;
                    cout << count << ". " << vertices[neighbor]->movie_title 
                         << " (" << vertices[neighbor]->title_year << ")\n";
                    cout << "   Based On: " << edge->connectionType << " - " << edge->connectionDetail << "\n";
                    cout << "   Rating: " << vertices[neighbor]->imdb_score << "\n\n";
                }

                edge = edge->next;
            }
        }

        if (count == 0) {
            cout << "No recommendations found. Try different options.\n";
        }
        
        delete[] visited;
    }

    void recommendBFS(string movieTitle, int maxRec = 10) {
        int startIdx = getMovieIndex(movieTitle);
        
        if (startIdx == -1) {
            cout << "Movie not found.\n";
            return;
        }

        cout << "\n===== RECOMMENDATIONS (BFS) =====\n";
        cout << "For: " << movieTitle << "\n\n";

        bool* visited = new bool[numVertices];
        for (int i = 0; i < numVertices; i++) visited[i] = false;

        Queue queue;
        queue.enqueue(startIdx);
        visited[startIdx] = true;

        int count = 0;

        while (!queue.isEmpty() && count < maxRec) {
            int current = queue.dequeue();

            Edge* edge = adjList[current].head;
            while (edge && count < maxRec) {
                int neighbor = edge->targetIndex;

                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    queue.enqueue(neighbor);

                    count++;
                    cout << count << ". " << vertices[neighbor]->movie_title 
                         << " (" << vertices[neighbor]->title_year << ")\n";
                    cout << "   Connection: " << edge->connectionType 
                         << " - " << edge->connectionDetail << "\n";
                    cout << "   Rating: " << vertices[neighbor]->imdb_score 
                         << " | Director: " << vertices[neighbor]->director_name << "\n\n";
                }

                edge = edge->next;
            }
        }

        if (count == 0) cout << "No recommendations found.\n";
        
        delete[] visited;
    }

    void recommendDFS(string movieTitle, int maxRec = 10) {
        int startIdx = getMovieIndex(movieTitle);
        
        if (startIdx == -1) {
            cout << "Movie not found.\n";
            return;
        }

        cout << "\n===== RECOMMENDATIONS (DFS) =====\n";
        cout << "For: " << movieTitle << "\n\n";

        bool* visited = new bool[numVertices];
        for (int i = 0; i < numVertices; i++) visited[i] = false;

        Stack stack;
        stack.push(startIdx);
        visited[startIdx] = true;

        int count = 0;

        while (!stack.isEmpty() && count < maxRec) {
            int current = stack.pop();

            Edge* edge = adjList[current].head;
            while (edge && count < maxRec) {
                int neighbor = edge->targetIndex;

                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    stack.push(neighbor);

                    count++;
                    cout << count << ". " << vertices[neighbor]->movie_title 
                         << " (" << vertices[neighbor]->title_year << ")\n";
                    cout << "   Connection: " << edge->connectionType 
                         << " - " << edge->connectionDetail << "\n";
                    cout << "   Rating: " << vertices[neighbor]->imdb_score 
                         << " | Director: " << vertices[neighbor]->director_name << "\n\n";
                }

                edge = edge->next;
            }
        }

        if (count == 0) cout << "No recommendations found.\n";
        
        delete[] visited;
    }

    void shortestPath(string movie1, string movie2) {
        int start = getMovieIndex(movie1);
        int end = getMovieIndex(movie2);

        if (start == -1 || end == -1) {
            cout << "Movie not found.\n";
            return;
        }

        if (start == end) {
            cout << "Same movie.\n";
            return;
        }

        cout << "\n===== SHORTEST PATH =====\n";
        cout << "From: " << movie1 << "\n";
        cout << "To: " << movie2 << "\n\n";

        double* dist = new double[numVertices];
        int* parent = new int[numVertices];
        Edge** connEdge = new Edge*[numVertices];
        bool* visited = new bool[numVertices];

        for (int i = 0; i < numVertices; i++) {
            dist[i] = 999999;
            parent[i] = -1;
            connEdge[i] = nullptr;
            visited[i] = false;
        }

        dist[start] = 0;

        for (int count = 0; count < numVertices - 1; count++) {
            int u = minDistance(dist, visited);
            if (u == -1) break;
            
            visited[u] = true;

            Edge* edge = adjList[u].head;
            while (edge) {
                int v = edge->targetIndex;
                if (!visited[v] && dist[u] + edge->weight < dist[v]) {
                    dist[v] = dist[u] + edge->weight;
                    parent[v] = u;
                    connEdge[v] = edge;
                }
                edge = edge->next;
            }
        }

        if (dist[end] == 999999) {
            cout << "No path found.\n";
        } else {
            printPath(parent, connEdge, start, end);
            cout << "\nTotal distance: " << dist[end] << "\n";
        }

        delete[] dist;
        delete[] parent;
        delete[] connEdge;
        delete[] visited;
    }

    void findCoStars(string actorName) {
        cout << "\n===== CO-STARS OF " << actorName << " =====\n";
        
        bool found = false;
        for (int i = 0; i < numVertices; i++) {
            movieNode* m = vertices[i];
            if (m->actor_1_name == actorName || m->actor_2_name == actorName || m->actor_3_name == actorName) {
                cout << "\nIn " << m->movie_title << " (" << m->title_year << "):\n";
                if (m->actor_1_name != actorName && !m->actor_1_name.empty()) 
                    cout << "  - " << m->actor_1_name << "\n";
                if (m->actor_2_name != actorName && !m->actor_2_name.empty()) 
                    cout << "  - " << m->actor_2_name << "\n";
                if (m->actor_3_name != actorName && !m->actor_3_name.empty()) 
                    cout << "  - " << m->actor_3_name << "\n";
                found = true;
            }
        }
        
        if (!found) cout << "Actor not found.\n";
    }

    void displayStats() {
        cout << "\n===== GRAPH STATISTICS =====\n";
        cout << "Total Vertices: " << numVertices << "\n";
        cout << "Total Edges: " << totalEdges << "\n";

        if (numVertices > 0) {
            int maxConn = 0;
            int maxIdx = 0;

            for (int i = 0; i < numVertices; i++) {
                if (adjList[i].edgeCount > maxConn) {
                    maxConn = adjList[i].edgeCount;
                    maxIdx = i;
                }
            }

            cout << "Most Connected: " << vertices[maxIdx]->movie_title 
                 << " (" << maxConn << " connections)\n";
        }
    }

private:
    void addAllVertices(Node* node) {
        if (!node) return;
        addAllVertices(node->left);
        addVertex(node->data);
        addAllVertices(node->right);
    }

    void createEdges() {
        int sampleSize = 200;
        
        for (int i = 0; i < numVertices; i++) {
            
            int limit = (i + sampleSize < numVertices) ? i + sampleSize : numVertices;
            for (int j = i + 1; j < limit; j++) {
                checkAndConnect(i, j);
            }
        }
    }

    void checkAndConnect(int idx1, int idx2) {
        movieNode* m1 = vertices[idx1];
        movieNode* m2 = vertices[idx2];
        
        if (m1->director_name == m2->director_name && !m1->director_name.empty()) {
            addEdge(idx1, idx2, "Director", m1->director_name, 1.0);
            return;
        }
        
        string sharedGenre = getSharedGenre(m1, m2);
        if (!sharedGenre.empty()) {
            addEdge(idx1, idx2, "Genre", sharedGenre, 2.0);
            return;
        }
        
        string coStar = getCoStar(m1, m2);
        if (!coStar.empty()) {
            addEdge(idx1, idx2, "Co-Star", coStar, 1.5);
            return;
        }
        
        if (similarRating(m1, m2)) {
            addEdge(idx1, idx2, "Rating", "Similar ~" + m1->imdb_score, 3.0);
            return;
        }
        
        if (!m1->title_year.empty() && m1->title_year == m2->title_year) {
            addEdge(idx1, idx2, "Year", m1->title_year, 4.0);
        }
    }

    string getSharedGenre(movieNode* m1, movieNode* m2) {
        if (!m1->genres || !m2->genres) return "";
        
        for (int i = 0; i < m1->genreCount; i++) {
            for (int j = 0; j < m2->genreCount; j++) {
                if (normalize(m1->genres[i]) == normalize(m2->genres[j])) {
                    return m1->genres[i];
                }
            }
        }
        return "";
    }

    bool similarRating(movieNode* m1, movieNode* m2) {
        if (m1->imdb_score.empty() || m2->imdb_score.empty()) return false;
        
        double r1 = stringToDouble(m1->imdb_score);
        double r2 = stringToDouble(m2->imdb_score);
        
        double diff = r1 > r2 ? r1 - r2 : r2 - r1;
        return diff <= 0.5;
    }

    string getCoStar(movieNode* m1, movieNode* m2) {
        string actors1[3] = {m1->actor_1_name, m1->actor_2_name, m1->actor_3_name};
        string actors2[3] = {m2->actor_1_name, m2->actor_2_name, m2->actor_3_name};
        
        for (int i = 0; i < 3; i++) {
            if (actors1[i].empty()) continue;
            for (int j = 0; j < 3; j++) {
                if (actors2[j].empty()) continue;
                if (normalize(actors1[i]) == normalize(actors2[j])) {
                    return actors1[i];
                }
            }
        }
        return "";
    }

    double stringToDouble(string str) {
        double result = 0.0;
        double fraction = 0.0;
        bool isDecimal = false;
        int decimalPlaces = 0;
        
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == '.') {
                isDecimal = true;
            } else if (str[i] >= '0' && str[i] <= '9') {
                if (!isDecimal) {
                    result = result * 10 + (str[i] - '0');
                } else {
                    fraction = fraction * 10 + (str[i] - '0');
                    decimalPlaces++;
                }
            }
        }
        
        for (int i = 0; i < decimalPlaces; i++) {
            fraction /= 10.0;
        }
        
        return result + fraction;
    }

    int getMovieIndex(string title) {
        string normTitle = normalize(title);
        for (int i = 0; i < numVertices; i++) {
            if (normalize(vertices[i]->movie_title) == normTitle) {
                return i;
            }
        }
        return -1;
    }

    int minDistance(double dist[], bool visited[]) {
        double minDist = 999999;
        int minIdx = -1;

        for (int i = 0; i < numVertices; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                minIdx = i;
            }
        }

        return minIdx;
    }

    void printPath(int parent[], Edge* connEdge[], int start, int end) {
        if (parent[end] == -1) {
            cout << "START: " << vertices[start]->movie_title 
                 << " (" << vertices[start]->title_year << ")\n";
            return;
        }
        printPath(parent, connEdge, start, parent[end]);
        cout << "  ↓\n";
        cout << "  [" << connEdge[end]->connectionType << ": " << connEdge[end]->connectionDetail << "]\n";
        cout << "  ↓\n";
        cout << vertices[end]->movie_title << " (" << vertices[end]->title_year << ")\n";
    }

    void resize() {
        int newCap = capacity * 2;
        movieNode** newVert = new movieNode*[newCap];
        AdjList* newAdj = new AdjList[newCap];

        for (int i = 0; i < capacity; i++) {
            newVert[i] = vertices[i];
            newAdj[i].head = adjList[i].head;
            newAdj[i].edgeCount = adjList[i].edgeCount;
            adjList[i].head = nullptr;
        }

        for (int i = capacity; i < newCap; i++) {
            newVert[i] = nullptr;
        }

        delete[] vertices;
        delete[] adjList;

        vertices = newVert;
        adjList = newAdj;
        capacity = newCap;
    }
};