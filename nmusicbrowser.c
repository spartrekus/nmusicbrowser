

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h> 
#include <time.h>
/// dir
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>


#include <ncurses.h>



char searchitem[PATH_MAX];
void listdir(const char *name, int indent)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_DIR) 
	{
            char path[1024];

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            snprintf( path, sizeof(path), "%s/%s", name, entry->d_name);
            //printf("%*s[%s]\n", indent, "", entry->d_name);

            listdir( path, indent + 2);
        } 
	else 
	{
	    if ( strstr( entry->d_name , searchitem ) != 0 ) 
	    {
              printf("DIR %s\n", name );
              //printf("%*s- %s (Found)\n", indent, "", entry->d_name);
              printf("%*s- %s\n", indent, "", entry->d_name);
	    }
        }
    }
    closedir(dir);
}




















int rows, cols;  
int nconsole_explorer = 1;
int gameselection = 1;
char gamefilter[PATH_MAX];
char fileselection[PATH_MAX];

void selectioninit()
{
    strncpy( fileselection, "" , PATH_MAX );
}







void listprint(const char *name, int indent, char *searchitem )
{
    int posy = 1; 
    getmaxyx( stdscr, rows, cols);

    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    int entryselnb = 0;
    while ((entry = readdir(dir)) != NULL) 
    {
        attroff( A_REVERSE );

        if (entry->d_type == DT_DIR) 
	{
            char path[1024];

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            snprintf( path, sizeof(path), "%s/%s", name, entry->d_name);

            //listdir( path, indent + 2);
	    if ( strstr( entry->d_name , searchitem ) != 0 ) 
            if ( posy <= rows-3 )
            {
              entryselnb++;
              if ( gameselection == entryselnb ) attron( A_REVERSE );
              color_set( 6, NULL );
              mvprintw( posy++, 0, "[%s]", entry->d_name );
              if ( gameselection == entryselnb ) 
                  if ( strcmp( entry->d_name , "" ) != 0 )
                   strncpy( fileselection, entry->d_name , PATH_MAX );
             }

        } 
	else 
	{
	    if ( strstr( entry->d_name , searchitem ) != 0 ) 
	    {
              //printf("DIR %s\n", name );
              //printf("%*s- %s\n", indent, "", entry->d_name);
              if ( posy <= rows-3 )
              {
                 entryselnb++;
                 if ( gameselection == entryselnb ) attron( A_REVERSE );
                 color_set( 4, NULL );
                 mvprintw( posy++, 0, "%s", entry->d_name );
                 if ( gameselection == entryselnb ) 
                  if ( strcmp( entry->d_name , "" ) != 0 )
                   strncpy( fileselection, entry->d_name , PATH_MAX );
              }
	    }
        }
    }
    closedir(dir);
}








