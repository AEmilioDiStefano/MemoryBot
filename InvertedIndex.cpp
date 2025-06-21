//////////////////////////////////////////////////////////////////////////////
///                                                                         //
///   CLASS NAME: InvertedIndex                                             //
///      VERSION: 1.0                                                       //
///         DATE: June 21, 2025                                             //
///    DEVELOPER: Andrew Emilio DiStefano                                   //
///                                                                         //
///  DESCRIPTION: The functions included in the InvertedIndex class allow   //
///  this program to search through large data sets for a desired string    //
///  based on string input.  This logic is what powers many popular search  //
///  engines at the time of writing.                                        //
///                                                                         //
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "external/nlohmann/json.hpp"
#include "InvertedIndex.h"
#include "Rememberer.h"

// This library helps us to perform CRUD operations on our JSON files.
using json = nlohmann::json;

/*
 * This function breaks each string down into individual words (or tokens).
 */ 
std::vector<std::string> InvertedIndex::tokenize(const std::string& text) 
{
    // This instream object will process user input strings.
    std::istringstream stream(text);
    std::string word;
    // This vector will hold each word (or token) at its own index. 
    std::vector<std::string> tokens;
    // For each word in the stream, 
    while (stream >> word) 
    {
        // remove punctution (non-letter characters) from input,
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        // make all characters lowercase,
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        // and add the token to the tokens vector.
        tokens.push_back(word);
    }
    // Return a vector of words (tokens) which make up the processed input string.
    return tokens;
}

/*
 * THis function indexes questions (tokens) to the inverted index.
 */
void InvertedIndex::indexQuestion(const std::string& question) 
{
    for (const auto& word : tokenize(question)) 
    {
        invertedIndex[word].insert(question);
    }
}

/*
 * This function loads data into the inverted index from a JSON file.
 */
void InvertedIndex::loadFromJson(const std::string& filename) {
    std::ifstream inFile(filename);
    // If the file is not opened succesfully,
    if (!inFile.is_open()) 
    {
        // then throw an error.
        throw std::runtime_error("Could not open file for reading.");
    }
    json j;
    inFile >> j;
    for (auto& [q, a] : j.items()) 
    {
        if (qaMap.find(q) == qaMap.end()) 
        {
            qaMap[q] = a;
            indexQuestion(q);
        }
    }
}

/*
 * This function saves all information in the question/answer 
 * unordered map into our JSON file.
 */
void InvertedIndex::saveToJson(const std::string& filename) 
{
    json j;
    for (const auto& [q, a] : qaMap) 
    {
        j[q] = a;
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open()) 
    {
        throw std::runtime_error("Could not open file for writing.");
    }

    outFile << j.dump(4);
}

/*
 * This function adds a question-answer (key-value) pair to our inverted index.
 */ 
void InvertedIndex::addEntry(const std::string& question, const std::string& answer) {
    if (qaMap.find(question) == qaMap.end()) 
    {
        qaMap[question] = answer;
        indexQuestion(question);
    }
}

/*
 * This function removes a question-answer (key-value) pair to our inverted index.
 */
void InvertedIndex::removeEntry(const std::string& question) {
    if (qaMap.find(question) != qaMap.end()) 
    {
        qaMap.erase(question);
        removeQuestionFromIndex(question);
    }
}

/*
 * This function defines the behavior of the Linear Merge Count sorting algorithm
 * which makes this system so efficient at searching large data sets of 
 * question-answer pairs for the best possible match.
 */
int InvertedIndex::linearMergeCount(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    // Shared tokens within user input strings and saved answer keys are 
    // kept track of via a two-dimensional grid made from two vectors.   
    int i = 0, j = 0, count = 0;
        while (i < a.size() && j < b.size()) 
        {
            if (a[i] == b[j]) 
            {
                ++count;
                ++i;
                ++j;
            } 
            else if (a[i] < b[j]) 
            {
                ++i;
            } 
            else 
            {
                ++j;
            }
        }
        return count;
    }

/*
 * This is the function that searches our inverted index for the question string 
 * which has most tokens (words) in common with the user input question.
 */
std::string InvertedIndex::findBestMatch(const std::string& input_question, std::string not_found_string) {
    Rememberer rememberer;

    // Tokenize the input question into a vector of token (word) elements.
    auto input_tokens = tokenize(input_question);

    // Sort through these tokens comparing for common tokens (words).
    std::sort(input_tokens.begin(), input_tokens.end());

    // The best match is the question (key) with the most tokens (words)
    // shared with the user input string.
    std::string best_match;
    int best_score = 0;

    // For each question-answer pair in our map of questions and answers,
    for (const auto& [question, answer] : qaMap) {
        auto question_tokens = tokenize(question);
        std::sort(question_tokens.begin(), question_tokens.end());
        int score = linearMergeCount(input_tokens, question_tokens);
        if (score > best_score) 
        {
            best_score = score;
            best_match = question;
        }
    }

    // If there are matches, return the best match.
    if (!best_match.empty()) 
    {
        return qaMap.at(best_match);
    } 

    // If there are no matches, 
    else 
    {
        // then note that the string was not found.
        return not_found_string;
    }
}

