Solarstrings Sprite Rotater
---------------------------
Version 0.2
Code: C
Libraries: SDL, SDL_image, libpng
IDE: Code::Blocks

USE AT OWN RISK!!!
------------------
This Code & program comes with no warranty what so ever!
Whatever may happen if you use this code or program is your
own responsibility.

Changes 0.1b -> v0.2
--------------------
- Fixed a memory leak
- Toggle Fullscreen (F4)
- Removed 2x & 4x Image output
- It's now possible to tweak the image output to the best version using the following:
 * Rotate image with 0.1 degrees (Keys Q/E)
 * Manually choose offset 1-8 (Keys Z/C)


Usage:
------
Place the images you want to rotate in the "rotateme" folder
Rotated images will be saved in the "output" folder.

Supported Image formats: BMP, PNM (PPM/PGM/PBM), XPM, LBM, PCX, GIF, JPEG, PNG, TGA, and TIFF

Output format is .png

Compiling in windows
--------------------
Change the paths in the code blocks project 
to where you have your mingw32 setup

Currently the path is:
c:\program files (x86)\codeblocks\Mingw32\


This program uses code from the following:
--------------------------------------------
"scale2x", by sebt3 on www.gp32x.com
"IMG_savepng", by Philip D. Bober
LazyFoo - Good tutorials for SDL by this Guy
--------------------------------------------

Hi!

I was working on my Indie game "The Forgotten Planet" ( http://theforgottenplanet.blogspot.com ) and wanted to do better rotated sprites than the usual "jagged" ones. I searched the net to see if I could find something interesting.
Almost right away I found the program "RotSprite" by Xenowhirl, and tried the program. The results was by far the best I've seen from any algorithm. There was only one problem though: No Source Code!
Anyway, he did not leave me totally clueless. He describes how the program works here: http://forums.sonicretro.org/index.php?showtopic=8848&st=15&p=159754&#entry159754

Quoted from the website (link above):
-------------------------------------
"The algorithm is dead simple, so I'll just describe it and you can decide: First it scales the image to 8x size, using a "pixel guessing" algorithm to add detail. Then it scales the image to 1/8 size and also rotates it using standard aliased rotation and scaling. That's basically it. To get a big speed increase for a small penalty in quality, you could use 4x instead of 8x and skip some other optional steps I did, but I wanted high quality above all else.

Here's the more detailed version: First it scales the image to double size using something similar to Scale2x, but checking if pixels are more similar to each other instead of if they're equal, which makes the result less blocky and ultimately leads to smoother output. The important thing is that the scaling algorithm works by choosing a pixel instead of by blending pixels. It does that 3 times to achieve an 8x scale, determined empirically to be a good place to stop. Then (optional step) it tries to find the best rotation offsets by looping through a small grid of offsets between 0 and 7 pixels in x and y, and for each one looping through the 8x image at the rotation angle with step size 8 pixels, and adding the squared distance of the difference in color components between each point and its immediate (1 pixel) neighbors in the 8x image, which will be 0 except on the boundaries between 8x pixels. Then it simply performs standard nearest-neighbor scaling+rotation, starting at the offsets that gave minimal sum of squared differences, and using 1/8 scale to return the image to normal size while rotating it. Finally (optional step), it tries to fix any overlooked single-pixel details by checking for any pixels in the output image that have 3 or 4 identical neighbors equal to them and unequal to the color at the corresponding place in the source image, and replacing those pixels with the one from the source image.

You might say I'm cheating by not vectorizing the graphics into lines and curves and rotating those, but I say this method is an approximation of that and works better in practice. When the original image has little detail at the angle it's being sampled, aliased rotation makes too many arbitrary decisions, but the smoothing of a pixel-choosing enlargement algorithm is sufficient to resolve most of the ambiguity."

End Quote

To most of us,"the more detailed version" is a lot of "mumbo-jumbo" (Lots of extra calculations and math and.... stuff) so I decided to try a more simple aproach to it and see what results I could produce.
I decided to keep the scale2x part, because that sounded just right & I could see how this could be what improved the image.

And Lo And Behold! I came up with something very similar with good results: 
See example.png in the example folder. (The alien is the main character for "The Forgotten planet")

My aproach is the following:

1. Scale the image with scale2x, creating 3 different sizes: 2x, 4x & 8x.
2. Rotate & scale down each result to normal size.
3. Save the results scaled down from 2x,4x and 8x

Why 3 different Images? I found that sometimes small details look better in one of the other versions. 
Saving all three versions will give the artist the posibility to cut & paste in-between the images to produce the best looking sprite, thus saving a lot of time with manually pixeling in the missing details.



