#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct sWord {
	string data;
	sWord* next = nullptr;
};

struct dWord {
	string data;
	sWord* synonym = nullptr, *antonym = nullptr;
	dWord* next = nullptr, *prev = nullptr;
};

struct Dictionary {
	dWord* head = nullptr, *tail = nullptr;
};

dWord* insertAtEnd(Dictionary* L, const string& val) {
    dWord* tmp = new dWord;
    tmp->data = val;
    tmp->synonym = nullptr;
    tmp->antonym = nullptr;
    tmp->next = nullptr;
    tmp->prev = L->tail;

    if (L->tail) {
        L->tail->next = tmp;
    }
    else {
        L->head = tmp;
    }

    L->tail = tmp;
    return tmp;
}

Dictionary* insertAtHead(Dictionary* L, dWord* newWord) {
    if (L->head == nullptr) {
        L->head = newWord;
        L->tail = newWord;
    }
    else {
        newWord->next = L->head;
        L->head->prev = newWord;
        L->head = newWord;
    }
    return L;
}

Dictionary* insertAtTail(Dictionary* L, dWord* newWord) {
    if (L->head == nullptr) {
        L->head = newWord;
        L->tail = newWord;
    }
    else {
        newWord->prev = L->tail;
        L->tail->next = newWord;
        L->tail = newWord;
    }
    return L;
}

bool isEmpty(Dictionary* L) {
	return (L->head == NULL);
}

void DeleteHead(Dictionary* D) {
	if (isEmpty(D)) {
		cout << " Error, list is empty";
		return;
	}
	dWord* cur;
	cur = D->head->next;
	delete D->head;
	D->head = cur;
	D->head->prev = NULL;
}

void Display_Head_to_Tail(Dictionary* L) {
	dWord* cur = L->head;
	if (isEmpty(L)) {
		cout << "List Is Empty";
		return;
	}
	while (cur != NULL) {
		for (int i = 0; i < 30; i++) {
			cout << cur->data[i];
		}
		cout << " ";
		cur = cur->next;
	}
}

void Display_Tail_To_Head(const Dictionary* D) {
	if (D == nullptr || D->tail == nullptr) {
		cout << "Dictionary is empty." << endl;
		return;
	}

	dWord* current = D->tail;

	while (current != nullptr) {
		cout << current->data << endl;
		current = current->prev;
	}
}

void swap(dWord* a, dWord* b) {
	string tmp;
	tmp = a->data;
	a->data = b->data;
	b->data = tmp;
}

void appendSynonym(dWord* mainWord, const string& synonym) {
	sWord* newSynonym = new sWord;
	newSynonym->data = synonym;
	newSynonym->next = nullptr;

	if (mainWord->synonym == nullptr) {
		mainWord->synonym = newSynonym;
	}
	else {
		sWord* lastSynonym = mainWord->synonym;
		while (lastSynonym->next != nullptr) {
			lastSynonym = lastSynonym->next;
		}
		lastSynonym->next = newSynonym;
	}
}

void appendAntonym(dWord* mainWord, const string& antonym) {
	sWord* newAntonym = new sWord;
	newAntonym->data = antonym;
	newAntonym->next = nullptr;

	if (mainWord->antonym == nullptr) {
		mainWord->antonym = newAntonym;
	}
	else {
		sWord* lastAntonym = mainWord->antonym;
		while (lastAntonym->next != nullptr) {
			lastAntonym = lastAntonym->next;
		}
		lastAntonym->next = newAntonym;
	}
}

void parseLine(const string& line, Dictionary* dictionary) {
	istringstream iss(line);
	string mainWord;
	getline(iss, mainWord, ':');

	dWord* mainWordNode = insertAtEnd(dictionary, mainWord);

	string synonyms;
	getline(iss, synonyms, '#');
	istringstream synonymsStream(synonyms);
	sWord* lastSynonym = nullptr;
	string synonym;
	while (getline(synonymsStream, synonym, ':')) {
		appendSynonym(mainWordNode, synonym);
	}

	string antonyms;
	getline(iss, antonyms);
	istringstream antonymsStream(antonyms);
	sWord* lastAntonym = nullptr;
	string antonym;
	while (getline(antonymsStream, antonym, '#')) {
		appendAntonym(mainWordNode, antonym);
	}
}

