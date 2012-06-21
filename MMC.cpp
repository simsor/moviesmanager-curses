/*
 * =====================================================================================
 *
 *       Filename:  MMC.cpp
 *
 *    Description:  Implementation of MMC.h functions
 *
 *        Version:  1.0
 *        Created:  11/06/2012 21:28:35
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Simon Garrelou (Simsor) 
 *
 * =====================================================================================
 */
#include "MMC.h"

#define WIDTH COLS-4
#define HEIGHT LINES-4

MMC::MMC()
{
	m_isFirstRun = false;
	m_videosPath = "";

	initscr();
	cbreak();
	
	box(stdscr,0,0);

	loadConfig();

	if (m_isFirstRun)
		firstRun();
	else
		drawMainInterface();
}

MMC::~MMC()
{
	endwin();
}

void MMC::loadConfig()
{
	ifstream config_file;
	string current_line;
	config_file.open("config", ios::in);

	if (config_file.is_open())
	{
		while (!config_file.eof())
		{
			getline(config_file, current_line);
			if (current_line == "[path]")
				getline(config_file, m_videosPath);
		}
	}
	else
		m_isFirstRun = true; // file.open() will fail if the file doesn't exist

	config_file.close();
}

void MMC::drawMainInterface()
{
	DIR *videos_dir = opendir(m_videosPath.c_str());
	if (videos_dir == NULL)
	{
		printw("ERROR: Can't open videos directory!");
		getch();
		return;
	}

	struct dirent *entry = NULL;
	string current_video;
	do
	{
		entry = readdir(videos_dir);
		if (entry != NULL)
		{
			if (entry->d_name != "." && entry->d_name != "..")
			{
				current_video.assign(entry->d_name);
				m_videosList.push_back(current_video);
		
			}
		}

	} while (entry != NULL);

	int highlight = 1;
	int choice = 0;
	int c;

	int startx = 2;
	int starty = 2;

	WINDOW *menuWin;

	menuWin = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menuWin, TRUE);

	refresh();

	printMenu(menuWin, m_videosList, highlight);

	while (1)
	{
		c = wgetch(menuWin);
		switch(c)
		{
			case KEY_UP:
				if (highlight == 1)
					highlight = m_videosList.size()-1;
				else
					highlight--;
				break;
			case KEY_DOWN:
				if (highlight == m_videosList.size()-1)
					highlight = 1;
				else
					highlight++;
				break;
			case 10:
				playVideo(m_videosList[highlight-1]);
				break;
		}
		printMenu(menuWin, m_videosList, highlight);
	}
	getch();

	closedir(videos_dir);
}

void MMC::playVideo(string toBePlayed)
{
	string fullPath = "";
	if (m_videosPath[m_videosPath.size()-1] == "/")
		fullPath = m_videosPath + toBePlayed;
	else
		fullPath = m_videosPath + "/" + toBePlayed;

	string cmd = "mplayer " + fullPath + " -fs -idx";
	system(cmd.c_str());
}

void MMC::firstRun()
{
	mvprintw(5,(COLS/2)-53,"It appears it is your first time running MMC. Please take some time to configure it.");
	mvprintw(7,(COLS/2)-53,"Please enter the location of your video files: ");
	char path[255];
	getstr(path);

	m_videosPath.assign(path);

	ofstream config_file("config");
	config_file << "[path]\n";
	config_file << m_videosPath;

	config_file.close();
	refresh();
	
	drawMainInterface();
}

void MMC::printMenu(WINDOW *menu_win, vector<string> menuList, int highlight)
{
	int x, y, i;
	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	
	if (has_colors() != FALSE)
	{
		start_color();
		init_pair(1, COLOR_YELLOW, COLOR_BLACK);
		wattron(menu_win, COLOR_PAIR(1)); 
	}

	for (i = 0; i < menuList.size() - 1; i++)
	{
		if (highlight == i + 1)
		{
			wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", menuList[i].c_str());
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", menuList[i].c_str());
		y++;
	}

	if (has_colors() != FALSE)
		wattroff(menu_win, COLOR_PAIR(1));
}
