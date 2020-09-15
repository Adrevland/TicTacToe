#include <iostream>
#include <array>
#include <string>
#include <stdlib.h>

//clean the code

//global vars
std::string box[] = { "1","2","3","4","5","6","7","8","9" };
std::string box_clone[] = { "1","2","3","4","5","6","7","8","9" };
int taken[10];


void clear_board() {
	for (int k = 0; k < 10; k++)
	{
		// clears box array to "default" [1-9]
		std::string pls_work = std::to_string(k+1);
		box[k] = pls_work;

		//clears taken[]
		taken[k] = 0;
	}
	
}
// checks for win. horizontally, vertical and diagonal. returns who won
std::string check_win() {
	if (box[0] == box[1] && box[1] == box[2])
		return box[0];
	else if (box[3] == box[4] && box[4] == box[5])
		return box[3];
	else if (box[6] == box[7] && box[7] == box[8])
		return box[6];
	else if (box[0] == box[3] && box[3] == box[6])
		return box[0];
	else if (box[1] == box[4] && box[4] == box[7])
		return box[1];
	else if (box[2] == box[5] && box[5] == box[8])
		return box[2];
	else if (box[0] == box[4] && box[4] == box[8])
		return box[0];
	else if (box[2] == box[4] && box[4] == box[6])
		return box[2];


	//check for tie. returns "t" to call out a tie.
	int count_taken{ 0 };
	for (int i = 0; i < 10; i++)
	{
		if (box[i] == "O" || box[i] == "X")
			count_taken++;
		else if (count_taken >= 9)
			return "t";
	}
	return "none";
}
int choose(int player) {
	int p_chose;
	std::cout << "player " << player << " choose a number [1-9]: ";
	std::cin >> p_chose;

	//checks if space is taken
	for (int a{1}; a < 10; a++)
	{
		if (p_chose == taken[a])
		{
			std::cout << "Place is taken\n";
			std::cout << "player " << player << " choose a number [1-9]: ";
			std::cin >> p_chose;
		}
	}

	taken[p_chose] = p_chose;

	if (player == 1)
	{
		box[p_chose -1] = "X";
		box_clone[p_chose - 1] = "X";
	}
	else if (player == 2)
	{
		box[p_chose - 1] = "O";
		box_clone[p_chose - 1] = "O";
	}
	
	return 0;
}

// checks if space is taken
bool check_taken(int placement) {
	for (int i = 0; i < 10; i++)
	{
		if (placement == taken[i])
		{
			return true;
			// std::cout << "place taken.";
		}
	}
	return false;
}

// computer checks if random spot is taken. if not then places an "O" in that spot.
void easy_ai_choose() {
	int placement{1};
	do
	{
		placement = rand() % 9 + 1;
		//std::cout << placement << std::endl;
	} while (check_taken(placement) == true);
	box[placement-1] = "O";
	taken[placement] = placement;
}

//checking score for spesifide input
//dont need depth
int minimax(int check_number, int depth, bool ai_turn) {
	int depth2 = depth;
	if (ai_turn == true) 
	{
		box[check_number - 1] = "O";
	}
	else
	{
		box[check_number - 1] = "X";
	}

	int score{-99};
	std::string result = check_win();
	std::cout << "check win for nr: "<<check_number << " result is : " << result << std::endl;
	box[check_number - 1] = std::to_string(check_number);
	std::cout << "dept : " << depth2 << std::endl;

	//give points depending of the next move
	if (result == "t")
	{
		return 0;
	}
	else if (result == "O")
	{
		return 10;
	}
	else if (result == "X")
	{
		return 5; 
	}
// 	else if (depth2 >= 3) 
// 	{
// 		return -1;
// 	}
 	else if (result == "none")
 	{
		return -5; 
 	}

	// maximizing player. ai
	if (ai_turn == true)
	{
		int best_score{ -99 };

		for (int i = 0; i < 9; i++)
		{
			if (check_taken(i) == false)
			{
				int score = minimax(check_number, depth2, false);
				if (score > best_score)
				{
					best_score = score;
				}
			}
		}
		std::cout << "Ai's turns \n";
		return best_score;
	}
	// minimizing player, player
 	else
 	{
 		int best_score{ 99 };
 		for (int i = 0; i < 9; i++)
 		{
 			if (check_taken(i) == false)
 			{
 				int score = minimax(check_number, depth2++, true);
 				if (score < best_score)
 				{
 					best_score = score;
 				}
 			}
 		}
		std::cout << "humans turns \n";
 		return best_score;
 	}
	return -1;
}

