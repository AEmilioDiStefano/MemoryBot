#ifndef REMEMBERER_H
#define REMEMBERER_H

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>
#include "InvertedIndex.h"

class Rememberer {
public:

  InvertedIndex invertedIndex;

  void introduction();

  void memoryBotConversation();

  void talkToDeadPeople();

  void conversation();

  void writeStringToFile(std::string string_line, std::string& file_name);

  std::string upper(std::string& str);

  std::string lower(std::string& str);

  std::string capitalize(std:: string str);

  std::vector<std::string> split(std::string& str);

  std::vector<std::string> readlines(std::string& file_name);

  bool a_is_in_b(std::string sub_string, std::string full_string);

  int randomIndex(std::vector<std::string> input_vector);
  
  std::string entity_nametag;
  std::string user_nametag;
  std::string memory_file_name;
  std::vector<std::string> greeting_strings;
  std::vector<std::string> unsure_of_answer_strings;
  std::vector<std::string> goodbye_strings;

private:
  std::string response;
  std::vector<std::string> brain_vector;
  std::string first_name; 
  std::string first_name_upper;
  
  // Security Functions:
  
  // This function limits the number of characters in user input 
  // strings in order to prevent overflow attacks involving  
  // excessiveuser input.
  bool limitStringLength(std::string str);

};

#endif