#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "IMG_savepng.h"
#include "gfx.h"
#include "scale2x.h"
#include "dirent.h"
#include "font.h"
#include "scandir.h"


#define SELECTED_SPRITE_VIEW_2X     0
#define SELECTED_SPRITE_VIEW_8X     1

char image_filename[255];       //the image we open for rotating
char output_filename[255];      //the image output filename
char only_filename[255];        //only the filename (no path included)

void Init_SDL()
{
	FILE *out;  //file pointer
	int i=0;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {

			//open error.txt if we failed
			out = fopen( "video_error.txt", "w" );

			if( out != NULL )
			{
				//report what went wrong
				fprintf(out,"Could not initialize SDL: %s\n", SDL_GetError());

				//close the file
				fclose(out);
			}

        //exit the program
		exit(1);
	}

	//make sure we close down SDL at exit
	atexit(SDL_Quit);

	screen=SDL_SetVideoMode(640,480,32,SDL_HWSURFACE);

		//if creating the screen surface failed
		if (screen == NULL) {

			//open error.txt
			out = fopen( "video_error.txt", "w" );

			if( out != NULL )
			{
				//report what went wrong
				fprintf(out,"Could not set video mode: %s\n", SDL_GetError());

				//close the file
				fclose(out);
			}

            //exit program
			exit(1);
		}

    Create_tripple_buffer(640,480);

    for(i=0;i<NUM_CHARACTERS;i++)
    {
        cleanfont_space[i] = 8;
    }
}

int Select_Image_To_Load(void)
{
	int done=0;
	struct dirent **filelist = {0};
	char *directory = "rotateme";
	char filename[80];
	int file_counter = -1;
	int i = 0;
	int text_xpos=0;
	int text_ypos=34;
	int selectbox_x=0,selectbox_y=0;
	int filenumber=-1;
	int Mousex=0,Mousey=0;

	SDL_Rect 	Rect_Mouse,
	            Rect_Close;


	//create an event
	SDL_Event event;

	Setup_Rectangle(&Rect_Close,576,0,64,32);

	//get how many files there are in the directory
	file_counter = scandir(directory, &filelist, 0, alphasort);

	//if there are no files
	if(file_counter < 0) {

		//return 0
		return 0;
	}

	//load the top bar
	TempImage = Load_And_Convert_Bitmap("data/topbar.png");

	while(!done)
	{

		//uppdate mouse position
        SDL_GetMouseState(&Mousex,&Mousey);

		//update the mouse rectangle
		Setup_Rectangle(&Rect_Mouse,Mousex,Mousey,1,1);

		//update where the little box is on screen
		selectbox_x = (Mousex/156)*156;
		selectbox_y = 3 + (Mousey/10)*10;

		while ( SDL_PollEvent(&event) )
		{

			switch (event.type)
			{
					case SDL_KEYUP:
					{

					}break;

				//if a mousebutton was released
				case SDL_MOUSEBUTTONUP:
				{
					//if it was the left one
					if( event.button.button == SDL_BUTTON_LEFT )
					{

						//if the user pressed the close button
						if(SDL_CollideBoundingBox(Rect_Close,Rect_Mouse))
						{
							//end the loop
							done=1;
							return 0;
						}

						//make sure the mouse is not on an illegal post
						if(Mousey>34 && Mousey<469 && Mousex<623)
						{
							//if the file exist
							if(filenumber<=file_counter-1)
							{
								//write the path tofilename
								sprintf(image_filename,"rotateme/%s",filelist[filenumber]->d_name);
								sprintf(only_filename,"%s",filelist[filenumber]->d_name);

								//close down file selection
								done =1;

								//returnera 1
								return 1;
							}
						}


					}

				}break;

			default:break;
			}

		}
		//lock screen surface
		Slock(screen);

            Clear_Surface(TrippleBuffer);

			//draw the topbar
			Draw_Image(TempImage,TrippleBuffer,0,0);

			//tell the user what to do
			Font_String(14,13,"Select which image you want to rotate.",TrippleBuffer,font_clean,cleanfont_space);

			//set text x & y position
			text_xpos=0; text_ypos=34;

				//loop through all filenames
				for(i = 2; i < file_counter; i++)
				{

					//write the filename
					sprintf(filename,"%s",filelist[i]->d_name);

						//display it on screen
						Font_String(text_xpos,text_ypos,filename,TrippleBuffer,font_clean,cleanfont_space);

						//move 10 pixels down to next row
						text_ypos+=10;

					//if we reached the end of the screen
					if(i == 45 || i == 89 || i == 133)
					{
						text_ypos=34;	//reset text y position
						text_xpos+=156;	//go to next column
					}
				}

		//convert the box x & y position the what filenumber we want to load

			//make sure the mouse is not on an illegal post
			if(Mousey>34 && Mousey<469 && Mousex<623)
			{
				//if its the first column
				if(selectbox_x==0){

					//convert y position to right filenummer
					filenumber = -1 + (selectbox_y-3)/10;
				}

                    //2nd column
					if(selectbox_x==156){

						//convert y position to right filenummer
						filenumber = 46 + (-3 + (selectbox_y-3)/10);
					}

					//3rd column
					if(selectbox_x==312){

						//convert y position to right filenummer
						filenumber = 90 + (-3 + (selectbox_y-3)/10);
					}

				//4th column
				if(selectbox_x==468){

					//convert y position to right filenummer
					filenumber = 134 + (-3 + (selectbox_y-3)/10);
				}
			}

				//if the mouse is below the top bar
				if(Mousey>34 && Mousey<469 && Mousex<623)
				{
					//draw the selection box
					Draw_Image(GFX_Select_Box,TrippleBuffer,selectbox_x,selectbox_y);
				}

            //copy the tripple buffer to screen
            Surface_Copy(TrippleBuffer,screen,0,0,640,480);

			//flip the screen
			SDL_Flip(screen);

		//unlock screen surface
		Sulock(screen);

	}

	free(filelist);
	return 0;
}