void printDictionary(const Dictionary* dictionary) {
    dWord* currentWord = dictionary->head;

    while (currentWord) {
        // Check for duplication
        bool isDuplicated = false;
        dWord* checkDuplicate = dictionary->head;
        while (checkDuplicate != currentWord) {
            if (checkDuplicate->data == currentWord->data) {
                isDuplicated = true;
                break;
            }
            checkDuplicate = checkDuplicate->next;
        }

        // If not duplicated, print the word
        if (!isDuplicated) {
            cout << "Word: " << currentWord->data << endl;

            // Print synonyms
            sWord* currentSynonym = currentWord->synonym;
            cout << "Synonyms: ";
            while (currentSynonym) {
                cout << currentSynonym->data << " ";
                currentSynonym = currentSynonym->next;
            }
            cout << endl;

            // Print antonyms
            sWord* currentAntonym = currentWord->antonym;
            cout << "Antonyms: ";
            while (currentAntonym) {
                cout << currentAntonym->data << " ";
                currentAntonym = currentAntonym->next;
            }
            cout << endl;

            cout << "-------------------" << endl;
        }

        // Move to the next word
        currentWord = currentWord->next;
    }
}

void deleteDictionary(Dictionary*& D) {
	dWord* currentWord = D->head;
	while (currentWord) {
		sWord* currentSynonym = currentWord->synonym;
		while (currentSynonym) {
			sWord* nextSynonym = currentSynonym->next;
			delete currentSynonym;
			currentSynonym = nextSynonym;
		}

		sWord* currentAntonym = currentWord->antonym;
		while (currentAntonym) {
			sWord* nextAntonym = currentAntonym->next;
			delete currentAntonym;
			currentAntonym = nextAntonym;
		}

		dWord* nextWord = currentWord->next;
		delete currentWord;
		currentWord = nextWord;
	}

	delete D;
	D = nullptr;
}

void addSynonymsorAntonyms(Dictionary* dictionary, const string& existingWord, const string& newWord, bool isSynonym);

void replaceSynonyms(dWord* mainWord, const string& synonymData) {
    sWord* currentSynonym = mainWord->synonym;
    while (currentSynonym != nullptr) {
        if (currentSynonym->data == mainWord->data) {
            currentSynonym->data = synonymData;
        }
        currentSynonym = currentSynonym->next;
    }
}

void SyandAnConversion(Dictionary*& D) {
    Dictionary* convertedDictionary = new Dictionary;
    convertedDictionary->head = nullptr;
    convertedDictionary->tail = nullptr;

    dWord* currentWord = D->head;

    while (currentWord != nullptr) {
        // Process main word
        dWord* mainWord = insertAtEnd(convertedDictionary, currentWord->data);

        // Convert and link synonyms
        sWord* currentSynonym = currentWord->synonym;
        while (currentSynonym != nullptr) {
            // Process synonym
            dWord* synonymWord = insertAtEnd(convertedDictionary, currentSynonym->data);
            // Add relationships for the synonym
            appendSynonym(mainWord, synonymWord->data);
            synonymWord->synonym = mainWord->synonym;
            synonymWord->antonym = mainWord->antonym;
            replaceSynonyms(mainWord, synonymWord->data);
            // Use addSynonymsorAntonyms to fill synonyms' synonyms bidirectionally
            /*addSynonymsorAntonyms(convertedDictionary, currentSynonym->data, mainWord->data, true)*/;
           
           
            currentSynonym = currentSynonym->next;
        }


        // Convert and link antonyms
        sWord* currentAntonym = currentWord->antonym;
        while (currentAntonym != nullptr) {
            // Process antonym
            dWord* antonymWord = insertAtEnd(convertedDictionary, currentAntonym->data);
            // Add relationships for the antonym
            appendAntonym(mainWord, antonymWord->data);
            antonymWord->synonym = mainWord->antonym;
            antonymWord->antonym = mainWord->synonym;
           replaceSynonyms(mainWord, antonymWord->data);
            // Use addSynonymsorAntonyms to fill antonyms' antonyms bidirectionally
            /*addSynonymsorAntonyms(convertedDictionary, currentAntonym->data, mainWord->data, false);*/

          
            currentAntonym = currentAntonym->next;
        }

        currentWord = currentWord->next;
    }

    // Update the main dictionary to the converted one
    deleteDictionary(D);
    D = convertedDictionary;
}