////////////////////////////////////////////////////////////////////
char *strrlf(char *str) 
{     // copyleft, C function made by Spartrekus 
      char ptr[strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if (str[i] != '\n' && str[i] != '\n') 
        ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}
////////////////////////////////////////////////////////////////////
char *strcut( char *str , int myposstart, int myposend )
{     // copyleft, C function made by Spartrekus 
      char ptr[strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if ( ( str[i] != '\0' ) && ( str[i] != '\0') )
         if ( ( i >=  myposstart-1 ) && (  i <= myposend-1 ) )
           ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}


////////////////////////////////
void nvim( char *thecmd   )
{
       char cmdi[PATH_MAX];
       def_prog_mode();
       endwin();
       strncpy( cmdi , " vi   " , PATH_MAX );
       strncat( cmdi , " \"" , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , thecmd , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , "\"" , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
       system( cmdi );
       reset_prog_mode();
}


////////////////////////////////
void nrunwith( char *theprg , char *thefile )
{
       char cmdi[PATH_MAX];
       def_prog_mode();
       endwin();
       strncpy( cmdi , "   " , PATH_MAX );
       strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , theprg , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , "  " , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " \"" , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , thefile , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , "\"" , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
       system( cmdi );
       reset_prog_mode();
}


////////////////////////////////
void nruncmd( char *thecmd   )
{
       char cmdi[PATH_MAX];
       def_prog_mode();
       endwin();
       strncpy( cmdi , "  " , PATH_MAX );
       strncat( cmdi , thecmd , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
       system( cmdi );
       reset_prog_mode();
}


////////////////////////
char *strninput( char *myinitstring )
{
   int strninput_gameover = 0; 
   char strmsg[PATH_MAX];
   char charo[PATH_MAX];
   strncpy( strmsg, myinitstring , PATH_MAX );
   int ch ;  int foo ; 
   int fooj; 
   while ( strninput_gameover == 0 )
   {
                  getmaxyx( stdscr, rows, cols);
                  attroff( A_REVERSE );
                  for ( fooj = 0 ; fooj <= cols-1;  fooj++)
                  {
                    //mvaddch( rows-2, fooj , ' ' );
                    mvaddch( rows-1, fooj , ' ' );
                  }
                  mvprintw( rows-1, 0, ":> %s" , strrlf( strmsg ) );
                  attron( A_REVERSE );
                  printw( " " );
                  attroff( A_REVERSE );
                  refresh() ; ch = getch();
		  if ( ch == 127  ) 
		  {
			   strncpy( strmsg, strcut( strmsg, 1 , strlen( strmsg ) -1 )  ,  PATH_MAX );
		  }
		  else if ( ch == 4  ) 
		  {
			   strncpy( strmsg, ""  ,  PATH_MAX );
		  }
		  else if ( ch == 27  ) 
		  {
			   strncpy( strmsg, ""  ,  PATH_MAX );
		  }
	          else if (
			(( ch >= 'a' ) && ( ch <= 'z' ) ) 
		        || (( ch >= 'A' ) && ( ch <= 'Z' ) ) 
		        || (( ch >= '1' ) && ( ch <= '9' ) ) 
		        || (( ch == '0' ) ) 
		        || (( ch == '~' ) ) 
		        || (( ch == '!' ) ) 
		        || (( ch == '&' ) ) 
		        || (( ch == '=' ) ) 
		        || (( ch == ':' ) ) 
		        || (( ch == ';' ) ) 
		        || (( ch == '<' ) ) 
		        || (( ch == '>' ) ) 
		        || (( ch == ' ' ) ) 
		        || (( ch == '|' ) ) 
		        || (( ch == '#' ) ) 
		        || (( ch == '?' ) ) 
		        || (( ch == '+' ) ) 
		        || (( ch == '/' ) ) 
		        || (( ch == '\\' ) ) 
		        || (( ch == '.' ) ) 
		        || (( ch == '$' ) ) 
		        || (( ch == '%' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == '{' ) ) 
		        || (( ch == '}' ) ) 
		        || (( ch == '(' ) ) 
		        || (( ch == ')' ) ) 
		        || (( ch == ']' ) ) 
		        || (( ch == '[' ) ) 
		        || (( ch == '*' ) ) 
		        || (( ch == '"' ) ) 
		        || (( ch == '@' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == '_' ) ) 
		        || (( ch == '^' ) ) 
		        || (( ch == '\'' ) ) 
	             ) 
		  {
                           foo = snprintf( charo, PATH_MAX , "%s%c",  strmsg, ch );
			   strncpy( strmsg,  charo ,  PATH_MAX );
		  }
		  else if ( ch == 10 ) 
		  {
                        strninput_gameover = 1;
		  }
     }
     char ptr[PATH_MAX];
     strncpy( ptr, strmsg, PATH_MAX );
     size_t siz = sizeof ptr ; 
     char *r = malloc( sizeof ptr );
     return r ? memcpy(r, ptr, siz ) : NULL;
}



void drawit()
{
        int fooj; 
        char foocwd[PATH_MAX];
        getmaxyx( stdscr, rows, cols);
        color_set( 2 , NULL );
        attroff( A_REVERSE );
        erase();  


        mvprintw( 0, 0, "|CONSOLE|" );
        color_set( 2 , NULL );
        attroff( A_REVERSE );

        if ( nconsole_explorer == 1 )
        {
           selectioninit();
           listprint( ".", 0 , gamefilter ) ;
        }


        color_set( 15 , NULL );
        attron( A_REVERSE );
        for ( fooj = 0 ; fooj <= cols-1;  fooj++)
        {
            mvaddch( rows-2, fooj , ' ' );
            mvaddch( 0, fooj , ' ' );
        }
        mvprintw( rows-2, 0 , "|%s|", fileselection );
        if ( strcmp( gamefilter, "" ) != 0 ) 
          printw( "Filter|" );
        else
          printw( "*|" );

        move( rows-1, 0 );
        color_set( 0 , NULL );
        attroff( A_REVERSE );
        mvprintw( rows-1, 0, "%s", getcwd( foocwd, PATH_MAX ) );

        move( rows-1, 0 );
        color_set( 2 , NULL );
        attroff( A_REVERSE );
}


void makro_create_slides()
{
   FILE *fp;
   fp = fopen( "slides.bmr" , "ab+" );
         fputs( "!include{beamer.mrk}\n" , fp );
         fputs( "!beamer\n" , fp );
         fputs( "!title Slides\n" , fp );
         fputs( "!author Username\n" , fp );
         fputs( "!gpath{figs/}\n" , fp );
         fputs( "!begin\n" , fp );
         fputs( "!cover\n" , fp );
         fputs( "!clr\n" , fp );
         fputs( "= Header\n" , fp );
         fputs( "> line1\n" , fp );
         fputs( "- item1\n" , fp );
         fputs( "- item2\n" , fp );
         fputs( "\n" , fp );
         fputs( "!enddoc\n" , fp );
   fclose( fp );
}




///////////////////////////////////////////////////////////
int main( int argc, char *argv[])
{


    ////////////////////////////////////////////////////////
    if ( argc == 2)
      if ( strcmp( argv[1] , "" ) !=  0 )
       {
          chdir( argv[ 1 ] );
       }



    ////////////////////////////////////////////////////////
    if ( argc == 3)
     if ( strcmp( argv[1] , "--find" ) ==  0 ) 
     {
         strncpy( searchitem, argv[ 2 ], PATH_MAX );          
         listdir( ".", 0 ) ;
         return 0;
     }





   initscr();			
   curs_set( 0 );
   start_color();
   init_pair(0, COLOR_WHITE, COLOR_BLACK);
   init_pair(1, COLOR_RED, COLOR_BLACK);
   init_pair(2, COLOR_GREEN, COLOR_BLACK);
   init_pair(3, COLOR_BLUE, COLOR_BLACK);
   init_pair(4, COLOR_CYAN, COLOR_BLACK);
   init_pair(6, COLOR_YELLOW, COLOR_BLACK);
   init_pair(15, COLOR_BLUE, COLOR_WHITE);

   color_set( 2, NULL );

   int ch ; 
   int rows, cols;  
   getmaxyx( stdscr, rows, cols);
   char userinp[PATH_MAX];
   int gameover_nsc = 0;
   strncpy( gamefilter, "", PATH_MAX );

   while ( gameover_nsc == 0)
   {
           drawit();
	   refresh();			/* Print it on to the real screen */
           ch = getch();
           if ( ch == 'q' ) gameover_nsc =1 ;

           else if ( ch == '~' ) { chdir( getenv( "HOME" ) ); gameselection = 1;  }
           else if ( ch == '!' ) { nrunwith( strninput( "" ) , fileselection  ); }
           else if ( ch == 'o' ) { chdir( strninput( "" ) ); gameselection = 1;  }
           else if ( ch == 'h' ) { chdir( ".." ); gameselection = 1;  }
           else if ( ch == 'l' ) { chdir( fileselection ); gameselection=1; }
           else if ( ch == 'k' ) gameselection--;
           else if ( ch == 'j' ) gameselection++; 
           else if ( ch == 'u' ) gameselection-=4;  
           else if ( ch == 'n' ) gameselection+=4; 
           else if ( ch == 'd' ) gameselection+=4; 
           else if ( ch == 'g' ) gameselection = 1;
           else if ( ch == 'f' ) 
           {
             strncpy( gamefilter, strninput( "" ), PATH_MAX );
             gameselection=1; 
           }
           else if ( ch ==  32 )  strncpy( gamefilter, "" , PATH_MAX );
           else if ( ch ==  'v' ) nvim( fileselection );
           else if ( ch ==  'c' ) nruncmd( " bash " );

           else if ( ch == '*' ) { nruncmd( " mpg123 -a hw:1,0 *.mp3 "  ); }
           else if ( ch == 10 ) { nrunwith( " mpg123 -a hw:1,0  " , fileselection  ); }

               else if ( ch == ':' ) 
               {
                strncpy( userinp, strninput( "" ), PATH_MAX );

               }
 
           }

    curs_set( 1 );
    endwin();			/* End curses mode		  */
    return 0;
}



