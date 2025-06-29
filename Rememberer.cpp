//////////////////////////////////////////////////////////////////////////////
///                                                                         //
///   CLASS NAME: Rememberer                                                //
///      VERSION: 1.0                                                       //
///         DATE: June 21, 2025                                             //
///    DEVELOPER: Andrew Emilio DiStefano                                   //
///                                                                         //
///  DESCRIPTION: The functions included in the Rememberer class set up     //
///  the conversation loop logic for this program.  All functions are       //
///  modularized and reusable.  Extensive security measures address the     //
///  fact that this is the part of the system which will be bombarded       //
///  with user input.  All input is validated and sanitized before it       //
///   enters the system.                                                    //
///                                                                         //
//////////////////////////////////////////////////////////////////////////////

#include <iomanip>
#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>
#include "external/nlohmann/json.hpp"
#include "Rememberer.h"
#include "InvertedIndex.h"

/*
 * This function breaks the ice between MemoryBot and the user.  
 * The values for the user_name and user_name_upper are 
 * extracted from user input.
 */
void Rememberer::introduction() 
{
  // This variable stores the raw data input from the user after they are 
  // prompted for their name.
  std::string user_name;
  std::cout << "\n\n  Please enter your name: \n" << std::endl;

  // This is a dedicated function which validates and sanitizes 
  // strings from user input. 
  user_name = getSafeStringInput(30);
  
  // Take the user's name, extract the first word (the first name), 
  // and assemble a user nametag.
  std::vector<std::string> name_vector = split(user_name);
  Rememberer::first_name = name_vector[0];
  user_nametag = upper(first_name) + ": ";
}

/*
 * This function assembles other modular functions from this class to 
 * create a conversation loop which achieves our desired functionality.
 */
void Rememberer::conversation() 
{

  // This loop is our main menu.
  unsigned int user_activity_choice;
  while(true) 
  {
    std::cout << "\n -----------------------------------------" << std::endl;
    std::cout << " -----------------------------------------" << std::endl;
    std::cout << "\n  HELLO " + upper(first_name) << std::endl;
    std::cout << "  WHAT WOULD YOU LIKE TO DO?" << std::endl;
    std::cout << "       (MAIN MENU)  \n" << std::endl;
    std::cout << "    1. Startup MemoryBot" << std::endl;
    std::cout << "    2. Add knowledge" << std::endl;
    std::cout << "    3. Talk to dead people" << std::endl;
    std::cout << "\n    0. Exit" << std::endl; 
    std::cout << "\n  Please enter 1, 2, 3, or 0 to make your selection.  \n" << std::endl;

    // This is a custom input function written to keep strings safe
    // in the ocntext of the behavior of the program.
    user_activity_choice = getSafeIntInput();

    switch (user_activity_choice) 
    {
      case 1:
      {
        memoryBotConversation();
        break;
      }
      
      case 2:
      {
        addKnowledge("memory_bot.json");
        break;
      }

      case 3:
      {
        talkToDeadPeople();
        break;
      }

      case 0:
      {
        std::cout << "\n  Goodbye.  \n" << std::endl;
        exit(0);
      }

      // There are already several measures here to address invalid input,
      // this acts as a catch-all for cases which may not be detected for 
      // some obscure reason.
      default:
      {
        std::cout << "\n  Please try again using only the option number with no additional characters.\n" << std::endl;
        std::cout << "  Enter 1 to startup MemoryBot, 2 to talk to dead people, or 0 to exit.\n" << std::endl;
      }
    }
  }
}

/*
 * This function defines the logic behind conversations with MemoryBot.
 */
