#define NUM_CHARACTERS 96			//number of charecters in font

SDL_Surface *font_clean[NUM_CHARACTERS];		//surface for "cleanfont"

//how much space there should be between the characters
int cleanfont_space[NUM_CHARACTERS];

//This function writes a string of characters on screen
int Font_String(int x,int y,char *string,SDL_Surface *surface,SDL_Surface *fnt[], int space[]);
