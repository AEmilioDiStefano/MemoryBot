#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class InvertedIndex {
public:
    void loadFromJson(const std::string& filename);
    void saveToJson(const std::string& filename);

    void addEntry(const std::string& question, const std::string& answer);
    void removeEntry(const std::string& question);

    std::string findBestMatch(const std::string& inputQuestion);

    int linearMergeCount(const std::vector<std::string>& a, const std::vector<std::string>& b);

private:
    std::unordered_map<std::string, std::string> qaMap;
    std::unordered_map<std::string, std::unordered_set<std::string>> invertedIndex;

    static std::vector<std::string> tokenize(const std::string& text);
    void indexQuestion(const std::string& question);
    void removeQuestionFromIndex(const std::string& question);
};

#endif