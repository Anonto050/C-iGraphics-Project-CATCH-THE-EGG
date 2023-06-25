# include "iGraphics.h"
# include "gl.h"

int game_state;
int bx,by;

char background[10][20]= {"bc.bmp","bc2.bmp","bc3.bmp"};
char button[10][20]= {"play.bmp","continue.bmp","level.bmp","score.bmp","help.bmp","exit.bmp","pause.bmp"};
char object[14][20]= {"normal_egg.bmp","shit.bmp","normal_egg.bmp","blue_egg.bmp","normal_egg.bmp","black_egg.bmp","shit.bmp","normal_egg.bmp","golden_egg.bmp","normal_egg.bmp","shit.bmp","normal_egg.bmp","blue_egg.bmp","black_egg.bmp"};

int score,tm,diff,chic_amount=1,egg_num,sound=1,pos,visited_chicken[1300]= {0},b,c,pause=1,air,name_len,player_num[3],active_box,active_basket,error_message;
float egg_dy[5][14],b_dy,c_dy;
char str1[1000],str2[1000],str[1000],str3[1000],temp_name[200];

typedef struct
{
  int height;
  int width;
  int index;
  char pic[5][20] = {"basket1.bmp","basket2.bmp","basket3.bmp"};
}Basket;
Basket bskt;

typedef struct
{
    float x;
    float y;
} co_ordinates;

co_ordinates btn[10],chicken[8],bonus = {-1,-1},bonus_time = {-1,-1};
co_ordinates egg[8][20] = {-1,-1},basket = {500,70};

typedef struct
{
    int minute;
    int sec;
} Clock;

Clock clck;
int minute=0,sec=40,check;

typedef struct
{
    int level;
    int timer;
    char level_time[3][20] = {"time0.bmp","time1.bmp","time2.bmp"};
    char level_chicken[3][20] = {"chicken0.bmp","chicken1.bmp","chicken2.bmp"};
}set_level;
set_level Level;

struct Player{
 char name[1000];
 int Score;
}player[10000],temp;

/*
	function iDraw() is called again and again by the system.
*/
void place_buttons();
void resume_game();
void continue_game();
void sort_score();
void show_score();
void PassiveMousemove(int,int);

int collison(co_ordinates position)
{
    if((position.x >= basket.x && position.x <= basket.x+bskt.width)&&(position.y >= bskt.height-20 && position.y <= bskt.height+20))
    {
        return 1;
    }
    return 0;
}

void namebox()
{
    iSetColor(0,0,0);
    iText(500,450,str1,GLUT_BITMAP_TIMES_ROMAN_24);
    sprintf(str1,"Your Score : %d",score);
    iText(500,400,"Enter your name : ",GLUT_BITMAP_TIMES_ROMAN_24);
    iRectangle(500,300,400,80);
    iText(500,250,"Click to activate the box and enter to finish",GLUT_BITMAP_TIMES_ROMAN_24);
}

void show_score()
{
    int x=150;
    for(int k=0;k<3;k++){

     FILE *fp,*fp1;
	int i,y=500,value;
	char name[500],scores[100];

    if(k==0)
	   fp=fopen("easy_highscore.txt","r");
	else if(k==1)
       fp=fopen("medium_highscore.txt","r");
    else
       fp=fopen("hard_highscore.txt","r");

		iSetColor(0,0,0);
    if(k==0)
	   fp1=fopen("easy_player_number.txt","r");
	else if(k==1)
      fp1=fopen("medium_player_number.txt","r");
    else
       fp1=fopen("hard_player_number.txt","r");

		fscanf(fp1,"%d",&player_num[k]);
		fclose(fp1);

		if(player_num[k]>10)
		{
			for(i=0;i<10;i++)
			{

					fscanf(fp,"%s%d",&name,&value);
					int j=0;
					while(name[j]!='\0')
                    {
                        if(name[j]=='+')
                            name[j] = ' ';
                            j++;
                    }
					sprintf(scores,"%d",value);
					iText(x,y,name,GLUT_BITMAP_TIMES_ROMAN_24);
					iText(x+175,y,scores,GLUT_BITMAP_TIMES_ROMAN_24);
					y-=40;

			}
		}
		else
		{
			for(i=0;i<player_num[k];i++)
			{
					fscanf(fp,"%s%d",&name,&value);
					int j=0;
					while(name[j]!='\0')
                    {
                        if(name[j]=='+')
                            name[j] = ' ';
                            j++;
                    }
					sprintf(scores,"%d",value);
					iText(x,y,name,GLUT_BITMAP_TIMES_ROMAN_24);
					iText(x+175,y,scores,GLUT_BITMAP_TIMES_ROMAN_24);
					y-=40;

			}
		}

	fclose(fp);
	x += 425;
 }
}


