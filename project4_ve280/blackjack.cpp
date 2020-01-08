#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "deck.h"
#include "card.h"
#include "player.h"
#include "hand.h"
#include "rand.h"
using namespace std;

string split(string &str){
	//EFFECTS:cut the first word of str(words are split by a space), 
	//			original str will be modified, return the first word
	//MODIFIES: str becomes the original string without its first word
	int len = str.length();
	string temp = "";
	for(int i=0; i<len; i++){
		if (str[i] != ' ')
			temp += str[i]; 
		else{
			str = (i==len-1) ? "" : str.substr(i+1,len); 
			break;
		}
		if(i==len-1)
			str = "";
	}
	return temp;
} 

void prescribed_shuffle(Deck &deck, string file_name){
	//EFFECTS: shuffle the deck with a given file, file contains a single line
	//			showing a series of (integers)cut place, seperated by space
	//MODIFIES: deck
	//REQUIRES: file exist
	//NOTES: it uses function split()
	int cut_place = 0;
	ifstream f(file_name.c_str());
	string f_content;
	getline(f, f_content);
	string temp;
	cout << "# Shuffling the deck" << endl;
	for(int i=0; i<1000; i++){
		temp = split(f_content);
		if(temp!=""){
			cut_place = atoi(temp.c_str());
			deck.shuffle(cut_place);
			cout<<"cut at "<<cut_place<<endl;
		}
		else
			break;
	}
}

void random_shuffle(Deck &deck){
	//EFFECTS: shuffle the deck 7 times randomly with the shuffle method of deck
	//MODIFIES: deck
	//NOTES: it uses get_cut() method in rand.h
	int cut_place;
	cout << "# Shuffling the deck" << endl;
	for(int i=0; i<7; i++){
		cut_place = get_cut();
		deck.shuffle(cut_place);
		cout<<"cut at "<<cut_place<<endl;
	}
}

void dealt_card(Card c, Hand &player, Spot (&dealt_spot)[DeckSize],\
	 Suit (&dealt_suit)[DeckSize], int &dealt_num){
	//EFFECTS: deal a card, changes related container & variable
	//MODIFIES: Card c, Hand &player, Spot (&dealt_spot)[DeckSize], 
	//			Suit (&dealt_suit)[DeckSize], int &dealt_num
	player.addCard(c);
	dealt_spot[dealt_num] = c.spot;
	dealt_suit[dealt_num] = c.suit;
	dealt_num += 1;
}

void discard_hands(Hand &player, Hand &dealer){
	//EFFECTS: clear the hands
	//MODIFIES: player & dealer's hands
	player.discardAll();
	dealer.discardAll();
}

//some exception structure used to interrupt the one_turn() function
//symbolizing one turn is ended with natural_21/lose/win/push
struct natural_21{};
struct lose{};
struct push{};
struct win{};

