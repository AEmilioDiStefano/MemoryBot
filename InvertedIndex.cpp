#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "external/nlohmann/json.hpp"
#include "InvertedIndex.h"

using json = nlohmann::json;

std::vector<std::string> InvertedIndex::tokenize(const std::string& text) {
    std::istringstream stream(text);
    std::string word;
    std::vector<std::string> tokens;
    while (stream >> word) {
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        tokens.push_back(word);
    }
    return tokens;
}

void InvertedIndex::indexQuestion(const std::string& question) {
    for (const auto& word : tokenize(question)) {
        invertedIndex[word].insert(question);
    }
}

void InvertedIndex::removeQuestionFromIndex(const std::string& question) {
    for (const auto& word : tokenize(question)) {
        if (invertedIndex[word].count(question)) {
            invertedIndex[word].erase(question);
            if (invertedIndex[word].empty()) {
                invertedIndex.erase(word);
            }
        }
    }
}

void InvertedIndex::loadFromJson(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        throw std::runtime_error("Could not open file for reading.");
    }

    json j;
    inFile >> j;
    for (auto& [q, a] : j.items()) {
        if (qaMap.find(q) == qaMap.end()) {
            qaMap[q] = a;
            indexQuestion(q);
        }
    }
}

void InvertedIndex::saveToJson(const std::string& filename) {
    json j;
    for (const auto& [q, a] : qaMap) {
        j[q] = a;
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not open file for writing.");
    }

    outFile << j.dump(4);
}

void InvertedIndex::addEntry(const std::string& question, const std::string& answer) {
    if (qaMap.find(question) == qaMap.end()) {
        qaMap[question] = answer;
        indexQuestion(question);
    }
}

void InvertedIndex::removeEntry(const std::string& question) {
    if (qaMap.find(question) != qaMap.end()) {
        qaMap.erase(question);
        removeQuestionFromIndex(question);
    }
}

std::pair<std::string, std::string> InvertedIndex::findBestMatch(const std::string& inputQuestion) {
    std::unordered_map<std::string, int> matchCount;
    for (const auto& word : tokenize(inputQuestion)) {
        if (invertedIndex.count(word)) {
            for (const auto& q : invertedIndex[word]) {
                matchCount[q]++;
            }
        }
    }

    std::string bestMatch;
    int maxMatches = 0;
    for (const auto& [q, count] : matchCount) {
        if (count > maxMatches) {
            maxMatches = count;
            bestMatch = q;
        }
    }

    if (!bestMatch.empty()) {
        
        return { bestMatch, qaMap[bestMatch] };
    } else {
        return { "", "No match found." };
    }
}