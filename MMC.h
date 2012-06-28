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
#include <stdlib.h>

#include <ncurses.h>
#include <menu.h>

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
		void playVideo(const char*);
	
	private:
		bool m_isFirstRun;
		string m_videosPath;
		vector<string> m_videosList;

		short m_foreColor;
		short m_backColor;
};
