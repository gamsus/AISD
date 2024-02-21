#include <random>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
using namespace std;

static std::random_device rd;
static std::mt19937 rng{rd()}; 

int dice(int size)
{
    static std::uniform_int_distribution<int> uid(0,2*size-1); 
    return uid(rng);
}

int main(int argc, char **argv)
{
    if(argc > 1) {
        if (strlen(argv[1]) == 0) {
            return 1;
        }
        char* p;
        errno = 0;
        long arg = strtol(argv[1], &p, 10);
        if (*p != '\0' || errno != 0) {
            return 1;
        }

        if (arg < INT_MIN || arg > INT_MAX) {
            return 1;
        }
        int n = arg;
        cout << n << endl;
        for(int i = 0; i < n; ++i)
            std::cout << dice(n) << " ";
    }
    else {
        cout << "Usage: generator <number>" << endl;
    }
}