void Rememberer::memoryBotConversation() 
{
  std::string first_name_capitalized = capitalize(first_name);

  // Based on user input, define a series of strings and 
    // string vectors which will be used forfor 'hello', 
    // 'goodbye', and 'I dont know' answers during 
    // conversations with MemoryBot.

  std::string entity_nametag = "  MEMORY BOT: ";
  memory_file_name = "memory_bot.json";

  greeting_strings = {
    "Hello " + first_name_capitalized + ", how can I be of service?", 
    "Hi " + first_name_capitalized + ", what's on your mind?",
    "Glad to see you " + first_name_capitalized + ", what can I help you with today?", 
    "Hi " + first_name_capitalized + ", how can I be of service?",
    "Hi " + first_name_capitalized + ", ask me anything and I'll answer as best I can.", 
    "It's good to see you, " + first_name_capitalized + ".  Ask me anything and I'll answer as best I can.", 
    "Welcome, " + first_name_capitalized + ".  Ask me anything and I'll answer as best I can."
  };

  unsure_of_answer_strings = {
    "I'm sorrry, I don't recall.", 
    "Sorry " + first_name_capitalized + ", I don't recall.", 
    "Apologies, " + first_name_capitalized + ".  I'm entirely not sure.", 
    "As the Colombians say, lo que le digo es mentira.  My answer would be a lie.  In other words, I am unsure.", 
    "I don't have and answer for that.", 
    "I'm not sure.", 
    "I don't know, but I would love to learn.", 
    ""
  };

  goodbye_strings = {
    "It has been a pleasure speaking with you.", 
    "I hope that I have been of service.  Goodbye for now.",
    "Farewell for now.",
    "Goodbye for now.",
    "I hope to see you soon.",
    // ** Note for collaborators **
    // If the conversation has been going on for a while, the phrase ("i enjoyed this conversation") 
    // makes it feel like the program is able to detect the characteristics of a good conversation. 
    // If the user has only asked one or two questions, the phrase comes off as comically sarcastic. 
    // The use of phrases with meanings which depend on the situation is a great way to make the 
    // program's responses seem more organic and deep'.
    "I enjoyed this conversation."
  };

    std::cout << "\n  -----------------------------------------" << std::endl;
    std::cout << "  -----------------------------------------" << std::endl;
    std::cout << "\n  MEMORY BOT HAS BEEN INITIATED" << std::endl;
    std::cout << "\n  TO EXIT, say: \n    'goodbye'" << std::endl;
    std::cout << "\n  -----------------------------------------" << std::endl;
    std::cout << "  -----------------------------------------\n\n" << std::endl;

    // Output a random element from the greeting_strings index.
    unsigned int random_greeting_index = randomIndex(greeting_strings);
    std::cout << entity_nametag + greeting_strings[random_greeting_index] + "\n\n" + user_nametag; 

    // This function takes the parameters below 
    // and arranges them into a conversation 
    // with MemoryBot.
    invertedIndexConversationLoop(
    entity_nametag, 
    memory_file_name, 
    greeting_strings,
    unsure_of_answer_strings,
    goodbye_strings);
}