void one_turn(Deck &deck, Hand &player, Hand &dealer, Player* p, 
				Spot (&dealt_spot)[DeckSize], Suit (&dealt_suit)[DeckSize], int &dealt_num){ 
	//EFFECTS: Do one hand in the game
	//MODIFIES: player & dealer's hands, two arrays to store the dealt spot & suit, dealt_num 
	Card card_1 = deck.deal(); //the first four cards in the game
	p->expose(card_1);
	Card dealer_up = deck.deal();
	p->expose(dealer_up);
	Card card_2 = deck.deal();
	p->expose(card_2);
	cout<<"Player dealt "<< SpotNames[card_1.spot] <<" of " << SuitNames[card_1.suit]<<endl;
	dealt_card(card_1, player, dealt_spot, dealt_suit, dealt_num);
	cout<<"Dealer dealt "<< SpotNames[dealer_up.spot] <<" of " << SuitNames[dealer_up.suit]<<endl;
	dealt_card(dealer_up, dealer, dealt_spot, dealt_suit, dealt_num);
	cout<<"Player dealt "<< SpotNames[card_2.spot] <<" of " << SuitNames[card_2.suit]<<endl;
	dealt_card(card_2, player, dealt_spot, dealt_suit, dealt_num);
	Card dealer_hole = deck.deal();
	dealt_card(dealer_hole, dealer, dealt_spot, dealt_suit, dealt_num);

	if(player.handValue().count == 21){ //natural_21 happens, end the turn
		cout<< "# Player dealt natural 21"<<endl;
		natural_21 end_turn;
		discard_hands(player, dealer);
		throw end_turn;
	}
	
	while(p->draw(dealer_up, player)){//no natural_21, player draw cards until stand or bust
		Card card = deck.deal();
		dealt_card(card, player, dealt_spot, dealt_suit, dealt_num);
		cout<<"Player dealt "<< SpotNames[card.spot]<<" of " << SuitNames[card.suit] <<endl;
		p->expose(card);
		if(player.handValue().count > 21){ // player bust, end the turn
			cout<<"Player's total is "<< player.handValue().count << endl;
			cout<<"# Player busts"<<endl;
			lose end_turn;
			discard_hands(player, dealer);
			throw end_turn;
		}
	}
	if(!p->draw(dealer_up, player))// player stand
		cout<<"Player's total is "<< player.handValue().count << endl;

	//dealer's draw cards until 17 is reached(either sort or hard)
	cout<<"Dealer's hole card is "<< SpotNames[dealer_hole.spot] <<" of " << SuitNames[dealer_hole.suit]<<endl;
	while(dealer.handValue().count < 17){ //17 is not reached, draw card
		Card card = deck.deal();
		dealt_card(card, dealer, dealt_spot, dealt_suit, dealt_num);
		cout<<"Dealer dealt "<< SpotNames[card.spot] <<" of " << SuitNames[card.suit]<<endl;
		p->expose(card);		
		if(dealer.handValue().count > 21){ // dealer bust, end the turn
			cout<<"Dealer's total is "<< dealer.handValue().count <<endl;
			p->expose(dealer_hole);
			cout<<"# Dealer busts"<<endl;
			win end_turn;
			discard_hands(player, dealer);
			throw end_turn;			
		}
	}

	//nobody bust, expose the hole card of dealer, compare two hands' count
	p->expose(dealer_hole); 
	//Dealer has larger count, Dealer win, end the turn
	if(dealer.handValue().count > player.handValue().count){
		cout<<"Dealer's total is "<< dealer.handValue().count << endl;
		cout<<"# Dealer wins"<<endl;
		lose end_turn;
		discard_hands(player, dealer);
		throw end_turn;
	}//equivalent count, push, end the turn
	else if (dealer.handValue().count == player.handValue().count){
		cout<<"Dealer's total is "<< dealer.handValue().count << endl;
		cout<<"# Push"<<endl;
		push end_turn;
		discard_hands(player, dealer);
		throw end_turn;		
	}
	else{//Player has larger count, Player win, end the turn
		cout<<"Dealer's total is "<< dealer.handValue().count << endl;
		cout<<"# Player wins"<<endl;
		win end_turn;
		discard_hands(player, dealer);
		throw end_turn;		
	}
}

void blackjack(int argc, char* argv[]){
	//args of the game
	int bankroll = atoi(argv[1]);
	int min_bet = atoi(argv[2]);
	int hand_num = atoi(argv[3]);
	string fourth_arg = argv[4];
	bool simple = (fourth_arg=="simple") ? true : false;
	string file_name = (argc==6) ? argv[5] : "";

	//initialize the deck
	Deck deck;
	if(file_name != "")
		prescribed_shuffle(deck, file_name);
	else
		random_shuffle(deck);

	//initialize dealer and player, the dealt array
	Hand dealer;
	Hand player;
	Player *p1 = (simple) ? get_Simple() : get_Counting();
	Spot dealt_spot[DeckSize];
	Suit dealt_suit[DeckSize];

	for(int this_hand=1; this_hand<=hand_num; this_hand++){
		if (bankroll < min_bet){ //can't afford min_bet, end the game
			cout << "# Player has "<< bankroll<< " after "<< this_hand-1 <<" hands"<<endl;
			break;
		}

		int dealt_num = DeckSize-deck.cardsLeft();
		
		cout << "# Hand " << this_hand << " bankroll " << bankroll << endl;
		if(deck.cardsLeft() < 20){ //shuffle the deck when less than 20 cards
			random_shuffle(deck);
			p1->shuffled();
			dealt_num = 0;
		}
		//bet according to player's strategy
		int bet_num = p1->bet((unsigned int)bankroll, (unsigned int)min_bet);
		cout << "# Player bets "<< bet_num << endl;
		//do one turn, change the bankroll according to the result
		try{
			one_turn(deck, player, dealer, p1, dealt_spot, dealt_suit, dealt_num);
		}
		catch(natural_21){bankroll += 3.0*(float)bet_num/2.0;}
		catch(win){bankroll += bet_num;}
		catch(lose){bankroll -= bet_num;}
		catch(...){}
		//reach the maximum hand, end the game
		if(this_hand == hand_num){
			cout << "# Player has "<< bankroll<< " after "<< this_hand <<" hands"<<endl;
			break;			
		}
	}
}

int main(int argc, char* argv[]){  
	blackjack(argc,argv);
    return 0;
}  