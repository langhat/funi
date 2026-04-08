#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>

using namespace std;
using namespace std::string_literals;

string main_file, run_cmd;

int main() {
	string input,temp;
	vector<string> obj;
	while(1) {
		cout<<"$ ";
#ifdef _WIN32
		system("cd");
#else
		system("pwd");
#endif
		cout << "> " << flush;

		getline(cin, input);
		stringstream ss(input);
		while(ss) {
			ss >> temp;
			obj.emplace_back(temp);
		}

		for(int index = 0; index < obj.size(); index++) {
			if(obj[index] == "run"s) {
				if(main_file.empty()) cerr << "no source file\n";
				else if(run_cmd.empty()){system(("funi visit \""s + main_file + "\" main()"s).c_str());}
				else system((run_cmd).c_str());
			} else if(obj[index] == "main"s) {
				cout << "make sure u want to change main file[y/.]: " << flush;
				cin >> input;
				if(input == "y"s)
					main_file = obj[++index];
				else
					cerr << "operation canceled\n";
			} else if(obj[index] == "edit"s) {
				system(("vim \""s + obj[++index] + "\""s).c_str());
				system((
#ifdef _WIN32
					"del \""s
#else
					"rm \""s
#endif
				+ obj[index] + "~\""s).c_str());
			} else if(obj[index] == "exit"s) {
				return 0;
			}
		}

		obj.clear();
		input = "";
		cout << flush;
		cin.clear();
		//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}
