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

// This function breaks the ice between MemoryBot and the user.  
// The values for the user_name and user_name_upper are 
// extracted from user input.
void Rememberer::introduction() 
{
  // This variable stores the raw data input from the user after they are 
  // prompted for their name.
  std::string user_name;
  std::cout << "\n\n  Please enter your name: \n" << std::endl;
  std::getline(std::cin, user_name);
  
  std::vector<std::string> name_vector = split(user_name);
  Rememberer::first_name = name_vector[0];
  user_nametag = upper(first_name) + ": ";
}

void Rememberer::conversation() 
{
  int user_activity_choice;
  while(true) 
  {
    std::cout << "\n  HELLO " + upper(first_name) << std::endl;
    std::cout << "  WHAT WOULD YOU LIKE TO DO?" << std::endl;
    std::cout << "       (MAIN MENU)  \n" << std::endl;
    std::cout << "    1. Startup MemoryBot" << std::endl;
    std::cout << "    2. Add knowledge" << std::endl;
    std::cout << "    3. Talk to dead people" << std::endl;
    std::cout << "\n    0. Exit" << std::endl; 
    std::cout << "\n  Please enter 1, 2, 3, or 0 to make your selection.  \n" << std::endl;

    std::cin >> user_activity_choice;
    std::cin.ignore();

    switch (user_activity_choice) 
    {
      case 1:
      {
        memoryBotConversation();
        break;
      }
      
      case 2:
      {
        
        addKnowledge(memory_file_name);
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

      default:
      {
        std::cout << "\n  Please try again using only the option number with no additional characters.\n" << std::endl;
        std::cout << "  Enter 1 to startup MemoryBot, 2 to talk to dead people, or 0 to exit.\n" << std::endl;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

void Rememberer::memoryBotConversation() 
{

  std::string first_name_capitalized = capitalize(first_name);

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

    int random_greeting_index = randomIndex(greeting_strings);

    std::cout << entity_nametag + greeting_strings[random_greeting_index] + "\n\n" + user_nametag; 

    invertedIndexConversationLoop(
    entity_nametag, 
    memory_file_name, 
    greeting_strings,
    unsure_of_answer_strings,
    goodbye_strings);
}

void Rememberer::talkToDeadPeople()
{
  
  std::cout << "\n  WHO WOULD YOU LIKE TO SPEAK WITH?\n" << std::endl;
  std::cout << "   ENTITY:         NAME:         DESCRIPTION:" << std::endl;
  std::cout << "    1483      Marcus Veturius    A soldier from ancient Rome" << std::endl;
  std::cout << "     535     Benjamin Franklin   A renissance man from early America" << std::endl;
  std::cout << "     225           Grkb          A caveman from 85,000 BC" << std::endl;
  std::cout << "\n          (Enter 0 to go back to the Main Menu)" << std::endl;
  std::cout << "\n   Enter ONLY THE ENTITY NUMBER to make your selection,\n   or ENTER 0 to go back to the MAIN MENU\n\n" + user_nametag;

  int user_entity_choice;
  std::cin >> user_entity_choice;
  std::cin.ignore();

  switch (user_entity_choice) {
    
    case 1483:
    {
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
      entity_nametag = "  GRKB: ";
      memory_file_name = "people/grkb.json";

      greeting_strings = {
        "I am strong Grkb.  Grkb land from green mountain to wide river.", 
        "Grkb is strong.", 
        "Tell old woman from red mountain cave to give back spear.  If not I find her.", 
        "Grkb is happy, found woman from red mountain cave.  Grkb is tired.", 
        "Grkb is hunter of elk, killer of bear.  Grkb wears wolf pelt on shoulders. ", 
        "Take Grkb to the women from green mountains.  Grkb can not find them, Grkb is lost.", 
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

    default:
    {
      std::cout << "Your input did not match any registered entities.  Please try again using only the entity number with no additional characters.\n" << std::endl;
      break;
    }
  }  

  int random_greeting_index = randomIndex(greeting_strings);
  std::cout << "\n" + entity_nametag + greeting_strings[random_greeting_index] + "\n\n" + user_nametag;

  invertedIndexConversationLoop(
  entity_nametag, 
  memory_file_name, 
  greeting_strings,
  unsure_of_answer_strings,
  goodbye_strings);
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

    std::getline(std::cin, current_question);
    
    // If the word "goodbye" is in user input,
    if (a_is_in_b("goodbye", current_question) == true) 
    {

      int random_goodbye_index = randomIndex(goodbye_strings);
      
      // Say goodbye to the user and return to the main menu.
      std::cout << "\n -----------------------------------------" << std::endl;
      std::cout << " -----------------------------------------" << std::endl;
      std::cout << "\n" + entity_nametag + goodbye_strings[random_goodbye_index] + "\n" << std::endl;
      std::cout << " -----------------------------------------" << std::endl;
      std::cout << " -----------------------------------------" << std::endl;
      return;

      // Otherwise,
    } 
    else
    {
      

      int random_unsure_index = randomIndex(unsure_of_answer_strings);

      std::string unsure_string =  unsure_of_answer_strings[random_unsure_index];

      std::string answer = index.findBestMatch(current_question, unsure_string);

      std::cout << "\n" + entity_nametag + answer << std::endl << std::endl << user_nametag;
      continue;
    }
  }

  index.saveToJson(memory_file_name);
}
                  
/////  ____  ______ _____ _____ _   _ 
///// |  _ \|  ____/ ____|_   _| \ | |
///// | |_) | |__ | |  __  | | |  \| |
///// |  _ <|  __|| | |_ | | | | . ` |
///// | |_) | |___| |__| |_| |_| |\  |
///// |____/|______\_____|_____|_| \_|
/////                               
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void Rememberer::addKnowledge(const std::string& filename)
{
  json j = loadExistingJSON(filename);

  std::string question, answer, choice;

  do {
      // Loop until a non-duplicate question is entered
      while (true) {
        std::cout << "Enter a question: ";
        std::getline(std::cin, question);

        std::string sanitizedQuestion = sanitize(question);

        if (j.contains(sanitizedQuestion)) 
        {
            std::cout << "That question already exists. Please enter a new one.\n";
        } 
        else 
        {
            // Valid new question, now ask for the answer
            std::cout << "Enter the answer: ";
            std::getline(std::cin, answer);

            j[sanitizedQuestion] = answer;
            break;
        }
      }

      std::cout << "Add another question-answer pair? (yes/no): ";
      std::getline(std::cin, choice);

  } while (choice == "yes" || choice == "Yes" || choice == "YES");

  // Save updated JSON
  std::ofstream file(filename);
  if (!file) {
      std::cerr << "Error opening file for writing: " << filename << std::endl;
      return;
  }

  file << j.dump(4); // Pretty-printed JSON
  file.close();

  std::cout << "Updated question-answer pairs saved to " << filename << std::endl;
}

/**
 * Loads existing JSON from file if it exists, or returns an empty JSON object.
 */
json Rememberer::loadExistingJSON(const std::string& filename) {
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

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
///  ______ _   _ _____  
/// |  ____| \ | |  __ \ 
/// | |__  |  \| | |  | |
/// |  __| | . ` | |  | |
/// | |____| |\  | |__| |
/// |______|_| \_|_____/ 
//\\ 

int Rememberer::randomIndex(std::vector<std::string> input_vector) 
{
  int random_index;

    // Seed the random number generator
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

    // Check if the vector is not empty to avoid errors
    if (!input_vector.empty()) {
        // Create a distribution for indices
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

//////////////////////////////////////////////////////////////////////////
//////////////// FIXME: CITE GOOGLE AI FOR THIS FUNCTION /////////////////
//////////////////////////////////////////////////////////////////////////

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
std::string Rememberer::upper(std::string& str) 
{
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
std::string Rememberer::lower(std::string& str) 
{
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

//   _____                      _ _             
//  / ____|                    (_) |            
// | (___   ___  ___ _   _ _ __ _| |_ _   _     
//  \___ \ / _ \/ __| | | | '__| | __| | | |    
//  ____) |  __/ (__| |_| | |  | | |_| |_| |    
// |_____/ \___|\___|\__,_|_| _|_|\__|\__, |    
// |  ____|              | | (_)       __/ |    
// | |__ _   _ _ __   ___| |_ _  ___  |___/ ___ 
// |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |  | |_| | | | | (__| |_| | (_) | | | \__ \
// |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//
// The following functions enhance system security. 

std::string Rememberer::sanitize(const std::string& input) {
  std::string output;
    for (char c : input) {
        if (std::isalnum(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c))) {
            output += std::tolower(static_cast<unsigned char>(c));
        }
    }
    return output;
}