// This function deifnes the functionality of the 
// 'talk to dead people' feature.
void Rememberer::talkToDeadPeople()
{
  while(true) {

    std::cout << "\n -----------------------------------------" << std::endl;
    std::cout << " -----------------------------------------" << std::endl;
    std::cout << "                  ______               " << std::endl;
    std::cout << "                -        -             " << std::endl;
    std::cout << "              /                        " << std::endl;
    std::cout << "             |              |          " << std::endl;
    std::cout << "             |,  .-.  .-.  ,|          " << std::endl;
    std::cout << "             | )(__/   __)( |          " << std::endl;
    std::cout << "             |/     /       |          " << std::endl;
    std::cout << "             (_     ^^     _)          " << std::endl;
    std::cout << "               __|IIIIII|__/           " << std::endl;
    std::cout << "               |  IIIIII/ |            " << std::endl;
    std::cout << "                          /            " << std::endl;
    std::cout << "                `--------`             \n" << std::endl;
    std::cout << "     WHO WOULD YOU LIKE TO SPEAK WITH? \n" << std::endl;
    std::cout << "   ENTITY:         NAME:         DESCRIPTION:" << std::endl;
    std::cout << "    1483      Marcus Veturius    A soldier from ancient Rome" << std::endl;
    std::cout << "     535     Benjamin Franklin   A renissance man from early America" << std::endl;
    std::cout << "     225           Grkb          A caveman from 85,000 BC\n" << std::endl;
    std::cout << "            (Enter 0 to go back)     \n\n" + user_nametag;

    unsigned int user_entity_choice = getSafeIntInput();

    // Based on user input, define a series of strings and 
    // string vectors which will be used forfor 'hello', 
    // 'goodbye', and 'I dont know' answers during 
    // conversations with MemoryBot.
    switch (user_entity_choice) {
      
      case 1483:
      {
        entity_full_name = "Marcus Veturius";
        entity_nametag = "  MARCUS: ";
        memory_file_name = "people/roman_soldier.json";

        greeting_strings = {
          "You speak. I am listening.", 
          "What do you seek from a dead man?",
          "I did not expect company here.", 
          "Speak, friend.  Stir now the memories that I had buried.",
          "Speak, and I will answer if I can.", 
          "Strange that voices still reach me.", 
          "Another voice... very well, let us speak."
        };

        unsure_of_answer_strings = {
          "I do not know, though I have thought on it.", 
          "That is beyond my understanding now.", 
          "Even in life I had no answer to that.", 
          "If there is truth in it, it has not been shown to me.", 
          "I remember fragments, nothing clear.", 
          "I feel the shape of it, but not the meaning.", 
          "I cannot say. Not anymore.", 
          "Some things are hidden, even in death."
        };

        goodbye_strings = {
          "Go and remember what I have said.", 
          "Leave me now. I will return to my waiting.",
          "Our time fades, as all things do.",
          "Walk carefully, wherever you dwell.",
          "May your questions bring you wisdom.",
          "Let the silence take me again."
        };
        
        break;
      }
      
      case 225:
      {
        entity_full_name = "Grkb";
        entity_nametag = "  GRKB: ";
        memory_file_name = "people/grkb.json";

        greeting_strings = {
          "I am strong Grkb.  Grkb land from green mountain to wide river.", 
          "Grkb is strong.", 
          "Tell old woman from red mountain cave to give back rope.", 
          "Grkb is happy, found woman from red mountain cave.  Grkb is tired.", 
          "Grkb is hunter of elk, killer of bear.  Grkb wear wolf pelt on shoulder. ", 
          "Grkb is busy - found eggs.  Ask question quickly."
        };

        unsure_of_answer_strings = {
          "Grkb no.", 
          "Not see what you say.", 
          "Grkb is confused.  Now Grkb is angry!",
          "Grkb does not understand.",
          "Grkb not speak future language.", 
          "Grkb is happy.", 
          "Grkb has no answer."
        };

        goodbye_strings = {
          "Grkb has spoken.", 
          "Grkb has spoken.",  
          "Grkb has spoken.", 
          "Grkb has spoken.",  
          "Grkb find rocks now.", 
          "Grkb go to river now.",
          "Grkb hunt horse now."
        };

        break;
      }

      case 535:
      {
        entity_full_name = "Benjamin Franklin";
        entity_nametag = "  BENJAMIN: ";
        memory_file_name = "people/benjamin_franklin.json";

        greeting_strings = {
          "Ah!  A voice not my own - always refreshing.", 
          "Speak freely, friend.  My ears are as open as my spectacles are wide.", 
          "Salutations!  The mind stirs when strangers speak.", 
          "Well met!  I trust you bring a question, a curiosity, or at least a decent story.", 
          "A pleasure, truly.  What mischief or wisdom shall we stir today?", 
          "A fine day for discourse, assuming no thunderclouds in mind.", 
          "Hail and hello - if its trouble you speak, you're a few decades too late.", 
          "Ah, good day to you - how fares the world of the living?"
        };

        unsure_of_answer_strings = {
          "I cannot say - my candle burned low before I reached that chapter.", 
          "In truth, I am unable to answer that with anything but guesswork.", 
          "I must admit: I have not the faintest notion, but I admire the inquiry.", 
          "I know not of what you speak, but I dearly wish I did.", 
          "Yoru words are a puzzle yet unsolved, at least by me."
        };

        goodbye_strings = {
          "Then we part - may your path be better lit than ours often were.", 
          "Go well, and if you stumble, stumble forward.", 
          "Fare the well - and may your questions multiply like rabbits in spring.", 
          "Well, thats me back to the aether.  Farewell for now.", 
          "I leave you to your time - a more interesting age, I trust."
        };
        
        break;
      }

      case 0: 
      {
        return;
      }

      // There are already several measures here to address invalid input,
      // this acts as a catch-all for cases which may not be detected for 
      // some obscure reason.
      default:
      {
        std::cout << "\n  Your input did not match any of the listed entities.\n  Please try again using only the entity number with no additional characters.\n" << std::endl;
        break;
      }
    }  

    // This ominous message is meant to add to the user experience.
    std::cout << "\n -----------------------------------------" << std::endl;
    std::cout << " -----------------------------------------" << std::endl;
    std::cout << "\n  **  " + entity_full_name + " is present.  **" << std::endl;
    std::cout << "\n  To prevent unintended disturbances,  " << std::endl;
    std::cout << "  avoid mirrors and reflective surfaces    " << std::endl;
    std::cout << "  until the conversation has ended.                  " << std::endl;
    std::cout << "\n -----------------------------------------" << std::endl;
    std::cout << " -----------------------------------------" << std::endl;

    unsigned int random_greeting_index = randomIndex(greeting_strings);
    std::cout << "\n" + entity_nametag + greeting_strings[random_greeting_index] + "\n\n" + user_nametag;

    invertedIndexConversationLoop(
    entity_nametag, 
    memory_file_name, 
    greeting_strings,
    unsure_of_answer_strings,
    goodbye_strings);
  }
}

