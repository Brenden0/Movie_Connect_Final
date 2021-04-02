#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;

struct Movie {
	string title;
	int rating = -1;
	int id = -1;
};

struct User {
	string username;
	Movie movies[25];
	string recomendations[3];
	int top5ID[5];

	bool operator==(const User& U) const
	{
		return (this->username == U.username);
	}
};

class userHash {
public:
	size_t operator()(const User& U) const {
		return hash<string>()(U.username);
	}
};

bool compareMovie(Movie m1, Movie m2) {
	return (m1.rating > m2.rating);
}


class userList {
public:

	unordered_map <string, int> userMap;
	unordered_map <string, int> movieMap;
	vector <User> userList;
	string movieTitles[25];
	int usersEnd = 0;
	int movieEnd = 0;

	bool compareMovies(Movie m1, Movie m2) {
		return m1.rating > m2.rating;
	}

	int addMovie(string key, int number) {
		string temp = "";
		for (int i = 0; i < key.length(); i++) {
			if (key[i] == ' ') continue;
			temp += key[i];
		}
		key = temp;
		cout << "Adding " << key << "::" << number << endl;
		movieMap.insert({ key, number });
		movieEnd++;
		movieTitles[number] = key;
	}

	int getMovieNumber(string key) {

		//Remove Spaces
		string temp = "";
		for (int i = 0; i < key.length(); i++) {
			if (key[i] == ' ') continue;
			temp += key[i];
		}
		key = temp;

		unordered_map<string, int>::const_iterator itr = movieMap.find(key);
		if (itr == movieMap.end()) {
			cout << "Movie not found" << endl;
			return -1;
		}
		return itr->second;

	}



	int createUser(string username) {
		User U;
		U.username = username;
		userList.push_back(U);
		userMap.insert({ username, userList.size() - 1});
		usersEnd++;
		cout << "Added user " << username << endl;
	}

	int getUserID(string username) {

		unordered_map <string, int>::const_iterator itr;
		itr = userMap.find(username);
		if (itr == userMap.end()) {
			cout << "User not found" << endl;
			return -1;
		}
		else {
			return itr->second;
		}
	}

	User* findUser(string username) {
		
		int id = getUserID(username);
		if (id == -1) return NULL;
		return &userList[id];

	}

	int addUserRating(string username, string movieTitle, int rating) {

		int movieNum = getMovieNumber(movieTitle);
		if (movieNum == -1) {
			cout << "Movie not found" << endl;
			return 0;
		}
		User* U = findUser(username);
		if (U == NULL) {
			cout << "User not found" << endl;
			return 0;
		}
		
		U->movies[movieNum].id = movieNum;
		U->movies[movieNum].title = movieTitle;
		U->movies[movieNum].rating = rating;
		
		return 0;
	}

	void printUsers() {

		if (userMap.size() == 0) {
			cout << "User list is empty!" << endl;
		}
		else
		{
			for (int i = 0; i < userList.size(); i++) {
				cout << userList[i].username << endl;
			}
		}
	}


	void printUserRatings() {

		if (userMap.size() == 0) {
			cout << "User list is empty!" << endl;
		}
		else {
			for (int i = 0; i < userList.size(); i++) {
				cout << "User: ";
				cout << userList[i].username << endl;
				cout << "Ratings: " << endl;
				for (int j = 0; j < movieEnd; j++) {
					cout << movieTitles[j] << " -> " << userList[i].movies[j].rating << endl;
				}

			}

		}
	}

	void swap(Movie* m1, Movie* m2)
	{

		cout << "Swapping: " << m1->rating << " and " << m2->rating << endl;
		Movie temp = *m1;
		*m1 = *m2;
		*m1 = temp;
	}

	int* getTop5(User* U) {
		
		Movie arr[movieEnd];
		for (int i = 0; i < movieEnd; i++) {
			arr[i] = U->movies[i];
		}
		
		int* top5ratings = new int[5];
		int* top5ids = new int[5];

		int n = sizeof(arr) / sizeof(arr[0]);
		sort(arr, arr + n, compareMovie);

		top5ratings[0] = arr[0].id;
		top5ratings[1] = arr[1].id;
		top5ratings[2] = arr[2].id;
		top5ratings[3] = arr[3].id;
		top5ratings[4] = arr[4].id;
		
		return top5ratings;
	}

	void setTop5() {
		
		for (int i = 0; i < usersEnd; i++) {
			int* arr = getTop5(&userList[i]);
			for (int j = 0; j < 5; j++) {
				userList[i].top5ID[j] = arr[j];
			}
		}

	}