void iDraw()
{
    //place your drawing codes here

    iClear();

    if(game_state==0)
    {
        iShowBMP(0,0,background[0]);
        iShowBMP2(800,20,"chic.bmp",0x00f7f7f7);
        place_buttons();
        if(error_message)
            iShowBMP(350,250,"error.bmp");
    }
    else if(game_state == 1)
    {
        resume_game();
    }
    else if(game_state == 2)
    {
        continue_game();
    }
    else if(game_state == 3)
    {
        //level
        iShowBMP(0,0,background[0]);
        iShowBMP2(800,20,"chic.bmp",0x00f7f7f7);
        iShowBMP2(1240,730,"home.bmp",0x00000000);
        iShowBMP2(200,600,"easy.bmp",0x00ffffff);
        iShowBMP2(200,420,"medium.bmp",0x00ffffff);
        iShowBMP2(200,240,"hard.bmp",0x00ffffff);
        int x = 200;
        int y = 530;
        for(int i=0;i<3;i++)
        {
            iShowBMP2(x,y,Level.level_time[i],0x00ffffff);
            iShowBMP2(x+230,y,Level.level_chicken[i],0x00ffffff);
            y -= 180;
        }
    }
    else if(game_state == 4)
    {
         //score
         iShowBMP(0,0,"score_board.bmp");
        iShowBMP2(1240,730,"home.bmp",0x00000000);
        show_score();
    }
    else if(game_state == 5)
    {
        //help
        iShowBMP(0,0,"help_page.bmp");
        iShowBMP2(1240,730,"home.bmp",0x00000000);
    }
    else if(game_state == 7)
    {
       iShowBMP(0,0,background[1]);
       iShowBMP2(100,0,"chic3.bmp",0x00ffffff);
       iShowBMP2(600,400,"resume.bmp",0x00ffffff);
       iShowBMP2(600,280,"menu.bmp",0x00ffffff);

    }
    else if(game_state == 8)
    {
        iShowBMP(0,0,background[2]);
        namebox();
        if(active_box)
        {
            iSetColor(0,0,0);
            iText(550,330,str3,GLUT_BITMAP_TIMES_ROMAN_24);
            sprintf(str3,"%s",temp_name);
        }
    }
    if(sound)
        iShowBMP2(1320,730,"sound_on.bmp",0x00ffffff);
    else
        iShowBMP2(1320,730,"sound_off.bmp",0x00ffffff);
}

void place_buttons()
{
    int i,j;
    if(game_state==0)
    {
        j=600;
        for(i=0; i<=5; i++)
        {
            btn[i].x=200;
            btn[i].y=j;
            iShowBMP2(btn[i].x,btn[i].y,button[i],0x00ffffff);
            j=j-100;
        }
    }
}

