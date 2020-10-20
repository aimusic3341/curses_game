#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <curses.h>
using namespace std;

void initial();

void initial()
{
	initscr();
	cbreak();
	nonl();
	noecho();
	intrflush(stdscr,FALSE);
	keypad(stdscr,TRUE);
	refresh();
}

class game 
{
	public:
		void map();
		void showP();
		void start(int Pnumber,int Cnumber); 
		void PCMove(); 
		void copy();
		void eat();
		void day();
		void cDie();
		bool checkSpace(int x,int y);
	private:
		int Pmap[80][24];	 
};

void game::map()
{	
	for(int i=0;i<80;i++)
	{
		for(int j=0;j<24;j++)
		{
			Pmap[i][j]=0;
		}
	}
	
	for(int i=0;i<80;i++)
	{
		Pmap[i][0]=9;
		Pmap[i][23]=9;
	}
	
	for(int j=0;j<24;j++)
	{
		Pmap[0][j]=9;
		Pmap[79][j]=9;
	}
}
 
void game::start(int Pnumber,int Cnumber)
{
	int a,b;
	
	for(int i=0;i<Pnumber;i++)
	{
		a=rand()%80;
		b=rand()%24;
		
		while(Pmap[a][b]>0)
		{
			a=rand()%80;
			b=rand()%24;
		}
		
		Pmap[a][b]=1;
	}
	
	for(int i=0;i<Cnumber;i++)
	{
		a=rand()%80;
		b=rand()%24;
		
		while(Pmap[a][b]>0)
		{
			a=rand()%80;
			b=rand()%24;
		}
		
		Pmap[a][b]=101;
	}
}

void game::day()
{
	for(int i=0;i<80;i++)
	{
		for(int j=0;j<24;j++)
		{
			if(Pmap[i][j]>=1&&Pmap[i][j]<=4)
			{
				Pmap[i][j]+=1;
			}
			
			if(Pmap[i][j]>=101&&Pmap[i][j]<=103)
			{
				Pmap[i][j]+=1;
			}
		}
	}
}

void game::showP()
{
	for(int i=0;i<80;i++)
	{
		for(int j=0;j<24;j++)
		{
			if(Pmap[i][j]==0)
			{
				move(j,i);
				addch(' ');
			}
			
			if(Pmap[i][j]>0&&Pmap[i][j]<4)
			{
				move(j,i);
				addch('*');
			}
			
			if(Pmap[i][j]>100&&Pmap[i][j]<104)
			{
				move(j,i);
				addch('C');
			}
			
			if(Pmap[i][j]==9)
			{
				move(j,i);
				addch('#');
			}
		}
	}
}

void game::PCMove()
{
	for(int i=0;i<80;i++)
	{
		for(int j=0;j<24;j++)
		{
			int tmp = Pmap[i][j];
			
			if(Pmap[i][j]>0 && checkSpace(i,j)==1 && Pmap[i][j]!=9)  
			{
				int m,n;
				
				m=rand()%3-1;
				n=rand()%3-1;
				
				while(Pmap[i+m][j+n]>0)
				{
					m=rand()%3-1;
					n=rand()%3-1;
				}
				
				Pmap[i+m][j+n]=tmp;
				Pmap[i][j]=0;
			}
		}	
	}
}

void game::copy()
{	
	for(int i=0;i<80;i++)
	{
		for(int j=0;j<24;j++)
		{
			if(Pmap[i][j]==4)
			{
				if(checkSpace(i,j)==1)
				{
					int k,l;
					
					k=rand()%3-1;
					l=rand()%3-1;
					
					while(Pmap[i+k][j+l]>0)
					{
						k=rand()%3-1;
						l=rand()%3-1;
					}
					
					Pmap[i+k][j+l]=1;
				}
				
				Pmap[i][j]=1;
			}		
		}	
	}
}

void game::eat()
{
	bool out=0;
	
	for(int i=0;i<80;i++)
	{
		for(int j=0;j<24;j++)
		{
			if(Pmap[i][j]>100&&Pmap[i][j]<104)
			{
				for(int k=-1;k<2;k++)
				{
					for(int l=-1;l<2;l++)
					{
						if(i+k>0 && i+k <80 && j+l>0 && j+l<24)
						{
							if(Pmap[i+k][j+l]>0&&Pmap[i+k][j+l]<5)
							{
								Pmap[i][j]=101;
								Pmap[i+k][j+l]=101;
							
								out=1;
							}
						}
						
						if(out==1)
						{
							break;
						}	
					}
					
					if(out==1)
					{
						out=0;
						break;
					}
				}
			}
		}
	}
}

void game::cDie()
{
	for(int i=0;i<80;i++)
	{
		for(int j=0;j<24;j++)
		{
			if(Pmap[i][j]==104)
			{
				Pmap[i][j]=0;
			}
		}
	}
}

bool game::checkSpace(int x, int y)
{
	for(int i=-1;i<2;i++)
	{
		for(int j=-1;j<2;j++)
		{
			if(Pmap[x+i][y+j]==0)
			{
				return true;
			}
		}
	}
	 
	return false;
}

int main()
{
	srand(time(0));
	

	int a,b;
	
	cout<<"Please enter the number of producer:";
	cin>>a;
	
	cout<<"Please enter the number of consumer:";
	cin>>b;
	
	initial();
	
	game game;
	
	game.map();  
	game.start(a,b);  
	
	while(1)
	{
		game.showP();   
		game.PCMove(); 
		game.eat();  
		game.day();   
		game.copy();  
		game.cDie();  
		refresh();  
		Sleep(200); 
	}
	
	getch();
	endwin();
}
