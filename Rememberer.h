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

  void conversation();

  void writeStringToFile(std::string string_line, std::string& file_name);

  std::string upper(std::string& str);

  std::string lower(std::string& str);

  std::vector<std::string> split(std::string& str);

  std::vector<std::string> readlines(std::string& file_name);

  bool a_is_in_b(std::string sub_string, std::string full_string);

  // This variable stores the name of our robot.  
  // Changing this will change the robot's name globally.
  std::string bot_name = "Memory Bot";
  
  std::string bot_name_all_caps = upper(bot_name);

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

  // This variable stores the name of our robot's brain file.  
  // Changing this will change the name of the brain file 
  // searched for by the system.
  std::string memory_file_name = "brain.json";

};

#endif