void writeToFile(const Dictionary* dictionary, const string& filename) {
	ofstream outputFile(filename);

	if (!outputFile.is_open()) {
		cerr << "Error opening the output file." << endl;
		return;
	}

	dWord* currentWord = dictionary->head;
	while (currentWord) {
		
		outputFile << currentWord->data << ":";

		
		sWord* currentSynonym = currentWord->synonym;
		while (currentSynonym) {
			outputFile << currentSynonym->data;
			if (currentSynonym->next) {
				outputFile << ":";
			}
			currentSynonym = currentSynonym->next;
		}
		outputFile << "#";

		
		sWord* currentAntonym = currentWord->antonym;
		while (currentAntonym) {
			outputFile << currentAntonym->data;
			if (currentAntonym->next) {
				outputFile << "#";
			}
			currentAntonym = currentAntonym->next;
		}
		outputFile << "#\n";

		currentWord = currentWord->next;
	}


	outputFile.close();
}

void addWordToDictionary(Dictionary* dictionary, string word) {
	dWord* newWord = new dWord;
	newWord->data = word;

	if (dictionary->head == nullptr) {
		dictionary->head = newWord;
		dictionary->tail = newWord;
	}
	else {
		dictionary->tail->next = newWord;
		newWord->prev = dictionary->tail;
		dictionary->tail = newWord;
	}
}


// Function 3 - MERGE SORT ALGORITHM 
dWord* merge(dWord* first, dWord* second) {
    if (first == nullptr)
        return second;

    if (second == nullptr)
        return first;

    if (first->data < second->data) {
        first->next = merge(first->next, second);
        first->next->prev = first;
        first->prev = nullptr;
        return first;
    }

    else {
        second->next = merge(first, second->next);
        second->next->prev = second;
        second->prev = nullptr;
        return second;
    }
}

dWord* mergeSort(dWord* head) {
    if (head == nullptr || head->next == nullptr)
        return head;

    dWord* middle = head;
    dWord* end = head->next;

    while (end != nullptr && end->next != nullptr) {
        middle = middle->next;
        end = end->next->next;
    }

    dWord* secondHalf = middle->next;
    middle->next = nullptr;
    secondHalf->prev = nullptr;

    dWord* firstHalfSorted = mergeSort(head);
    dWord* secondHalfSorted = mergeSort(secondHalf);

    return merge(firstHalfSorted, secondHalfSorted);
}

void Sort(Dictionary* L) {
    L->head = mergeSort(L->head);

    dWord* cur = L->head;
    while (cur->next != nullptr) {
        cur->next->prev = cur;
        cur = cur->next;
    }

    L->tail = cur;
}

// Function 4 - Insertion

Dictionary* InsertWord(Dictionary* L, string Word) {
    dWord* cur = L->head;
    bool exist = false;

    while (cur != nullptr) {
        dWord* next = cur->next;

        if (cur->data == Word) {
            exist = true;
            break;
        }

        cur = next;
    }


    if (exist == true) {
        cout << "Word already exists!" << endl;
        return L;
    }


    if (L->head == NULL) // This condition is to remove the NULL Deferencing Warning 
        return L;


    if (exist == false) {
        dWord* newWord = new dWord;
        newWord->data = Word;
        newWord->prev = nullptr;
        newWord->next = nullptr;



        if (newWord->data > L->tail->data)
            insertAtTail(L, newWord);


        else if (newWord->data < L->head->data)
            insertAtHead(L, newWord);


        else {
            cur = L->head;

            while (cur->next != nullptr) {

                if (newWord->data > cur->data && newWord->data < cur->next->data) {
                    cur->next->prev = newWord;
                    newWord->next = cur->next;
                    cur->next = newWord;
                    newWord->prev = cur;
                }
                cur = cur->next;
            }
        }
    }
    return L;
}

