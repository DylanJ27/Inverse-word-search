#include <iostream>
#include <fstream>
#include "search.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: inverse_word_search.exe <input_file> <output_file> <mode>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string mode = argv[3];

    if (mode != "one_solution" && mode != "all_solutions") {
        std::cerr << "Error: Mode must be 'one_solution' or 'all_solutions'\n";
        return 1;
    }

    Search search(inputFile);
    search.solve(mode == "one_solution");
    search.printSolutions(outputFile, mode == "one_solution");

    return 0;
}