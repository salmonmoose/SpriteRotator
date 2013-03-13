#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "gfx.h"

//This function locks a surface
void Slock(SDL_Surface *screen)
{
  if ( SDL_MUSTLOCK(screen) )
  {
    if ( SDL_LockSurface(screen) < 0 )
    {
      return;
    }
  }
}

//This function unlocks a surface
void Sulock(SDL_Surface *screen)
{
  if ( SDL_MUSTLOCK(screen) )
  {
    SDL_UnlockSurface(screen);
  }
}
//This function clears a surface
void Clear_Surface(SDL_Surface *dst)
{
    SDL_FillRect(dst, NULL, 0x000000);
}

void PutPixel(SDL_Surface* Surface, int x, int y, Uint32 pixel)
{
    int bpp = Surface->format->BytesPerPixel;
    /* p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)Surface->pixels + y * Surface->pitch + x * bpp;
    switch(bpp) {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16 *)p = pixel;
            break;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4:
            *(Uint32 *)p = pixel;
            break;
     default:break;
    }
}

Uint32 GetPixel(SDL_Surface* Surface, int x, int y)
{
    int bpp = Surface->format->BytesPerPixel;
    /* p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)Surface->pixels + y * Surface->pitch + x * bpp;
    switch(bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                return p[0] << 16 | p[1] << 8 | p[2];
            } else {
                return p[0] | p[1] << 8 | p[2] << 16;
            }
            break;
        case 4:
            return *(Uint32 *)p;
        default:
            return 0;
    }
}
//this function will rotate a surface (a bit slow, but ok for our purpose)
SDL_Surface *Rotate_Surface (SDL_Surface *src,float angle, float scale,int pixel_offset)
{
int x=0,y=0;
float sourcex=0,sourcey=0;

//make sure the pixel offset is above zero
//otherwise the for loops for scaling the image won't work.
if(pixel_offset<=0)
{
    pixel_offset=1;
}

    //convert angle to radians
    float radians = (angle*2*PI) / 360;

    //calculate cosine and sine
	float cosine = (float)cos(radians);
	float sine = (float)sin(radians);


    //calculate the size of the new image
    float Point1x=(-src->h*sine);
    float Point1y=(src->h*cosine);
    float Point2x=(src->w*cosine-src->h*sine);
    float Point2y=(src->h*cosine+src->w*sine);
    float Point3x=(src->w*cosine);
    float Point3y=(src->w*sine);

    float minx=min(0,min(Point1x,min(Point2x,Point3x)));
    float miny=min(0,min(Point1y,min(Point2y,Point3y)));
    float maxx=max(Point1x,max(Point2x,Point3x));
    float maxy=max(Point1y,max(Point2y,Point3y));

		if(angle>90 && angle<180)
		{
		maxx=0;
		}
		if(angle>180 && angle<270)
		{
		maxy=0;
		}

        //calculate the destination image width & height
        int dest_width = (int)ceil(abs(maxx) - minx);
        int dest_height = (int)ceil(abs(maxy) - miny);

    // the destination surface
    SDL_Surface *dest;

    //get current pixel format
    PixelFormat = src->format;

    //create the new surface
    dest = SDL_CreateRGBSurface(SDL_HWSURFACE,dest_width,dest_height,32,PixelFormat->Rmask,PixelFormat->Gmask,PixelFormat->Bmask,0);

        if(dest == NULL)
        {

        }
        else
        {

            Slock(dest);

            //fill it with magenta
            SDL_FillRect(dest, NULL, 0xff00ff);

            //set transparent color to magenta
            SDL_SetColorKey(dest,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(dest->format,255,0,255));

            Sulock(dest);

            //lock the source surface
            Slock(src);

            //loop through the image
            for(x=0;x<dest_width;x+=pixel_offset)
            {
                for(y=0;y<dest_height;y+=pixel_offset)
                {
                    //calculate the new pixel we want to copy
                    sourcex=((x+minx)*cosine+(y+miny)*sine);
                    sourcey=((y+miny)*cosine-(x+minx)*sine);

                    //if the pixel exist on the src surface
                    if(sourcex >=0 && sourcex <src->w && sourcey >=0 && sourcey < src->h)
                    {
                        //get it from the src surface and place it on the dest surface
                        //multipy with scale
                        PutPixel(dest,x*scale,y*scale,GetPixel(src,sourcex,sourcey));
                    }
                }
            }

        //unlock the source surface
        Sulock(src);

        //return the rotated destination surface
        return dest;
    }
    return NULL;
}


//surface converter
SDL_Surface *Load_And_Convert_Bitmap(char *file)
{
FILE *out;

	//temporary surface
	SDL_Surface* temp_image;

	//converted surface
	SDL_Surface* converted_image;

	//load the image
	temp_image = IMG_Load(file);

		//if we could load the image
		if(temp_image != 0)
		{
			//convert the surface to displayformat
			converted_image = SDL_DisplayFormat(temp_image);

			//remove surface
			SDL_FreeSurface(temp_image);
		}
		//if we could not load the file
		else
		{
			//open error.txt
			out = fopen( "error.txt", "wb" );

			//if all is ok
			if( out != NULL )
			{
				//write what went wrong
				fprintf(out,"Could not load image:%s\n",file);

				//close the file
				fclose(out);
			}
		//exit the program
		exit(1);
		}

	//return the converted image
	return converted_image;
}

//this function will draw an image on the desired surface
void Draw_Image(SDL_Surface *img,SDL_Surface *where, int x, int y)
{
  //set up the destination rectangle
  SDL_Rect dest;
  dest.x = x;
  dest.y = y;

  //Draw it on the screen
  SDL_BlitSurface(img, NULL, where, &dest);
}

//This cunction copies a portion of a surface to another
void Surface_Copy(SDL_Surface *from, SDL_Surface *where,int x,int y,int w,int h)
{
	int width=0,height=0;	//width & height of surface
	width = w-x;		    //calculate width
	height=h-y;		        //calculate height

	//Rectangle of the original surface
	SDL_Rect Rsrc;
	Rsrc.x=x;
	Rsrc.y=y;
	Rsrc.w=w;
	Rsrc.h=h;

	//Rectangle of the new surface
	SDL_Rect Rdst;
	Rdst.x=0;
	Rdst.y=0;
	Rdst.w=width;
	Rdst.h=height;

		//make the copy
		SDL_BlitSurface(from, &Rsrc, where, &Rdst);
}

void Create_tripple_buffer(int width,int height)
{
FILE *out;

	PixelFormat = screen->format;

	TrippleBuffer = SDL_CreateRGBSurface(SDL_HWSURFACE,width,height,32,PixelFormat->Rmask,PixelFormat->Gmask,PixelFormat->Bmask,0);

		if(TrippleBuffer==NULL)
		{

			out = fopen( "Error.txt", "w" );

			if( out != NULL )
			{
				fprintf(out,"could not create tripple buffer\n");

				fclose(out);
			}

			exit(1);
		}
}


//This function clips many sprites and place them in an array
void Get_Anim(char *file,SDL_Surface *image, SDL_Surface *anim[],int x,int y,int w,int h,int num_frames, int num_sprites_per_line,int grid_width)
{
	FILE *out;	//filepointer

	int i=0,j=0;
	int width=0;
	int height=0;
	int num_rows=0;
	int xs=0;
	SDL_Surface *convert = NULL;

	width  = w-x;
	height = h-y;
	xs = x;

	//calculate number of rows in the image
	num_rows = num_frames/num_sprites_per_line+1;

		//load the image and convert it to current screen format
		image = Load_And_Convert_Bitmap(file);

		//get the pixelformat
		PixelFormat = image->format;

			//if we could not load the image
			if(image==NULL)
			{
				//open error.txt
				out = fopen( "error.txt", "wb" );

				//if the file was opened
				if( out != NULL )
				{
					//write what went wrong
					fprintf(out,"could not load image: %s\n",file);

					//clos the file
					fclose(out);
				}
			//exit program
			exit(1);
			}

	//go through all the frames
	for(i=0;i<num_frames;i++)
	{
		for(j=1;j<num_rows;j++)
		{
			//if we're at the end of a row
			if(i==num_sprites_per_line*j)
			{
				//go to next row
				y+=height+grid_width;
				x=xs;
			}
		}
		//create a surface for each new sprite
		convert = SDL_CreateRGBSurface(SDL_HWSURFACE,width,height,32,PixelFormat->Rmask,PixelFormat->Gmask,PixelFormat->Bmask,0);
		anim[i] = SDL_DisplayFormat(convert);

			//if we could not create a surface
			if(anim[i] == NULL)
			{
				//open error.txt
				out = fopen( "Error.txt", "w" );

				//om det gick att öppna Error.txt
				if( out != NULL )
				{
					//write what went wrong
					fprintf(out,"could not create surface from image: %s, sprite frame %d",file,i);

					//close the file
					fclose(out);
				}
			//exit the program
			exit(1);
			}

		//magenta as transparent color
		SDL_SetColorKey(anim[i],SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(anim[i]->format,255,0,255));

		//copy the sprite
		Surface_Copy(image, anim[i],x,y,x+w,y+h);

		//go to next sprite in the image
		x+=width+grid_width;
	}
//free the allocated memory
SDL_FreeSurface(image);
SDL_FreeSurface(convert);
}

void Setup_Rectangle(SDL_Rect *Rect,int x,int y,int width,int height)
{
Rect->x=x;
Rect->y=y;
Rect->w=width;
Rect->h=height;
}

//this function checks if two boxes have collided
int SDL_CollideBoundingBox(SDL_Rect a , SDL_Rect b)
{
	if(b.x + b.w < a.x)	return 0;	//kolla ifall de två
	if(b.x > a.x + a.w)	return 0;	//rektanglarna rör vid varandra

	if(b.y + b.h < a.y)	return 0;
	if(b.y > a.y + a.h)	return 0;

	return 1;
}