void resume_game()
{

    iShowBMP(0,0,background[1]);
    iSetColor(0,0,0);
    for(int i=1,j=0; i<=3; i++,j=j+498)
        iShowBMP(j,560,"bamboo.bmp");
    iLine(0,560,1400,560);

    for(int i=0; i<chic_amount; i++)
    {
        iShowBMP2(chicken[i].x,chicken[i].y,"chic2.bmp",0x00ffffff);
        for(int k=0; k<14; k++)
        {
            if(egg[i][k].y >= bskt.height+15)
            {
                iShowBMP2(egg[i][k].x,egg[i][k].y,object[k],0x00ffffff);
            }
            else if(egg[i][k].y >= bskt.height-20 && egg[i][k].y < bskt.height+15)
            {
                if(!collison(egg[i][k]))
                {
                    if(k==1 || k==6 || k==10)
                        iShowBMP2(egg[i][k].x,75,object[k],0x00ffffff);
                    else
                        iShowBMP2(egg[i][k].x,75,"broken_egg.bmp",0x00ffffff);
                }
                else
                {
                    iShowBMP2(egg[i][k].x,egg[i][k].y,object[k],0x00ffffff);
                }

            }
        }
    }
    if(bskt.index<2){
    if(bonus.y >= 75)
       iShowBMP2(bonus.x,bonus.y,"bonus.bmp",0x00ffffff);
    else if(bonus.y >= 45 && bonus.y <= 75)
       iShowBMP2(bonus.x,78,"bonus.bmp",0x00ffffff);
    }
    if(bonus_time.y >= 75)
       iShowBMP2(bonus_time.x,bonus_time.y,"clock.bmp",0x00ffffff);
    else if(bonus_time.y >= 45 && bonus_time.y <= 75)
       iShowBMP2(bonus_time.x,78,"clock.bmp",0x00ffffff);

    if(bskt.index<2)
        iShowBMP2(basket.x,basket.y,bskt.pic[bskt.index],0x00ffffff);
    else
        iShowBMP2(basket.x,basket.y,bskt.pic[2],0x00ffffff);

    iShowBMP2(1230,730,"pause.bmp",0x00ffffff);
    iText(50,750,str1,GLUT_BITMAP_TIMES_ROMAN_24);
    sprintf(str1,"Score : %d",score);
    iText(1050,750,str2,GLUT_BITMAP_TIMES_ROMAN_24);
    sprintf(str2,"Time : %02d : %02d",clck.minute,clck.sec);
}

void store_everything(){
   FILE* fptime;
   fptime=fopen("time.txt","w");
   fprintf(fptime,"%d %d\n",clck.minute,clck.sec);
   fclose(fptime);

   FILE* fpchic;
   fpchic = fopen("chicken_position.txt","w");
   for(int i=0;i<chic_amount;i++)
   {
       fprintf(fpchic,"%f %f\n",chicken[i].x,chicken[i].y);
   }
   fclose(fpchic);

   FILE* fpegg;
   fpegg = fopen("egg.txt","w");
   for(int i=0;i<chic_amount;i++)
   {
       for(int j=0;j<14;j++){
        fprintf(fpegg,"%f %f %f\n",egg[i][j].x,egg[i][j].y,egg_dy[i][j]);
       }
   }
   fclose(fpegg);

   FILE* fplevel;
   fplevel = fopen("level.txt","w");
   fprintf(fplevel,"%d %d %d\n",Level.level,chic_amount,bskt.index);
   fclose(fplevel);

   FILE* fpscore;
   fpscore = fopen("continue_game_score.txt","w");
   fprintf(fpscore,"%d\n",score);
   fclose(fpscore);
}

void continue_game()
{
   FILE* fp;

   fp = fopen("time.txt","r");
    while(EOF!=fscanf(fp,"%d%d",&clck.minute,&clck.sec))
    {
        printf("%d %d\n",clck.minute,clck.sec);
    }
    fclose(fp);

    if(clck.minute==0 && clck.sec==0){
         game_state = 0;
        error_message = 1;
    }

    if(error_message == 0){
    fp = fopen("level.txt","r");
    while(EOF!=fscanf(fp,"%d%d%d",&Level.level,&chic_amount,&bskt.index))
    {
        printf("%d %d %d\n",Level.level,chic_amount,bskt.index);
    }
    fclose(fp);
    if(bskt.index == 1)
          {
              bskt.width = 160;
              bskt.height = 150;
          }
    else if(bskt.index == 2)
          {
              bskt.width = 190;
              bskt.height = 150;
          }

     fp = fopen("chicken_position.txt","r");
     int i=0;
    while(EOF!=fscanf(fp,"%f%f",&chicken[i].x,&chicken[i].y))
    {
        printf("%f %f\n",chicken[i].x,chicken[i].y);
        i++;
    }
    fclose(fp);

    fp = fopen("egg.txt","r");
    for(int i=0;i<chic_amount;i++){
        for(int j=0;j<14;j++){
            fscanf(fp,"%f%f%f",&egg[i][j].x,&egg[i][j].y,&egg_dy[i][j]);
        }
    }
    fclose(fp);

    fp = fopen("continue_game_score.txt","r");
    fscanf(fp,"%d",&score);
    fclose(fp);

    iResumeTimer(0);
    iResumeTimer(1);
    iResumeTimer(2);

    game_state = 1;
    }
}

