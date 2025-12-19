
#pragma once
#include <iostream>
#include <sstream>
using namespace std;

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

string normalize(const string& initial) {

    string result;
    int start = 0;
    while(start < initial.length() && initial[start] == ' ' ) start++;
    int end = initial.length();
    while(end > start && initial[end - 1] == ' ') end--;

    for(int i = start; i < end; i++) {
        char alphabet = initial[i];
        if(alphabet >= 'A' && alphabet <= 'Z') alphabet = alphabet - 'A' + 'a';
        result += alphabet;
    }

    return result;
}

string normalizeAlpha(const string& initial) {
    string result;
    for(int i = 0; i < initial.length(); i++){
        char alphabet = initial[i];
        if((alphabet >= 'A' && alphabet <= 'Z')||(alphabet >= 'a' && alphabet <= 'z')||(alphabet == ' ')){
            result += alphabet;
        }
    }
    return result;
}

int length (string a){
    int length = 0;

    while(a[length] != '\0')
    {
        ++length;
    }
    return length;
}

int target_counter(string str, char target){
    int space = 0, i = 0;
    while(str[i] != '\0'){
        if(str[i] == target){
            ++space;
        }
        i++;
    } 
    return space;
}

int str_arr_size(string str,char spliter){ 
    int size = target_counter(str, spliter)+ 1;
    return size;
}

string* split(string str, char spliter){
    int len = length(str);
    int size = str_arr_size(str, spliter);
    string temp_str = "";
    string *arr = new string[size];

    int j = 0;

    for(int i = 0; i < len ;i++){
        if(str[i] != spliter){
            temp_str += str[i];
        }
        if(str[i] == spliter){
            arr[j++] = temp_str;
            temp_str = "";
        }
    }
    
    if(temp_str != ""){
        arr[j++] = temp_str;
    }

    return arr;
}

string join(string* arr, char joiner, int size){
    string str = "";

    for(int i = 0; i < size; i++){
        if(arr[i] != ""){
            str += arr[i];
            if(i != size-1){
                str += joiner;
            }
        }
    }

    return str;
}

string find_and_remove(string str, string target){
    string* target_split = split(target,  ' ');
    string* str_split = split(str, ' ');

    int len_target_arr = str_arr_size(target, ' ');
    int len_str_arr = str_arr_size(str, ' ');
    
    int startIndex = 0;
    int indexes_of_letters[100] = {0}; 
    int period[100];
    int p = 0;
    for (int i = 0; i < len_target_arr ; i++){
        period[i] = -1;
    }
    
    for (int i = 0; i < len_target_arr; i++) {
        bool found_first = false;
        for (int j = startIndex; j < len_str_arr; j++) {
            bool status = true;
            int k = 0;
            while (status && (target_split[i][k] != '\0' || str_split[j][k] != '\0')) {
                if (target_split[i][k] == '.' || str_split[j][k] == '.') {
                    k++;
                    period[p++] = j;
                    continue;
                }
                if (target_split[i][k] != str_split[j][k]) {
                    status = false;
                }
                k++;
            }
            if (status == true) {
                startIndex = j + 1;
                found_first = true;
                if (i == len_target_arr - 1) {
                    int index = j - (len_target_arr - 1);
                    for (int o = 0; o < len_target_arr; o++) {
                        indexes_of_letters[o] = index++;
                    }
                }
                p++;
                break;
            }
        }
        if(!found_first){
            return str;
        }
    }

    for(int i = 0 ; i < len_target_arr; i++){
        if(indexes_of_letters[i] == period[i]){
            str_split[indexes_of_letters[i]] = " . ";
        }
        else str_split[indexes_of_letters[i]] = ""; 
    }

    string altered = join(str_split, ' ', len_str_arr);
    return altered;
}