int main(int argc, char *argv[])
{
	int done=0;
	float angle=0;
	Uint8 *keys;
	int update_graphics=TRUE;
	int pixel_offset_8X=1;
	int selected_sprite_view = SELECTED_SPRITE_VIEW_8X;
	char msg[100];
	int fullscreen = FALSE;
	int show_save_msg=FALSE;
	int showcounter=0;

	Init_SDL();

    //set window caption
    SDL_WM_SetCaption("SolarStrings Sprite Rotater Version: 0.2",NULL);

	//get the clean font
	Get_Anim("data/cleanfont.png",TempImage,font_clean,0,0,8,8,NUM_CHARACTERS,16,1);

    //get the select box
    Get_Anim("data/selectbox.png",TempImage,&GFX_Select_Box,0,0,156,10,1,1,0);

    //if the image could not be loaded
    if(Select_Image_To_Load() == 0)
    {
        //exit the program
        exit(1);
    }

    //load the image the user wants to rotate
    Image2Rotate = Load_And_Convert_Bitmap(image_filename);

    //set pixel format to the format of the screen
    PixelFormat = Image2Rotate->format;

    //create the 2x scaled surface
    Rotated_2X = SDL_CreateRGBSurface(
    	SDL_SRCALPHA,
    	Image2Rotate->w*2,
    	Image2Rotate->h*2,
    	32,
    	PixelFormat->Rmask,
    	PixelFormat->Gmask,
    	PixelFormat->Bmask,
    	PixelFormat->Amask
	);

    //SDL_SetColorKey(Rotated_2X,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(Rotated_2X->format,255,0,255));

    //create the 4x scaled surface
    Rotated_4X = SDL_CreateRGBSurface(
    	SDL_SRCALPHA,
    	Image2Rotate->w*4,
    	Image2Rotate->h*4,
    	32,
    	PixelFormat->Rmask,
    	PixelFormat->Gmask,
    	PixelFormat->Bmask,
    	PixelFormat->Amask
	);

    //SDL_SetColorKey(Rotated_4X,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(Rotated_4X->format,255,0,255));

    //create the 8x scaled surface
    Rotated_8X = SDL_CreateRGBSurface(
    	SDL_SRCALPHA,
    	Image2Rotate->w*8,
    	Image2Rotate->h*8,
    	32,
    	PixelFormat->Rmask,
    	PixelFormat->Gmask,
    	PixelFormat->Bmask,
    	PixelFormat->Amask
	);

    //SDL_SetColorKey(Rotated_8X,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(Rotated_8X->format,255,0,255));

    //create the scaled surfaces
    scale2x(Image2Rotate,Rotated_2X);
    scale2x(Rotated_2X,Rotated_4X);
    scale2x(Rotated_4X,Rotated_8X);

    //remove the 4x surface, since we won't need it any longer
    SDL_FreeSurface(Rotated_4X);

    //create the SDL event
    SDL_Event event;

	while(!done)
	{
	    //get key states
        keys = SDL_GetKeyState(NULL);

		while ( SDL_PollEvent(&event) )
		{
			switch (event.type)
			{

			    case SDL_KEYUP:
				{
				    //if the user releases the left key
				    if ( event.key.keysym.sym == SDLK_LEFT)
				    {
				        //set update graphics to true
				        update_graphics = FALSE;

				        selected_sprite_view = SELECTED_SPRITE_VIEW_8X;
				    }
				    //if the user releases the right key
				    if ( event.key.keysym.sym == SDLK_RIGHT)
				    {
				        //set graphics update to true
				        update_graphics = FALSE;

				        selected_sprite_view = SELECTED_SPRITE_VIEW_8X;
				    }

                    //if the user pressed the escape key
				    if ( event.key.keysym.sym == SDLK_ESCAPE)
				    {
                        //quit the program
				        done=1;
				    }

                    //if the user presses the s key
				    if ( event.key.keysym.sym == SDLK_s)
				    {

                        //create the rotated surface from the 8x scale2x surface
                        RotateResult_8X = Rotate_Surface(Rotated_8X,angle,0.125,pixel_offset_8X);

                        //create the save surface
                        SaveSurface = SDL_CreateRGBSurface(
                        	SDL_SRCALPHA,
                        	RotateResult_8X->w/8,
                        	RotateResult_8X->h/8,
                        	32,
                        	PixelFormat->Rmask,
                        	PixelFormat->Gmask,
                        	PixelFormat->Bmask,
                        	PixelFormat->Amask
                    	);

                        //SDL_SetColorKey(SaveSurface,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(SaveSurface->format,255,0,255));

				        //save the 8X image
				        sprintf(output_filename,"output/angle%.1f_offset_%d_%s",angle,pixel_offset_8X,only_filename);
				        
				        Surface_Copy(
				        	RotateResult_8X,
				        	SaveSurface,
				        	0,0,
				        	RotateResult_8X->w/8,
				        	RotateResult_8X->h/8
			        	);
				        
				        IMG_SavePNG(output_filename,SaveSurface,9);

				        show_save_msg = TRUE;
				        showcounter=1500;
				    }
				}
				break;

				case SDL_KEYDOWN:
				{
                    //if the player presses "a", rotate the image 1 degre left
					if (event.key.keysym.sym == SDLK_a)
                        {
                        angle--;

                            if(angle<0)
                            {
                                angle = 360;
                            }
                        update_graphics = TRUE;
                        }

                    //if the player presses "a"  rotate the image 1 degree right
					if (event.key.keysym.sym == SDLK_d)
                    {
                        angle++;

                            if(angle>360)
                            {
                                angle = 0;
                            }
                        update_graphics = TRUE;
                    }

                    //if the player presses "q", rotate the image 0.1 degre left
					if (event.key.keysym.sym == SDLK_q)
                        {
                        angle-=0.1;

                            if(angle<0)
                            {
                                angle = 360;
                            }
                        update_graphics = TRUE;
                        }

                    //if the player presses "e"  rotate the image 0.1 degree right
					if (event.key.keysym.sym == SDLK_e)
                    {
                        angle+=0.1;

                            if(angle>360)
                            {
                                angle = 0;
                            }
                        update_graphics = TRUE;
                    }

                    //decrease pixel offset
					if (event.key.keysym.sym == SDLK_z)
                    {
                        pixel_offset_8X--;

                        //make sure we don't go to low
                        if(pixel_offset_8X<=0){ pixel_offset_8X=1; }

                        update_graphics = TRUE;
                    }

                    //increase pixel offset
					if (event.key.keysym.sym == SDLK_c)
                    {
                        pixel_offset_8X++;

                        //make sure we don't go to far
                        if(pixel_offset_8X>=8){ pixel_offset_8X=8; }

                        update_graphics = TRUE;
                    }

                        //if the user presses F4
					    if(event.key.keysym.sym == SDLK_F4)
					    {
                            if(fullscreen==FALSE)
                            {
                                //set fullscreen mode
                                screen=SDL_SetVideoMode(640,480,32,SDL_HWSURFACE|SDL_FULLSCREEN);
                                fullscreen=TRUE;
                            }
                            //if we're in fullscreen mode
                            else if(fullscreen==TRUE)
                            {
                                //Set 640x480x32bpp windowed mode (In linux SDL has a bug which sometimes sets y to some strange values
                                //when you go from fullscrenn -> windowed mode. If we set it two times, it corrects itself, don't ask me why :)
                                screen=SDL_SetVideoMode(640,480,32,SDL_HWSURFACE);
                                screen=SDL_SetVideoMode(640,480,32,SDL_HWSURFACE);
                                fullscreen = FALSE;
                            }
					    }


				}
				break;
			}

        }
                    //if the user presses left & no shift key is pressed
 					if(keys[SDLK_LEFT])
					{
                        angle--;

                            if(angle<0)
                            {
                                angle = 360;
                            }
                        update_graphics = TRUE;

                        //while rotating, we do it in 2x, since it's a lot faster than 8x
                        selected_sprite_view = SELECTED_SPRITE_VIEW_2X;
					}

                    //if the user presses right & no shift key is pressed
                    if(keys[SDLK_RIGHT])
					{
                        angle++;

                            if(angle>360)
                            {
                                angle = 0;
                            }
                        update_graphics = TRUE;

                        //while rotating, we do it in 2x, since it's a lot faster than 8x
                        selected_sprite_view = SELECTED_SPRITE_VIEW_2X;
					}

        //lock the screen surface
        Slock(screen);

        //if it's time to update the graphics
        if(update_graphics == TRUE)
        {
            //clear the tripple buffer
            Clear_Surface(TrippleBuffer);

            if(selected_sprite_view == SELECTED_SPRITE_VIEW_2X)
            {
                if(Rotated_2X!=NULL)
                {
                    SDL_FreeSurface(RotateResult_2X);
                }

                //create the rotated surface from the 2x scale2x surface
                RotateResult_2X = Rotate_Surface(Rotated_2X,angle,0.5,1);

                //draw the result on screen
                if(RotateResult_2X != NULL)
                {
	                Draw_Image(
	                	Rotated_2X,
	                	TrippleBuffer,
	                	320-RotateResult_2X->w/4,
	                	220-RotateResult_2X->h/4
	            	);
            	}
            }

            if(selected_sprite_view == SELECTED_SPRITE_VIEW_8X)
            {
                if(RotateResult_8X!=NULL)
                {
                    SDL_FreeSurface(RotateResult_8X);
                }
                //create the rotated surface from the 2x scale2x surface
                RotateResult_8X = Rotate_Surface(Rotated_8X,angle,0.125,pixel_offset_8X);

                //draw the result on screen if its not a NULL surface
                if(RotateResult_8X != NULL)
                {
                    Draw_Image(
                    	RotateResult_8X,
                    	TrippleBuffer,
                    	320-RotateResult_8X->w/16,
                    	220-RotateResult_8X->h/16
                	);
                }
            }
        }
            //copy the tripple buffer to screen
            Surface_Copy(TrippleBuffer,screen,0,0,640,480);

            sprintf(msg,"Left/Right = rotate image, S = save,F4 = Toggle Fullscreen, ESC = quit");
            Font_String(0,0,msg,screen,font_clean,cleanfont_space);
            sprintf(msg,"A/D = rotate 1 degree, Q/E = rotate 0.1 degree");
            Font_String(0,10,msg,screen,font_clean,cleanfont_space);
            sprintf(msg,"Z/C = choose pixel offset");
            Font_String(0,20,msg,screen,font_clean,cleanfont_space);
            sprintf(msg,"angle:%.1f,offset:%d",angle,pixel_offset_8X);
            Font_String(0,470,msg,screen,font_clean,cleanfont_space);

            if(show_save_msg==TRUE)
            {
                showcounter--;

                    if(showcounter<=0)
                    {
                        showcounter=0;
                        show_save_msg=FALSE;
                    }
                Font_String(0,30,"Rotated sprite saved",screen,font_clean,cleanfont_space);
            }

			//flip the screen
			SDL_Flip(screen);

		//unlock screen surface
		Sulock(screen);
	}

	return 0;
}
