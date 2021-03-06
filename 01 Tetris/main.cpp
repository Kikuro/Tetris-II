#include <SFML/Graphics.hpp>
#include <time.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>

using namespace sf;
using namespace std;

const int M = 20;
const int N = 10;

int field[M][N] = {0};

struct Point
{int x,y;} a[4], b[4];

int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

bool check()
{
   for (int i=0;i<4;i++)
	  if (a[i].x<0 || a[i].x>=N || a[i].y>=M) return 0;
      else if (field[a[i].y][a[i].x]) return 0;

   return 1;
};

long double inFile()
{
    fstream plik;
    plik.open("score.KRap", ios::in);
    string dane;
    if(plik.good() == true)
    {
        getline(plik,dane);
        long double highScore = atof(dane.c_str());
        return highScore;
    }
    else
        return 0;
    plik.close();
}

void outFile(long double score)
{
    fstream plik;
    plik.open("score.KRap", ios::out);
    plik<<score;
    plik.close();
}


int main()
{
    srand(time(0));

    Font f1;
    f1.loadFromFile("arial.ttf");
    Text text;
    text.setFont(f1);
    text.setFillColor(Color::Black);

    long double highScore = inFile();
    long double score = 0;

	RenderWindow window(VideoMode(320, 480), "The Game!");

    Texture t1,t2,t3;
	t1.loadFromFile("images/tiles.png");
	t2.loadFromFile("images/background.png");
	t3.loadFromFile("images/frame.png");

	Sprite s(t1), background(t2), frame(t3);

    int dx=0; bool rotate=0; int colorNum=1;
	float timer=0,delay=0.3;

	Clock clock;

    while (window.isOpen())
    {
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer+=time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

			if (e.type == Event::KeyPressed)
			  if (e.key.code==Keyboard::Up) rotate=true;
			  else if (e.key.code==Keyboard::Left) dx=-1;
			  else if (e.key.code==Keyboard::Right) dx=1;
		}

	if (Keyboard::isKeyPressed(Keyboard::Down)) delay=0.05;

	//// <- Move -> ///
	for (int i=0;i<4;i++)  { b[i]=a[i]; a[i].x+=dx; }
    if (!check()) for (int i=0;i<4;i++) a[i]=b[i];

	//////Rotate//////
	if (rotate)
	  {
		Point p = a[1]; //center of rotation
		for (int i=0;i<4;i++)
		  {
			int x = a[i].y-p.y;
			int y = a[i].x-p.x;
			a[i].x = p.x - x;
			a[i].y = p.y + y;
	 	  }
   	    if (!check()) for (int i=0;i<4;i++) a[i]=b[i];
	  }

	///////Tick//////
	if (timer>delay)
	  {
	    for (int i=0;i<4;i++) { b[i]=a[i]; a[i].y+=1; }

		if (!check())
		{
		 for (int i=0;i<4;i++) field[b[i].y][b[i].x]=colorNum;

		 colorNum=1+rand()%7;
		 int n=rand()%7;
		 for (int i=0;i<4;i++)
		   {
		    a[i].x = figures[n][i] % 2;
		    a[i].y = figures[n][i] / 2;
		   }
		}

	  	timer=0;
	  }

	///////check lines//////////
    int k=M-1;
	for (int i=M-1;i>0;i--)
	{
		int count=0;
		for (int j=0;j<N;j++)
		{
		    if (field[i][j]) count++;
		    field[k][j]=field[i][j];
		}
		if (count<N) k--;
		if(count>=N) score+=10;
	}

    dx=0; rotate=0; delay=0.3;

    /////////game over//////////
    for(int i=N-1; i>0;i--)
    {
        if(field[1][i])
        {
            if(score>=highScore)
                outFile(score);
            Sleep(5000);
            window.close();
        }
    }

    /////////draw//////////
    window.clear(Color::White);
    window.draw(background);

    for(int i=0;i<4;i++)
    {
        if(i==0)
        {
            text.setString("Highscore:");
            text.setCharacterSize(13);
            text.setPosition(235.f,15.f);
            text.setStyle(Text::Bold);
        }
        if(i==1)
        {
            unsigned int hScore = static_cast<unsigned int>(highScore);
            string _str = to_string(hScore);
            text.setString(_str);
            text.setCharacterSize(12);
            text.setPosition(235.f,30.f);
        }
        if(i==2)
        {
            text.setString("Score:");
            text.setCharacterSize(13);
            text.setPosition(235.f,45.f);
            text.setStyle(Text::Bold);
        }
        if(i==3)
        {
            unsigned int Score = static_cast<unsigned int>(score);
            string _str = to_string(Score);
            text.setString(_str);
            text.setCharacterSize(12);
            text.setPosition(235.f,60.f);
        }
        window.draw(text);
    }

	for (int i=0;i<M;i++)
	 for (int j=0;j<N;j++)
	   {
         if (field[i][j]==0) continue;
		 s.setTextureRect(IntRect(field[i][j]*18,0,18,18));
		 s.setPosition(j*18,i*18);
		 s.move(28,31); //offset
		 window.draw(s);
	   }

	for (int i=0;i<4;i++)
	  {
		s.setTextureRect(IntRect(colorNum*18,0,18,18));
		s.setPosition(a[i].x*18,a[i].y*18);
		s.move(28,31); //offset
		window.draw(s);
	  }

	window.draw(frame);
 	window.display();
	}

    return 0;
}
