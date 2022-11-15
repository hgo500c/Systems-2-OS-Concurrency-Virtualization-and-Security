/** A program to programmatically run the find command
 *
 * Copyright (C) 2018 caoz7@miamiOH.edu
 */

#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>

 // Convenience shortcut for a vector-of-strings
using StrVec = std::vector<std::string>;

/**
 * Copy-pasted from lecture slides -- Convenience method to setup
 * arguments (as pointers) and call the execvp system call.  For
 * example, this method can be called as --
 *           myExec({"ls", "-l", "/usr"});
 *
 * @param argList The list of arguments with the 1st entry always
 * being the path to the full command to be run.
 */
void myExec(StrVec argList) {
	std::vector<char*> args;  // list of pointers to each word
	const int pid = fork();
	if (pid == 0) {
		for (auto& s : argList) {
			args.push_back(&s[0]);  // address of 1st character in string
		}
		// nullptr is very important
		args.push_back(nullptr);
		// Make execvp system call to run desired process
		execvp(args[0], &args[0]);
	}
	std::cout << std::endl;
	int exitcode = 0;
	waitpid(pid, &exitcode, 0);
	std::cout << "Exit code: " << exitcode << std::endl;
}

//  another myExec2 just for parallel method
void myExec2(StrVec argList, std::vector<int>& ppid) {
	std::vector<char*> args;
	const int pid = fork();
	if (pid == 0) {
		for (auto& s : argList) {
			args.push_back(&s[0]);  // address of 1st character in str
		}
		// nullptr is very important
		args.push_back(nullptr);
		// Make execvp system call to run desired process
		execvp(args[0], &args[0]);
	}
	ppid.push_back(pid);
}

/**
 * The main method runs a fixed program as a separate child process
 * and waits for it to finish.
 */
 //  transfer string to array
StrVec array(std::string cmd) {
	StrVec c;
	std::istringstream is(cmd);
	std::string x;
	while (is >> std::quoted(x)) {
		c.push_back(x);
		x = "";
	}
	return c;
}
//  method for serial command only include file
void serial(std::ifstream& file) {
	std::string line;
	while (std::getline(file, line)) {
		if (line == "") {  //  if the line is empty get next line
		}
		else {
			StrVec c = array(line);
			if (c[0] == "#") {  //  if the line start with # return nothing
				std::cout << "";
			}
			else {
				if (line == "exit") {  //  if the input is exit, stop loop
					break;
				}
				std::cout << "Running:";
				for (auto& i : c) {
					std::cout << " " << i;
				}
				myExec(c);
			}
		}
	}
}

//  method for parallel command only include file
void parallel(std::ifstream& file) {
	std::string line;
	std::vector<int> ppid;
	while (std::getline(file, line)) {
		if (line == "") {  //  if the line is empty get next line
		}
		else {
			StrVec c = array(line);
			if (c[0] == "#") {    //  if the line start with # return nothing
				std::cout << "";
			}
			else {
				if (line == "exit") {  //  if the input is exit, stop loop
					break;
				}
				std::cout << "Running:";
				for (auto& i : c) {
					std::cout << " " << i;
				}
				myExec2(c, ppid);
				std::cout << "" << std::endl;
			}
		}
	}
	for (auto i : ppid) {
		int exitcode = 0;
		waitpid(i, &exitcode, 0);
		std::cout << "Exit code: " << exitcode << std::endl;
	}
}

int main(int argc, char** argv) {
	// Create a child process.
	std::string line;
	while (std::cout << "> ", std::getline(std::cin, line)) {
		if (line == "") {  //  if the line is empty get next line
		}
		else {
			StrVec c = array(line);
			if (c[0] == "#") {  //  if the line start with # return nothing
				std::cout << "";
			}
			else {
				if (line == "exit") {  //  if the input is exit, stop loop
					break;
				}
				if (c[0] == "SERIAL") {
					std::ifstream file(c[1]);
					serial(file);
				}
				else {
					if (c[0] == "PARALLEL") {
						std::ifstream pile(c[1]);
						parallel(pile);
					}
					else {
						std::cout << "Running:";
						for (auto& i : c) {
							std::cout << " " << i;
						}
						myExec(c);
					}
				}
			}
		}
	}
	return 0;
}
