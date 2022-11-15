/*
 * This head file need one class and 2 public methods 
 * 1. first method that loads process data from a given file into 2 unordered
 *    maps (discussed below) 
 * 2. second method that prints the process tree for a given PID. 
 *    You may add any private helper methods as you see fit.It is up to you to 
 *    decide meaningful names for methods and their arguments.
 *    Copyright (C) 2019 zhangc29@miamiOH.edu
 */

/* 
 * File:   zhangc29_hw3.h
 * Author: 14716
 * Copyright (C) 2019 zhangc29@miamiOH.edu
 * Created on September 18, 2019, 12:23 AM
 */

#ifndef ZHANGC29_HW3_H
#define ZHANGC29_HW3_H

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>

class zhangc29{
public:
    void loading(std::ifstream & file, 
            std::unordered_map<std::string, std::string> & pid, 
            std::unordered_map<std::string, std::string> & cmd1);
    // this is the method which include the input file, and two unordered map 
    // which can let us to store all data in pid and cmd
    void print(std::string inputnumber, 
    std::unordered_map<std::string, std::string> & pid, 
            std::unordered_map<std::string, std::string> & cmd1);
    // this methods, we put the number we enter and we put two unordered map 
    // and which can find the number 
};


#endif /* ZHANGC29_HW3_H */