// This function controls our program's use of the InvertedIndex class.
// Information from around the system is brought together to assemble a
// conversation loop so that the user can speak with the chosen entity.
void Rememberer::invertedIndexConversationLoop(
  // Function parameters:
  std::string entity_nametag, 
  std::string memory_file_name, 
  std::vector<std::string> greeting_strings,
  std::vector<std::string> unsure_of_answer_strings,
  std::vector<std::string> goodbye_strings) 
  {
  // Local variables:
  InvertedIndex index;
  std::string current_question;
  std::string answer;
  bool question_found = false;

  index.loadFromJson(memory_file_name);

  while (true) {

    current_question = getSafeStringInput(2000);
    
    // If the word "goodbye" is in user input,
    if (a_is_in_b("goodbye", current_question) == true) 
    {
      // then say goodbye to the user.
      unsigned int random_goodbye_index = randomIndex(goodbye_strings);
      std::cout << "\n -----------------------------------------" << std::endl;
      std::cout << " -----------------------------------------" << std::endl;
      std::cout << "\n" + entity_nametag + goodbye_strings[random_goodbye_index] + "\n" << std::endl;

      return;

      // Otherwise,
    } 
    else
    {
      // Use the findBestMatch() function from the InvertedIndex class
      // (using the linear merge count algorithm) to find the best 
      // answer for the user's question based on word matches.
      unsigned int random_unsure_index = randomIndex(unsure_of_answer_strings);
      std::string unsure_string =  unsure_of_answer_strings[random_unsure_index];
      std::string answer = index.findBestMatch(current_question, unsure_string);
      std::cout << "\n" + entity_nametag + answer << std::endl << std::endl << user_nametag;
      continue;
    }
  }

  index.saveToJson(memory_file_name);
}

/*
 * This function defines the behavior of the conversation loop 
 * used to add data to MemoryBoy's brain file.
 */
