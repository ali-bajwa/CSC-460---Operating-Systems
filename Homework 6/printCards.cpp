/*============================================================
DESCRIPTION:		This file defines the class printCardClass
					 and function printCards().
                    printCardClass:
                        Holds four card dimensions,
                         accessed by four get-functions.
                        Has four more functions to output the
                         card's ten rows.
                    startPrintCards() is used to print two given
                     cards. It also requires a printCardClass
                     pointer so it can call printTwoCards()
                     within printCardClass.
============================================================*/
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;


class printCardClass {
	private:
		////// Variables x4. Determines printed card size.
		// Card Size _ Center. Room for a suit.
		static int const CS_CENTER = 7;
		// Card Size _ Side. Includes border.
		static int const CS_SIDE = 4;
		// Card Size _ Width. Includes borders.
		static int const CS_WIDTH = 2 * CS_SIDE + CS_CENTER;
		// Space between the cards. Excludes borders.
		static int const SPACE_BETWEEN = 10;

	public:
        // Get functions x4.
		static int getBET() {return SPACE_BETWEEN; }
		static int getCEN() {return CS_CENTER; }
		static int getSID() {return CS_SIDE; }
		static int getWID() {return CS_WIDTH; }

		static void printCardRow(
				string pc1="", string pc2="", string pc3="",
				string oc1="", string oc2="", string oc3="") {
			// Input:	3 strings for each card's contents.
			// Output:	Prints one row of two cards.
			printOneCard(pc1, pc2, pc3);
			cout << setw(getBET()) << "";
			printOneCard(oc1, oc2, oc3);
			cout << endl;
		}

		static void printHorizonBorder() {
			// Print horizontal border of 2 cards.
			cout << setw(1) << left << " ";
			cout << setw(getWID() - 2) << setfill('-') << "";
			cout << setw(getBET() + 2) << setfill(' ') << "";
			cout << setw(getWID() - 2) << setfill('-') << "" << endl;
		}

		static void printOneCard(
				string s1="", string s2="", string s3="") {
			// Print one row of one card. No 'endl' at end.
			cout << setw(1) << left << "|";
			cout << setw(getSID() - 1) << setfill(' ') << left << s1;
			cout << setw(getCEN()) << s2;
			cout << setw(getSID() - 1) << right << s3;
			cout << setw(1) << left << "|";
		}

		static void printTwoCards(
				string playerCVal, string playerCSuit,
				string oppCVal,    string oppCSuit) {
			/*
			Function Inputs:
				string playerCVal:	Player Card Value.
				string oppCVal:		Opponent Card Value.
				string playerCSuit:	Player Card Suit.
				string oppCSuit:	Opponent Card Suit.

			Function Output:
				Two cards. Player Card is on the left,
				 Opponent card is on the right.

			Procedure:
				Print two cards in 10 rows.
			*/

			// ROW 1: Label cards by owner.
			cout << setw(getWID()) << setfill(' ')
				 << left << " Your Card";
			cout << setw(getBET()) << "";
			cout << setw(getWID()) << " Opponent's Card" << endl;
			// ROW 2: Horizontal border.
			printHorizonBorder();
			// ROW 3: Include value in top-left corner.
			printCardRow(playerCVal, "", "", oppCVal);
			// ROWS 4 to 8: 2 blank sections, 1 suit, 2 blank sections.
			printCardRow();
			printCardRow();
			printCardRow("", playerCSuit, "", "", oppCSuit);
			printCardRow();
			printCardRow();
			// ROW 9: Include value in bottom-right corner.
			printCardRow("", "", playerCVal, "", "", oppCVal);
			// ROW 10: Horizontal border.
			printHorizonBorder();
			cout << setfill(' ') << endl;
		}
};

void startPrintCards(printCardClass* myPCCPointer,
					 int const playerCard, int const oppCard) {
	/*
	Function Inputs:
		myPCCPointer: Pointer to printCardClass. Used to access
		 printTwoCards() from printCardClass.
		integer playerCard: A number ranging between 0 and 51
		 to determine the player's card
		integer oppCard: A number ranging between 0 and 51
		 to determine the opponent's card

	Function Output:
		Prints two cards, horizontally-level to each other.

	Procedure:
		This function takes two card values and recognizes their
		 name/suit and value. Then it uses myPCCPointer to call
		 printTwoCards() from the class 'printCardClass', which
		 will print those two cards.
	*/

	// Array of suits. Found by division. (Length=7) strings.
	string A_SUIT[] = {" HEART ", " CLUB  ", " SPADE ", "DIAMOND"};
	// Array of values. Found by modulo.
	string A_VALUES[] = {"2", "3", "4", "5",  "6",
						 "7", "8", "9", "10", "J",
						 "Q", "K", "A"};
	// Size of A_VALUES.
	int const COUNT_VALUES = 13;
	int suitNumber, valueNumber;

	////// Variables made from 'playerCard'
	suitNumber  = playerCard / COUNT_VALUES;
	valueNumber = playerCard % COUNT_VALUES;
	string const playerCSuit = A_SUIT[suitNumber];
	string const playerCVal  = A_VALUES[valueNumber];

	////// Variables made from 'oppCard'
	suitNumber  = oppCard / COUNT_VALUES;
	valueNumber = oppCard % COUNT_VALUES;
	string const oppCSuit = A_SUIT[suitNumber];
	string const oppCVal  = A_VALUES[valueNumber];

	// Print two cards in 10 rows.
	myPCCPointer->printTwoCards(playerCVal, playerCSuit,
								 oppCVal,    oppCSuit);
}
