# MemoryBot

This program is written in C++ and uses an inverted index data structure along with a linear merge count algorithm to find the best answers to questions asked by the user. The system uses a local JSON data store and does not require an internet connection to run. Large data sets can be stored locally for each agent in order to achieve a high level of precision in answering questions. The application is lightweight and can run on embedded systems. 

The MemoryBot project is a system for seamless access to information regardless of internet accessibility. Tell MemoryBot anything from the melting point of potassium to the name of your fourth cousin Constancia’s fifth daughter, and MemoryBot will remember it for later. No more forgotten birthdays, no more awkwardly asking questions which you should already know the answer to, and no more depending on the internet for answers.

Also, simulate necromancy with the “Talk to dead people” function. Have you ever wanted to interview Benjamin Franklin on life in early America?  How about asking an ancient Roman soldier abnout his experience on campaign in the time of Julius Cesar?  MemoryBot’s “Talk to dead people” function can make that happen. The system is designed for the easy addition of new entities. Just add a JSON file of question-answer (key-value) pairs and make some minor code adjustments to converse with the historical figure or fictional character of your choice. JSON files with question-answer (key-value) pairs can be made manually or generated from an AI service like ChatGTP. As more questions and answers are added to the JSON file, the entity’s dialogue will become more convincing.

Run the propgram with the following commands:
'''console
g++ main.cpp Rememberer.cpp InvertedIndex.cpp
'''
then:
'''console
'''console
./a.out main.cpp Rememberer.cpp InvertedIndex.cpp
'''
'''