int best_move(bool ai_turn) {
	int best_score = -99;
	int chose;
	//looks for first available spot
	for (int i = 1; i < 10; i++)
	{
		if (check_taken(i) == false)
		{
			int score = minimax(i, 1, ai_turn);
			std::cout << "the score is: " << score << std::endl;
			if (score > best_score)
			{
				chose = i;
				best_score = score;
				
			}
		}
		std::cout << "best score is currently: " << best_score << std::endl;
	}
	return chose;
	
}
// ai places O
void bot_choose(int place) {
	box[place-1] = "O";
	taken[place] = place;
}



//draws game board
void board() {

	std::cout << "\t-------------\n";
	std::cout << "\t| " << box[0] << " | " << box[1] << " | " << box[2] << " |\n";
	std::cout << "\t-------------\n";
	std::cout << "\t| " << box[3] << " | " << box[4] << " | " << box[5] << " |\n";
	std::cout << "\t-------------\n";
	std::cout << "\t| " << box[6] << " | " << box[7] << " | " << box[8] << " |\n";
	std::cout << "\t-------------\n";
}

//compares score of human and ai spot
int compare_score(int human, int ai) {
	int human_score = minimax(human, 0, false);
	int ai_score = minimax(ai, 0, true);

	if (human_score > ai_score)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// using minimax algorithm, very hard to win
void impossible() {
	while (check_win() == "none")
	{
		
		board();
		choose(1);
		system("cls");
		
		if (check_win() == "X")
		{
			std::cout << "Very well, You are a lucky player\n";
			board();
			break;
		}
		else if (check_win() == "t")
		{
			std::cout << "Well played, its a tie!\n";
			board();
			break;
		}
		// compares score of minimax algoritm, for player and ai move
		int move;
		int ai_move = best_move(true);
		std::cout << "best ai move: " << ai_move << std::endl << std::endl;
		int human_move = best_move(false);
		std::cout << "Best human move: " << human_move << std::endl;
		if (compare_score(human_move, ai_move) == 1)
		{
			move = human_move;
		}
		else
		{
			move = ai_move;
		}
		bot_choose(move);
		system("cls");

		if (check_win() == "O")
		{
			std::cout << "You lost. but its fine. u cant win\n";
			board();
			break;
		}
		else if (check_win() == "t")
		{
			std::cout << "Well played, its a tie!\n";
			board();
			break;
		}
		
	}
}

//start game for 1 player. ai is only random
// void players_1() {
// 	while (true)
// 	{
// 		board();
// 		choose(1);
// 		system("cls");
// 		if (check_win() == "O")
// 		{
// 			std::cout << "Congrats you won!!\n";
// 			board();
// 			break;
// 		}
// 		else if (check_win() == "t")
// 		{
// 			std::cout << "Well played, its a tie!\n";
// 			board();
// 			break;
// 		}
// 		board();
// 		easy_ai_choose();
// 		system("cls");
// 		if (check_win() == "X")
// 		{
// 			std::cout << "you just got beaten by a random bot\n";
// 			board();
// 			break;
// 		}
// 		else if (check_win() == "t")
// 		{
// 			std::cout << "Well played, its a tie!\n";
// 			board();
// 			break;
// 		}
// 	}
// }
//starts game for 2 players
void players_2() {
	while (true)
	{
		board();
		choose(1);
		system("cls");
		if (check_win() == "X")
		{
			std::cout << "Congrats player 1! you won!!\n";
			board();
			break;
		}
		else if (check_win() == "t")
		{
			std::cout << "Well played, its a tie!\n";
			board();
			break;
		}
		board();
		choose(2);
		system("cls");
		if (check_win() == "O")
		{
			std::cout << "Congrats player 2! you won!!\n";
			board();
			break;
		}
		else if (check_win() == "t")
		{
			std::cout << "Well played, its a tie!\n";
			board();
			break;
		}
	}
}

int main() {
	system("cls");
	int player_count;
	char yn;
	bool new_game = true;
	std::cout << "\n***Welcome to 1 and 2 player tic tac toe***" << std::endl;
	do 
	{
		std::cout << "\nhow many players want to play? [1-2]: ";
		std::cin >> player_count;

		switch (player_count)
		{
		case 1:
			system("cls");
			clear_board();
			impossible();
			break;
		case 2:
			system("cls");
			clear_board();
			players_2();
			break;
//  		case 3: // this is for testing purposes
// 			system("cls");
// 			clear_board();
// 			impossible();
//  			break;
		default:
			break;
		}
		do
		{
			std::cout << "\nDo you want to play another one ? [y/n]: ";
			std::cin >> yn;
			if (yn == 'y' || yn == 'n')
				break;
			else
				std::cout << "that is not a valid input";
		}while (new_game == true);
		if (yn == 'n')
		break;
	} while (true);

	return 0;
}
