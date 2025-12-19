#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "movieAVL.h"
#include "customString.h"
#include "0_HashMap.h"
#include "MovieGraph.h"
using namespace std;

void safeGetline(stringstream &source, string &destination) {
    if (!getline(source, destination, ',')) {
        destination = " ";
        return;
    }
    if (destination.empty()) {
        destination = " ";
    }
}

movieAVL* dataLoader(string filename, int& index) {
    movieAVL* df = new movieAVL();
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Error: File could not be opened: " << filename << endl;
        return nullptr;
    }

    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string color, director_name, num_critic_for_reviews, duration, director_facebook_likes;
        string actor_3_facebook_likes, actor_2_name, actor_1_facebook_likes, gross, genres;
        string actor_1_name, movie_title, num_voted_users, cast_total_facebook_likes, actor_3_name;
        string facenumber_in_poster, plot_keywords, movie_imdb_link, num_user_for_reviews;
        string language, country, content_rating, budget, title_year, actor_2_facebook_likes;
        string imdb_score, aspect_ratio, movie_facebook_likes;

        safeGetline(ss, color);
        safeGetline(ss, director_name);
        safeGetline(ss, num_critic_for_reviews);
        safeGetline(ss, duration);
        safeGetline(ss, director_facebook_likes);
        safeGetline(ss, actor_3_facebook_likes);
        safeGetline(ss, actor_2_name);
        safeGetline(ss, actor_1_facebook_likes);
        safeGetline(ss, gross);
        safeGetline(ss, genres);
        safeGetline(ss, actor_1_name);
        safeGetline(ss, movie_title);
        safeGetline(ss, num_voted_users);
        safeGetline(ss, cast_total_facebook_likes);
        safeGetline(ss, actor_3_name);
        safeGetline(ss, facenumber_in_poster);
        safeGetline(ss, plot_keywords);
        safeGetline(ss, movie_imdb_link);
        safeGetline(ss, num_user_for_reviews);
        safeGetline(ss, language);
        safeGetline(ss, country);
        safeGetline(ss, content_rating);
        safeGetline(ss, budget);
        safeGetline(ss, title_year);
        safeGetline(ss, actor_2_facebook_likes);
        safeGetline(ss, imdb_score);
        safeGetline(ss, aspect_ratio);
        safeGetline(ss, movie_facebook_likes);

        if (title_year != "") {
            try {
                int year = stoi(title_year);
                if (year < 1900 || year > 2025) continue;
            } catch (...) {
                continue;
            }
        } else {
            continue;
        }

        movie_title = normalizeAlpha(movie_title);
        director_name = normalizeAlpha(director_name);
        actor_2_name = normalizeAlpha(actor_2_name);
        actor_3_name = normalize(actor_3_name);
        actor_1_name = normalize(actor_1_name);

        if (actor_2_name == "") actor_2_name = "";
        if (actor_1_name == "") actor_1_name = "";
        if (actor_3_name == "") actor_3_name = "";

        df->insert(color, director_name, duration, actor_2_name, gross, genres, actor_1_name, 
                   movie_title, actor_3_name, plot_keywords, language, country, content_rating, 
                   budget, title_year, imdb_score, aspect_ratio);
        index++;
    }

    file.close();
    cout << "Dataset loaded successfully. Total movies: " << index << "\n";
    return df;
}

class MovieManager {
private:
    movieAVL* avlTree;
    hashMap* directorMap;
    hashMap* genreMap;
    hashMap* actorMap;
    MovieGraph* graph;
    int totalMovies;
    int choice;

public:
    MovieManager() {
        avlTree = nullptr;
        directorMap = nullptr;
        genreMap = nullptr;
        actorMap = nullptr;
        graph = nullptr;
        totalMovies = 0;

        initializeSystem();
        if (avlTree != nullptr) {
            mainMenu();
        }
    }

    ~MovieManager() {
        if (avlTree) delete avlTree;
        if (directorMap) delete directorMap;
        if (genreMap) delete genreMap;
        if (actorMap) delete actorMap;
        if (graph) delete graph;
    }

private:
    void clearInputBuffer() {
        cin.clear();
        cin.ignore(10000, '\n');
    }

    bool getValidChoice(int& choice, int minVal, int maxVal) {
        cin >> choice;
        if (cin.fail() || choice < minVal || choice > maxVal) {
            clearInputBuffer();
            cout << "Invalid input. Please enter a number between " << minVal << " and " << maxVal << ".\n";
            return false;
        }
        clearInputBuffer();
        return true;
    }

    string getValidString(const string& prompt) {
        string input;
        cout << prompt;
        getline(cin, input);

        if (input.empty()) {
            cout << "Input cannot be empty.\n";
            return "";
        }
        return input;
    }

    void initializeSystem() {
        cout << "\n===== INITIALIZING MOVIE DATABASE =====\n";
        cout << "Loading dataset...\n";

        string filename = "dataset.csv";
        avlTree = dataLoader(filename, totalMovies);

        if (avlTree == nullptr) {
            cerr << "Failed to load dataset. Exiting application.\n";
            return;
        }

        cout << "Building graph structure...\n";
        graph = new MovieGraph();
        graph->buildFromAVL(avlTree);

        cout << "Building director index...\n";
        directorMap = new hashMap();
        directorMap->buildFromAVL(avlTree, "director");

        cout << "Building genre index...\n";
        genreMap = new hashMap();
        genreMap->buildFromAVL(avlTree, "genre");

        cout << "Building actor index...\n";
        actorMap = new hashMap();
        actorMap->buildFromAVL(avlTree, "actor");

        cout << "System initialized successfully!\n";
    }

