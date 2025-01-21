#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <list>
#include <fstream>
#include <unordered_map>
#include <cctype>
#include <limits>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>  
#endif

using namespace std;


#if defined(_WIN32) || defined(_WIN64)
void enableANSI() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (hConsole != INVALID_HANDLE_VALUE && GetConsoleMode(hConsole, &dwMode)) {
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hConsole, dwMode);
    }
}
#else
void enableANSI() {
    
}
#endif

void clearScreen() {
    #if defined(_WIN32) || defined(_WIN64) // For Windows
        std::system("cls");
    #else // For Unix-based systems (Linux/macOS)
        std::system("clear");
    #endif
}

// ANSI color codes for console text
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define SEA_BLUE  "\033[38;5;32m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define WHITE   "\033[37m"

// Class for the text editor
class TextEditor {
private:
    list<string> document;  // Linked list to store document lines
    stack<pair<string, string>> undoStack; // Stack for undo functionality (store action and content)
    queue<pair<string, string>> redoQueue; // Queue for redo functionality (store action and content)
    string currentFilename;  // Stores the current filename being edited

public:
    // Function to handle input safely (to avoid invalid entries)
    int getIntInput(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // clear input buffer
                return value;
            } else {
                cout << RED << "Invalid input. Please enter a valid number.\n" << RESET;
                cin.clear();  // clear error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // discard invalid input
            }
        }
    }

    // Function to get string input safely
    string getStringInput(const string& prompt) {
        string input;
        cout << prompt;
        getline(cin, input);
        return input;
    }

    // 1. Create a new document (reset the text)
    void createNewDocument() {
        document.clear();
        currentFilename = "";
        cout << GREEN << "New document created.\n" << RESET;
    }

    // 2. Load an existing document from a file
    void loadDocument(const string& filename) {
        document.clear();
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                document.push_back(line);
            }
            file.close();
            currentFilename = filename;
            cout << GREEN << "Document loaded successfully.\n" << RESET;
            displayDocument();  // Show the content after loading
        } else {
            cout << RED << "Failed to load document.\n" << RESET;
        }
    }

    // 3. Save the current document to a file
    void saveDocument() {
        if (document.empty()) {
            cout << RED << "The document is empty. Add content before saving.\n" << RESET;
            return; // Exit the function to prevent saving
        }

        if (currentFilename.empty()) {
            currentFilename = getStringInput("Enter the filename to save as: ");
        }

        ofstream file(currentFilename);
        if (file.is_open()) {
            for (const auto& line : document) {
                file << line << endl;
            }
            file.close();
            cout << GREEN << "Document saved successfully to " << currentFilename << ".\n" << RESET;
        } else {
            cout << RED << "Failed to save document. Please check the file path or permissions.\n" << RESET;
        }
    }

    // 4. Display the document
    void displayDocument() {
        if (document.empty()) {
            cout << YELLOW << "The document is empty.\n" << RESET;
            return;
        }
        int lineNumber = 1;
        for (const auto& line : document) {
            cout << CYAN << lineNumber++ << ": " << RESET << line << endl;
        }
    }

    // 5. Add a new line 
    void addLine(const string& text) {
        document.push_back(text);
        undoStack.push({"add", text});
        redoQueue = queue<pair<string, string>>(); // Clear redo history
    }

    // 6. Remove 
    void removeLine() {
        if (!document.empty()) {
            string lastLine = document.back();
            document.pop_back();
            undoStack.push({"remove", lastLine});
            redoQueue = queue<pair<string, string>>(); // Clear redo history
        } else {
            cout << RED << "No line to remove.\n" << RESET;
        }
    }

    // 7. Undo 
    void undo() {
        if (!undoStack.empty()) {
            auto action = undoStack.top();
            undoStack.pop();

            if (action.first == "add") {
                document.pop_back();
                redoQueue.push({"add", action.second});
            } else if (action.first == "remove") {
                document.push_back(action.second);
                redoQueue.push({"remove", action.second});
            }
        } else {
            cout << RED << "Nothing to undo.\n" << RESET;
        }
    }

    // 8. Redo 
    void redo() {
        if (!redoQueue.empty()) {
            auto action = redoQueue.front();
            redoQueue.pop();

            if (action.first == "add") {
                document.push_back(action.second);
                undoStack.push({"add", action.second});
            } else if (action.first == "remove") {
                document.pop_back();
                undoStack.push({"remove", action.second});
            }
        } else {
            cout << RED << "Nothing to redo.\n" << RESET;
        }
    }


    // 9. Search for a word in the document
    void searchWord(const string& word) {
        unordered_map<int, string> wordMap;
        int lineNumber = 1;
        bool found = false;

        // Build the hash table (map)
        for (const auto& line : document) {
            wordMap[lineNumber] = line;
            lineNumber++;
        }

        // Search for the word in the map
        for (const auto& entry : wordMap) {
            if (entry.second.find(word) != string::npos) {
                cout << CYAN << "Found at line " << entry.first << ": " << RESET << entry.second << endl;
                found = true;
            }
        }

        if (!found) {
            cout << RED << "Word not found in the document.\n" << RESET;
        }
    }
    
   
    //10. Replace word
    void replaceWord(const string& oldWord, const string& newWord) {
        unordered_map<int, string> wordMap;
        int lineNumber = 1;

        // Build the hash table (map)
        for (auto& line : document) {
            wordMap[lineNumber] = line;
            lineNumber++;
        }

        // Replace the word in the map
        for (auto& entry : wordMap) {
            size_t pos = 0;
            while ((pos = entry.second.find(oldWord, pos)) != string::npos) {
                entry.second.replace(pos, oldWord.length(), newWord);
                pos += newWord.length();
            }
        }

        // Update the document with the modified content
        lineNumber = 0;
        for (auto& entry : wordMap) {
            auto it = document.begin();
            advance(it, lineNumber);
            *it = entry.second;
            lineNumber++;
        }
    }

    // 11. Insert text at a specific line number
    void insertAtLine(int lineNumber, const string& text) {
        if (lineNumber < 1 || lineNumber > document.size() + 1) {
            cout << RED << "Invalid line number.\n" << RESET;
            return;
        }

        auto it = document.begin();
        advance(it, lineNumber - 1); // Move iterator to the specified line number
        document.insert(it, text);
        undoStack.push({"insertAtLine", text});
        redoQueue = queue<pair<string, string>>(); // Clear redo history
    }

    // 12. Delete a specific line by number
    void deleteLineByNumber(int lineNumber) {
        if (lineNumber < 1 || lineNumber > document.size()) {
            cout << RED << "Invalid line number.\n" << RESET;
            return;
        }

        auto it = document.begin();
        advance(it, lineNumber - 1);
        document.erase(it);
        undoStack.push({"deleteLineByNumber", ""});
        redoQueue = queue<pair<string, string>>(); // Clear redo history
    }

    // 13. Change content of a specific line
    void changeLine(int lineNumber, const string& newContent) {
        if (lineNumber < 1 || lineNumber > document.size()) {
            cout << RED << "Invalid line number.\n" << RESET;
            return;
        }

        auto it = document.begin();
        advance(it, lineNumber - 1);
        *it = newContent;
        undoStack.push({"changeLine", newContent});
        redoQueue = queue<pair<string, string>>(); // Clear redo history
    }

    // 14. Count total lines in the document
    void countLines() {
        cout << CYAN << "Total lines in the document: " << RESET << document.size() << endl;
    }

    // 15. Count occurrences of a word in the document using a hash table
    void countWordOccurrences(const string& word) {
        unordered_map<string, int> wordCount; // Hash table to store word frequencies

        // Populate the hash table with word frequencies
        for (const auto& line : document) {
            size_t start = 0, end;
            while ((end = line.find_first_of(" \t\n,.!?;:", start)) != string::npos) {
                if (start < end) {
                    string currentWord = line.substr(start, end - start);
                    wordCount[currentWord]++;
                }
                start = end + 1;
            }
            if (start < line.size()) {
                wordCount[line.substr(start)]++;
            }
        }

        // Display the occurrence of the given word
        if (wordCount.find(word) != wordCount.end()) {
            cout << CYAN << "The word \"" << word << "\" appears " << RESET << wordCount[word] << " times.\n";
        } else {
            cout << RED << "The word \"" << word << "\" does not appear in the document.\n" << RESET;
        }
    }

    // 16. Bold the entire document (simulated by wrapping text with **)
    void boldText() {
        for (auto& line : document) {
            line = "**" + line + "**";
        }
    }

    // 17. Italicize the entire document (simulated by wrapping text with _)
    void italicizeText() {
        for (auto& line : document) {
            line = "_" + line + "_";
        }
    }

    // 18. Convert the document to lowercase
    void convertToLowerCase() {
        for (auto& line : document) {
            for (auto& ch : line) {
                ch = tolower(ch);
            }
        }
    }

    // 19. Convert the document to uppercase
    void convertToUpperCase() {
        for (auto& line : document) {
            for (auto& ch : line) {
                ch = toupper(ch);
            }
        }
    }
};



