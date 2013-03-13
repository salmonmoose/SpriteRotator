#include <stdlib.h>
#include <string.h>

#include "SDL/SDL.h"
#include "SDL_image.h"

#include "gfx.h"
#include "font.h"

//This function writes a string of characters on screen
int Font_String(int x,int y,char *string,SDL_Surface *surface,SDL_Surface *fnt[], int space[])
{
int i;		//used for looping
int length;	//length of text string
int c=0;	//what character to draw

	//get length of string
        length=strlen(string);

		//loop through the string
                for(i=0;i<length;i++)
                {
                //get character
                    c = string[i] - 32;

                    //draw the character
                    Draw_Image(fnt[c],surface,x,y);

                    x+=space[c];
                }
return c;
}