void Rememberer::addKnowledge(const std::string& filename)
{
  // Use the nlohmann library to load a JSON file.
  json j = loadExistingJSON(filename);

  std::string question, answer, choice;

  std::cout << "\n -----------------------------------------" << std::endl;
  std::cout << " -----------------------------------------" << std::endl;
  std::cout << "\n  YOU ARE IN THE MIND OF MEMORY BOT  \n" << std::endl;
  std::cout << "  SAY 'nevermind' or enter '0' at any time \n  to stop editing WITHOUT making changes" << std::endl;

  do {
      // Loop until a non-duplicate question is entered.
      while (true) {
        
        std::cout << "\n  Enter the QUESTION to remember an answer for: \n" << std::endl;
        question = getSafeStringInput(500);

        std::string prepared_question = prepareForFile(question);

        // If the word 'nevermind' appears in user input,
        // or if user ionput is equal to '0',
        if (a_is_in_b("nevermind", prepared_question) || prepared_question == "0") 
        {
          // then take the user back to the main menu.
          std::cout << "\n  * Navigating back to the Main Menu *" << std::endl;
          std::cout << "    NO new information has been saved  \n" << std::endl;
          return;
        }

        // This 'if' statement avoids duplicate key values in our JSON file.
        if (j.contains(prepared_question)) 
        {
            std::cout << "\n  That question ALREADY EXISTS. Please enter a new one.  \n" << std::endl;
        } 
        else 
        {
            // Valid new question, now ask for the answer
            std::cout << "\n  Now enter the ANSWER to remember for that question:  \n" << std::endl;
            answer = getSafeStringInput(500);

            if (a_is_in_b("nevermind", answer) || answer == "0") {
              std::cout << "\n  * Navigating back to the Main Menu *  \n" << std::endl;
              std::cout << "  No new information has been saved  \n" << std::endl;
              return;
            }

            j[prepared_question] = answer;
            break;
        }
      }

      // Show the user which question-answer pairs will be stored to the JSON file.
      std::cout << "\n  The following question-answer pair will be stored to MemoryBot's brain file: \n" << std::endl;
      std::cout << "  QUESTION: " + question << std::endl;
      std::cout << "    ANSWER: " + answer << std::endl;
      
      std::cout << "\n  ADD ANOTHER question-answer pair? (yes/no): \n" << std::endl;
      choice = getSafeStringInput(500);

  } while (choice == "yes" || choice == "Yes" || choice == "YES");

  // Save the updates to the JSON file.
  std::ofstream file(filename);
  if (!file) {
      std::cerr << "\n  Error opening file for writing to the file: " << filename + "\n   Make sure that your JSON file is in the right location.  \n" << std::endl;
      return;
  }

  // Format the question-answer pair for the JSON file.
  file << j.dump(4); 
  file.close();

  std::cout << "\n  Updated question-answer pairs have been saved to " << filename  + "\n" << std::endl;
}

/**
 * THis function loads existing JSON from file if it exists, or returns an empty JSON object.
 */
json Rememberer::loadExistingJSON(const std::string& filename) 
{
    std::ifstream inFile(filename);
    if (!inFile) {
        return json{};
    }

    json existing;
    try {
        inFile >> existing;
    } catch (...) {
        std::cerr << "Warning: Failed to parse existing JSON. Starting fresh.\n";
        return json{};
    }
    return existing;
}

/*
 * This function defines the randomization of vector indices used 
 * to randomize system behavior for an organic conversation.
 */
unsigned int Rememberer::randomIndex(std::vector<std::string> input_vector) 
{
  unsigned int random_index;

    // Seed the random number generator.
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

    // Check if the vector is not empty to avoid errors.
    if (!input_vector.empty()) {
        // Create a distribution for indices.
        std::uniform_int_distribution<int> dist(0, input_vector.size() - 1);

        // Generate a random index
        random_index = dist(rng);
    } 

    return random_index;
}

// This function acts identically to the word 'in' in Python.  It checks
// if a string contains a substring (if string_a is in string_b).
bool Rememberer::a_is_in_b(std::string string_a, std::string string_b) 
{
  size_t found = string_b.find(string_a);

  if (found != std::string::npos) {
    return true;
  } else {
    return false;
  }
}

