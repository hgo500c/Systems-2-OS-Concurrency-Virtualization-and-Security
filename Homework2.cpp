/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 * Copyright cse 
 */

/* 
 * File:   main.cpp
 * Author: hex10
 *
 * 
 */

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>

using namespace std;

void storeUid(ifstream& in1, unordered_map<int, string>& passwd);
void storeGroupName(ifstream& in2, unordered_map<int, string>& groupNames);
void storeGid(ifstream& in3, unordered_map<int, vector<int>>& groups);
void printResult(int gid, unordered_map<int, string> groupNames, 
        unordered_map<int, vector<int>> groups, 
        unordered_map<int, string> passwd);

/*
 * 
 */
int main(int argc, char** argv) {
    ifstream in1("passwd");
    ifstream in2("groups");
    ifstream in3("groups");
    unordered_map<int, string> passwd;
    
    storeUid(in1, passwd);
    
    unordered_map<int, string> groupNames;
    
    storeGroupName(in2, groupNames);
    
    unordered_map<int, vector<int>> groups;
    
    storeGid(in3, groups);
    
    // printResult(100, groupNames, groups, passwd);
    for (int i = 1; i < argc; i++) {
        int gid = atoi(argv[i]);
        printResult(gid, groupNames, groups, passwd);
    }

    
    in1.close();
    in2.close();
    in3.close();
}

void printResult(int gid, unordered_map<int, string> groupNames, 
        unordered_map<int, vector<int>> groups, 
        unordered_map<int, string> passwd) {
    if (groupNames[gid] == "") {
        cout << gid << " = Group not found." << endl;
        return;
    } 
    vector<int> members = groups.at(gid);
    if (members.size() == 0) {
        cout << gid << " = " << groupNames[gid] << ":" << endl;
        return;
    }
    cout << gid << " = " << groupNames[gid] << ": ";
    
    
    for (unsigned int i = 0; i < members.size(); i++) {
        if (i == members.size() - 1) 
            cout << passwd[members[i]] << "(" << members[i] << ")";
        else 
            cout << passwd[members[i]] << "(" << members[i] << ") ";
    }
    cout << "\n";
}

void storeUid(ifstream& in1, unordered_map<int, string>& passwd) {
    string line;
    while (getline(in1, line)) {
        string copy = line;
        replace(copy.begin(), copy.end(), ':', ' ');
        istringstream is(copy); 
        string loginID, passkey;
        int uid;
        is >> loginID >> passkey >> uid;
        passwd[uid] = loginID;
        // cout << passwd[uid] << " : " << uid << endl;
    }
}

void storeGroupName(ifstream& in2, unordered_map<int, string>& groupNames) {
    string line;
    while (getline(in2, line)) {
        string copy = line;
        replace(copy.begin(), copy.end(), ':', ' ');
        istringstream is(copy); 
        string groupID, passkey;
        int gid;
        is >> groupID >> passkey >> gid;
        groupNames[gid] = groupID;
        // cout << groupNames[gid] << " : " << gid << endl;
    }
}

void storeGid(ifstream& in3, unordered_map<int, vector<int>>& groups) {
    string line;
    while (getline(in3, line)) {
        vector<int> temp;
        string copy = line;
        replace(copy.begin(), copy.end(), ':', ' ');
        replace(copy.begin(), copy.end(), ',', ' ');
        istringstream is(copy);
        string groupID, passkey;
        int gid, uid;
        is >> groupID >> passkey >> gid;
        while (is >> uid) {
            temp.push_back(uid);
        }
        groups[gid] = temp;
    }
}




