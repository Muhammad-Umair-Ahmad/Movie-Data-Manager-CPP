#pragma once
#include <iostream>
#include <string>
#include "customString.h"
using namespace std;

class movieNode {
public:
    string color;
    string director_name;
    string duration;
    string gross;
    string* genres;
    int genreCount;
    string gen;
    string* actors;
    string movie_title;
    string plot_keywords;
    string language;
    string country;
    string actor_1_name, actor_2_name, actor_3_name;
    string content_rating;
    string budget;
    string title_year;
    string imdb_score;
    string aspect_ratio;

    movieNode(string color, string director_name, string duration, string actor_2_name, string gross, string genres, string actor_1_name, string movie_title, string actor_3_name, string plot_keywords, string language, string country, string content_rating, string budget, string title_year, string imdb_score, string aspect_ratio)
    {
        this->color = color;
        this->director_name = director_name;
        this->duration = duration;
        this->gross = gross;
        this->gen = genres;
        this->genres = split(genres, '|');
        this->genreCount = str_arr_size(gen,'|');
        this->actor_1_name = actor_1_name;
        this->actor_2_name = actor_2_name;
        this->actor_3_name = actor_3_name;

        // actors = new string[3];
        // actors[0] = this->actor_1_name;
        // actors[1] = this->actor_2_name;        
        // actors[2] = this->actor_3_name;     

        this->movie_title = movie_title;
        this->plot_keywords = plot_keywords;
        this->language = language;
        this->country = country;
        this->content_rating = content_rating;
        this->budget = budget;
        this->title_year = title_year;
        this->imdb_score = imdb_score;
        this->aspect_ratio = aspect_ratio;
    }

    void display() {
        cout << "Movie Title: " << movie_title << endl;
        cout << "Director: " << director_name << endl;
        cout << "Actors: " << actor_1_name << ", " << actor_2_name << ", " << actor_3_name << endl;
        cout << "Actors: " ;
        for(int i = 0; i <= target_counter(gen, '|'); i++ ){
            cout << genres[i] << " | ";
        }
        cout << endl;
        cout << "Duration: " << duration << endl;
        cout << "Language: " << language << endl;
        cout << "Country: " << country << endl;
        cout << "Content Rating: " << content_rating << endl;
        cout << "Budget: " << budget << endl;
        cout << "Gross: " << gross << endl;
        cout << "Year: " << title_year << endl;
        cout << "IMDB Score: " << imdb_score << endl;
        cout << "Aspect Ratio: " << aspect_ratio << endl;
        cout << "Plot Keywords: " << plot_keywords << endl;
        cout << "Color: " << color << endl;
        cout << "------------------------------------------------\n" << endl;
    }
};

class Node{
public:
    movieNode* data;
    Node* left;
    Node* right;
    int height;

    Node(movieNode* m) {
        data = m;
        left = right = nullptr;
        height = 1;
    }
};

class movieAVL{
private:
    Node* root;

    int gethieght(Node* node){
        if(node) return node->height;
        else return 0;
    }

    int calcBF(Node* node){
        return gethieght(node->left) - gethieght(node->right);
    }

    Node* rightRotate(Node* node){
        Node* newRoot = node->left;
        Node* righty = newRoot->right;

        newRoot->right = node;
        node->left = righty;

        node->height = 1 + max(gethieght(node->left), gethieght(node->right));
        newRoot->height = 1 + max(gethieght(newRoot->left), gethieght(newRoot->right));
        return newRoot;
    }

    Node* leftRotate(Node* node){
        Node* newRoot = node->right;
        Node* lefty = newRoot->left;

        newRoot->left = node;
        node->right = lefty;

        node->height = 1 + max(gethieght(node->left), gethieght(node->right));
        newRoot->height = 1 + max(gethieght(newRoot->left), gethieght(newRoot->right));
        return newRoot;
    }

    Node* balance(Node* node){
        int bf = calcBF(node);

        if (bf > 1) {
            if(calcBF(node->left) < 0) node->left = leftRotate(node->left);
            return rightRotate(node); 
        }

        if (bf < -1){
            if(calcBF(node->right) > 0) node->right = rightRotate(node->right);
            return leftRotate(node); 
        }

        return node; 
    }

    Node* insertNode(Node* node, movieNode* m){
        if (!node) return new Node(m);

        if(m->movie_title < node->data->movie_title) node->left = insertNode(node->left, m);
        else if(m->movie_title > node->data->movie_title) node->right = insertNode(node->right, m);
        else return node; 
        node->height = 1 + max(gethieght(node->left), gethieght(node->right));
        return balance(node);
    }

Node* searchBYtitle(Node* cur, string title) {
    if(!cur) return nullptr;
    if(normalize(cur->data->movie_title) == title) return cur;
    if(title < normalize(cur->data->movie_title)) return searchBYtitle(cur->left, title);  
    return searchBYtitle(cur->right, title); 
}

public:
    movieAVL(){ 
        root = nullptr;
    }

    Node* getRoot() {
        return root;
    }

    movieNode* insertNewMovie(string color, string director_name, string duration, string actor_2_name, string gross, string genres, string actor_1_name, string movie_title, string actor_3_name, string plot_keywords, string language, string country, string content_rating, string budget, string title_year, string imdb_score, string aspect_ratio) 
    {
        movieNode* n = new movieNode( color, director_name, duration, actor_2_name, gross,genres, actor_1_name, movie_title, actor_3_name,plot_keywords, language, country, content_rating, budget, title_year, imdb_score, aspect_ratio);

        root = insertNode(root, n);
    
    return n;
    }

    void insert(string color, string director_name, string duration, string actor_2_name, string gross, string genres, string actor_1_name, string movie_title, string actor_3_name, string plot_keywords, string language, string country, string content_rating, string budget, string title_year, string imdb_score, string aspect_ratio){

    movieNode* n = new movieNode(color, director_name, duration, actor_2_name, gross, genres, actor_1_name, movie_title, actor_3_name, plot_keywords, language, country, content_rating, budget, title_year, imdb_score, aspect_ratio);

        root = insertNode(root, n);
    }

    void search(string title){
        Node* node = searchBYtitle(root, normalize(title));
        if(node) node->data->display();
        
        else cout << "Movie not found.\n";
    }

    void inOrder(Node* node){
        if(!node) return;
        inOrder(node->left);
        node->data->display();
        inOrder(node->right);
    }

    void Display(){
        inOrder(root);
    }
};