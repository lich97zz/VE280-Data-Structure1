
#include "hand.h"

Hand::Hand(){
	this->curValue.count = 0;
	this->curValue.soft = false;
}

void Hand::discardAll(){
	this->curValue.count = 0;
	this->curValue.soft = false;	
}

void Hand::addCard(Card c){
	int spot_val = 0; //the spot
	if(c.spot==ACE){ //match the enum Spot type with the true value of spot
		spot_val = (this->curValue.soft) ? 1 : 11; //if an ace is already existed, new ace must be 1
		this->curValue.soft = true;
	}
	else if(c.spot>=9 && c.spot<=11)
		spot_val = 10;
	else
		spot_val = c.spot + 2;
	this->curValue.count += spot_val;
}

HandValue Hand::handValue() const{
	if((this->curValue.count)>21 && this->curValue.soft){ //if exist ACE and point>21, make ACE count as 1
		HandValue temp;
		temp.count = (this->curValue.count-10);
		temp.soft = false;
		return temp;
	}
	return this->curValue;
}