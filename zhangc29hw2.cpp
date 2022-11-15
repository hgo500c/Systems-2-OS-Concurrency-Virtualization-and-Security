/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

 /*
  * File:   main.cpp
  * Author: 14716
  * Copyright (C) 2019 zhangc29@miamiOH.edu
  * Created on September 10, 2019, 7:49 PM
  */

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>


using namespace std;

void storeUserid(ifstream& f1, unordered_map<int, string>& pass);
void storeGroupname(ifstream& f2, unordered_map<int, string>& groupNames);
void storeGroupid(ifstream& f3, unordered_map<int, vector<int>>& groups);
void print(int gid, unordered_map<int, string> group,
	unordered_map<int, vector<int>> groups,
	unordered_map<int, string> pass);

/*
 *
 */
int main(int argc, char** argv) {
	ifstream f1("passwd");
	ifstream f2("groups");
	ifstream f3("groups");
	unordered_map<int, string> pass;

	unordered_map<int, string> group;

	unordered_map<int, vector<int>> groups;

	storeUserid(f1, pass);

	storeGroupname(f2, group);

	storeGroupid(f3, groups);

	for (int i = 1; i < argc; i++) {
		int gid = atoi(argv[i]);
		print(gid, group, groups, pass);
	}


	f1.close();
	f2.close();
	f3.close();
}

void print(int gid, unordered_map<int, string> group,
	unordered_map<int, vector<int>> groups,
	unordered_map<int, string> pass) {
	if (group[gid] == "") {
		cout << gid << " = Group not found." << endl;
		return;
	}
	vector<int> members = groups.at(gid);
	if (members.size() == 0) {
		cout << gid << " = " << group[gid] << ":" << endl;
		return;
	}
	cout << gid << " = " << group[gid] << ": ";


	for (unsigned int i = 0; i < members.size(); i++) {
		if (i == members.size() - 1)
			cout << pass[members[i]] << "(" << members[i] << ")";
		else
			cout << pass[members[i]] << "(" << members[i] << ") ";
	}
	cout << "\n";
}

void storeUserid(ifstream& f1, unordered_map<int, string>& pass) {
	string line;
	while (getline(f1, line)) {
		string ex = line;
		replace(ex.begin(), ex.end(), ':', ' ');
		istringstream is(ex);
		string loginid, passk;
		int userid;
		is >> loginid >> passk >> userid;
		pass[userid] = loginid;
	}
}

void storeGroupname(ifstream& f2, unordered_map<int, string>& groupNames) {
	string line;
	while (getline(f2, line)) {
		string ex = line;
		replace(ex.begin(), ex.end(), ':', ' ');
		istringstream is(ex);
		string groupID, passkey;
		int groupid;
		is >> groupID >> passkey >> groupid;
		groupNames[groupid] = groupID;
	}
}

void storeGroupid(ifstream& f3, unordered_map<int, vector<int>>& groups) {
	string line;
	string copy = line;
	for (int line = 0; line < groups.size() - 1; line++) {
		vector<int> temp;
		replace(copy.begin(), copy.end(), ':', ' ');
		replace(copy.begin(), copy.end(), ',', ' ');
		istringstream is(copy);
		string groupID, passkey;
		int gid, uid;
		is >> groupID >> passkey >> gid;
		if (is >> uid) {
			temp.push_back(uid);
		}
		groups[gid] = temp;
	}
}