void sort_score()
{
     FILE *fp;
     int c,j,k,i=0;

     if(Level.level==0)
        fp = fopen("easy_player_list.txt","r");
     else if(Level.level==1)
        fp = fopen("medium_player_list.txt","r");
     else
        fp = fopen("hard_player_list.txt","r");

     while(EOF != fscanf(fp,"%s%d",&player[i].name,&player[i].Score))
                i++;
     fclose(fp);

     if(Level.level==0)
          fp = fopen("easy_player_number.txt","w");
     else if(Level.level==1)
          fp = fopen("medium_player_number.txt","w");
     else
         fp = fopen("hard_player_number.txt","w");

     fprintf(fp,"%d",i);
     fclose(fp);

     for(j=0;j<i-1;j++){
        for(k=j+1;k<i;k++){
            if(player[k].Score>player[j].Score){
                temp = player[j];
                player[j] = player[k];
                player[k] = temp;
            }
        }
     }

     if(Level.level==0)
        fp = fopen("easy_highscore.txt","w");
     else if(Level.level==1)
        fp = fopen("medium_highscore.txt","w");
     else
        fp = fopen("hard_highscore.txt","w");

     if(i>10)
     {
         for(c=0;c<10;c++){
            fprintf(fp,"%s\t%d\n",player[c].name,player[c].Score);
         }
     }
     else
     {
         for(c=0;c<i;c++){
            fprintf(fp,"%s\t%d\n",player[c].name,player[c].Score);
         }
     }
     fclose(fp);

}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
/*void PassiveMousemove(int x,int y){
   basket.x = x;
}

void passivemousemovehandler(int x,int y){
    iMouseX = x;
    iMouseY = 800-y;
    PassiveMousemove(iMouseX,iMouseY);
    glFlush();
}*/

