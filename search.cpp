#include "search.h"
#include <iostream>
#include <fstream>

Search::Search(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    file >> width >> height;
    char type;
    std::string word;
    while (file >> type >> word) {
        if (type == '+') {
            reqWords.insert(word);
        } else if (type == '-') {
            badWords.insert(word);
        }
    }
}

void Search::findSolutions(std::vector<std::vector<char>>& grid, std::set<std::string> remainingWords, 
                        int row, int col, bool findOne) {
    if (remainingWords.empty()) {
        if (!containsForbiddenWords(grid)) {
            allSolutions.push_back(grid);
            if (findOne) return;
        }
    return;
    }

    std::string word = *remainingWords.begin();
    remainingWords.erase(remainingWords.begin());

    int directions[8][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            for (int d = 0; d < 8; d++) {
                if (placeWord(grid, word, r, c, directions[d][0], directions[d][1])) {
                    findSolutions(grid, remainingWords, 0, 0, findOne);
                    if (findOne && !allSolutions.empty()) return;
                        removeWord(grid, word, r, c, directions[d][0], directions[d][1]); 
                }
            }
        }
    }
}


bool Search::containsForbiddenWords(const std::vector<std::vector<char>>& grid) const {
    int h = grid.size(), w = grid[0].size();

    for (std::set<std::string>::iterator it = badWords.begin(); it != badWords.end(); ++it) {
        std::string word = *it;
        for (int r = 0; r < h; r++) {
            std::string row = "", rowRev = "";
            for (int c = 0; c < w; c++) {
                row += grid[r][c];
                rowRev = grid[r][c] + rowRev;
            }
            if (row.find(word) != std::string::npos || rowRev.find(word) != std::string::npos) {
                return true;
            }
        }
        for (int c = 0; c < w; c++) {
            std::string col = "", colRev = "";
            for (int r = 0; r < h; r++) {
                col += grid[r][c];
                colRev = grid[r][c] + colRev;
            }
            if (col.find(word) != std::string::npos || colRev.find(word) != std::string::npos) {
                return true;
            }
        }
        for (int r = 0; r < h; r++) {
            for (int c = 0; c < w; c++) {
                std::string diag1 = "", diag1Rev = "", diag2 = "", diag2Rev = "";
                for (int d = 0; r + d < h && c + d < w; d++) {
                    diag1 += grid[r + d][c + d];
                    diag1Rev = grid[r + d][c + d] + diag1Rev;
                }
                for (int d = 0; r - d >= 0 && c + d < w; d++) {
                    diag2 += grid[r - d][c + d];
                    diag2Rev = grid[r - d][c + d] + diag2Rev;
                }
                if (diag1.find(word) != std::string::npos || diag1Rev.find(word) != std::string::npos || 
                    diag2.find(word) != std::string::npos || diag2Rev.find(word) != std::string::npos) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Search::placeWord(std::vector<std::vector<char>>& grid, const std::string& word, 
    int row, int col, int dirX, int dirY) {
    int x = row, y = col;
    for (size_t i = 0; i < word.length(); i++) {
        if (x < 0 || y < 0 || x >= height || y >= width || 
        (grid[x][y] != '.' && grid[x][y] != word[i])) {
            return false;
        }
    x += dirX;
    y += dirY;
    }
    x = row, y = col;
    for (size_t i = 0; i < word.length(); i++) {
    grid[x][y] = word[i];
    x += dirX;
    y += dirY;
    }
    return true;
}

void Search::removeWord(std::vector<std::vector<char>>& grid, const std::string& word, 
                        int row, int col, int dirX, int dirY) {
    int x = row, y = col;
    for (size_t i = 0; i < word.length(); i++) {
        grid[x][y] = '.'; 
        x += dirX;
        y += dirY;
    }
}

void Search::solve(bool findOne) {
    std::vector<std::vector<char>> grid(height, std::vector<char>(width, '.'));
    std::set<std::string> remainingWords = reqWords;
    
    findSolutions(grid, remainingWords, 0, 0, findOne);

    for (size_t i = 0; i < allSolutions.size(); i++) { 
        for (int r = 0; r < height; r++) {
            for (int c = 0; c < width; c++) {
                if (allSolutions[i][r][c] == '.') {
                    allSolutions[i][r][c] = 'a' + (rand() % 26);  
                }
            }
        }
    }
}


void Search::printSolutions(const std::string& outputFile, bool findOne) const {
    std::ofstream out(outputFile);
    if (allSolutions.empty()) {
        out << "No solutions found\n";
        return;
    }
    out << (findOne ? "Board:\n" : std::to_string(allSolutions.size()) + " solution(s)\n");
    for (size_t i = 0; i < (findOne ? 1 : allSolutions.size()); i++) {
        out<< "Board: \n";
        for (size_t r = 0; r < allSolutions[i].size(); r++) {
            out << "  ";
            for (size_t c = 0; c < allSolutions[i][r].size(); c++) {
                out << allSolutions[i][r][c];
            }
            out << '\n';
        }
    }
}
