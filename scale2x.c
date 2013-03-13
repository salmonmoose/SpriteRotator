/*
Credits: sebt3
www.gp32x.com
*/

#include "scale2x.h"

#define B sp[i - b]
#define D sp[i - (i>0?1:0)]
#define F sp[i + (i<wd?1:0)]
#define H sp[i + h]
#define E  sp[i]
#define E0 tp[i*2]
#define E1 tp[i*2 + 1]
#define E2 tp[i*2 + tpitch]
#define E3 tp[i*2 + 1 + tpitch]

//Scale a surface with the scale2x algorithm
void scale2x(SDL_Surface* surface, SDL_Surface* dest)
{
    register int i, j;
    int b, h;
    int bpp = surface->format->BytesPerPixel;
    if (SDL_MUSTLOCK(dest) != 0)
    {
      if (SDL_LockSurface(dest) < 0)
      {
        fprintf(stderr, "dest locking failed");
        return;
      }
    }

    const int wd = ((dest->w / 2) < (surface->w)) ? (dest->w / 2) : (surface->w);
    const int hg = ((dest->h) < (surface->h*2)) ? (dest->h / 2) : (surface->h);

    switch (bpp)
    {
      case 2:
      {
        int tpitch = dest->pitch / 2;
        int spitch = surface->pitch / 2;
        Uint16* tp = (Uint16*) dest->pixels;
        Uint16* sp = (Uint16*) surface->pixels;

        for (j = 0; j < hg; ++j)
        {
         b = j>0?spitch:0;
         h = j<hg?spitch:0;
          for (i = 0; i < wd; ++i)
          {
                if (B != H && D != F) {
                        E0 = D == B ? D : E;
                        E1 = B == F ? F : E;
                        E2 = D == H ? D : E;
                        E3 = H == F ? F : E;
                } else {
                        E0 = E;
                        E1 = E;
                        E2 = E;
                        E3 = E;
                }
          }
          tp += 2*tpitch;
          sp += spitch;
        }

        break;
      }
      case 4:
      {
        int tpitch = dest->pitch / 4;
        int spitch = surface->pitch / 4;
        Uint32* tp = (Uint32*) dest->pixels;
        Uint32* sp = (Uint32*) surface->pixels;

        for (j = 0; j < hg; ++j)
        {
         b = j>0?spitch:0;
         h = j<hg?spitch:0;
          for (i = 0; i < wd; ++i)
          {
                if (B != H && D != F) {
                        E0 = D == B ? D : E;
                        E1 = B == F ? F : E;
                        E2 = D == H ? D : E;
                        E3 = H == F ? F : E;
                } else {
                        E0 = E;
                        E1 = E;
                        E2 = E;
                        E3 = E;
                }
          }
          tp += 2*tpitch;
          sp += spitch;
        }

        break;
      }
      default:
        fprintf(stderr, "Unsupported bitdepth.n");
        break;
    }


    if (SDL_MUSTLOCK(dest) != 0)
    {
      SDL_UnlockSurface(dest);
    }
}