void iMouseMove(int mx, int my)
{
    //place your codes here
    if(active_basket && (my>50 && my<=220)&&(mx>=0 && mx<=1400-bskt.width))
                   basket.x = mx;

}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //printf("%d %d\n",mx,my);
         if((mx>=basket.x && mx<=basket.x+bskt.width) && (my>=basket.y && my<=basket.y+bskt.height+30))
              active_basket = 1;
        if((mx>=1230 && mx<=1292) && (my>=740 && my<=790) && game_state==1)
          {
              iPauseTimer(0);
              iPauseTimer(1);
              iPauseTimer(2);
              store_everything();
              game_state = 7;
              if(sound)
       PlaySound("Intro Theme.wav", NULL, SND_LOOP | SND_ASYNC);
            }
        if(game_state == 7){
        if((mx>=600 && mx<=1000) && (my>=400 && my<=490))
            {
                 game_state = 1;
                iResumeTimer(0);
                iResumeTimer(1);
                iResumeTimer(2);
            }
            if((mx>=600 && mx<=1000) && (my>=280 && my<=370))
            {
                game_state = 0;
            }
        }

        if((mx>=1320 && mx<=1382) && (my>=730 && my<=792))
        {
            if(sound)
            {
                PlaySound(0,0,0);
                PlaySound("click2.wav", NULL, SND_ASYNC);
                sound=0;
            }
            else
            {
                sound=1;
                PlaySound("click2.wav", NULL, SND_ASYNC);
                PlaySound("Intro Theme.wav", NULL, SND_LOOP | SND_ASYNC);
            }
        }
        if(game_state==0)
        {
            if(error_message){
                if((mx>=1010 && mx<=1045) && (my>=495 && my<=550))
                    error_message = 0;
            }
            for(int i=0; i<=5; i++)
            {
                if((mx>=btn[i].x && mx<=btn[i].x+390) && (my>=btn[i].y && my<=btn[i].y+87))
                {
                    PlaySound("click2.wav", NULL, SND_ASYNC);
                    if(i==0)
                    {
                        game_state=1;
                        iResumeTimer(0);
                        iResumeTimer(1);
                        iResumeTimer(2);
                        clck = {minute,sec};
    basket = {500,70};
    bskt.index = 0;
    bskt.height = 140;
    bskt.width = 130;
    score = 0;
    for(int i=0; i<chic_amount; i++)
        {
            for(int j=0; j<14; j++)
            {
                egg[i][j].x = -1;
                egg[i][j].y = -1;
            }
        }
    bonus = {-1,-1};
    bonus_time = {-1,-1};
    b_dy=0;
    c_dy=0;
    int s = 0;
    for(int k=0; k<chic_amount; k++)
    {
        chicken[k]= {s,560};
        s = s+200;
        for(int j=0;j<14;j++)
            egg_dy[k][j]=2;
    }
    if(Level.level == 0){
                   Level.level = 0;
                   //Level.timer = 80;
                   clck.minute = 0;
                   clck.sec = 40;
                   chic_amount = 1;
    }
    else if(Level.level == 1){
                   Level.level = 1;
                   //Level.timer = 75;
                   clck.minute = 1;
                   clck.sec = 0;
                   chic_amount = 2;
    }
    else
    {
                   Level.level = 2;
                   //Level.timer = 70;
                   clck.minute = 1;
                   clck.sec = 30;
                   chic_amount = 3;
    }

                    }
                    else if(i==1)
                    {
                        game_state=2;
                        if(sound)
                            PlaySound("Intro Theme.wav",NULL,SND_LOOP | SND_ASYNC);
                    }
                    else if(i==2)
                    {
                        game_state=3;
                        if(sound)
                            PlaySound("Intro Theme.wav",NULL,SND_LOOP | SND_ASYNC);
                    }
                    else if(i==3)
                    {
                        game_state=4;
                        if(sound)
                            PlaySound("Intro Theme.wav",NULL,SND_LOOP | SND_ASYNC);
                    }
                    else if(i==4)
                    {
                        game_state=5;
                        if(sound)
                            PlaySound("Intro Theme.wav",NULL,SND_LOOP | SND_ASYNC);
                    }
                    else if(i==5)
                    {
                        exit(0);
                    }
                }
            }
        }
        if(game_state == 3){
               if((mx>=200 && mx<=590) && (my>=600 && my<=685))
               {
                   Level.level = 0;
                   clck.minute = 0;
                   clck.sec = 40;
                   chic_amount = 1;
                   if(sound)
                   PlaySound("click2.wav", NULL, SND_ASYNC);
               }
               if((mx>=200 && mx<=590) && (my>=420 && my<=505))
               {
                   Level.level = 1;
                   clck.minute = 1;
                   clck.sec = 0;
                   chic_amount = 2;
                   if(sound)
                   PlaySound("click2.wav", NULL, SND_ASYNC);
               }
               if((mx>=200 && mx<=590) && (my>=240 && my<=325))
               {
                   Level.level = 2;
                   clck.minute = 1;
                   clck.sec = 30;
                   chic_amount = 3;
                   if(sound)
                   PlaySound("click2.wav", NULL, SND_ASYNC);
               }
        }
        if(game_state == 3 || game_state == 4 || game_state == 5){
            if((mx>=1240 && mx<=1305) && (my>=730 && my<=795)){
                    if(sound)
                    {
                        PlaySound("click2.wav", NULL, SND_ASYNC);
                PlaySound("Intro Theme.wav", NULL, SND_LOOP | SND_ASYNC);
                    }
                game_state = 0;
            }
        }
        if(game_state == 8){
            if((mx>=500 && mx<=900) && (my>=300 && my<=380))
            {
                for(int j=0;j<200;j++)
                    temp_name[j]='\0';
                name_len=0;
                active_box = 1;
            }
        }
        //place your codes here


    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        //place your codes here

    }
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    //place your codes for other keys here
   if(game_state == 8)
   {
       FILE *fp;
       if(key == '\r')
       {
           //remove enter
           int i=0;
           while(temp_name[i]!='\0')
           {
               if(temp_name[i]==' ')
                   temp_name[i] = '+';
                   i++;
           }

           if(Level.level==0){
           fp = fopen("easy_player_list.txt","a");
           fprintf(fp,"%s\t%d\n",temp_name,score);
           fclose(fp);
           player_num[0]++;
           }

           else if(Level.level==1){
                fp = fopen("medium_player_list.txt","a");
                fprintf(fp,"%s\t%d\n",temp_name,score);
                fclose(fp);
                player_num[1]++;
           }

           else{
              fp = fopen("hard_player_list.txt","a");
              fprintf(fp,"%s\t%d\n",temp_name,score);
              fclose(fp);
              player_num[2]++;
           }

           sort_score();
           game_state = 0;
           active_box = 0;
       }
       else if(key == '\b')
       {
           name_len--;
           temp_name[name_len] = '\0';
       }
       else
       {
           temp_name[name_len] = key;
           name_len++;
       }
   }
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{

    if(key == GLUT_KEY_F1)
    {

    }
    if(key == GLUT_KEY_END)
    {
    }
    if(key == GLUT_KEY_RIGHT)
    {
        if(basket.x<1400-200)
            basket.x += 20;
    }
    if(key == GLUT_KEY_LEFT)
    {
        if(basket.x-20>0)
            basket.x -= 20;
    }
    if(key == GLUT_KEY_HOME)
    {
    }
    //place your codes for other keys here
}

