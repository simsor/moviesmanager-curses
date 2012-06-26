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
	noecho();

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

	int c;
	int i;
	ITEM **videos;
	MENU *menuMovies;
	ITEM *current_item;
	WINDOW *menuWin;

	videos = (ITEM**) calloc(m_videosList.size(), sizeof(ITEM*));

	for (i=0; i < m_videosList.size(); i++)
		videos[i] = new_item(m_videosList[i].c_str(), m_videosList[i].c_str());
	videos[m_videosList.size()] = (ITEM*) NULL;

	menuMovies = new_menu((ITEM**) videos);

	keypad(stdscr, TRUE);

	menuWin = newwin(WIDTH, HEIGHT, 2, 2);
	box(menuWin, 0, 0);
	set_menu_win(menuMovies, menuWin);
	//set_menu_sub(menuMovies, menuWin);
	set_menu_mark(menuMovies, "*");
	keypad(menuWin, TRUE);	

	post_menu(menuMovies);

	wrefresh(menuWin);
	refresh();

	bool draw = true;
	while (draw)
	{
		c = wgetch(menuWin);
		switch(c)
		{
			case KEY_UP:
				menu_driver(menuMovies, REQ_UP_ITEM);
				break;
			case KEY_DOWN:
				menu_driver(menuMovies, REQ_DOWN_ITEM);
				break;
			/* case 10:
				playVideo(m_videosList[highlight-1]);
				break; */
			case 27: // ESCAPE
				draw = false;
				break;

			/* default:
				mvwprintw(menuWin,20,20,"%d",c);
				break; */
		}
		//printMenu(menuWin, m_videosList, highlight);
		wrefresh(menuWin);
	}

	free_item(videos[0]);
	free_item(videos[1]);
	free_menu(menuMovies);
	delwin(menuWin);
	closedir(videos_dir);
}

void MMC::playVideo(string toBePlayed)
{
	WINDOW* playing_window = newwin(HEIGHT, WIDTH, 2, 2);
	mvwprintw(playing_window, (HEIGHT)/2, (WIDTH)/2-20, "Video playing...");
	
	wrefresh(playing_window);

	string fullPath = "";
	int lastChar = m_videosPath.at( m_videosPath.size() - 1 );

	if (lastChar == '/')
		fullPath = m_videosPath + toBePlayed;
	else
		fullPath = m_videosPath + "/" + toBePlayed;

	string cmd = "mplayer \"" + fullPath + "\" -fs -idx &> /dev/null";
	system(cmd.c_str());

	delwin(playing_window);
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
