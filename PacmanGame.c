#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <windows.h> /*for windows environment*/
#include <time.h>
#include <conio.h>
#define true 1
#define false 0
#define  BOARDx  28
#define  BOARDy  36 
#define Mx  (BOARDx-1)/2
#define My  (BOARDy-1)/2
#define  MoveSpeed  200
#define  LEFT  1
#define  RIGHT  2
#define  UP  3
#define  DOWN  4
#define infinity BOARDx*BOARDy+1
enum
{
  ARROW_UP    = 256 + 72,
  ARROW_DOWN  = 256 + 80,
  ARROW_LEFT  = 256 + 75,
  ARROW_RIGHT = 256 + 77
};
struct Player
{
    int x, y;
  
} player;
struct Ghost
{
    int x, y, direction;
    char c;
    int inx;  
}ghost[4];

struct Score{
	int score;
	int energy;
}s1;

const char EMPTY = ' ';
const char PLAYER = '@';
const char GHOST = 'G';
const char DOT ='.';
const char BIG_DOT ='o';
const char WALL = '#';
int isValidPos(int x, int y)
{   int r=false;
	if(x >= 0 && x < BOARDx-1 && y >= 0 && y < BOARDy-1)
	r=true;
   return r;
}
int minimum(int a,int b,int c,int d){
	int r;
	if(a<=b&&a<=c&&a<=d)
	  r=a;
	else if(b<=a&&b<=c&&b<=d)
	r=b;
	else if(c<=a&&c<=b&&c<=d)
	r=c;
	else if(d<=a&&d<=b&&d<=c)
	r=d;
	return r;  
}