    void mainMenu() {
        while (true) {
            cout << "\n===== MOVIE DATABASE MANAGEMENT SYSTEM =====\n";
            cout << "Total Movies Loaded: " << totalMovies << "\n";
            cout << "1. Search Movie\n";
            cout << "2. Movie Recommendations\n";
            cout << "3. Path Finding\n";
            cout << "4. Actor Information\n";
            cout << "5. Display All Movies\n";
            cout << "0. Exit Application\n";
            cout << "Enter choice (0-5): ";

            if (!getValidChoice(choice, 0, 5)) {
                continue;
            }

            switch (choice) {
                case 1:
                    searchMovie();
                    break;
                case 2:
                    recommendationMenu();
                    break;
                case 3:
                    pathFindingMenu();
                    break;
                case 4:
                    actorInformationMenu();
                    break;
                case 5:
                    displayAllMovies();
                    break;
                case 0:
                    cout << "Exiting application. Thank you!\n";
                    return;
            }
        }
    }

    void searchMovie() {
        while (true) {
            cout << "\n===== SEARCH MOVIE =====\n";
            string title = getValidString("Enter movie title (or '0' to go back): ");

            if (title == "0") return;
            if (title.empty()) {
                cout << "Search term cannot be empty.\n";
                continue;
            }

            avlTree->search(title);
            break;
        }
    }

    void recommendationMenu() {
        while (true) {
            cout << "\n===== RECOMMENDATION SYSTEM =====\n";
            cout << "1. Get Recommendations\n";
            cout << "2. Back to Main Menu\n";
            cout << "0. Exit Application\n";
            cout << "Enter choice (0-2): ";

            if (!getValidChoice(choice, 0, 2)) {
                continue;
            }

            switch (choice) {
                case 1:
                    getRecommendations();
                    break;
                case 2:
                    return;
                case 0:
                    cout << "Exiting application. Thank you!\n";
                    exit(0);
            }
        }
    }

    void getRecommendations() {
        string title = getValidString("Enter movie title for recommendations (or '0' to go back): ");

        if (title == "0") return;
        if (title.empty()) {
            cout << "Title cannot be empty.\n";
            return;
        }

        int maxRec = 5;
        cout << "Enter maximum recommendations (default 5): ";
        cin >> maxRec;
        clearInputBuffer();

        if (maxRec < 1 || maxRec > 100) {
            cout << "Invalid number. Using default value of 5.\n";
            maxRec = 5;
        }

        graph->recommendGeneral(title, maxRec);
    }

    void pathFindingMenu() {
        while (true) {
            cout << "\n===== PATH FINDING =====\n";
            cout << "1. Find Shortest Path Between Movies\n";
            cout << "2. Back to Main Menu\n";
            cout << "0. Exit Application\n";
            cout << "Enter choice (0-2): ";

            if (!getValidChoice(choice, 0, 2)) {
                continue;
            }

            switch (choice) {
                case 1:
                    findShortestPath();
                    break;
                case 2:
                    return;
                case 0:
                    cout << "Exiting application. Thank you!\n";
                    exit(0);
            }
        }
    }

    void findShortestPath() {
        string movie1 = getValidString("Enter first movie title (or '0' to go back): ");

        if (movie1 == "0") return;
        if (movie1.empty()) {
            cout << "Movie title cannot be empty.\n";
            return;
        }

        string movie2 = getValidString("Enter second movie title: ");

        if (movie2 == "0") return;
        if (movie2.empty()) {
            cout << "Movie title cannot be empty.\n";
            return;
        }

        graph->shortestPath(movie1, movie2);
    }

    void actorInformationMenu() {
        while (true) {
            cout << "\n===== ACTOR INFORMATION =====\n";
            cout << "1. Find Co-Stars\n";
            cout << "2. Actor Filmography\n";
            cout << "3. Back to Main Menu\n";
            cout << "0. Exit Application\n";
            cout << "Enter choice (0-3): ";

            if (!getValidChoice(choice, 0, 3)) {
                continue;
            }

            switch (choice) {
                case 1:
                    findCoStars();
                    break;
                case 2:
                    actorFilmography();
                    break;
                case 3:
                    return;
                case 0:
                    cout << "Exiting application. Thank you!\n";
                    exit(0);
            }
        }
    }

    void findCoStars() {
        string actor = getValidString("Enter actor name (or '0' to go back): ");

        if (actor == "0") return;
        if (actor.empty()) {
            cout << "Actor name cannot be empty.\n";
            return;
        }

        graph->findCoStars(actor);
    }

    void actorFilmography() {
        string actor = getValidString("Enter actor name (or '0' to go back): ");

        if (actor == "0") return;
        if (actor.empty()) {
            cout << "Actor name cannot be empty.\n";
            return;
        }

        actorMap->printFilmography(actor, "actor");
    }

    void displayAllMovies() {
        cout << "\n===== ALL MOVIES IN DATABASE =====\n";
        cout << "Total Movies: " << totalMovies << "\n\n";
        avlTree->Display();
        cout << "\nPress Enter to continue...";
        cin.get();
    }
};