void get_bonus(){
   bskt.index++;
   if(bskt.index <3)
   {
       if(bskt.index == 1)
          {
              bskt.width = 160;
              bskt.height = 150;
          }
       else if(bskt.index == 2)
          {
              bskt.width = 190;
              bskt.height = 150;
          }
   }
}

void get_time(){
 if(clck.sec+5 < 60)
    clck.sec += 5;
 else
 {
     clck.sec = clck.sec+5-60;
     clck.minute++;
 }
}

void Change()
{
    if(game_state == 1)
    {
        for(int j=0; j<1200; j++)
            visited_chicken[j] = 0;
        for(int i=0; i<chic_amount; i++)
        {
            chicken[i].y = 560;
            while(1)
            {
                pos = rand()%1200;
                if(visited_chicken[pos] == 0)
                    break;
            }
            for(int j=0; j<111; j++)
            {
                visited_chicken[pos+j] = 1;
                if(pos-j>=0)
                    visited_chicken[pos-j] = 1;
            }
            chicken[i].x = pos;
            egg[i][egg_num].x = pos+30;
            egg[i][egg_num].y = 530;
            egg_dy[i][egg_num] = 2;
            egg_num = (egg_num+1)%14;
        }
        int t = clck.sec%20;
        if(t == rand()%20)
            b=1;
        int t2 = clck.sec%15;
        if(t2 == rand()%15)
                c=1;
        if(b == 1 && (bonus.y < 0 || bonus.y > 800))
        {
            bonus.x = rand()%1200;
            bonus.y = 800;
            b_dy = 2;
            b = 0;
        }
        if(c == 1 && (bonus_time.y < 0 || bonus_time.y > 800))
        {
            bonus_time.x = rand()%1200;
            bonus_time.y = 800;
            c_dy = 2;
            c = 0;
        }
        air = (air+1)%6;
    }
}

