/*
 * =====================================================================================
 *
 *       Filename:  MMC.h
 *
 *    Description:  Header for the main class
 *
 *        Version:  1.0
 *        Created:  11/06/2012 21:25:32
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Simon Garrelou (Simsor), 
 *
 * =====================================================================================
 */
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>
#include <fstream>
#include <dirent.h>

using namespace std;
class MMC
{
	public:
		MMC();
		~MMC();
		void drawMainInterface();
		void loadConfig();
		void firstRun();
		void printMenu(WINDOW*, vector<string>, int);
		void playVideo(string);
	
	private:
		bool m_isFirstRun;
		string m_videosPath;
		vector<string> m_videosList;
};
