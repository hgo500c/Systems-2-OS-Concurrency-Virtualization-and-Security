/* Copyright (C) 2019 zhangc29@miamiOH.edu
 * 
 * A simple web-server.  
 * 
 * The web-server performs the following tasks:
 * 
 *     1. Accepts connection from a client.
 *     2. Processes cgi-bin GET request.
 *     3. If not cgi-bin, it responds with the specific file or a 404.
 * 
 * Objective: The objective of this part of the homework
 *  is to develop 1 C++ program to:
1. Recollect (from CSE-278) the use of HTTP protocol for communication
2. Develop a web-server that can process HTTP GET requests
       Your web-server should be able to run a program with command-line 
       arguments and returns its outputs back to the client.
3. Continue to build strength with I/O streams & string processing
 * 
 */

#include <ext/stdio_filebuf.h>
#include <unistd.h>
#include <sys/wait.h>
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

// Using namespaces to streamline code below
using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;
using StrVec = vector<string>;

/** Forward declaration for method defined further below.  You will
    need to use this url_decode method in your serveClient method.
 */
std::string url_decode(std::string data);
void output();
void outputerror();
void serveClient(string line, ostream& os);
void notFound(string);
void myExec(StrVec);
vector<string> run(string, bool);
void print(istream&, int);
void printHex(string line, int status);

// Named-constants to keep pipe code readable below
const int READ = 0, WRITE = 1;
/**
 * Process HTTP request (from first line & headers) and
 * provide suitable HTTP response back to the client.
 * 
 * @param is The input stream to read data from client.
 * @param os The output stream to send data to client.
 */


/**
 * helper method to print hex num of size of line
 * @param line a string needs to be print
 * @param status a int that identify how the content needs to be printed
 */
void printHex(string line, int status) {
    if (status == 1) {
        cout << hex << line.size()+1 << "\r\n";
        cout << line << endl;
        cout << "\r\n";
    } else if (status == 2) {
        cout << hex << line.size() << "\r\n";
        cout << line << "\r\n";
        cout << "0\r\n\r\n";
    } else {
        cout << hex << line.size()+1 << "\r\n";
        cout << line << "\r\n\r\n";
        cout << "0\r\n\r\n";
    }
}

// print the correct one
void output() {
    cout << "HTTP/1.1 200 OK\r\n";
    cout << "Content-Type: text/plain\r\n";
    cout << "Transfer-Encoding: chunked\r\n";
    cout << "Connection: Close\r\n\r\n";
}

// print when it is not correct
void outputerror() {
    cout << "HTTP/1.1 404 Not Found\r\n";
    cout << "Content-Type: text/plain\r\n";
    cout << "Transfer-Encoding: chunked\r\n";
    cout << "Connection: Close\r\n\r\n";
}
/**
 * print the result of the running the command and exit code
 * @param is
 * @param pid 
 */
void print(istream& is, int pid) {
    if (pid != 0) {
        string line;
        while (getline(is, line)) {
            printHex(line, 1);
        }
        int exitcode;
        waitpid(pid, &exitcode, 0);
        line = "Exit code: " + to_string(exitcode);
        printHex(line, 2); 
    }
}

/**
 * read the line to the vector that I can run.
 * @param line URl from server
 * @param v   yes: it is good to run
 *            false: it cannot run 
 * @return 
 */
StrVec run(string line, bool v) {
    line = url_decode(line);  // this method help me to decode.
    if (v) {
        line = line.substr(18);
        int i = line.find("&args=");
        line.replace(i, 6, " ");
    }   // this help me replace the & args = to be the command
    istringstream is(line);
    string cmd;
    StrVec cmdtemp;
    while (is >> quoted(cmd)) {
        if (cmd != "HTTP/1.1") {
            cmdtemp.push_back(cmd);
        }  // this help me avoid the HTTP/1.1 what I do not want
    }
    return cmdtemp;  // this is the command that I want in the end.
}

/**
 * make the execute in system
 * @param argList vector include the command(cmd)
 */