	void printTop5Rating(User* U) {

		for (int i = 0; i < 5; i++) {
			if (U->top5ID[i] == -1) continue;
			cout << U->movies[U->top5ID[i]].title << "-> " << U->movies[U->top5ID[i]].rating << endl;
		}
		cout << endl;

		return;
	}

	void printAllTop5Ratings() {
		
		for (int i = 0; i < usersEnd; i++) {
			cout << userList[i].username << ": " << endl;
			printTop5Rating(&userList[i]);
			cout << endl;
		}
	}

	void printAverageRating() {

		cout << endl;
		cout << "Average Rating: " << endl << endl;
		float sum = 0;
		for (int i = 0; i < movieEnd; i++) {
			for (int j = 0; j < usersEnd; j++) {
				sum += userList[j].movies[i].rating;
			}
			sum = sum / usersEnd;

			cout << movieTitles[i] << ": " << sum << endl;
			sum = 0;

		}

		cout << endl;

	}

	void printAllBestMatches() {

		for (int i = 0; i < usersEnd; i++) {
			findMatch(userList[i].username);
		}

	}

	string removeSpaces(string str) {
		
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == '_') {
				str.erase(str.begin() + i);
			}
		}
		return str;
	}

	void findFans(string movieTitle, int ranking) {
		
		movieTitle = removeSpaces(movieTitle);
		int id = getMovieNumber(movieTitle);
		if (id == -1) {
			cout << "Movie not found" << endl;
			return;
		}

		cout << endl << "All Users who rated " << movieTitle << " a " << ranking << " or above: " << endl;

		for (int i = 0; i < usersEnd; i++) {
			if (userList[i].movies[id].rating >= ranking) {
				cout << userList[i].username << ": " << userList[i].movies[id].rating << endl;
			}
		}

		cout << endl;

	}

	void findMatch(string username) {

		User* U = findUser(username);
		if (U == NULL) {
			cout << "User not found" << endl;
			return;
		}
		if (usersEnd == 1) {
			cout << "Not enough users" << endl;
			return;
		}

		
		int temp;
		int min = 9999;
		int matchID = 0;
		for (int i = 0; i < usersEnd; i++) {
			int shareVal = 0;
			if (userList[i] == *U) continue;
			for (int j = 0; j < 5; j++) {
				temp = userList[i].movies[U->top5ID[j]].rating - U->movies[U->top5ID[j]].rating;
				temp = pow(temp, 2);
				shareVal += temp;
			}
			if (shareVal < min) {
				min = shareVal;
				matchID = i;
			}
		}

		int percentage = 100 - min;

		cout << U->username << "'s best match is: " << userList[matchID].username << "(" << percentage << "%)" << endl;
		cout << "Here are their recommended movies!" << endl;
		cout << "(1). " << userList[matchID].recomendations[0] << endl;
		cout << "(2). " << userList[matchID].recomendations[1] << endl;
		cout << "(3). " << userList[matchID].recomendations[2] << endl;
	}

	void printAllMatches(string username) {

		User* U = findUser(username);
		if (U == NULL) {
			cout << "User not found" << endl;
			return;
		}
		if (usersEnd == 1) {
			cout << "Not enough users" << endl;
			return;
		}


		int temp;
		int min = 9999;
		int matchID = 0;
		for (int i = 0; i < usersEnd; i++) {
			int shareVal = 0;
			if (userList[i] == *U) continue;
			for (int j = 0; j < 5; j++) {
				temp = userList[i].movies[U->top5ID[j]].rating - U->movies[U->top5ID[j]].rating;
				temp = pow(temp, 2);
				shareVal += temp;
			}
			if (shareVal < min) {
				min = shareVal;
				matchID = i;
			}
			int percentage = 100 - shareVal;
			cout << U->username << " and " << userList[i].username << ": " << percentage << "%" << endl;
		}

		int percentage = 100 - min;

		cout << U->username << "'s best match is: " << userList[matchID].username << "(" << percentage << "%)" << endl;


	}

	string deleteTimeStamp(string item) {
		for (int i = 0; i < item.length(); i++) {
			if (item[i] == '1') {
				for (int j = i; j < item.length(); j++) {
					item[j] = ' ';
				}
			}
		}
		return item;
	}

	int loadRatings(string filename) {
		ifstream myfile;
		string temp;
		int numUsers = 0;

		vector <string> tempArr;
		myfile.open(filename);
		if (!myfile.good()) {
			cout << "Error reading file ... Check argv[1]" << endl;
			return -1;
		}
		if (myfile.is_open()) {

			while (myfile.good()) {
				getline(myfile, temp, ',');
				numUsers++;
				tempArr.push_back(temp);
			}
			cout << "File successfully stored into vector. Now printing line by line." << endl;
			numUsers = numUsers / 35;
			for (int i = 0; i < tempArr.size(); i++) {
				cout << "Index " << i << " -> ";
				cout << tempArr[i] << endl;
			}


			cout << numUsers << " Users Added" << endl;
			for (int i = 0; i < numUsers; i++) {
				createUser(tempArr[(i * 31) + 63]);
			}

			cout << "adding movies" << endl;
			for (int i = 2; i <= 26; i++) {
				addMovie(tempArr[i], i - 2);
			}

			for (int i = 0; i < numUsers; i++) {
				userList[i].recomendations[0] = tempArr[(31 * i) + 89];
				userList[i].recomendations[1] = tempArr[(31 * i) + 92];
				string temp = tempArr[(31 * i) + 93];
				userList[i].recomendations[2] = deleteTimeStamp(temp);
			}

			for (int i = 0; i < numUsers; i++) {
				for (int j = 0; j < 25; j++) {
					cout << "Adding movie " << userList[i].movies[j].title << endl;
					userList[i].movies[j].title = movieTitles[j];
				}
				cout << "To user " << userList[i].username << endl;
			}

			for (int i = 0; i < userList.size(); i++) {
				
					
					for (int k = 0; k < 25; k++) {

						if (tempArr[(i * 31) + 64 + k] == "Haven't Seen") {
							cout << "HAVENT SEEN" << endl;
							addUserRating(userList[i].username, userList[i].movies[k].title, -1);
						}
						else {
							cout << userList[i].username << ": " << movieTitles[k] << " rating: " << stoi(tempArr[(i * 31) + 64 + k]) << endl;
							addUserRating(userList[i].username, userList[i].movies[k].title, stoi(tempArr[(i * 31) + 64 + k]));

						}

					}
			
			}

		}

		cout << endl << "All Information Loaded" << endl;
		return 0;
	}

};



