#include <iostream>
#include <cassert>
#include "MersenneTwister.h"

int makenextmove(const int *array,const int *top);
bool isthisgoingtowin(const int *array,const int *top,int move,int player);

int main(){
	//my inside data
	int first,input,nextmove,array[42],top[7],i;
	MTRand random;
	
	//instantiazition
	for(i = 0; i < 42; ++i){
		array[i] = 0;
	}
	for(i = 0; i < 7; ++i){
		top[i] = 0;
	}
	
	//tells the mod it's ready
	std::cout << 'p';
	std::cin >> first;
	
	//the meat of the program
	if(first == 2){
		std::cout << "?";
	}else{
		//i'm going first
		nextmove = random.randInt(3)+3;
		array[nextmove*top[nextmove]] = 1;
		++top[nextmove];
		std::cout << nextmove;
	}
	while(input > 0){
		std::cin >> input;
		array[input*top[input]] = 2;
		++top[input];
		nextmove = makenextmove(array,top);
		++top[nextmove];
		std::cout << nextmove;
	}
	
	//fun end messages
	if(input == 0){
		std::cout << "aww something wrong happened\n";
	}
	if((input == -1 && first == 1)||(input == -2 && first == 2)){
		std::cout << "alright.that was a good game you played.\n";
	}
	if((input == -2 && first == 1)||(input == -1 && first == 2)){
		std::cout << "that was a good game you played.lets play again sometimes\n";
	}
	if(input == -3){
		std::cout << "you played well. Let's decide this at another time\n";
	}
}

int makenextmove(const int *array,const int *top){
	int i;
	bool possiblemoves[7];
	//first find the moves not allowed due to height constraint
	for(i = 0; i < 7 ; ++i){
		if(top[i] == 6){
			possiblemoves[i] = false;
		}
	}
	
	//then find the moves that let me win
	//then remove the moves that allow my opponent to win
	//if there are no moves remaining...then I guess he wins
	//then look into the future three moves
	//don't choose any of those that let opponent win
	//favor those that let me win ... like the three in a row with two sides not blocked
	//look for the moves that let me get three in a row where one of the sides are not blocked
	//look for the two in a row that are unblocked and in the center
	//look for the two in a row
	//move randomly from the possible moves
	return 1;
}

bool isthisgoingtowin(const int *array,const int *top,int move,int player){
	return true;
}