void myExec(StrVec argList) {
    std::vector<char*> args;  // give the strvec to args
    for (auto& s : argList) {
        args.push_back(&s[0]);  
    }
    // make it as a char vector
    args.push_back(nullptr);
    // nullptr is very important and need to be push back to vector
    execvp(args[0], &args[0]);
}   // copy at the ppt 

void serveClient(std::istream& is, std::ostream& os) {   
    string line;
    vector<string> cmd;
    while (is >> line) {
        cmd.push_back(line);
        if (cmd.size() == 2) {
            if (cmd[1].find("/cgi-bin/exec") != string::npos) {
                int pipefd[2];
                pipe(pipefd);
                StrVec cmdList = run(cmd[1], 1); 
                output();
                int pid = fork();
                if (pid == 0) {
                 close(pipefd[READ]);
                 dup2(pipefd[WRITE], WRITE);
           __gnu_cxx::stdio_filebuf<char> fb(pipefd[WRITE], std::ios::out, 1);
                 std::ostream os(&fb);
                  myExec(cmdList);
                  // this ill be run when the pipe number is zero 
                } else {
                    close(pipefd[WRITE]);
    __gnu_cxx::stdio_filebuf<char> fb(pipefd[READ], std::ios::in, 1);
                    istream is(&fb);
                    print(is, pid);
                }  // run this in the pipe number is 
                   // not zero and give error         
            } else {
               string temp2 = cmd[1];
               line = url_decode(temp2);
               istringstream is(temp2);
               string cmd;
               StrVec cmdtemp;
               while (is >> quoted(cmd)) {
               if (cmd != "HTTP/1.1") {
               cmdtemp.push_back(cmd);
        }  // this help me avoid the HTTP/1.1 what I do not want
    }
               outputerror();
               string line = "Invalid request: " + 
               cmdtemp[0].substr(1, cmdtemp[0].length()-1);
               printHex(line, 3);;
            }
        }
    }}

// -----------------------------------------------------------
//       DO  NOT  ADD  OR MODIFY CODE BELOW THIS LINE
// -----------------------------------------------------------

/** Convenience method to decode HTML/URL encoded strings.

    This method must be used to decode query string parameters
    supplied along with GET request.  This method converts URL encoded
    entities in the from %nn (where 'n' is a hexadecimal digit) to
    corresponding ASCII characters.

    \param[in] str The string to be decoded.  If the string does not
    have any URL encoded characters then this original string is
    returned.  So it is always safe to call this method!

    \return The decoded string.
*/
std::string url_decode(std::string str) {
    // Decode entities in the from "%xx"
    size_t pos = 0;
    while ((pos = str.find_first_of("%+", pos)) != std::string::npos) {
        switch (str.at(pos)) {
            case '+': str.replace(pos, 1, " ");
            break;
            case '%': {
                std::string hex = str.substr(pos + 1, 2);
                char ascii = std::stoi(hex, nullptr, 16);
                str.replace(pos, 3, 1, ascii);
            }
        }
        pos++;
    }
    return str;
}

/**
 * Runs the program as a server that listens to incoming connections.
 * 
 * @param port The port number on which the server should listen.
 */
void runServer(int port) {
    io_service service;
    // Create end point
    tcp::endpoint myEndpoint(tcp::v4(), port);
    // Create a socket that accepts connections
    tcp::acceptor server(service, myEndpoint);
    std::cout << "Server is listening on port "
              << server.local_endpoint().port() << std::endl;
    // Process client connections one-by-one...forever
    while (true) {
        tcp::iostream client;
        // Wait for a client to connect
        server.accept(*client.rdbuf());
        // Process information from client.
        serveClient(client, client);
    }
}

/*
 * The main method that performs the basic task of accepting connections
 * from the user.
 */
int main(int argc, char** argv) {
    if (argc == 2) {
        // Process 1 request from specified file for functional testing
        std::ifstream input(argv[1]);
        serveClient(input, std::cout);
    } else {
        // Run the server on some available port number.
        runServer(0);
    }
    return 0;
}

// End of source code
