#include <string>
#include <chrono>
#include <iostream>

#include <ericw-tools-wrapper/wrapper.h>

int main(int argc, const char **argv)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    ericwtoolswrapper::QBsp_CreateBSP("/Users/tinogohlert/Downloads/MAP_DebugLevel.map");

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    std::cout << "map compiled in: " << time / std::chrono::milliseconds(1) << " ms\n";

    return 0;
}
