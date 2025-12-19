#include <iostream>
#include <fstream>
#include <sstream>
#include "movieAVL.h"
#include "customString.h"
#include "dataHandler.h"
#include "0_HashMap.h"
#include "MovieGraph.h"
using namespace std;

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
        insertNew();
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

    void initializeSystem() {
        string filename = "dataset.csv";
        avlTree = dataLoader(filename, totalMovies);

        if (avlTree == nullptr) {
            cerr << "Failed to load dataset. Exiting application.\n";
            return;
        }

        graph = new MovieGraph();
        graph->buildFromAVL(avlTree);
        directorMap = new hashMap();
        directorMap->buildFromAVL(avlTree, "director");

        genreMap = new hashMap();
        genreMap->buildFromAVL(avlTree, "genre");

        actorMap = new hashMap();
        actorMap->buildFromAVL(avlTree, "actor");

    }

    void mainMenu() {
        while (true) {
            cout << "\n===== MOVIE MANAGEMENT SYSTEM =====\n";
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
    public:
    void insertNew(){
        movieNode* n = avlTree->insertNewMovie("color", "Me", "230min", "Ali", "100Billion", "Horror", "Me", "Fast", "Umair", "", "Urdu, English", "Pakistan", "10", "230 Million", "2025", "10", "2:39");

    directorMap->addMovieToHashMaps(n);
    genreMap->addMovieToHashMaps(n);
    actorMap->addMovieToHashMaps(n);
    graph->addNewMovie(n);

    }
    private:
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

