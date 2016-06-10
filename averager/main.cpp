#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <iomanip>

#define D(x) cerr<<#x<<":"<<x<<endl; cerr.flush();

using namespace std;

struct Course {
	friend ostream &operator<<(std::ostream &s, Course c);
	string course_code;
	string name;
	string lessons_form;
	float weight;
	float grade;
	void loadFromString(string s) {
		char buffer[100];

		 //line format
		// /20/20COURSE_CODE/20/09/20/20LESSON_FORM/20/09WEIGHT/20/09GRADE
		char separator[2] = "\t"; ///20/09
		char course_marker[4] = "\t  "; ///20/09/20/20
		//parse course_code
		int offset = 2; //2 cuz there are allways two spaces at beggining of line
		size_t end_marker = s.find(course_marker);
		size_t length = s.copy(buffer, end_marker - offset, offset);
		buffer[length] = '\0';
		course_code = buffer;
		//parse name
		offset = end_marker + sizeof(course_marker) - 1;
		end_marker = s.find(separator, offset);
		length = s.copy(buffer, end_marker - offset, offset);
		buffer[length] = '\0';
		name = buffer;
		//parse lesson_form
		offset = end_marker + sizeof(separator) - 1;
		end_marker = s.find(separator, offset);
		length = s.copy(buffer, end_marker - offset, offset);
		buffer[length] = '\0';
		lessons_form = buffer;
		//parse weight
		offset = end_marker + sizeof(separator) - 1;
		end_marker = s.find(separator, offset);
		length = s.copy(buffer, end_marker - offset, offset);
		buffer[length] = '\0';
		weight = atof(buffer);
		//parse grade
		offset = end_marker + sizeof(separator) - 1;
		end_marker = s.find(separator, offset);
		length = s.copy(buffer, end_marker - offset, offset);
		buffer[length] = '\0';
		grade = atof(buffer);
	}
	Course() {};
	Course(string course_code, string name, string lessons_form, float weight, float grade) :
		course_code(course_code), name(name), lessons_form(lessons_form), weight(weight), grade(grade) {};
	Course(string s) { loadFromString(s); }
};

void printCoursesVector(vector<Course> vec);
float calcAverge(const vector<Course> &courses);

ostream &operator<<(std::ostream &s, Course c) {
	s << "Course Code: "<<setw(11)<< c.course_code << "\t";
	s << "Name: " << setw(11) << c.name << endl;
	s << "Lessons Form: " << setw(11) << c.lessons_form<<"\t";
	s << "Weight: " << setw(2) << c.weight << "\t";
	if(c.grade)
		s << "Grade: " << setw(3) << c.grade << endl;
	else
		s << "Grade: "<<setw(3)<<"N/A" << endl;
	return s;
}

struct  Semester {
	friend ostream &operator<<(std::ostream &s, Semester sem);

	string academic_year_and_semester;
	size_t semester_number;
	size_t year_of_studies;
	vector<Course> courses;
};

ostream &operator<<(std::ostream &s, Semester sem) {
	stringstream  buff;
	buff << "Academic year: " << sem.academic_year_and_semester << " ";
	buff << "Semester number: " << sem.semester_number << " ";
	buff << "Year of studies: " << sem.year_of_studies;
	string frame = "";
	size_t n = buff.str().size();
	for (size_t i=0; i < n+2; i++)
		frame += '*';
	s << frame<<endl<<"*"<< buff.str()<<"*"<<endl<<frame<<endl;
	s << "Courses : " << endl;

	printCoursesVector(sem.courses);
	s << "Average : " << calcAverge(sem.courses) <<  endl <<endl;
	return s;
}



void printVector(vector<string> vec);
vector<Semester> semesters;
int acctual_semester=-1;
vector<Course> courses;

int main(int argc, char ** args) {
	string temp = "\t  34 ";
	int na = atoi(temp.c_str());
	D(na * 2);
	fstream file;
	if (argc == 2) { 
		D(args[1]);
		file.open(args[1], ios::in);
		if (!file.good()) {
			cerr << "error! could  not open file!";
			return -2;
		}
	}
	else {
		cerr << "Drag .txt to my .exe!";
		cin >> argc;
		return -1;
	}


	string line;
	vector<string> raw_data;
	while (!file.eof()) {
		getline(file, line);
		raw_data.push_back(line);
		if (line.size() < 3)  continue;//i think thats not needed, but itsh here from the beggining
		switch (line[0])  {
		case 0x20: //space
			if (line[1] == 0x20)
				semesters[acctual_semester].courses.push_back(Course(line));
			break; 
		case 0x52: //'R'
			if (line.find("Rok i sem. akademicki",0) != string::npos) {
				semesters.push_back(Semester());
				acctual_semester++;
				size_t start_position = line.find("\t") + 1;
				semesters[acctual_semester].academic_year_and_semester = line.substr(start_position,line.length()-start_position);
			} else 
				if (line.find("Rok studiów", 0) != string::npos) {
					size_t start_position = line.find("\t") + 1;
					semesters[acctual_semester].year_of_studies = atoi(line.substr(start_position, line.length() - start_position).c_str());
				}
		case 0x53: //'S'
			if (line.find("Semestr", 0) != string::npos) {
				size_t start_position = line.find("\t") + 1;
				semesters[acctual_semester].semester_number = atoi(line.substr(start_position, line.length() - start_position).c_str());
			}
			break;
		case 0x4B: //'K' ignore
			break;
		default:
			cerr << "ERROR IN MAIN PARSE LOOP, arg: " << endl<<line<<"\n\n";
			break;
		}
	}
	//printVector(raw_data);
	size_t n = semesters.size();
	for (size_t i = 0; i < n; i++)
		cout << semesters[i];
	cin >> argc;
	return 0;
}

float calcAverge(const vector<Course> &courses) {
	float result = 0;

	size_t n = courses.size();
	float weight_sum = 0;
	
	for (size_t i = 0; i < n; i++) {
		if (courses[i].grade == 0) continue;	//dont  calc it to averge cuz lack of data
		result += courses[i].weight*courses[i].grade;
		weight_sum += courses[i].weight;
	}
	result /= weight_sum;
	return result;
}


void printVector(vector<string> vec) {
	size_t n = vec.size();
	for (int i = 0; i < n; i++)
		cout << vec[i] << endl;
}
void printCoursesVector(vector<Course> vec) {
	size_t n = vec.size();
	for (int i = 0; i < n; i++)
		cout << vec[i] << endl;
}

