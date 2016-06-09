#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define D(x) cerr<<#x<<":"<<x<<endl; cerr.flush()

using namespace std;


void printVector(vector<string> vec);

struct Course {
	string name;
	string lessons_form;
	float weight;
	float grade;

	Course(string name, string lessons_form, float weight, float grade) :
		name(name), lessons_form(lessons_form), weight(weight), grade(grade) {};
};
Course parseLine(string line);
vector<Course> Courses;

int main(int argc, char ** args) {
	//if (argc == 2) { // should be 2 if not debug
	//	file.open(args[1], ios::in);
	//	D(args[1]);		
	//}
	//else {
	//	cerr << "Drag .txt to my exe!";
	//	cin >> argc;
	//	return 0;
	//}


	fstream file;
	file.open("C:/oceny.txt", ios::in); //for fast debug
	D(file.good());
	string line;
	vector<string> data;
	while (!file.eof()) {
		getline(file, line);
		if (line.size() > 2);
		if ((line[0] == 32) && (line[1] == 32)) // when there is two spaces on the beggining of line
			data.push_back(line);
	}

	Course c = parseLine(data[0]);
	D(c.name);

	printVector(data);
	cin >> argc;
	return 0;
}

void printVector(vector<string> vec) {
	int n = vec.size();
	for (int i = 0; i < n; i++)
		cerr << vec[i] << endl;
}

Course parseLine(string line) {
	string lessons_form;
	float weight =0;
	float grade=0;

	//parse name of course
	char buffer[20];
	int offset_course_code = 2;
	size_t course_code_end = line.find(" \t  ");
	size_t length = line.copy(buffer, course_code_end - offset_course_code, offset_course_code);
	buffer[length] = '\0';
	string name = buffer;

	return 	Course(name,  lessons_form,  weight,  grade);
}