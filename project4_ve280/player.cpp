#include "player.h"
#include <iostream>
using namespace std;


class Simple_player : public Player{
 public:
	int bet(unsigned int bankroll, unsigned int minimum){
		return minimum; //always bet minimum
	}

	bool draw(Card dealer, const Hand &player){
		if(player.handValue().soft){ //soft strategy
			if(player.handValue().count <= 17)
				return true;
			if(player.handValue().count==18 && dealer.spot!=0 && dealer.spot!=5 && dealer.spot!=6)
				return true;
			return false;
		}
		else{ //hard strategy
			if(player.handValue().count <= 11)
				return true;
			if(player.handValue().count==12 && (dealer.spot<2 || dealer.spot>4))
				return true;
			if(player.handValue().count>=13 && player.handValue().count<=16 && dealer.spot>4)
				return true;
			return false;
		}
		return false;
	}

	void expose(Card c){} //simple player doesn't do expose or shuffled method
	void shuffled(){}
};

class Counting_player : public Player{
	int count; //works for his strategy on bet_num
 public:
	int bet(unsigned int bankroll, unsigned int minimum){
		if(bankroll>=2*minimum && count>=2)
			return minimum*2; //bet_num = 2*minimum
		return minimum;
	}

	bool draw(Card dealer, const Hand &player){
		if(player.handValue().soft){ //same strategy as simple player
			if(player.handValue().count <= 17)
				return true;
			if(player.handValue().count==18 && dealer.spot!=0 && dealer.spot!=5 && dealer.spot!=6)
				return true;
			return false;
		}
		else{
			if(player.handValue().count <= 11)
				return true;
			if(player.handValue().count==12 && (dealer.spot<2 || dealer.spot>4))
				return true;
			if(player.handValue().count>=13 && player.handValue().count<=16 && dealer.spot>4)
				return true;
			return false;
		}
		return false;
	}	

	void expose(Card c){ //change count via expose method
		if(c.spot <= 4)
			count += 1;
		if(c.spot >= 8)
			count -= 1;
	}
	void shuffled(){ //after shuffle, count=0
		count = 0;
	}
};

Player* get_Simple(){
	static Simple_player p;
	return &p;
}

Player* get_Counting(){
	static Counting_player p;
	return &p;
}