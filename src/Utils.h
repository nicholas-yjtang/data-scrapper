#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <thread>
#include <memory>

using namespace std;

class Utils {
    public:
        static string readFile(const string &filename);
        static void closeThread(shared_ptr<thread> thread);
        static bool compareFiles(const string &file1, const string &file2);
};
#endif