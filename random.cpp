#include <iostream>
#include <cassert>
#include "MersenneTwister.h"

int main(){
	char token;
	int first,input;
	MTRand random;
	
	std::cin >> token;
	assert(token == 'p');
	std::cin >> first;
	if(first == 2){
		std::cout << "?";
	}else{
		std::cout << random.randInt(6)+1;
	}
	while(input > 0){
		std::cin >> input;
		std::cout << random.randInt(6)+1;
	}
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