// Main menu function
void mainMenu() {
    TextEditor editor;
    int choice;

    do {
        // Main Menu UI
        cout << SEA_BLUE << "====================================\n";
        cout << "        TEXT EDITOR MENU\n";
        cout << "====================================\n";
        cout << "1. Create New Document\n";
        cout << "2. Load Document\n";
        cout << "3. Exit\n";
        cout << SEA_BLUE << "====================================\n" << RESET;
        choice = editor.getIntInput("Enter your choice: ");
        clearScreen() ;

        if (choice == 1) {
            editor.createNewDocument();
            // After creating a new document, display text editor menu (functions 20 actions)
            bool editing = true;
            while (editing) {
                cout << YELLOW << "====================================\n";
                cout << "          EDITING MENU\n";
                cout << "====================================\n";
                cout << "1. Add Line\n";
                cout << "2. Remove Line\n";
                cout << "3. Undo\n";
                cout << "4. Redo\n";
                cout << "5. Display Document\n";
                cout << "6. Save Document\n";
                cout << "7. Search Word\n";
                cout << "8. Replace Word\n";

                cout << "9. Insert at Line\n";
                cout << "10. Delete Line by Number\n";
                cout << "11. Change Line Content\n";
                cout << "12. Count Lines\n";
                cout << "13. Count Word Occurrences\n";
                cout << "14. Bold Text\n";
                cout << "15. Italicize Text\n";
                cout << "16. Convert to Lowercase\n";
                cout << "17. Convert to Uppercase\n";
                cout << "18. Exit\n";
                cout << YELLOW << "====================================\n" << RESET;
                int action = editor.getIntInput("Enter your choice: ");
                clearScreen() ; 
                switch (action) {
                    case 1: {
                        string text = editor.getStringInput("Enter line to add: ");
                        editor.addLine(text);
                        
                        break;
                        
                    }
                    case 2:
                        editor.removeLine();
                        
                        break;
                    case 3:
                        editor.undo();
                        
                        break;
                    case 4:
                        editor.redo();
                      
                        break;
                    case 5:
                        editor.displayDocument();
                        
                        break;
                    case 6:
                        editor.saveDocument();
                       
                        break;
                    case 7: {
                        string word = editor.getStringInput("Enter word to search: ");
                        editor.searchWord(word);
                        
                        break;
                    }
                    case 8: {
                        string oldWord = editor.getStringInput("Enter word to replace: ");
                        string newWord = editor.getStringInput("Enter new word: ");
                        editor.replaceWord(oldWord, newWord);
                        
                        break;
                    }
                   
                    case 9: {
                        int lineNumber = editor.getIntInput("Enter line number to insert at: ");
                        string text = editor.getStringInput("Enter text to insert: ");
                        editor.insertAtLine(lineNumber, text);
                        
                        break;
                    }
                    case 10: {
                        int lineNumber = editor.getIntInput("Enter line number to delete: ");
                        editor.deleteLineByNumber(lineNumber);
                       
                        break;
                    }
                    case 11: {
                        int lineNumber = editor.getIntInput("Enter line number to change: ");
                        string newText = editor.getStringInput("Enter new content: ");
                        editor.changeLine(lineNumber, newText);
                        
                        break;
                    }
                    case 12:
                        editor.countLines();
                       
                        break;
                    case 13: {
                        string word = editor.getStringInput("Enter word to count: ");
                        editor.countWordOccurrences(word);
                       
                        break;
                    }
                    case 14:
                        editor.boldText();
                        
                        break;
                    case 15:
                        editor.italicizeText();
                       
                        break;
                    case 16:
                        editor.convertToLowerCase();
                       
                        break;
                    case 17:
                        editor.convertToUpperCase();
                        
                        break;
                    case 18:
                        editing = false;
                        clearScreen() ;
                        break;
                    default:
                        cout << RED << "Invalid choice, try again.\n" << RESET;
                }
            }
        } else if (choice == 2) {
            string filename = editor.getStringInput("Enter the filename to load: ");
            editor.loadDocument(filename);
            bool editing = true;
            while (editing) {
            	cout << YELLOW << "====================================\n";
                cout << "          EDITING MENU\n";
                cout << "====================================\n";
            	cout << "\nText Editing Menu:\n";
                cout << "1. Add Line\n";
                cout << "2. Remove Line\n";
                cout << "3. Undo\n";
                cout << "4. Redo\n";
                cout << "5. Display Document\n";
                cout << "6. Save Document\n";
                cout << "7. Search Word\n";
                cout << "8. Replace Word\n";
                cout << "9. Insert at Line\n";
                cout << "10. Delete Line by Number\n";
                cout << "11. Change Line Content\n";
                cout << "12. Count Lines\n";
                cout << "13. Count Word Occurrences\n";
                cout << "14. Bold Text\n";
                cout << "15. Italicize Text\n";
                cout << "16. Convert to Lowercase\n";
                cout << "17. Convert to Uppercase\n";
                cout << "18. Exit\n";
                cout << YELLOW << "====================================\n" << RESET;
                cout << "Enter your choice: ";
                
                int action;
                cin >> action;
                cin.ignore();
                clearScreen() ; 

                switch (action) {
                    case 1: {
                        string text;
                        cout << "Enter line to add: ";
                        getline(cin, text);
                        editor.addLine(text);
                        break;
                    }
                    case 2:
                        editor.removeLine();
                        break;
                    case 3:
                        editor.undo();
                        break;
                    case 4:
                        editor.redo();
                        break;
                    case 5:
                        editor.displayDocument();
                        break;
                    case 6:
                        editor.saveDocument();
                        break;
                    case 7: {
                        string word;
                        cout << "Enter word to search: ";
                        getline(cin, word);
                        editor.searchWord(word);
                        break;
                    }
                    case 8: {
                        string oldWord, newWord;
                        cout << "Enter word to replace: ";
                        getline(cin, oldWord);
                        cout << "Enter new word: ";
                        getline(cin, newWord);
                        editor.replaceWord(oldWord, newWord);
                        break;
                    }
                   
                        
                    case 9: {
                        int lineNumber;
                        string text;
                        cout << "Enter line number to insert at: ";
                        cin >> lineNumber;
                        cin.ignore();
                        cout << "Enter text to insert: ";
                        getline(cin, text);
                        editor.insertAtLine(lineNumber, text);
                        break;
                    }
                    case 10: {
                        int lineNumber;
                        cout << "Enter line number to delete: ";
                        cin >> lineNumber;
                        editor.deleteLineByNumber(lineNumber);
                        break;
                    }
                    case 11: {
                        int lineNumber;
                        string newContent;
                        cout << "Enter line number to change: ";
                        cin >> lineNumber;
                        cin.ignore();
                        cout << "Enter new content for the line: ";
                        getline(cin, newContent);
                        editor.changeLine(lineNumber, newContent);
                        break;
                    }
                    case 12:
                        editor.countLines();
                        break;
                    case 13: {
                        string word;
                        cout << "Enter word to count: ";
                        getline(cin, word);
                        editor.countWordOccurrences(word);
                        break;
                    }
                    case 14:
                        editor.boldText();
                        break;
                    case 15:
                        editor.italicizeText();
                        break;
                    case 16:
                        editor.convertToLowerCase();
                        break;
                    case 17:
                        editor.convertToUpperCase();
                        break;
                    case 18:
                        editing = false;
                        clearScreen() ;
                        break;
                    default:
                        cout << "Invalid choice, try again.\n";
                    }
                // The same editing menu logic as above for loaded file...
            }
        } else if (choice == 3) {
            cout << GREEN << "Exiting program.\n" << RESET;
        } else {
            cout << RED << "Invalid choice, please try again.\n" << RESET;
        }
    } while (choice != 3);
}

int main() {
	enableANSI();
    mainMenu();
    
    return 0;
}