#include <iostream>
#include <array>
#include <string>
#include <stdlib.h>

//global vars
//tried char. didn't like it, string works fine.
std::string box[] = { "1","2","3","4","5","6","7","8","9" };
int taken[10];

void clear_board() {
	// clears box array to "default" [1-9]
	for (int k = 0; k < _countof(box); k++)
	{
		std::string pls_work = std::to_string(k+1);
		box[k] = pls_work;
	}
	//clears taken
	for (int j = 0; j < _countof(taken); j++)
		taken[j] = 0;
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
	std::cout << "\n\n player " << player << " choose a number [1-9]: ";
	std::cin >> p_chose;

	//checks if space is taken
	for (int a{1}; a < _countof(taken); a++)
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
		box[p_chose -1] = "X";
	else if (player == 2)
		box[p_chose - 1] = "O";
	return 0;
}

// checks if space is taken
bool check_taken(int placement) {
	for (int i = 0; i < _countof(taken); i++)
	{
		if (placement == taken[i])
			return true;
	}
	return false;
}

//checking score for specific input
int minimax(int check_number, bool ai_turn) {
	//places O or X depends on whos turn
	if (ai_turn == true) 
		box[check_number - 1] = "O";
	else
		box[check_number - 1] = "X";
	//checks for win then removes the last placed X or O
	int score{-99};
	std::string result = check_win();
	box[check_number - 1] = std::to_string(check_number);

	//give points depending of the next move
	if (result == "t")
		return 0;
	else if (result == "O")
		return 10;
	else if (result == "X")
		return 5; 
 	else if (result == "none")
		return -5; 

	// maximizing player. ai score
	if (ai_turn == true)
	{
		int best_score{ -99 };

		for (int i = 0; i < 9; i++)
		{
			if (check_taken(i) == false)
			{
				int score = minimax(check_number, false);
				if (score > best_score)
					best_score = score;
			}
		}
		return best_score;
	}
	// minimizing player, players score
 	else
 	{
 		int best_score{ 99 };
 		for (int i = 0; i < 9; i++)
 		{
 			if (check_taken(i) == false)
 			{
 				int score = minimax(check_number, true);
 				if (score < best_score)
 					best_score = score;
 			}
 		}
 		return best_score;
 	}
	return -1;
}

int best_move(bool ai_turn) {
	int best_score = -99; // a tiny number
	int chose;
	//looks for first available spot
	for (int i = 1; i < 10; i++)
	{
		if (check_taken(i) == false)
		{
			int score = minimax(i, ai_turn);
			std::cout << "the score is: " << score << std::endl;
			if (score > best_score)
			{
				chose = i;
				best_score = score;
			}
		}
	}
	return chose;
}
// ai places O
void bot_choose(int place) {
	box[place-1] = "O";
	taken[place] = place;
}



//draws game board
void board() 
{
	for (int i = 0; i < _countof(box) ; i++)
	{
		if (i % 3 == 0)
			std::cout << "\n\t-------------\n\t| ";
		std::cout << box[i] << " | ";
	}
	std::cout << "\n\t-------------\n";
}

//compares score of human and ai choses
int compare_score(int human, int ai) {
	int human_score = minimax(human, false);
	int ai_score = minimax(ai, true);

	if (human_score > ai_score)
		return 1;
	else
		return 0;
}

// using minimax algorithm, very hard to win
void impossible()
{
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
		// compares score of minimax algorithm, for player and ai move
		int move;
		int ai_move = best_move(true);
		std::cout << "best ai move: " << ai_move << std::endl << std::endl;
		int human_move = best_move(false);
		std::cout << "Best human move: " << human_move << std::endl;
		if (compare_score(human_move, ai_move) == 1)
			move = human_move;
		else
			move = ai_move;
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
