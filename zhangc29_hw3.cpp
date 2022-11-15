/* This class contains two methods which can read the file and print what we 
 * want
 * 1. first method that loads process data from a given file into 2 unordered
 *    maps (discussed below) 
 * 2. second method that prints the process tree for a given PID. 
 *    You may add any private helper methods as you see fit.It is up to you to 
 *    decide meaningful names for methods and their arguments.
 *    Copyright (C) 2019 zhangc29@miamiOH.edu
 * 
 */

/* 
 * File:   main.cpp
 * Author: 14716
 * Copyright (C) 2019 zhangc29@miamiOH.edu
 * Created on September 17, 2019, 11:08 PM
 */
// #define ZHANGC29_HW3_H
#include <cstdlib>
#include<fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include "zhangc29_hw3.h"

using namespace std;

/*
 * 
 */

    void zhangc29::loading(ifstream & file, unordered_map<string, string> 
    & pid, unordered_map<string, string> & cmd1) {
        // this method is used for reading and storing all data from input
        // file, and get the data we want and put in the file.
        string line;
        while (getline(file, line)) {
            istringstream is(line);
            string id, ppid, uid, stime, tty, time, cmd, c, ex;
            // use this to divde the line to every segments
            is >> uid >> id >> ppid >> c >> stime >> time >> ex >> cmd;
            int CmdPosition = line.find(cmd, 0);
            string cmdWhole = line.substr(CmdPosition);
            // I want to use the int postion to find the whole cmd 
            pid[id] = ppid;
            cmd1[id] = cmdWhole;
        }
    }
    void zhangc29::print(string inputnumber, std::unordered_map<string, string> 
    & pid, std::unordered_map<string, string> & cmd1) {
        // this method can print the data what I want, I will type a number,
        // and search the number, then we print in here. 
    std::cout << "Process tree for PID: " << inputnumber << endl;
    std::cout << "PID" << "\t" << "PPID" << "\t" << "CMD" << endl;
    string output;
    string temp = inputnumber;
    while (temp.compare("0") != 0) {
        // if this equal to the 0, I stop to find new one.
        string line;
        line = temp + "\t" + pid[temp] + "\t" + " " + cmd1[temp] + "\n";
        // string line is read line by line. 
        output = line + output;
        // it should add in the output below again and again. 
        temp = pid[temp];
    }
    //  add all outputs into one string
    std::cout << output;
    }
int main(int argc, char** argv) {
    unordered_map<string, string> pid;
    unordered_map<string, string> cmd1;
    ifstream file(argv[1]);  // we put the file name in here 
    zhangc29 zc;
    zc.loading(file, pid, cmd1);  // this method we read and store information
    // we need in the two map.
    // string x = "1";
    // std::cout << pid[x] << " " << cmd1[x] << endl;
    string inputnumber =  argv[2];  // writing the number to search in here
    zc.print(inputnumber, pid, cmd1);
    return 0;
}


