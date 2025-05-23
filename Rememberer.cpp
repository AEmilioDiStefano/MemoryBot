#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>
#include "Rememberer.h"

// This function breaks the ice between MemoryBot and the user.  
// The values for the user_name and user_name_upper are 
// extracted from user input.
void Rememberer::introduction() {
  // This variable stores the raw data input from the user after they are 
  // prompted for their name.
  std::string user_name;
  std::cout << "\n\nPlease enter your name: \n" << std::endl;
  std::cin >> user_name;

  //////////////// FIXME: Add a security measure to limit the 'size' ////////
  /////////////////////// of user input for the user_name variable.  ////////
  
  std::vector<std::string> name_vector = split(user_name);
  Rememberer::first_name = name_vector[0];
  
  std::string bot_name_upper = upper(bot_name);
  first_name_upper = upper(first_name);
  std::cout << "\n" + bot_name_upper +   ": Hello " + first_name + ", how can I be of service? \n\n" + first_name_upper + ": ";
}



void Rememberer::conversation() {
  std::string question;
  std::string answer;
  bool question_found = false;
  
  while (a_is_in_b("goodbye", question) != true) {
    
    std::cin >> question;

    // If the word "goodbye" is in user input,
    if (a_is_in_b("goodbye", question) == true) {
      // Say goodbye to the user and terminate the program.
      std::cout << "\n\nIt has been a pleasure, I hope that I have been of service. Goodbye for now." << std::endl;
      return;
      
      // Otherwise,
    } else {
      // turn the question to lowercase to facilitate comparison,
      question = lower(question);

      // use the Rememberer class's readlines() function to create a vector
      // where each line of the txt file is an index,
      std::vector<std::string> q_and_a_vector = readlines(memory_file_name);
      
      // Check each element in the vector to see if the user's question is in any of them.
      for (size_t i = 0; i < q_and_a_vector.size(); ++i) {
        if (a_is_in_b(question, q_and_a_vector[i]) == true) {
           std::cout << "\n\n" + upper(bot_name) + ": " + q_and_a_vector[i + 1] << std::endl << std::endl << first_name_upper + ": ";
           question_found = true;
           break;
        }
      }

      if (!question_found) {
        std::cout << upper(bot_name) + ": I'm sorry, I can't recall the answer to that question.  Please provide the answer and I'll make sure not to forget it, or say 'skip' to skip that question and ask a different question." << std::endl << std::endl << first_name_upper + ": ";
        std::cin >> answer;
        
        if (a_is_in_b("skip", answer) == true) {
/////////////////////////////////////////////////////////////////////
//////////////// FIXME: Somethign within a few lines of this coment is dokubling name sin the output.
/////////////////////////////////////////////////////////////////////
          std::cout << upper(bot_name) + ": The question has been skipped, no answer was saved for that question.";
          break;
        }
        else {
          writeStringToFile(question, memory_file_name);
          writeStringToFile(answer, memory_file_name);
          break;
        }
      }
    }
  } 
}


//////////////////////////////////////////////////////////////////////////
//////////////// FIXME: CITE GOOGLE AI FOR THIS FUNCTION /////////////////
//////////////////////////////////////////////////////////////////////////

// This method acts identically to Python's split() function.  It takes a  
// string and creates a vector where each word in the string is an item  
// in the vector.
std::vector<std::string> Rememberer::split(std::string& str) {
  std::vector<std::string> words;
  std::stringstream ss(str);
  std::string word;
  while (std::getline(ss, word, ' ')) {
    words.push_back(word);
  }
  return words;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//////////////// FIXME: CITE GOOGLE AI FOR THIS FUNCTION /////////////////
//////////////////////////////////////////////////////////////////////////

// This function acts identically to Python's upper() function.  It turns 
// all of a string's characters to uppercase.

// this line is modified (there was a const here for the string which threw 
// an error)
std::string Rememberer::upper(std::string& str) {
  // this line is unchanged from google ai
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  // HERE, a line was omitted.
  // this next line was added by me, what was there originally (return 0) 
  // threw an error since the function was written by the AI as a 
  // main function (type int)
  return str;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////// FIXME: CITE GOOGLE AI FOR THIS FUNCTION /////////////////
//////////////////////////////////////////////////////////////////////////

// This function acts identically to Python's upper() function.  It turns 
// all of a string's characters to uppercase.

// THIS IS NOTHING MORE THAN THE INVERSE OF THE ABOVE FUNTION
// this line is modified (there was a const here for the string which threw 
// an error)
std::string Rememberer::lower(std::string& str) {
  // this line is unchanged from google ai
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  // HERE, a line was omitted.
  // this next line was added by me, what was there originally (return 0) 
  // threw an error since the function was written by the AI as a 
  // main function (type int)
  return str;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


std::vector<std::string> Rememberer::readlines(std::string& file_name) {
    
  std::ifstream input_file(file_name);
  if (!input_file.is_open()) {
    std::cerr << "Data file could not be opened." << std::endl;
    exit(0);
  }

  std::vector<std::string> lines;
  std::string line;

  while (std::getline(input_file, line)) {
    lines.push_back(line);
  }

  input_file.close();

  return lines;

}


// This function acts identically to the word 'in' in Python.  It checks
// if a string contains a substring, or if string_a is in string_b.
bool Rememberer::a_is_in_b(std::string string_a, std::string string_b) {
  size_t found = string_b.find(string_a);

  if (found != std::string::npos) {
    return true;
  } else {
    return false;
  }
}


//////////////////////////////////////////////////////////////////////////
//////////////// FIXME: CITE GOOGLE AI FOR THIS FUNCTION /////////////////
//////////////////////////////////////////////////////////////////////////

void Rememberer::writeStringToFile(std::string string_line, std::string& file_name) {
  std::ofstream outputFile;

  // Open the file in append mode. This ensures that new data is written 
  // to the end of the file without overwriting existing content.
  outputFile.open(file_name, std::ios::app);

  if (outputFile.is_open()) {
    std::getline(std::cin, string_line);

    // Write the string to the file, followed by a newline character.
    outputFile << string_line << std::endl;

    // Close the file
    outputFile.close();
    std::cout << "Text written to file successfully." << std::endl;
  } else {
    std::cerr << "SYSTEM: There was a problem opening the file and unfortunately your response was not saved." << std::endl;
  }
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

// SECURITY FUNTIONS 
// The following functions enhance system security: 
//////////////// FIXME: Make this function validate user input length ///
bool Rememberer::limitStringLength(std::string str) {
  if (str == str) {
    return true;
  }
  else {
    return false;

  }
}





