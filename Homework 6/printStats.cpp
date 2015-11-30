/*============================================================
DESCRIPTION:		This file defines printStats(). It
                     calculates and prints a game's
                     win-loss-tie statistics,
                     based on 3 integers.
NOTE:               Arbitrary max size limit is 10-digit numbers.
                    If the game is played 10^9 times, the
                     statistics formatting will suffer.
============================================================*/
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;


void printStats(int won, int lost, int tied) {
	/*
	Function Inputs:
		integer won: Number of games that the player has won
		integer lost: Number of games that the player has won
		integer tied: Number of games that were a tie

	Function Output:
		prints statistics along with percentages of games
		 won/lost/tied based on total games played

	Procedure:
		This function takes the games won, lost, and tied and
		 computes the total games played. It then displays
		 statistics based on these numbers.
	*/
	// Width of statistic labels
	int const W_LABEL = 16;
    int const W_COUNT = 10;
	// Max length of percentage.
    // Examples x3: '100.00', '12.34', 0.81'
	int const W_PERCENT = 6;

	// Statistics from 3 provided integers
	int const totalGames = won + lost + tied;
	float const winPercentage = (won / (float)totalGames) * 100;
	float const lossPercentage = (lost / (float)totalGames) * 100;
	float const tiePercentage = (tied / (float)totalGames) * 100;

	// Display total games played
	cout << setw(W_LABEL) << left << "Total battles:";
    cout << setw(W_COUNT) << right << totalGames << endl;

	// Display games won and win %
	cout << setw(W_LABEL) << left << "You won:";
    cout << setw(W_COUNT) << right << won;
	cout << "  ( ";
	cout << fixed << setw(W_PERCENT) << setprecision(2) << winPercentage;
	cout << " %)" << endl;

	// Display games lost and loss %
	cout << setw(W_LABEL) << left << "Opponent won:";
    cout << setw(W_COUNT) << right << lost;
	cout << "  ( ";
	cout << setw(W_PERCENT) << setprecision(2) << lossPercentage;
	cout << " %)" << endl;

	// Display games tied and tie %
	cout << setw(W_LABEL) << left << "Tie:";
    cout << setw(W_COUNT) << right << tied;
	cout << "  ( ";
	cout << setw(W_PERCENT) << setprecision(2) << tiePercentage;
	cout << " %)" << endl;

	return;
}
