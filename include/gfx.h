#ifndef GFX_H

#define TRUE	1
#define FALSE	0
#define PI      3.1415926535897932384626433832795f	//PI

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

SDL_Surface *screen;			//screen surface
SDL_Surface *TempImage;			//temporary surface to loading graphics
SDL_PixelFormat *PixelFormat;	//what pixelformat that is being used (24bpp, 32bpp etc)
SDL_Surface *TripleBuffer;     //the tripple buffer

SDL_Surface *Image2Rotate;      //the image we load in to rotate
SDL_Surface *Rotated_2X;        //the scale2x 2x version of the rotated image
SDL_Surface *Rotated_4X;        //the scale2x 4x version of the rotated image
SDL_Surface *Rotated_8X;        //the scale2x 8x version of the rotated image
SDL_Surface *RotateResult_2X;      //the result of the image 2x
SDL_Surface *RotateResult_4X;      //the result of the image 4x
SDL_Surface *RotateResult_8X;      //the result of the image 8x

SDL_Surface *SaveSurface;

SDL_Surface *GFX_Select_Box;    //the selectbox


//Good ol putpixel
void PutPixel(SDL_Surface* Surface, int x, int y, Uint32 pixel);

//Good ol getpixel
Uint32 GetPixel(SDL_Surface* Surface, int x, int y);

//This function locks a surface
void Slock(SDL_Surface *screen);

//This function unlocks a surface
void Sulock(SDL_Surface *screen);

//This function clears a surface
void Clear_Surface(SDL_Surface *dst);

//This function will draw an image on the desired surface and position
void Draw_Image(SDL_Surface *img,SDL_Surface *where, int x, int y);

//Convert a surface the the screens display format
SDL_Surface *Load_And_Convert_Bitmap(char *file);

//this function will rotate a surface (a bit slow, but ok for our purpose)
SDL_Surface *Rotate_Surface (SDL_Surface *src,float angle, int zoom,int pixel_offset);

//This cunction copies a portion of a surface to another
void Surface_Copy(SDL_Surface *from, SDL_Surface *where,int x,int y,int w,int h);

//This function creates the triple buffer
void Create_triple_buffer(int width,int height);

//This function clips many sprites and place them in an array
void Get_Anim(char *file,SDL_Surface *image, SDL_Surface *anim[],int x,int y,int w,int h,int num_frames, int num_sprites_per_line,int grid_width);

//This function sets up an SDL Rectangle
void Setup_Rectangle(SDL_Rect *Rect,int x,int y,int width,int height);

//this function checks if two boxes have collided
int SDL_CollideBoundingBox(SDL_Rect a , SDL_Rect b);

#define GFX_H
#endif
