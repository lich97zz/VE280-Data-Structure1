#include <iostream>
#include "p1.h"
using namespace std;

string init(){
	//Initialize the game, randomize a target word from given words with an asking seed
	//Args: NULL
	//Return: The target word in string type
	cout << "Please input seed: ";
	int seed;
	cin >> seed;
	p1_srand(seed);
	int word_index = p1_rand() % nWords;
	string target_word = words[word_index];
	return target_word;
}

string init_current_word(string target_word){
	//Initialize the current word, '_' in the string means hidden character, all chars are hidden
	//Args: the target word
	//Return: a string showing the current word, all chars in target_word is replaced by '_'
	int len = target_word.length();
	string result;
	for(int i=0; i<len; i++){
		result += '_';
	}
	return result;
}

void show_current_word(string current_word){
	//show the current word in required form, use space to split each char
	//Args: string currrent_word, '_' in the string means hidden character
	//Return: NULL
	int len = current_word.length();
	string result;
	for(int i=0; i<len-1; i++){
		result += current_word[i];
		result += ' ';
	}
	result += current_word[len-1];
	cout << result << endl;
}

void sort(string &proposed_char){
	int len = proposed_char.length();
	char temp;
	for(int j=0; j<len; j++){
	    for(int i=0; i<len-1-j; i++){
	    	if(proposed_char[i]>proposed_char[i+1]) 
	    	{
	       		temp=proposed_char[i];
	       		proposed_char[i]=proposed_char[i+1];
	        	proposed_char[i+1]=temp;
	    	}
	    }
	}
}

void show_proposed_char(string &proposed_char){
	//prompt the proposed char in required form, use space to split each char
	//Args: string proposed_char, the proposed char in previous trial
	//Return: NULL
	sort(proposed_char);
	cout << "Already proposed characters:" << endl;
	int len = proposed_char.length();
	string result;
	for(int i=0; i<len; i++){
		if (proposed_char[i] != '_'){
			result += proposed_char[i];
			result += ' ';
		}
	}
	cout << result << endl;
}

void first_prompt(string current_word){
	//Show the prompt in game at beginning, including current score(=0), the exposed char of target_word(NULL) and the already proposed char(NULL)
	//Args: string currrent_word, '_' in the string means hidden character
	//Return: NULL
	cout << "Score: " << 0 << endl;
	show_current_word(current_word);
	show_proposed_char(current_word);
}

bool reasonable(char c, string s){
	//check the input is in range 'a'-'z' and does not overlap chars in shown_char, return a boolean value
	//Args: 
	//	char c, tested char
	//	string s, containing tested chars
	//Return: whether the choice is in range 'a'-'z', and does not overlap chars in shown_char
	if (c<97 || c>122)
		return false;
	int len = s.length();
	for(int i=0; i<len; i++){
		if(c == s[i])
			return false;
	}
	return true;
}

bool belong(char c, string s){
	//check if char c is in string s, return a boolean value
	int len = s.length();
	for(int i=0; i<len; i++){
		if (c == s[i])
			return true;
	}
	return false;
}

void process(char c, string &target_word, string &current_word, string &proposed_char){
	//modify the current_word and proposed_char after doing a reasonable guess
	//Args:
	//	char c, the input char
	//	string target_word, the target word in the game
	// 	string current_word, '_' in the string means hidden character
	// 	string proposed_char, contains the already proposed char in previous trials
	if (belong(c, target_word)){
		int len = target_word.length();
		for(int i=0; i<len; i++){
			if (target_word[i] == c){
				current_word[i] = c;
			}
		}
	}
	else{
		if(! belong(c, proposed_char))
			proposed_char += c;
	}
}

void halfway_prompt(int &score, string &target_word, string &current_word, string &proposed_char){
	//Show the prompt in game with required form, including current score, the exposed char of target_word and the already proposed char
	//Args:
	//	int score, current score as number of trials
	//	string target_word, the target word in the game
	//	string currrent_word, '_' in the string means hidden character
	//  string proposed_char, contains the already proposed char in previous trials
	//	bool processing, if won, show different prompt
	//Return: NULL
	cout << "Please enter a letter (a-z): ";
	char choice;
	cin >> choice;
	if (reasonable(choice, proposed_char)){ //Correct guess or reasonable incorrect guess
		if (!belong(choice, target_word)) //incorrect guess that has not appeared yet
			score += 1; 
		process(choice, target_word, current_word, proposed_char);
	}
	if (current_word != target_word && score < 10){
		cout << "Score: " << score << endl;
		show_current_word(current_word);
		show_proposed_char(proposed_char);
	}
}

void hangman(){
	// The game
	string target_word = init(); //randomize a target_word
	string current_word = init_current_word(target_word); //the status of guess, with "_" symbolizing hidden char
	string proposed_char = ""; //the already proposed char
	int score = 0;
	int TRIAL_NUM_LIMIT = 10; //time of trial that can't exceed
	string final_prompt = "The word was: " + target_word + "\nYou lost!";
	first_prompt(current_word);
	while(score < TRIAL_NUM_LIMIT){
		halfway_prompt(score, target_word, current_word, proposed_char);
		if (target_word == current_word){
			final_prompt = "The word was: " + target_word + "\nYou won!";
			break;
		}
	}
	cout << final_prompt << endl;
}

int main(){
	hangman();
	return 0;
}