int movePlayer(struct Player *player, int x, int y,struct Score *s1,char**map,
                         int*check)
{
    if (!isValidPos(x, y))
    {
        return false;
    }
    char ch = map[x][y];
  if(ch !=EMPTY&&ch!=DOT&&ch!=BIG_DOT)
    {  
	    if(ch==GHOST){
		s1->energy-=1;
		*check=1;
	    }  
        return false;
    }
  if (isValidPos(player->x, player->y))
    {
        map[player->x][player->y] = EMPTY;
    }
    player->x = x; player->y = y;
    map[player->x][player->y] = PLAYER;
    if(ch==DOT)
    s1->score+=1;
    if(ch==BIG_DOT){
    if(s1->energy<=8)
	 s1->energy+=2;
	 else
	 s1->energy=10;	
	}
    
    return true;
}
int moveGhost(struct Ghost *ghost, int x, int y,struct Score *s1,char**map,int *check)
{
    if (!isValidPos(x, y))
    {
        return false;
   }
 char ch = map[x][y];
  if (ch != EMPTY&&ch!=DOT&&ch!=BIG_DOT)
    {		
		if(ch==PLAYER){
        s1->energy-=1;
        *check=1;
       }
       else if(ch==WALL){
       	if(ghost->direction==LEFT)
	      ghost->direction=RIGHT;
	    else if(ghost->direction==UP)
		ghost->direction=DOWN;
		else if(ghost->direction==DOWN)
		   ghost->direction=UP;  
	    else if(ghost->direction==RIGHT)
	      ghost->direction=LEFT;
	   }
        
		return false;
    }
  if (isValidPos(ghost->x, ghost->y))
    {
      map[ghost->x][ghost->y] = ghost->c;
    }
    ghost->c=ch;
    ghost->x = x; ghost->y = y;
    map[ghost->x][ghost->y] = GHOST;
    return true;
}
void find_all_path(int ux,int uy,int px,int py,
                int **visited,
				int *path_index,
				char **map,int *min_path)
{
	int i,j;
	visited[ux][uy] = true; 
    (*path_index)++;	
    if (ux==px&&uy==py)
    {
            
         if((*path_index-1)<*min_path)
		 {
          *min_path=*path_index-1;
         }  
            
    }  
    else 
    {
        int a=infinity,b=infinity,c=infinity,d=infinity;
        if(isValidPos(ux-1,uy)&&visited[ux-1][uy]!=true&&map[ux-1][uy]!=WALL)
        a=abs(ux-1-px)+abs(uy-py);
        if(isValidPos(ux,uy-1)&&visited[ux][uy-1]!=true&&map[ux][uy-1]!=WALL)
        b=abs(ux-px)+abs(uy-1-py);
        if(isValidPos(ux+1,uy)&&visited[ux+1][uy]!=true&&map[ux+1][uy]!=WALL)
        c=abs(ux+1-px)+abs(uy-py);
        if(isValidPos(ux,uy+1)&&visited[ux][uy+1]!=true&&map[ux][uy+1]!=WALL)
        d=abs(ux-px)+abs(uy+1-py);
        int m= minimum(a,b,c,d);
	    if(m==a&&isValidPos(ux-1,uy)&&visited[ux-1][uy]!=true&&map[ux-1][uy]!=WALL)
		  find_all_path(ux-1, uy,px,py,visited, path_index,map,min_path);	
	   else if(m==b&&isValidPos(ux,uy-1)&&visited[ux][uy-1]!=true&&map[ux][uy-1]!=WALL)
		  find_all_path(ux, uy-1,px,py,visited, path_index,map,min_path);
	   else	if(m==c&&isValidPos(ux+1,uy)&&visited[ux+1][uy]!=true&&map[ux+1][uy]!=WALL)
		  find_all_path(ux+1, uy,px,py,visited, path_index,map,min_path);
	   else if(m==d&&isValidPos(ux,uy+1)&&visited[ux][uy+1]!=true&&map[ux][uy+1]!=WALL)	  
	   find_all_path(ux, uy+1,px,py,visited, path_index,map,min_path);	
	} 
}
void init_dfs(int gx,int gy,int px, int py,
       char **map,int *min_path){
	int i =0,j=0; 
     int **visited = (int**) malloc((BOARDx)*sizeof(int*));
     for (i =0 ; i < BOARDx; i++)
     visited[i] = malloc((BOARDy) * sizeof(int)); 
        int path_index = 0; 
    for (i = 0; i < BOARDx; i++)
       for(j=0;j<BOARDy;j++)
        visited[i][j] = false;
    find_all_path(gx, gy,px,py,visited, &path_index,map,min_path);
   for(i=0;i<BOARDx;i++)
    	    free(visited[i]);    
    	    free(visited);                              
} /* initial_dfs() */
int minimum_path(int gx,int gy,int px, int py,
       char **map)
{
int min_path=infinity;  
init_dfs(gx,gy,px,py,map,&min_path);
/*printf("min path= %d",min_path);*/
return min_path;
}

