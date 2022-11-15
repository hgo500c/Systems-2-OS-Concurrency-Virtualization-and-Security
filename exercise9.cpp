// Copyright (C) 2014 raodm@miamioh.edu

#include <iostream>
#include <algorithm>
#include <functional>
#include <thread>
#include <mutex>
#include <deque>
#include <string>
#include <condition_variable>

// Prototypes to keep compiler happy
void processStr(std::string value);
std::string createStr();

/**
   Maximum number of elements that can be held by the shared queue at
   any given instant of time.
*/
const size_t MaxQSize = 5;

/** As you may recollect vividly from CSE-274, deque (usually
    pronounced like "deck") is an irregular acronym of double-ended
    queue. Double-ended queues are a kind of sequence container. As
    such, their elements are ordered following a strict linear
    sequence.
*/
std::deque<std::string> queue;

/**
   A mutex to arbitrate access to the queue by the producers and
   consumers.
*/
std::mutex queueMutex;
std::condition_variable data_cond;
/**
   The consumer method that processes StrCount strings from the queue
   and prints the result to standard output.
*/
void consumer(const int StrCount) {
    int i = 0;
    while (i < StrCount) {
        std::string value;
        bool haveStr = false;
        {  // Begin critical Section
            std::unique_lock<std::mutex> lock(queueMutex);
                data_cond.wait(lock, []{return !queue.empty();
                });               
                value = queue.front();
                queue.pop_front();
                haveStr = true;  
                data_cond.notify_one();
        }  // End Critical Section

        // Process string value if one was obtained
        if (haveStr) {
            processStr(value);
            i++;
        }
    }
}

void producer(const int StrCount) {
    for (int i = 0; (i < StrCount); i++) {
        std::string value = createStr();
        bool stringAdded  = false;
        // Try to add the string to the queue. If the queue is full,
        // try again.
        while (!stringAdded) {
            // Begin critical Section
           std::unique_lock<std::mutex> lock(queueMutex);
           data_cond.wait(lock, []{return queue.size() <  MaxQSize; });
                queue.push_back(value);
                stringAdded = true;
                data_cond.notify_one();            
        }  // End Critical Section
    }
}

//--------------------------------------------------------------------
//  DO  NOT  MODIFY  CODE  BELOW  THIS  LINE
//  DO  NOT  MODIFY  CODE  BELOW  THIS  LINE
//--------------------------------------------------------------------

std::string rotate(std::string value, unsigned int *seed, int range) {
    const int times = rand_r(seed) % range;
    for (int i = 0; (i < times); i++) {
        std::rotate(value.begin(), value.begin() + 1, value.end());
    }
    return value;
}

void processStr(std::string value) {
    static unsigned int seed = 9059;
    // Run the roulette and print value to check luck
    std::cout << rotate(value, &seed, 10000) << std::endl;
}

std::string createStr() {
    static unsigned int seed = 8419;
    // Generate using a roulette.
    return rotate("abcdefghijklmnopqrstuvwxyz", &seed, 100);
}

int main() {
    std::thread prodThread(producer, 5000);
    std::thread consThread(consumer, 5000);
    prodThread.join();
    consThread.join();
    return 0;
}
