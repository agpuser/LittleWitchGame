/* ********************************************************************************* *
 * *                  SIT153 - Introduction to Game Development                    * *
 * *                Assignment #2 - Develop a Classic Arcade Game                  * *
 * *                                                                               * *
 * *  This code is provided to students as a basic framework for the purposes of   * *
 * *  developing a game that meets the requirements of the assessment guidelines.  * *
 * *  This code base does not provide all features necessary to implement such a   * *
 * *  game. Students must modify and extend this framework to achieve the          * *
 * *  specific functionality required of their game.                               * *
 * *                                                                               * *
 * *  The code is provided as-is and while significant efforts have been made to   * *
 * *  ensure that it is bug-free, this is not a guarantee. It is each student's    * *
 * *  responsibility to ensure that their final submission compiles and executes   * *
 * *  without error.                                                               * *
 * *                                                                               * *
 * *  COPYRIGHT NOTICE                                                             * *
 * *  ----------------                                                             * *
 * *  (C) [2012] - [2017] Deakin University                                        * *
 * *  All rights reserved.                                                         * *
 * *  All information contained herein is, and remains the property of Deakin      * *
 * *  University and the authors (Tim Wilkin, Michael Hobbs and Shaun Bangay).     * *
 * *  Dissemination of this information or reproduction of this material is        * *
 * *  strictly forbidden unless prior written permission is obtained from Deakin   * *
 * *  University. The right to create derivative works from this material is       * *
 * *  hereby granted to students enrolled in SIT153, but only for the purposes of  * *
 * *  assessment while an enrolled student at Deakin University.                   * *
 * *                                                                               * *
 * *  Please complete the following information as acknowledgement of the          * *
 * *  above conditions regarding the use of this code. Failure to complete this    * *
 * *  acknowledgement will mean that your assignment submission will not be marked * *
 * *                                                                               * *
 * ********************************************************************************* *
 * *  STUDENT ID:                                                                  * *
 * *  STUDENT NAME:                                                                * *
 * *  DATE:                                                                        * *
 * ********************************************************************************* *
 */

/*****************************************************************************************
 * Demonstration of usage of the WinCanvas class for a Windows console application - to  *
 * support basic '2D graphics' through simplified API surrounding the Windows GDI system.*
 *                                                                                       *
 * This code is based on the code developed by Shaun Bangay (2012) & Tim Wilkin (2013).  *
 * Extended by Michael Hobbs and Tim Wilkin (2014) and re-written by Tim Wilkin (2015).  *
 *                                                                                       *
 *****************************************************************************************/

 /*
 SIT153 Introduction to Programming
 Assignment 2 - Arcade Game
 Aaron Pethybridge
 Student#: 217561644

 Main() program file
 */

#include <iostream>
#include <fstream>
#include <string>

#include <time.h>
#include <vector>

#include "WinCanvas.h"
#include "WinTexture.h"
#include "DerivedClasses.h"
#include "InputState.h"
#include "Scene.h"
#include "WitchGame.h"
#include "TextureManager.h"

// Constant values used in the code - this is the whole window size including
// menu buttons and border - actual graphics canvas size is smaller!
const unsigned int	WINDOW_WIDTH = 810;
const unsigned int	WINDOW_HEIGHT= 640;

// The main function of the program
int main()
{
	// Declare which object from <iostream> and <string> the program will use
	using std::cout;
	using std::cin;
	using std::endl;
	using std::string;

	/********************  DO NOT EDIT THIS CODE *****************************
     *
	 *	Declare a WinCanvas object. We wouldn't normally create an object this way as
	 *  it is non-portable, but this is a very simple 'graphics canvas' interface to
	 *  the Windows Graphical Device Interface (GDI) API.
	 */

	WinCanvas wc(WINDOW_WIDTH, WINDOW_HEIGHT, L"Class Arcade Game");

	/*
	 *  An InputState object provides an interface to the Keyboard and Mouse devices. 
	 */
	
	InputState	  ui;
	OpenUIHandler(ui);	// Initialise the input interface

	/*********************END RESTRICTED CODE *********************************
	 **************************************************************************/

	/*
	 *  Console Splash Screen
	 */

	cout << "  SIT153 Project - 2D Game Framework" << endl;
	cout << "=======================================" << endl << endl;
	
	WitchGame game; // Instantiate WitchGame object
	game.runGame(wc, ui); // use WitchGame object to run game

	cout << "Game finished. Now exiting...." << endl;

	return 0;
}