void GhostLogic(struct Ghost *ghost, struct Player *player,char**map)
{
	int i=0,a=infinity,b=infinity,c=infinity,d=infinity;
   if(ghost->inx==0){
   	int dx =ghost->x-player->x;
   	int dy=ghost->y-player->y;
   		if(dx>0&&ghost->direction!=DOWN) 
   		ghost->direction=UP;
	   else if(dx<0&&ghost->direction!=UP)  
	   	ghost->direction=DOWN;
	   	else if(dy>0&&ghost->direction!=RIGHT) 
   		ghost->direction=LEFT;
	   else if(dy<0&&ghost->direction!=LEFT)  
	   	ghost->direction=RIGHT;
	  return; 
   }
   else if(ghost->inx==3){
	   a=rand()%4;
	   if(a==RIGHT&&ghost->direction!=LEFT)
	   	ghost->direction=RIGHT;
	    else if(a==DOWN&&ghost->direction!=UP)
	   	ghost->direction=DOWN;
	   	else if(a==UP&&ghost->direction!=DOWN)
	   	ghost->direction=UP;
	   	else if(a==LEFT&&ghost->direction!=RIGHT)
	   	ghost->direction=LEFT;
	   	return;
   }
   else if(ghost->inx==1){
   	if(isValidPos(ghost->x-1,ghost->y)&&map[ghost->x-1][ghost->y]!=WALL
	      &&ghost->direction!=DOWN)
   	a=minimum_path(ghost->x-1,ghost->y,player->x,player->y,map);
   	if(isValidPos(ghost->x,ghost->y-1)&&map[ghost->x][ghost->y-1]!=WALL
	               &&ghost->direction!=RIGHT)
    b=minimum_path(ghost->x,ghost->y-1,player->x,player->y,map);
    if(isValidPos(ghost->x+1,ghost->y)&&map[ghost->x+1][ghost->y]!=WALL
	                &&ghost->direction!=UP)
   	c=minimum_path(ghost->x+1,ghost->y,player->x,player->y,map); 
   	if(isValidPos(ghost->x,ghost->y+1)&&map[ghost->x][ghost->y+1]!=WALL
	              &&ghost->direction!=LEFT)
   	d=minimum_path(ghost->x,ghost->y+1,player->x,player->y,map); 
   }
  else if(ghost->inx==2){
   	if(isValidPos(ghost->x-1,ghost->y)&&map[ghost->x-1][ghost->y]!=WALL)
   	a=abs(ghost->x-1-player->x)+abs(ghost->y-player->y); /*UP*/
   	if(isValidPos(ghost->x,ghost->y-1)&&map[ghost->x][ghost->y-1]!=WALL)
   	b=abs(ghost->x-player->x)+abs(ghost->y-1-player->y);  /*LEFT*/
   	if(isValidPos(ghost->x+1,ghost->y)&&map[ghost->x+1][ghost->y]!=WALL)
   	c=abs(ghost->x+1-player->x)+abs(ghost->y-player->y);  /*DOWN*/
   	if(isValidPos(ghost->x,ghost->y+1)&&map[ghost->x][ghost->y+1]!=WALL)
   	d=abs(ghost->x-player->x)+abs(ghost->y+1-player->y);  /* RIGHT*/
   }
   int m=minimum(a,b,c,d);
   	if(m==a&& ghost->direction != DOWN){
	ghost->direction = UP;
	} 
    else if(m==b&& ghost->direction != RIGHT) {
	ghost->direction = LEFT;
	}
    else if(m==c && ghost->direction != UP){
	ghost->direction = DOWN;
	} 
    else if(m==d&&ghost->direction != LEFT){
	ghost->direction = RIGHT;
	} 
}
void showBoard(char **map)
{
	int x=0,y=0;
    for (x = 0; x < BOARDx-1; x++)
    {
    	for(y=0;y<BOARDy-1;y++)
        printf("%c",map[x][y]) ;
        printf("\n");
    }
}
void show_current_position(struct Player *player)
{
    printf("\nPlayerX: %d\n",player->x);
    printf("PlayerY: %d\n",player->y);
}
void showScoreEnergy(struct Score *s1){
	if(s1->energy<0)
	s1->energy=0;
	printf("\nHIGHEST SCORE= %d",s1->score);
	printf("\nENERGY LEVEL= %d",s1->energy);
}