int main(int argc, char* argv[]) {

	
	cout << setprecision(3);

	userList info;

	if (argc != 2) {
		cout << "Input argument error" << endl;
		cout << "Usage: " << endl;
		cout << "./a.out <RESULTS_FILE_NAME.csv" << endl;
		return 0;
	}

	int err = info.loadRatings(argv[1]);
	if (err == -1) return 0;


	info.setTop5();

	string inputStr;
	int input;
	while (input != 10) {
		cout << endl;
		cout << "1 -- Print users" << endl;
		cout << "2 -- All user ratings" << endl;
		cout << "3 -- All users' top 5 movies" << endl;
		cout << "4 -- Find user's top 5 movies" << endl;
		cout << "5 -- Find user's best match + reccomend movies" << endl;
		cout << "6 -- Find all matches for a user" << endl;
		cout << "7 -- All best matches" << endl;
		cout << "8 -- Average Movie Ratings" << endl;
		cout << "9 -- Find fans of a movie" << endl;
		cout << "10 - Quit" << endl << endl;

		getline(cin, inputStr);
		input = stoi(inputStr);

		if (input == 1) {
			//Print users
			info.printUsers();
		}
		else if (input == 2) {
			//Print all user ratings
			info.printUserRatings();
		}
		else if (input == 3) {
			//Print all user top 5 movies
			info.printAllTop5Ratings();
		}
		else if (input == 4) {
			//Print specific user top 5 movies
			cout << "Input user: " << endl;
			getline(cin, inputStr);
			User* U = info.findUser(inputStr);
			cout << endl;
			info.printTop5Rating(U);
		}
		else if (input == 5) {
			//Find user best match
			cout << "Input user: " << endl;
			getline(cin, inputStr);
			User* U = info.findUser(inputStr);
			cout << endl;
			info.findMatch(inputStr);
		}
		else if (input == 6) {
			//Print all matches for a user
			cout << "Input user: " << endl;
			getline(cin, inputStr);
			cout << endl;
			info.printAllMatches(inputStr);
		}
		else if (input == 7) {
			//Print all best matches
			info.printAllBestMatches();
		}
		else if (input == 8) {
			//Print Average Ratings
			info.printAverageRating();
		}
		else if (input == 9) {
			//Find fans of a movie
			while (true) {
				cout << "Enter movie: " << endl;
				getline(cin, inputStr);
				if (info.getMovieNumber(inputStr) == -1) {
					cout << "Try again" << endl;
					continue;
				}
				else {
					break;
				}
			}

			string movie = inputStr;
			cout << "Enter ranking: " << endl;
			getline(cin, inputStr);
			info.findFans(movie, stoi(inputStr));

		}
		else if(input == 10){
			cout << "Quitting ... Goodbye" << endl;
			return 0;
		}
		else {
			cout << "Incorrect input ... Try Again" << endl;
		}
	}



	return 0;

}
