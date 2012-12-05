#include <iostream>
#include <cassert>
#include "MersenneTwister.h"
#define width 7
#define height 6

int makenextmove(const int *array,const int *top);
bool isthisgoingtowin(const int *array,const int *top,int move,int player);
void copyarray(const int *tocopy, int *copyto, int numtocopy);
void debug(const int *array,const int *top);

//main driver program that interacts with the connect four moderator
int main(){
	//my inside data
	int first,input,nextmove,array[width*height],top[width],i;
	MTRand random;	
	//instantiazition
	for(i = 0; i < width*height; ++i){
		array[i] = 0;
	}
	for(i = 0; i < width; ++i){
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
		nextmove = random.randInt(2)+2;
		array[nextmove+width*top[nextmove]] = 1;
		++top[nextmove];
		++nextmove;
		std::cout << nextmove;
	}
	std::cin >> input;
	while(input > 0 || input == -4){
		if(input == -4){
			debug(array,top);
			std::cin >> input;
			continue;
		}
		--input;
		array[input+width*top[input]] = 2;
		++top[input];
		nextmove = makenextmove(array,top);
		array[nextmove+width*top[nextmove]] = 1;
		++top[nextmove];
		++nextmove;
		std::cout << nextmove;
		std::cin >> input;
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

//copies numtocopy elements from tocopy to copyto
void copyarray(const int *tocopy, int *copyto, int numtocopy){
	for(int i = 0; i < numtocopy; ++i){
		copyto[i] = tocopy[i];
	}
}

//will try to determine the next best move. Read the individual 
//comments to find out the steps it goes through to get that
int makenextmove(const int *array,const int *top){
	int i,j,nextmove,possiblemovesremaining=0;
	bool possiblemoves[width];
	int temparray[width*height],temptop[width];
	//first find the moves not allowed due to height constraint
	for(i = 0; i < width; ++i){
		if(top[i] == height){
			possiblemoves[i] = false;
		}else{
			possiblemoves[i] = true;
		}
	}
	//then find the moves that let me win or prevent my opponent from winning
	for(i = 0; i < width; ++i){
		if(possiblemoves[i]){
			nextmove = i;
		}
		if(possiblemoves[i] && (isthisgoingtowin(array,top,i,1) || isthisgoingtowin(array,top,i,2))){
			return i;
		}
	}
	//remove any of my moves that would allow my opponent to win
	for(i = 0; i < width; ++i){
		if(possiblemoves[i]){
			copyarray(array,temparray,width*height);
			copyarray(top,temptop,width);
			temparray[i+width*temptop[i]] = 1;
			++temptop[i];
			if(isthisgoingtowin(temparray,temptop,i,2)){
				possiblemoves[i] = false;
			}
		}
	}
	//if there are no moves remaining...then I guess he wins
	for(i = 0; i < width; ++i){
		if(possiblemoves[i]){
			++possiblemovesremaining;
		}
	}
	if(possiblemovesremaining == 0){
		return nextmove;
	}
	//then look into the future three moves
	//pick out the ones that will give me a chance of winning
	int numberofwins[width],max = 0;
	for(i = 0; i < width; ++i){
		int secondlayerarray[width*height],secondlayertop[width];
		numberofwins[i] = 0;
		if(possiblemoves[i]){
			copyarray(array,temparray,width*height);
			copyarray(top,temptop,width);
			temparray[i+width*temptop[i]] = 1;
			++temptop[i];
			copyarray(temparray,secondlayerarray,width*height);
			copyarray(temptop,secondlayertop,width);
			for(j = 0; j < width; ++j){
				secondlayerarray[j+width*secondlayertop[j]] = 2;
				++secondlayertop[j];
				if(isthisgoingtowin(secondlayerarray,secondlayertop,i,1)){
					++numberofwins[i];
				}
				--secondlayertop[j];
				secondlayerarray[j+width*secondlayertop[j]] = 0;
			}
		}
	}
	for(i = 0; i < width; ++i){
		if(numberofwins[i] > max){
			max = numberofwins[i];
		}
	}
	if(max > 1){
		for(i = 0; i < width; ++i){
			if(numberofwins[i] == max){
				return i;
			}
		}
	}
	//look for three out of four in a row
	//look for the two in a row that are unblocked and in the center
	//look for the two in a row
	//move randomly from the possible moves
	MTRand random;
	int randnum = int(random.rand() * 7);
	for(i = 0;randnum != 0 && i < width; ++i){
		if(possiblemoves[i]){
			--randnum;
			nextmove = i;
		}
	}
	return nextmove;
}

//given an board ,the number of pieces in each column,where to put the next piece
//and the player will return if that move will give a win for that player
bool isthisgoingtowin(const int *array,const int *top,int move,int player){
	int i,connectedpieces;
	//check if you win going down the row
	if(top[move] >= 3 /*at least three pieces in that row*/ 
		&& array[move + width*(top[move] - 1)] == player
		&& array[move + width*(top[move] - 2)] == player
		&& array[move + width*(top[move] - 3)] == player){
		return true;
	}
	//check if you win on the right diagonally
	connectedpieces = 0;
	for(i = 1; i < 4; ++i){
		if(move - i < 0 || top[move] - i < 0 || array[move - i + width*(top[move] - i)] != player){
			break;
		}
		++connectedpieces;
	}
	for(i = 1; i < 4; ++i){
		if(move + i >= width || top[move] + i >= width || array[move + i + width*(top[move] + i)] != player){
			break;
		}
		++connectedpieces;
	}
	if(connectedpieces >= 3){
		return true;
	}
	//check if you win on the left diagonal
	connectedpieces = 0;
	for(i = 1; i < 4; ++i){
		if(move - i < 0 || top[move] + i >= width || array[move - i + width*(top[move] + i)] != player){
			break;
		}
		++connectedpieces;
	}
	for(i = 1; i < 4; ++i){
		if(move + i >= width || top[move] - i < 0 || array[move + i + width*(top[move] - i)] != player){
			break;
		}
		++connectedpieces;
	}
	if(connectedpieces >= 3){
		return true;
	}
	//check if you win horizontally
	connectedpieces = 0;
	for(i = 1; i < 4; ++i){
		if(move - i < 0 || array[move - i + width*top[move]] != player){
			break;
		}else{
			++connectedpieces;
		}
	}
	for(i = 1; i < 4; ++i){
		if(move + i >= width || array[move + i + width*top[move]] != player){
			break;
		}else{
			++connectedpieces;
		}
	}
	if(connectedpieces >= 3){
		return true;
	}
	//you don't win sorry.
	return false;
}

bool unblockedthreeinarow(const int *array,const int *top,int move,int player){
	int i,connectedpieces;
	if(move == 0 || move == width - 1){
		return false;
	}
	//check if you have a unblocked 3 in a row on the right diagonal
	connectedpieces = 0;
	for(i = 1; i < 3; ++i){
		if(move - i < 0 || top[move] - i < 0 || array[move - i + width*(top[move] - i)] != player){
			break;
		}
		++connectedpieces;
	}
	if(move - i >= 0 && top[move] - i >= 0 && array[move - i + width*(top[move] - i)] != 0){
			connectedpieces = -10;
	}
	for(i = 1; i < 3; ++i){
		if(move + i >= width || top[move] + i >= width || array[move + i + width*(top[move] + i)] != player){
			break;
		}
		++connectedpieces;
	}
	if(move + i < width && top[move] + i < width && array[move + i + width*(top[move] + i)] != 0){
			connectedpieces = -10;
	}
	if(connectedpieces >= 2){
		return true;
	}
	//check if you have a unblocked 3 in a row on the left diagonal
	connectedpieces = 0;
	for(i = 1; i < 3; ++i){
		if(move - i < 0 || top[move] + i >= width || array[move - i + width*(top[move] + i)] != player){
			break;
		}
		++connectedpieces;
	}
	if(move - i >= 0 && top[move] + i < width && array[move - i + width*(top[move] + i)] != 0){
			connectedpieces = -10;
	}
	for(i = 1; i < 3; ++i){
		if(move + i >= width || top[move] - i < 0 || array[move + i + width*(top[move] - i)] != player){
			break;
		}
		++connectedpieces;
	}
	if(move + i < width && top[move] - i > 0 && array[move + i + width*(top[move] - i)] != 0){
			connectedpieces = -10;
	}
	if(connectedpieces >= 3){
		return true;
	}
	//check if you have a unblocked 3 in a row horizontally
	connectedpieces = 0;
	for(i = 1; i < 3; ++i){
		if(move - i < 0 || array[move - i + width*top[move]] != player){
			break;
		}else{
			++connectedpieces;
		}
	}
	if(move - i > 0 && array[move - i + width*top[move]] != 0){
			connectedpieces = -10;
	}
	for(i = 1; i < 3; ++i){
		if(move + i >= width || array[move + i + width*top[move]] != player){
			break;
		}else{
			++connectedpieces;
		}
	}
	if(move + i < width && array[move + i + width*top[move]] != player){
			connectedpieces = -10;
	}
	if(connectedpieces >= 3){
		return true;
	}
	return false;
}

//prints out the internal status of the board
void debug(const int *array,const int *top){
	int i,j,val;
	std::cout << "array\n";
	for(i = height - 1; i >= 0; --i){
		for(j = 0; j < width; ++j){
			val = array[j + i*width];
			std::cout << val;
		}	
		std::cout << "\n";
	}
	std::cout << "top\n";
	for(i = 0; i < width; ++i){
		std::cout << top[i];
	}
	std::cout << "\n";
}