# text-editor-console-based
Text Editor Console Based in C++ using DSA
This code implements a Text Editor in C++ with a menu-driven interface for creating, editing, and managing text documents. The application supports various operations like adding, deleting, editing, searching, and formatting text. It includes features for undo, redo, and file I/O to load or save documents.

Features of the Program
	1.	Cross-Platform ANSI Support:
	•	The enableANSI function ensures that ANSI escape sequences for color formatting work on both Windows and Unix-based systems.
	•	Colors like red, green, yellow, and cyan are used to enhance user feedback in the console.
 
	2.	Main Components:
	•	Document Storage: Uses a std::list<string> to store the text document as lines, enabling efficient insertion and deletion.
	•	Undo/Redo Functionality: Maintains an std::stack for undo operations and an std::queue for redo operations.
	•	File Operations: Supports loading a document from a file and saving the current document.
 
	3.	Text Editor Functionalities:
	•	Document Creation:
	•	Clears the current document to start afresh.
	•	File Loading:
	•	Reads a text file line by line into the document.
	•	File Saving:
	•	Saves the current document to a file, prompting the user for a filename if none exists.
	•	Text Manipulation:
	•	Add lines, remove the last line, delete a specific line, or change the content of a line.
	•	Insert a new line at a specific position.
	•	Search and Replace:
	•	Find occurrences of a word and optionally replace them.
	•	Word Count:
	•	Count occurrences of a specific word across the document using a hash table.
	•	Undo and Redo:
	•	Undo the last change or redo the most recently undone action.
	•	Formatting:
	•	Bold or italicize all lines in the document.
	•	Convert text to uppercase or lowercase.
	•	Display and Line Management:
	•	Display all lines in the document with line numbers.
	•	Count the total number of lines.
 
	4.	User Interface:
	•	A menu-driven interface allows users to:
	•	Choose to create a new document or load an existing file.
	•	Perform editing actions like adding, removing, and formatting text.
	•	Input validation ensures robust interaction, prompting the user for valid inputs where needed.
 
	5.	Main Menu Flow:
	•	Users start by choosing to create a new document or load an existing one.
	•	Once inside the editor, they can select from multiple actions using the Editing Menu.
	•	Options exist to save changes or exit the editor at any time.
 
	6.	Error Handling:
	•	Prompts users with descriptive error messages for invalid inputs or actions (e.g., invalid line numbers or empty documents).
 
	7.	ANSI Color Codes:
	•	Improves readability by highlighting different messages and sections of the menu with appropriate colors.

Technical Details
	•	Libraries Used:
	•	<list>: Efficient for dynamic line management.
	•	<stack> and <queue>: Facilitate undo/redo functionality.
	•	<unordered_map>: Enables efficient word searches and counts.
	•	<fstream>: Handles file input/output.
	•	<limits>: Ensures safe handling of numeric inputs.
	•	Input Handling:
	•	getIntInput and getStringInput ensure user input is valid and avoid crashes due to invalid data.
	•	Cross-Platform Support:
	•	The code uses conditional compilation for system-specific functionality like clearing the screen (clearScreen) and enabling ANSI on Windows.

Program Execution
	1.	The program starts with the main menu, allowing users to:
	•	Create a new document.
	•	Load an existing document from a file.
	•	Exit the program.
	2.	Once a document is created or loaded, the user can perform a variety of editing actions through the Editing Menu.
	3.	Users can save their work or exit the program at any time.

This program is a comprehensive text editor with a user-friendly interface and useful functionalities suitable for basic text processing tasks.
