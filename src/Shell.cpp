
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int offset;							// Offset for each Command
bool isBgProcess;					// Boolean for background process
vector<string> historyVec;			// Vector for storing history
string fileLoc;						// Path of history file

string builtinCmd[] = {				// Name of Builtin Commands
	"cd",
	"exit",
	"history",
	"help"
};

// Function Declarations
void initShell();
void LoopShell();
void quitShell();
string getPrompt();
vector<string> splitCmd(string str);
bool execCmd(vector<string> args);
bool launchCmd(vector<char *> args);
int numBuiltins();
string parse(string str);

bool cmdCD(vector<char *> args);
bool cmdEXIT(vector<char *> args);
bool cmdHISTORY(vector<char *> args);
bool cmdHELP(vector<char *> args);

// Function Pointer for Builtin Commands
bool (*builtinFunc[])(vector<char *>) = {
	&cmdCD,
	&cmdEXIT,
	&cmdHISTORY,
	&cmdHELP
};


// main Function
int main(int argc, char** argv) {

	// Init Shell();
	initShell();

	// Loop Shell
	LoopShell();											// Loops the basic shell

	// Quit Shell
	quitShell();

	return EXIT_SUCCESS;
}

void initShell() {
	offset = 0;												// Initial offset is zero
	
	string currDir = get_current_dir_name();
	fileLoc = currDir + "/cache.txt";
	
	ifstream reader;
	reader.open(fileLoc.c_str());
	string line;
	if(reader.is_open()) {
		while(getline(reader, line)) {
			historyVec.push_back(line);
		}
	} 
	offset = historyVec.size();

	reader.close();
}

void quitShell() {
	ofstream writer;
	writer.open(fileLoc.c_str());
	if(writer.is_open()) {
		for(int i = 0; i < historyVec.size(); i++) {
			writer << historyVec[i] << endl;
		}
	}
	
	writer.close();
}
// Function for Shell
void LoopShell() {
	string cmd;
	bool status = true;
	string prompt = "> ";
	vector<string> args;
	isBgProcess = false;	


	do {													// Take user input until the user exits
		prompt = getPrompt();								// Get the prompt
		cout << prompt;	
		
		getline(cin, cmd);									// Get the user input
		if(cmd == ""){
			continue;
		}

		stringstream ss;
		ss << offset;
		string str = ss.str();
		string historyInp = str + '\t' + cmd;				// Add Command to history
		
		historyVec.push_back(historyInp);
		offset++;

		// Parse Command
		cmd = parse(cmd);									// Parse the user input

		args = splitCmd(cmd);								// Split the command to get the argumets

		status = execCmd(args);								// execute the Command
	} while(status);										// check the status
}

// Function for parsing the command
string parse(string str) {
	// Trimming the spaces from the command
	size_t first = str.find_first_not_of(' ');				// triming initial spaces				
	if(first == string::npos) {
		return "";
	}
	size_t last = str.find_last_not_of(' ');				// trimming the end spaces

	str = str.substr(first,last - first + 1);
	size_t lastIn = str.size() - 1;
	if(str[lastIn] == '&') {								// checking if the process is background or not
		if(str[lastIn-1] == ' ') {
			str = str.substr(0, lastIn-1);
		} else {
			str = str.substr(0, lastIn);
		}
		isBgProcess = true;
	}
	return str;
}

// Function for getting the prompt
string getPrompt() {
	char* pwd = get_current_dir_name();						// Getting the current directory
	string prompt = pwd;
	prompt = prompt + "> ";									// formatting the prompt
	return prompt;
}

// function to split the command into arguments
vector<string> splitCmd(string str) {
	vector<string> args;
	string buf = "";
	for(int i = 0; i < str.size(); ++i) {
		if(str[i] == ' ') {
			args.push_back(buf);
			buf = "";
		} else {
			buf = buf + str[i];
		}
	}
	args.push_back(buf);
	return args;
}

//function for executing the commands
bool execCmd(vector<string> args) {
	if(args.empty()) {										// Checking if the command is empty
		return true;
	}

	// Convert to C Strings
	vector<char *> argc(args.size() + 1);					// Convert C++ strings to C String

	for(int i = 0; i < args.size(); ++i) {
		argc[i] = &args[i][0];
	}

	for(int i = 0; i < numBuiltins(); ++i) {				// Checking if the command is builtin or not
		if(args[0] == builtinCmd[i]) {
			return (*builtinFunc[i]) (argc);		
		}
	}

	return launchCmd(argc);									// lanuch the non-builtin commands
}

// function for launching the non-builtin Commands
bool launchCmd(vector<char *> args) {
	pid_t pid;												// process id
	pid_t waitPid;
	int status;

	pid = fork();											// Creating a new process
	if(pid == 0) {
		// Child Process
		string arg0 = args[0];
		if(arg0[0] != '.' && arg0.find(".") != string::npos) {
			arg0 = "./" + arg0;
		}
		int res = execvp(arg0.c_str(), args.data());		// execute the command
		if(res == -1) {
			perror("SimpleShell");							// Error
		}
		exit(EXIT_FAILURE);									// Error_Exit
	} else if(pid < 0) {
		// Fork Error
		perror("SimpleShell");								// Fork Error
	} else {
		// Parent Process
		if(!isBgProcess) {									// If isBgProcess than Parent process won't wait
			do {
				waitPid = waitpid(pid, &status, WUNTRACED);
			} while(!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	}
	return true;
}

// Function to return number of Builtins commands
int numBuiltins() {
	return sizeof(builtinCmd) / sizeof(char *);
}

// Implementation of 'cd' command
bool cmdCD(vector<char *> args) {
	if(args[1] == NULL) {
		fprintf(stderr, "SimpleShell: expected argument to \"cd\"\n");		// Check if the arguments are empty
	} else {
		if(chdir(args[1]) != 0) {											// chdir function which changes the directory
			perror("SimpleShell");											
		}
	}
	return true;
}

// Implementation of 'exit' command
bool cmdEXIT(vector<char *> args) {
	return false;
}

// Implementation of history Command
bool cmdHISTORY(vector<char *> args) {
	int num = 0;

	if(args[1] != NULL) {
		num = offset - atoi(args[1]); 							// for partial history
	}
	if(num < 0) {
		num = 0;
		cout << "Warning!! Please re-eneter the arg" << endl;
	}
	for(int i = num; i < offset; ++i) {					// Get the history via historyVec
		cout << historyVec[i] << endl;
	}	
	return true;
}

// Implementation of help command
bool cmdHELP(vector<char *> args) {
	cout << "+--Simple Shell--+" << endl;
	return true;
}
