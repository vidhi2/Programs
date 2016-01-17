
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>

using namespace std;

int offset;
bool isBgProcess;

string builtinCmd[] = {
	"cd",
	"exit",
	"history",
	"help"
};

void LoopShell();
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

bool (*builtinFunc[])(vector<char *>) = {
	&cmdCD,
	&cmdEXIT,
	&cmdHISTORY,
	&cmdHELP
};


int main(int argc, char** argv) {

	// Init Shell
	offset = 0;

	// Loop Shell
	LoopShell();

	// Quit Shell

	return EXIT_SUCCESS;
}

void LoopShell() {
	string cmd;
	bool status = true;
	string prompt = "> ";
	vector<string> args;
	ofstream writer;
	writer.open("History.txt");
	isBgProcess = false;


	do {
		prompt = getPrompt();
		cout << prompt;
		
		getline(cin, cmd);
		
		// Parse Command
		cmd = parse(cmd);

		if(writer.is_open()) {
			writer << offset << '\t' << cmd << endl;
			offset++;
		}

		args = splitCmd(cmd);

		status = execCmd(args);
	} while(status);

	writer.close();
}

string parse(string str) {
	size_t first = str.find_first_not_of(' ');
	if(first == string::npos) {
		return "";
	}
	size_t last = str.find_last_not_of(' ');

	str = str.substr(first,last - first + 1);
	size_t lastIn = str.size() - 1;
	if(str[lastIn] == '&') {
		if(str[lastIn-1] == ' ') {
			str[lastIn - 1] = '&';
		}
		str = str.substr(0, lastIn);
		isBgProcess = true;
	}
	return str;
}
string getPrompt() {
	char* pwd = get_current_dir_name();
	string prompt = pwd;
	prompt = prompt + "> ";
	return prompt;
}

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

bool execCmd(vector<string> args) {
	if(args.empty()) {
		return true;
	}

	// Convert to C Strings
	vector<char *> argc(args.size() + 1);

	for(int i = 0; i < args.size(); ++i) {
		argc[i] = &args[i][0];
	}

	for(int i = 0; i < numBuiltins(); ++i) {
		if(args[0] == builtinCmd[i]) {
			return (*builtinFunc[i]) (argc);
		}
	}

	return launchCmd(argc);
}

bool launchCmd(vector<char *> args) {
	pid_t pid;
	pid_t waitPid;
	int status;

	pid = fork();
	if(pid == 0) {
		// Child Process
		int res = execvp(args[0], args.data());
		if(res == -1) {
			perror("SimpleShell");
		}
		exit(EXIT_FAILURE);
	} else if(pid < 0) {
		// Fork Error
		perror("SimpleShell");
	} else {
		// Parent Process
		if(!isBgProcess) {
			do {
				waitPid = waitpid(pid, &status, WUNTRACED);
			} while(!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	}
	return true;
}

int numBuiltins() {
	return sizeof(builtinCmd) / sizeof(char *);
}
bool cmdCD(vector<char *> args) {
	if(args[1] == NULL) {
		fprintf(stderr, "SimpleShell: expected argument to \"cd\"\n");
	} else {
		if(chdir(args[1]) != 0) {
			perror("SimpleShell");
		}
	}
	return true;
}
bool cmdEXIT(vector<char *> args) {
	return false;
}
bool cmdHISTORY(vector<char *> args) {

	string line;
	int num = 0;
	ifstream reader ("History.txt");

	if(args[1] != NULL) {
		num = offset - atoi(args[1]); 
	}
	if(reader.is_open()) {
			while(getline(reader, line)) {
				if(num <= 0) {
					cout << line << endl;
				} else {
					num--;
				}
			}
		} else {
			cerr << "SimpleShell: Unable to Get History" << endl;
		} 

	reader.close();
	
	return true;
}
bool cmdHELP(vector<char *> args) {
	cout << "+--Simple Shell--+" << endl;
	return true;
}
