#pragma once
#include "movieAVL.h"
#include <fstream>
#include <sstream>
#include <iostream>
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
        cerr << "Error: File couldn't be opened: " << filename << endl;
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

        movie_title = normalizeAlpha(movie_title);
        director_name = normalizeAlpha(director_name);
        actor_2_name = normalizeAlpha(actor_2_name);
        actor_3_name = normalize(actor_3_name);
        actor_1_name = normalize(actor_1_name);

    if (actor_2_name == "") actor_2_name = "";
    if (actor_1_name == "") actor_1_name = "";
    if (actor_3_name == "") actor_3_name = "";

        df->insert(color, director_name, duration, actor_2_name, gross, genres, actor_1_name, movie_title, actor_3_name, plot_keywords, language, country, content_rating, budget, title_year, imdb_score, aspect_ratio);
        index++;
    }
    
    file.close();
    return df;
}