void delay(int milli_seconds)
{
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}
int get_code ( void )   /* for taking arrow keys input for moving player*/
{
  int ch=getch();

  if ( ch == 0 || ch == 224 )
	ch = 256 + getch();
	fflush(stdin);
  return ch;
}
void running_game()
{ 
system("cls");
int i=0,j=0,check=0,MAX=0;
char **map1 = (char**) malloc((BOARDx)*sizeof(char*));
     for (i =0 ; i < BOARDx; i++)
     map1[i] = malloc(BOARDy * sizeof(char));
    char **map=map1; 
  for(i=0;i<BOARDx-1;i++){
  	for(j=0;j<BOARDy-1;j++){
  		if(i==0||i==BOARDx-2||j==0||j==BOARDy-2||((i==Mx-1||i==Mx+1)&&((j>=5&&j<My/2-1)
		  ||(j>My/2+1&&j<=My-1)||(j>=My+1&&j<My+My/2-1)||(j>My+My/2+1&&j<=BOARDy-6)))||(i==5||i==Mx+4)
		  &&((j>=7&&j<My/2-1)||(j>My/2+1&&j<=My-4)||(j>=My+4&&j<My+My/2-1)||(j>My+My/2+1&&j<=BOARDy-9))
		  ||(i==Mx/2+1||i==Mx+Mx/2)&&((j>=My/2+1&&j<=My/2+3)||(j>=My+My/2-2&&j<=My+My/2))
		  ||((j==5||j==My-1||j==My+1||j==BOARDy-6)
		  &&((i>=3&&i<=Mx-1)||(i>=Mx+1&&i<=BOARDx-4)))||(j==7||j==My-4||j==My+4||j==BOARDy-9)&&((i>=5&&i<=Mx-3)||(i>=Mx+4&&i<=BOARDx-5))
		  ||(j==My/2+2||j==(My+My/2-1))&&((i>Mx/2+1&&i<=Mx-4)||(i>Mx+Mx/2&&i<=BOARDx-6)))
  		map1[i][j]=WALL;
  		else if((i==1&&(j==1||j==BOARDy-3))||(i==BOARDx-3&&(j==1||j==BOARDy-3)))
  		map1[i][j]=BIG_DOT;
  		else if(((i==1||i==Mx||i==BOARDx-3)&&(j>=3&&j<=BOARDy-5))||
		((j==3||j==My||j==BOARDy-5)&&(i>=3&&i<=BOARDx-4))
		||((i==3||i==Mx-2||i==Mx+2||i==BOARDx-4)&&((j>=6&&j<=My-2)
		||j>=My+2&&j<=BOARDy-7))||((j==6||j==My-2||j==My+2||j==BOARDy-7)&&((i>=3&&i<=Mx-2)
		||(i>=Mx+2&&i<=BOARDx-4)))||(j==9||j==My-6||j==My+6||j==BOARDy-11)&&((i>=8&&i<=Mx-4)||(i>=Mx+7&&i<=BOARDx-6))
		||(i==Mx/2||i==Mx+Mx/2-1)&&((j>=My/2+1&&j<=My/2+3)||(j>=My+My/2-2&&j<=My+My/2)))
		{
			map1[i][j]=DOT;
			MAX++;
		}
		else
		map1[i][j]=EMPTY;                   
	  }
  }
       player.x=-1;
       player.y=-1;
       ghost[0].x=-1;ghost[1].x=-1;ghost[2].x=-1;ghost[3].x=-1;
       ghost[0].y=-1;ghost[1].y=-1;ghost[2].y=-1;ghost[3].y=-1;
ghost[0].direction=LEFT;ghost[1].direction=LEFT;ghost[2].direction=LEFT;ghost[3].direction=LEFT;
      ghost[0].inx=0;ghost[1].inx=1;ghost[2].inx=2;ghost[3].inx=3;
	  int direction = RIGHT;
      s1.energy=10;s1.score=0;
    movePlayer(&player, 1, 2,&s1,map,&check);
    moveGhost(&ghost[0], Mx, My,&s1,map,&check);
    moveGhost(&ghost[1], Mx, My-1,&s1,map,&check);
    moveGhost(&ghost[2], Mx, My+2,&s1,map,&check);
    moveGhost(&ghost[3], Mx, My+1,&s1,map,&check);
    while (s1.energy>0&&s1.score<MAX)
    {
        system("cls");
        showBoard(map);
        /*show_current_position(&player);*/
        showScoreEnergy(&s1);
       
        if (GetAsyncKeyState(VK_UP)) /*taking arrow keys input
	                                   for moving player in windows environment*/
        {
            direction = UP;
        }
        else if (GetAsyncKeyState(VK_DOWN))
        {
            direction = DOWN;
        }
        else if (GetAsyncKeyState(VK_LEFT))
        {
            direction = LEFT;
        }
        else if (GetAsyncKeyState(VK_RIGHT))
        {
            direction = RIGHT;
        }
    /*switch ( get_code() ) { /*for taking arrow keys input for moving player*/
    /*case ARROW_UP:
      direction=UP;
      break;
    case ARROW_DOWN:
      direction=DOWN;
      break;
    case ARROW_LEFT:
     direction=LEFT;
      break;
    case ARROW_RIGHT:
      direction=RIGHT;
      break;
    }*/
        switch (direction)
        {
        case UP:
            movePlayer(&player, player.x-1, player.y,&s1,map,&check);
            break;
        case DOWN:
            movePlayer(&player, player.x+1, player.y,&s1,map,&check);
            break;
        case LEFT:
            movePlayer(&player, player.x, player.y-1,&s1,map,&check);
            break;
        case RIGHT:
            movePlayer(&player, player.x, player.y+1,&s1,map,&check);
            break;
        }
        if(check==1){
        	check=0;
        direction=RIGHT;
ghost[0].direction=LEFT;ghost[1].direction=LEFT;ghost[2].direction=LEFT;ghost[3].direction=LEFT;
    movePlayer(&player, 1, 2,&s1,map,&check);
    moveGhost(&ghost[0], Mx, My,&s1,map,&check);
    moveGhost(&ghost[1], Mx, My-1,&s1,map,&check);
    moveGhost(&ghost[2], Mx, My+2,&s1,map,&check);
    moveGhost(&ghost[3], Mx, My+1,&s1,map,&check);
            delay(MoveSpeed);
		}
        int inx=0;
        for ( inx = 0; inx < 4; inx++)
        {
           GhostLogic(&ghost[inx], &player,map);
            switch (ghost[inx].direction)
            {
            case UP:
                moveGhost(&ghost[inx], ghost[inx].x-1, ghost[inx].y,&s1,map,&check);
                break;
            case DOWN:
                moveGhost(&ghost[inx], ghost[inx].x+1, ghost[inx].y,&s1,map,&check);
                break;
            case LEFT:
                moveGhost(&ghost[inx], ghost[inx].x, ghost[inx].y-1,&s1,map,&check);
                break;
            case RIGHT:
                moveGhost(&ghost[inx], ghost[inx].x, ghost[inx].y+1,&s1,map,&check);
                break;
            }
            if(check==1){
        	check=0;
        direction=RIGHT;	
ghost[0].direction=LEFT;ghost[1].direction=LEFT;ghost[2].direction=LEFT;ghost[3].direction=LEFT;
    movePlayer(&player, 1, 2,&s1,map,&check);
    moveGhost(&ghost[0], Mx, My,&s1,map,&check);
    moveGhost(&ghost[1], Mx, My-1,&s1,map,&check);
    moveGhost(&ghost[2], Mx, My+2,&s1,map,&check);
    moveGhost(&ghost[3], Mx, My+1,&s1,map,&check);
            delay(MoveSpeed);
		}
        }
        delay(MoveSpeed);
    }
    showScoreEnergy(&s1);
    printf("\nGAME OVER");
    char choice;
    while(true){
    printf("\nRestart Game?y\\n : ");
	scanf("%1c",&choice);
    fflush(stdin);
    if(choice=='y'){
    	for(i=0;i<BOARDx;i++)
    	    free(map1[i]);    
    	    free(map1);
	    running_game();
	}
	else if(choice=='n') exit(0);
     }
}

int main()
{   
system("cls");
srand((unsigned int)time(NULL));
    running_game();
    return 0;
}