void egg_position_change()
{
    if(game_state == 1)
    {
        for(int i=0; i<chic_amount; i++)
        {
            for(int j=0; j<14; j++)
            {
                if(Level.level==2 && air == 2)
                   egg[i][j].x += 8;
                egg[i][j].y -= egg_dy[i][j];
                if(Level.level == 0)
                    egg_dy[i][j] += 3;
                if(Level.level == 1)
                    egg_dy[i][j] += 3.5;
                if(Level.level == 2)
                    egg_dy[i][j] += 4;
                //printf("%d %d %f %f %f\n",i,j,egg[i][j].x,egg[i][j].y,egg_dy[i][j]);
                if(collison(egg[i][j]))
                {
                    if(j==1 || j==6 || j==10)
                    {
                        if(sound)
                            PlaySound("minus.wav", NULL, SND_ASYNC);
                        score -= 10;
                    }
                    else if(j==5 || j==13)
                    {
                        if(sound)
                            PlaySound("minus.wav", NULL, SND_ASYNC);
                        score -= 5;
                    }
                    else if(j==3 || j==12)
                    {
                        if(sound)
                            PlaySound("Bonus.wav", NULL, SND_ASYNC);
                        score += 5;
                    }
                    else if(j==8)
                    {
                        if(sound)
                            PlaySound("Bonus.wav", NULL, SND_ASYNC);
                        score += 10;
                    }
                    else
                    {
                        if(sound)
                            PlaySound("Bonus.wav", NULL, SND_ASYNC);
                        score++;
                    }
                }
                else if(egg[i][j].y >= bskt.height-20 && egg[i][j].y < bskt.height+15)
                {
                    if(sound)
                        PlaySound("egg_crack.wav", NULL, SND_ASYNC);
                }

            }
        }
        bonus.y -= b_dy;
        b_dy += 1;
        if(collison(bonus))
           {
           if(sound)
              {
                PlaySound("Bonus.wav", NULL, SND_ASYNC);
              }
            get_bonus();
           }
        bonus_time.y -= c_dy;
        c_dy += 1;
        if(collison(bonus_time))
        {
            if(sound)
             {
                 PlaySound("Bonus.wav", NULL, SND_ASYNC);
             }
             get_time();
        }
    }
}

void time_count()
{
    if(game_state == 1){
    tm = clock()% 60000;
    if(!(clck.minute == 0 && clck.sec == 0))
    {
        if(diff>tm)
            check = tm+60000-diff;
        else
            check = tm-diff;
        if(check >= 1000 && clck.sec != 0)
        {
            clck.sec -= 1;
            diff = tm;
        }
        else if(check >= 1000 && clck.sec == 0)
        {
            clck.minute -= 1;
            clck.sec = 59;
            diff = tm;
        }
    }
    else
    {
        game_state = 8;
        iPauseTimer(0);
        iPauseTimer(1);
        iPauseTimer(2);
        store_everything();
        if(sound)
        PlaySound("Intro Theme.wav",NULL,SND_LOOP | SND_ASYNC);
    }
    }
}

int main()
{
//    PlaySound((LPCSTR) "C:\\Users\\Public\\Music\\Sample Music\\Sleep Away.mp3", NULL, SND_FILENAME | SND_ASYNC);
//    DWORD dwError = GetLastError();

    //place your own initialization codes here.
    clck = {minute,sec};
    iSetTimer(100,time_count);
    bskt.index = 0;
    bskt.height = 140;
    bskt.width = 130;
    Level.timer = 80;
    int s = 0;
    for(int k=0; k<chic_amount; k++)
    {
        chicken[k]= {s,560};
        s = s+200;
        for(int j=0;j<14;j++)
            egg_dy[k][j]=2;
    }
    iSetTimer(900, Change);
    iSetTimer(80,egg_position_change);
    //iSetTimer(80,store_everything);
    if(sound)
        PlaySound("Intro Theme.wav",NULL,SND_LOOP | SND_ASYNC);

    //glutPassiveMotionFunc(passivemousemovehandler);
    iInitialize(1400, 800, "Catch the Egg!");

    return 0;
}