// Function 5 
dWord* findOrCreateWord(Dictionary* dict, string word) {
    dWord* current = dict->head;
    while (current != nullptr) {
        if (current->data == word) {
            return current;
        }
        current = current->next;
    }

    dWord* newWord = new dWord();
    newWord->data = word;

    if (dict->head == nullptr) {
        dict->head = newWord;
        dict->tail = newWord;
    }
    else {
        dict->tail->next = newWord;
        newWord->prev = dict->tail;
        dict->tail = newWord;
    }

    return newWord;
}

void addSynonymsorAntonyms(Dictionary* dictionary, const string& existingWord, const string& newWord, bool isSynonym) {
    dWord* currentWord = findOrCreateWord(dictionary, existingWord);

    if (currentWord == nullptr) {
        cout << "Word not found in the dictionary." << endl;
        return;
    }


    sWord* newRelationship = new sWord;
    newRelationship->data = newWord;
    newRelationship->next = nullptr;

    if (isSynonym) {
        appendSynonym(currentWord, newWord);
    }
    else {
        appendAntonym(currentWord, newWord);
    }
}

// Function 6
void deleteWordsContaining(Dictionary* L, string letters) {
    dWord* cur = L->head;

    while (cur != nullptr) {
        dWord* nextNode = cur->next;

        bool containsLetters = false;
        for (char c : cur->data) {
            if (letters.find(c) != string::npos) {
                containsLetters = true;
                break;
            }
        }

        if (containsLetters) {
            if (cur == L->head) {
                L->head = cur->next;
                if (L->head != nullptr) {
                    L->head->prev = nullptr;
                }
            }
            else if (cur == L->tail) {
                L->tail = cur->prev;
                L->tail->next = nullptr;
            }
            else {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
            }

            delete cur;
        }
        cur = nextNode;
    }
}

// Function 7
void printWordsStartingWith(Dictionary* L, string letters) {
    dWord* cur = L->head;
    bool found = false;

    if (L->head == nullptr) {
        cout << "The dictionary is empty!";
        return;
    }

    while (cur != nullptr) {
        dWord* nextNode = cur->next;

        if (cur->data.find(letters) == 0) {
            cout << cur->data << " " << endl;
            found = true;
        }

        cur = nextNode;
    }
    if (!found) {
        cout << "There are no words that start with these letters.";
    }
}

// Function 8
void printSynonymsAntonyms(Dictionary* L) {
    dWord* cur = L->head;
    dWord* next = cur->next;
    bool exist = false;
    string word;

    cout << endl << endl << endl << endl;
    printDictionary(L);
    cout << "Choose a word to display all its synonyms and antonyms: ";
    cin >> word;
    while (cur != nullptr) {
        if (word == cur->data) {
            cout << "The synonyms are: ";
            while (cur->synonym != nullptr) {
                cout << cur->synonym->data << ", ";
                cur->synonym = cur->synonym->next;
            }
            cout << "The antonyms are: ";
            while (cur->antonym != nullptr) {
                cout << cur->antonym->data << ", ";
                cur->antonym = cur->antonym->next;
            }
        }
        else {
            cout << "This word does not exist!";
        }
    }

}

// Function 9
void deleteWordsStartingWith(Dictionary* L, string letters) {
    dWord* cur = L->head;
    bool exist = false;

    while (cur != nullptr) {
        dWord* nextNode = cur->next;

        if (cur->data.find(letters) == 0) {
            if (cur == L->head) {
                L->head = cur->next;
                if (L->head != nullptr) {
                    L->head->prev = nullptr;
                }
            }
            else if (cur == L->tail) {
                L->tail = cur->prev;
                L->tail->next = nullptr;
            }
            else {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
            }

            delete cur;
        }
        cur = nextNode;
        exist = true;
    }

    if (exist) {
        cout << "No Words Exists with this letter!";
    }
}