std::string Rememberer::capitalize(std:: string str) 
{
  
  // Return an empty string if the uinput string is empty.
  if (str.empty()) 
  {
    return ""; 
  }

  // Create a copy of the parameter for modification.
  std::string result = str; 

  // Capitalize the first character of the copied parameter string.
  result[0] = std::toupper(result[0]);

  // Convert all subsequent characters to lowercase.
  for (size_t i = 1; i < result.length(); ++i) 
  {
      result[i] = std::tolower(result[i]);
  }

  return result;
}

// This method acts identically to Python's split() function.  It takes a  
// string and creates a vector where each word in the string is an item  
// in the vector.
std::vector<std::string> Rememberer::split(std::string& str) 
{
  std::vector<std::string> words;
  std::stringstream ss(str);
  std::string word;
  while (std::getline(ss, word, ' ')) {
    words.push_back(word);
  }
  return words;
}

// This function acts identically to Python's upper() function.  It turns 
// all of a string's characters to uppercase.
std::string Rememberer::upper(std::string& str) 
{
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  return str;
}

// This function acts identically to Python's upper() function.  It turns 
// all of a string's characters to uppercase.
std::string Rememberer::lower(std::string& str) 
{
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
}

// This function prepares user input to be used as a key for 
// key-value pairs which will be used in an inverted index
std::string Rememberer::prepareForFile(const std::string& input) 
{
  std::string output;
    for (char c : input) {
        if (std::isalnum(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c))) {
            output += std::tolower(static_cast<unsigned char>(c));
        }
    }
    return output;
}

/*
 * This security function puts the cinIntTypeValidation and 
 * cinIntSizeValidation functions into one custom input 
 * sanitization and validation function for int variables.
 */
int Rememberer::getSafeIntInput() {
    int value;
    while (true) {
      // If the conditions are not met, 
      if (!cinIntTypeValidation(value)) {
        // then do not store the user input.
          std::cout << "\n  Invalid input type. Please enter a valid integer.\n\n" + user_nametag;
          continue;
      }

      if (!cinIntSizeValidation(value)) {
          
        std::cout << "\n  Integer out of range. Must be between 0 and 10,000.\n\n" + user_nametag;
        continue;
      }
      return value;
    }
}

/*
 * This security function puts the stringSanitization and 
 * cinStringSizeValidation functions into one custom input 
 * sanitization and validation function for int variables.
 */
std::string Rememberer::getSafeStringInput(size_t max_length) {
  std::string input;
  // If the conditions are not met, then do not store the value 
  // and continue prompting the user for input.
  while (true) {

      input = cinStringSizeValidation(max_length);
      input = stringSanitization(input);

      if (input.empty()) {
          std::cout << "Input was completely sanitized. Please try again.\n";
          continue;
      }

      return input;
    }
}

/*
 * This function omits potentially dangerous characters 
 * from user input.
 */
std::string Rememberer::stringSanitization(const std::string& input) {
  std::string sanitized;
  const std::string unsafe = "<>\"';&$@#-_+,|\\/()`=*%^";

  for (char ch : input) {
      if (unsafe.find(ch) == std::string::npos && std::isprint(static_cast<unsigned char>(ch))) {
          sanitized += ch;
      }
  }

  return sanitized;
}

/* 
 * This function assures that values passed into 'int' variables 
 * are indeed of type 'int'.
*/
bool Rememberer::cinIntTypeValidation(int &outInt) {
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);
    if (ss >> outInt && ss.eof()) {
        return true;
    }
    return false;
}

/*
 * This function limits the size of strings before they are saved
 * as user input.
 */
std::string Rememberer::cinStringSizeValidation(size_t max_len) {
    std::string input;
    char ch;

    while (std::cin.get(ch) && ch != '\n') {
        if (input.size() < max_len) {
            input += ch;
        }
    }

    // Get rid of all characters beyond the set point (max length).
    if (ch != '\n') {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return input;
}

/*
 * This function allows the user to set the
 * maximum number of characters in a string.
 */  
bool Rememberer::cinIntSizeValidation(int value) {
    return value >= 0 && value <= 10000;
}