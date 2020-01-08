#include "deck.h"
#include "card.h"
using namespace std;

Deck::Deck(){
	this->next = 0;
	Card c;
	for(int j=0; j<SUIT_SIZE; j++){
		for(int i=0; i<SPOT_SIZE; i++){
			c.spot = Spot(i);
			c.suit = Suit(j);
			this->deck[13*j+i] = c;
		}
	}
}

void Deck::reset(){
	this->next = 0;
	Card c;
	for(int j=0; j<SUIT_SIZE; j++){
		for(int i=0; i<SPOT_SIZE; i++){
			c.spot = Spot(i);
			c.suit = Suit(j);
			this->deck[13*j+i] = c;
		}
	}
}


void Deck::shuffle(int n){
	int l_index = 0;
	int r_index = n;
	int order[DeckSize]; //store the new order of the deck
	int i = 0;
	bool now_add_r = true; //need to add card alternately, this shows add from which part(right/left)
	while(l_index<n && r_index<DeckSize){ //add card alternately
		order[i] = (now_add_r) ? r_index : l_index;
		r_index = (now_add_r) ? r_index+1 : r_index;
		l_index = (now_add_r) ? l_index : l_index+1;
		now_add_r = !now_add_r;
		i += 1;
	}
	if(l_index>=n){ //left part is used up
		for(int j=i; j<DeckSize; j++)
			order[j] = j;
	}
	else{ //right part is used up
		for(int j=i; j<DeckSize; j++)
			order[j] = n+j-DeckSize;		
	}
	Card c; 
	int old_order[DeckSize];
	for(int j=0; j<DeckSize; j++){ // the old_order(status of deck before shuffle)
		old_order[j] = this->deck[j].suit*13 + this->deck[j].spot;
	}
	for(int j=0; j<DeckSize; j++){ //replace the deck with a deck with new order
		c.spot = Spot(old_order[order[j]]%13);
		c.suit = Suit((int) old_order[order[j]]/13);
		this->deck[j] = c;
	}
	this->next = 0; 
}

Card Deck::deal(){
	if(next >= DeckSize){ //no card to deal
		DeckEmpty err;
		throw err;
	}
	next += 1;
	return this->deck[next-1];
}

int Deck::cardsLeft(){
	return DeckSize-next;
}