int main() {
	ifstream inputFile("Words.txt");

	if (!inputFile.is_open()) {
		cerr << "Error opening the file." << endl;
		return 1;
	}

	Dictionary* dictionary = new Dictionary;
	dictionary->head = nullptr;
	dictionary->tail = nullptr;
    string wordToAdd, letters;
    int choice = -1;



	
	string line;
	
	
	while (getline(inputFile, line)) {
		parseLine(line, dictionary);

	}
	

	
    cout << "Dictionary Extracted From File: " << endl <<  endl;
	printDictionary(dictionary);
 
	
	SyandAnConversion(dictionary);

	
	//cout << "\nAfter Conversion:" << endl;
	//printDictionary(dictionary);

    writeToFile(dictionary, "WordOutput.txt");
    
    


    while (choice != 0) {

        cout << "------------------------------------------------------------------------" << endl;
        cout << "----------------------------- DICTIONARY -------------------------------" << endl;
        cout << "------------------------------------------------------------------------" << endl;
        cout << "  1- Display the dictionary \n  2- Display specific words \n  3- Sort the dictionary \n";
            cout << "  4- Insert a new word \n  5- Delete specific words \n  6- Delete words starting with \n  7- Add Synonym(Antonym) to existing Word \n";
        cout << "------------------------------------------------------------------------" << endl;
        cout << "------------------------------------------------------------------------" << endl;

        cin >> choice;
        while (choice == -1) {
            
        }

        cout << "------------------------------------------------------------------------" << endl;
        cout << "------------------------------------------------------------------------" << endl << endl;

        if (choice == 1)
            printDictionary(dictionary);

        else if (choice == 2) {
            cout << "Enter a set of letters: ";
            cin >> letters;
            printWordsStartingWith(dictionary, letters);
            cout << "------------------------------------------------------------------------" << endl;
        }

        else if (choice == 3) {
            Sort(dictionary);
            cout << "Dinctionary sorted successfully!" << endl;
            cout << "------------------------------------------------------------------------" << endl;
            printDictionary(dictionary);
            cout << "------------------------------------------------------------------------" << endl;
        }

        else if (choice == 4) {
            cout << "------------------------------------------------------------------------" << endl;
            cout << "Enter a new word to insert: ";
            cin >> letters;
            Sort(dictionary);
            InsertWord(dictionary, letters);
            printDictionary(dictionary);
            cout << "------------------------------------------------------------------------" << endl;
        }

        else if (choice == 5) {
            cout << "Enter set of letters of the words you want to delete: ";
            cin >> letters;
            Sort(dictionary);
            deleteWordsContaining(dictionary, letters);
            printDictionary(dictionary);
            cout << "------------------------------------------------------------------------" << endl;
        }

        else if (choice == 6) {
            cout << "Enter the set of letters of the word you want to delete: ";
            cin >> letters;
            Sort(dictionary);
            deleteWordsStartingWith(dictionary, letters);
            cout << endl;
            printDictionary(dictionary);
            cout << "------------------------------------------------------------------------" << endl;
        }

        else if (choice == 7) {
            dWord* DesiredWord = new dWord;
            string addWord;
            string selectedWord;
            int choice = -1;
            bool isSynonym = true;
            while (choice != 1 && choice != 2) {
                cout << "If you wish to add a Synonym Press 1 " << endl << "If you wish to add an Antonym Press 2:";
                cin >> choice;
                if (choice == 1)
                    isSynonym = true;
                else if (choice == 2)
                    isSynonym = false;
            }
        

            cout << endl <<"Enter Desired Word that you want to add Synonym or Antonym to It: ";
            cin >> selectedWord;
            DesiredWord = findOrCreateWord(dictionary, selectedWord);
            cout << "Enter Synonym or Antonym that you want to add: ";
            cin >> addWord;
            addSynonymsorAntonyms(dictionary, DesiredWord->data, addWord, isSynonym);
            printDictionary(dictionary);
            cout << "------------------------------------------------------------------------" << endl;
        }

    }
    writeToFile(dictionary, "Dictionary.txt");
    delete dictionary;
    inputFile.close();
    return 0;
}


