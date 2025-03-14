#include <vector>
#include <set>
#include <string>

class Search {
private:
    int width, height;
    std::set<std::string> reqWords;
    std::set<std::string> badWords;
    std::vector<std::vector<std::vector<char>>> allSolutions;

    void findSolutions(std::vector<std::vector<char>>& grid, std::set<std::string> remainingWords, 
                       int row, int col, bool findOne);
    bool containsForbiddenWords(const std::vector<std::vector<char>>& grid) const;
    bool placeWord(std::vector<std::vector<char>>& grid, const std::string& word, int row, int col, 
                   int dirX, int dirY);
    void removeWord(std::vector<std::vector<char>>& grid, const std::string& word, int row, int col, 
                    int dirX, int dirY);

public:
    Search(const std::string& filename);
    void solve(bool findOne);
    void printSolutions(const std::string& outputFile, bool findOne) const;
};
