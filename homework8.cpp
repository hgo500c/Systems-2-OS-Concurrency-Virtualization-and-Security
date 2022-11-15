/* @copyright zhangc29@miamioh.edu
 * A simple Banking-type web-server.  
 * 
 * This multithreaded web-server performs simple bank transactions on
 * accounts.  Accounts are maintained in an unordered_map.  
 * 
 */

// All the necessary includes are present
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <iomanip>
#include <vector>
// Setup a server socket to accept connections on the socket
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std;
using TcpStreamPtr= std::shared_ptr<tcp::iostream>;
std::mutex gate;
// Forward declaration for method defined further below
std::string url_decode(std::string);
unordered_map<string, double> data;  // this double is money and string
     // is account
/**
 * Top-level method to run a custom HTTP server to process bank
 * transaction requests using multiple threads. Each request should
 * be processed using a separate detached thread. This method just loops 
 * for-ever.
 *
 * @param server The boost::tcp::acceptor object to be used to accept
 * connections from various clients.
 */

void out(string& output, ostream& os);
// put the command in this method
  vector<string> getv(string cmd) {
    vector<string> vec;
    if (cmd.find("&")== string::npos) {
        vec.push_back(cmd.substr(6));
    }  else { 
        vec.push_back(cmd.substr(6, cmd.find("&", 6)-6));
        vec.push_back(cmd.substr(cmd.find("=", 7)+1, 4));  // this is find 
        //  first " = "
        if (cmd.find("=", cmd.find("=", 7)+1)!= string::npos) {
            // if there is second "=" continue add in the vector 
            int a = cmd.find("=", cmd.find("=", 7)+1) + 1; 
            // this is the position of the '='
            vec.push_back(cmd.substr(a));
        }
    }  // this method help me to get the thing I want from command 
    return vec;    
  }   // I use this to find the the vector for the command.
  
  void reset(vector<string> vec, ostream& os) {
       string output = "All accounts reset";
        data.clear();    
        out(output, os);
  }
  void create(vector<string>vec, ostream& os) {
  string output;
        if (data.find(vec[1]) == data.end()) {
            data[vec[1]] = 0.0;  // in the begin, it is zero
            output = "Account " + vec[1] + " created";  // this is created 
        } else {
            output = "Account " + vec[1] + " already exists";
        }  //  when the account is already created 
        out(output, os);
        }
  
  void out(string& output, ostream& os) {
    os << "HTTP/1.1 200 OK\r\n"
                "Server: BankServer\r\n"
                "Content-Length: " << output.size() << "\r\n"  
            //  what should be different size 
                "Connection: Close\r\n"
                "Content-Type: text/plain\r\n\r\n"
                << output;
}  // this is help me to put the out put in the os

void debit(vector<string> vec, ostream& os) {
    string output;
    if (data.find(vec[1]) == data.end()) {
        output = "Account not found";   //  check the account is created
    } else {
        data[vec[1]] -= stod(vec[2]);
        output = "Account balance updated";
    }  //  it will help me to deduct the money on the account
    out(output, os);
}

void credit(vector<string> vec, ostream& os) {
    string output;
    if (data.find(vec[1]) == data.end()) {
        output = "Account not found";
    } else {
        data[vec[1]] += stod(vec[2]);  // this will be add in the unorder map
        output = "Account balance updated";  // this is added the int
    }
    out(output, os);
}

void outstatus(string& output, ostream& os, double ex, int number) {
    os << "HTTP/1.1 200 OK\r\n"
            "Server: BankServer\r\n"
            "Content-Length: " << output.size() + number << "\r\n"
            "Connection: Close\r\n"
            "Content-Type: text/plain\r\n\r\n"
            << output << std::fixed << std::setprecision(2) << ex;
}  // when the status use, it will change the situation 
    // this is a little different with the nor mal output 

void status(vector<string> vec, ostream& os) {
    string output;
    string output2;
    double ex;
    int number;
    if (data.find(vec[1]) == data.end()) {
        output = "Account not found";  // if it is not found
    } else {
        ex = data[vec[1]];
        output = "Account " + vec[1] + ": $";
        number = to_string(ex).substr(0, to_string(ex).find(".", 0)).size();
        output2 = "Account " + vec[1] + ": $";
        number += 3;
    }    //  what is in different number 
    outstatus(output, os, ex, number);
}   // what help me make number in correct way 

void serveClient(std::istream& is, std::ostream& os) { 
    std::lock_guard<std::mutex> guard(gate);  // this is help me to ensure one
      //  thread is running
    std::string trans, w, cmd, ex;
    vector<string> vec;
    std::unordered_map<std::string, int> cmdmap;
    getline(is, trans);   // get the first line
    while (getline(is, ex) && ex != "\r" && !ex.empty()) {} 
       // it is not equal to blank and empty 
    cmd = trans.substr(5, trans.find(" ", 5)-5);
     // what means the TransactionInfo , this is what I want.
    vec = getv(cmd);
     // when the cmd equal to different command, will get in different output  
  if (vec[0] == "reset") {
      reset(vec, os);
    } else if (vec[0] == "create") {
       create(vec, os);
    } else if (vec[0] == "credit") {
        credit(vec, os); 
    } else if (vec[0] == "debit") {
        debit(vec, os);  // it will happen in different situation 
    } else {
        status(vec, os);
    }    
    }

void threadMain(TcpStreamPtr client) {
    // Call routine/regular helper method. For now, we assume 
    // threads never update shared variables/objects.
    serveClient(*client, *client);
}
  // mutiple threads
void runServer(tcp::acceptor& server) {
    // Implement this method to meet the requirements of the homework.
    // Needless to say first you should create stubs for the various 
    // operations, write comments, and then implement the methods.
    //
    // First get the base case to be operational. Then you can multithread. 
       while (true) {        
         TcpStreamPtr client = std::make_shared<tcp::iostream>();
        server.accept(*client->rdbuf());  // wait to connect
        std::thread thr(threadMain, client);  //  beginning 
        thr.detach();  // Let thread run by itself   
      }
} 


//-------------------------------------------------------------------
//  DO  NOT   MODIFY  CODE  BELOW  THIS  LINE
//-------------------------------------------------------------------

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

// Helper method for testing.
void checkRunClient(const std::string& port);

/*
 * The main method that performs the basic task of accepting
 * connections from the user and processing each request using
 * multiple threads.
 */
int main(int argc, char** argv) {
    // Setup the port number for use by the server
    const int port = (argc > 1 ? std::stoi(argv[1]) : 0);
    io_service service;
    // Create end point.  If port is zero a random port will be set
    tcp::endpoint myEndpoint(tcp::v4(), port);
    tcp::acceptor server(service, myEndpoint);  // create a server socket
    // Print information where the server is operating.    
    std::cout << "Listening for commands on port "
              << server.local_endpoint().port() << std::endl;
    // Check run tester client.
#ifdef TEST_CLIENT
    checkRunClient(argv[1]);
#endif

    // Run the server on the specified acceptor
    runServer(server);
    
    // All done.
    return 0;
}

// End of source code

