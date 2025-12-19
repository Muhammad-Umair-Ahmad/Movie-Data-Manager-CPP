#pragma once
#include <iostream>
#include "movieAVL.h"
#include "customString.h"
using namespace std;

struct MoviesInList{ 
    public:
    movieNode* node;
    MoviesInList* next;

    MoviesInList(){
        node = nullptr;
        next = nullptr;
    }
    MoviesInList(movieNode* incoming){
        node = incoming;
        next = nullptr;
    }

    bool alreadyPresent(movieNode* movie){
        if (!movie) return false;
        
        MoviesInList* current = this;
        while (current != nullptr) {
            if (current->node == movie) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void printFilmography(string key,string basedOn){
        if(basedOn == "actor"){        cout << "\nFilmography of " << key << ":\n";
        MoviesInList* current = this;
        while(current){
            if(node->actor_1_name == key || node->actor_2_name == key || node->actor_3_name == key)
                cout << "> " << current->node->movie_title << " (" << current->node->title_year << ")\n";
            current = current->next;
        }
        }

        if(basedOn == "genre"){        
        cout << "\nFilmography of " << key << ":\n";
        MoviesInList* current = this;
        while(current){
            cout << "> " << current->node->movie_title << " (" << current->node->title_year << ")\n";
            current = current->next;
        }
        }
    }
};

class keyTable{
    public:
    MoviesInList* headofList;  
    string actorsTable;          

    keyTable() {
        actorsTable = "";
        headofList = nullptr;
    }
    
    bool addMovie(movieNode* movie) {
        if (!movie) return false;

        if (headofList != nullptr && headofList->alreadyPresent(movie)) {
            return false;
        }

        MoviesInList* newNode = new MoviesInList(movie);
        newNode->next = headofList;
        headofList = newNode;
        return true;
    }
};

class hashMap {
    keyTable* table;
    int totalEntries;
    int capacity;
    const float loadFactor = 0.75;

public:
    hashMap(int cap = 1009) {
        capacity = cap;
        totalEntries = 0;
        table = new keyTable[capacity];
    }

    ~hashMap() {
        delete[] table;
    }

    int hashFunction(string key) {
        unsigned long hash = 5381;
        key = normalize(key);
        for (char c : key) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash % capacity;
    }


    string* getAllKeys(int &count) {
        count = 0;
        for (int i = 0; i < capacity; i++) {
            if (!table[i].actorsTable.empty()) count++;
        }

        string* keys = new string[count];
        int idx = 0;
        for (int i = 0; i < capacity; i++) {
            if (!table[i].actorsTable.empty()) {
                keys[idx++] = table[i].actorsTable;
            }
        }
        return keys;
    }


    void insert(movieNode* movie, string key) {
        if (!movie || key.empty()) return;

        key = normalize(key);
        int index = hashFunction(key);
        if (table[index].headofList == nullptr) table[index].actorsTable = key;

        if (table[index].addMovie(movie)) totalEntries++;

        if ((float)totalEntries / capacity > loadFactor) rehash();
    }

    void addMovieToHashMaps(movieNode* movie) {
        if (!movie) return;

        if (!movie->actor_1_name.empty()) insert(movie, movie->actor_1_name);
        if (!movie->actor_2_name.empty()) insert(movie, movie->actor_2_name);
        if (!movie->actor_3_name.empty()) insert(movie, movie->actor_3_name);

        int genreCount = target_counter(movie->gen, '|') + 1;
        for (int i = 0; i < genreCount; i++) {
            if (!movie->genres[i].empty()) insert(movie, movie->genres[i]);
        }
    }

    keyTable* search(string key) {
        if (key.empty()) return nullptr;

        key = normalize(key);
        int index = hashFunction(key);
        if (table[index].headofList != nullptr && normalize(table[index].actorsTable) == key) 
            return &table[index];

        return nullptr;
    }

    MoviesInList* getMovies(string key) {
        keyTable* kt = search(key);
        return kt ? kt->headofList : nullptr;
    }

    void buildFromAVL(movieAVL* storage, string basedOn) {
        if (!storage) return;
        Node* root = storage->getRoot();
        if (!root) return;

        if (basedOn == "actor") buildActorMapRecursive(root);
        else if (basedOn == "genre") buildGenreMapRecursive(root);
    }

    void printFilmography(string key, string basedOn) {
        keyTable* kt = search(key);
        if (!kt) {
            cout << "'" << key << "' not found in hash map.\n";
            return;
        }
        if (kt->headofList) kt->headofList->printFilmography(key, basedOn);
    }

    void getTopN(int n, string key) {
        MoviesInList* ll = getMovies(key);
        if (!ll) return;

        cout << "\nTop " << n << " movies for " << key << ":\n";
        MoviesInList* current = ll;
        int count = 0;
        while (current && count < n) {
            cout << "- " << current->node->movie_title << " (" << current->node->title_year << ")\n";
            current = current->next;
            count++;
        }
    }

private:
    void buildActorMapRecursive(Node* node) {
        if (!node) return;
        buildActorMapRecursive(node->left);
        if (node->data) addMovieToHashMaps(node->data);
        buildActorMapRecursive(node->right);
    }

    void buildGenreMapRecursive(Node* node) {
        if (!node) return;
        buildGenreMapRecursive(node->left);
        if (node->data) addMovieToHashMaps(node->data);
        buildGenreMapRecursive(node->right);
    }

    void rehash() {
        int oldCapacity = capacity;
        keyTable* oldTable = table;

        capacity = getNextPrime(capacity * 2);
        table = new keyTable[capacity];
        totalEntries = 0;

        for (int i = 0; i < oldCapacity; i++) {
            MoviesInList* current = oldTable[i].headofList;
            string key = oldTable[i].actorsTable;
            while (current) {
                insert(current->node, key);
                current = current->next;
            }
        }

        delete[] oldTable;
    }

    int getNextPrime(int n) {
        while (!isPrime(n)) n++;
        return n;
    }

    bool isPrime(int n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6)
            if (n % i == 0 || n % (i + 2) == 0) return false;
        return true;
    }
};