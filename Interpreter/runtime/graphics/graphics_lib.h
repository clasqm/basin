/*
   basin ,simple QBASIC interpreter.
   Copyright (C) 2009-2011  #pragma, Evg

   This program is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation,  either  version  3  of the License, or(at your option) any later
   version.

   This  program  is  distributed  in  the  hope  that  it  will  be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
   details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef GRAPHICS_LIB_H_INCLUDED
#define GRAPHICS_LIB_H_INCLUDED

#include <SDL/SDL.h>
#include <sys/time.h>
#include "types.h"

/* -------------------------------------------------------------------------- */
#ifndef DISABLE_SDL_GRAPHICS
#define TICK_INTERVAL  50
/* -------------------------------------------------------------------------- */
  typedef Uint32 _uint32_t;
  typedef Uint16 _uint16_t;
  typedef Uint8  _uint8_t;

  _uint32_t time_left (void);
  _ulong_t usec_stopper(void);
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_SDL_PROTECTION
  // For internal use for checking that pixel is in range
  typedef struct scrdims {
     _int_t width;
     _int_t height;
  } sdim_t;

  sdim_t scrdim;
#endif // DISABLE_SDL_PROTECTION
/* -------------------------------------------------------------------------- */
  /*
  * Set the pixel at (x, y) to the given value
  * NOTE: The surface must be locked before calling this!
  */
  inline int
  putpixel(SDL_Surface *surface, _int_t x, _int_t y, _uint32_t pixel)
  {

#ifndef DISABLE_SDL_PROTECTION
     // Check that pixel is inside the screen
     if ( x >= scrdim.width || y >= scrdim.height || x < 0 || y < 0 )
        return -1;
#endif // DISABLE_SDL_PROTECTION

     int bpp = surface->format->BytesPerPixel;
     /* Here p is the address to the pixel we want to set */
     _uint8_t *p = (_uint8_t *)surface->pixels + y * surface->pitch + x * bpp;

     switch(bpp) {
     case 1:
        *p = pixel;
        break;

     case 2:
        *(_uint16_t *)p = pixel;
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
        *(_uint32_t *)p = pixel;
        break;
     default: assert(0);
    }
    return 0;
  }
/* -------------------------------------------------------------------------- */
  /*
  * Put line on display at (x1, y1) -> (x2, y2)
  * NOTE: The surface must be locked before calling this!
  */
  inline void
  putline(SDL_Surface *display, _int_t *x1, _int_t *y1,_int_t *x2, _int_t *y2,
                                                            _uint32_t *pixdata)
  {
     _int_t l,dx,dy;
     _int_t xr = abs(*x2-*x1);
     _int_t yr = abs(*y2-*y1);
     if (xr > yr) {l=xr;} else {l=yr;}
     _int_t px = (*x1 << 12) + (1 << 11);//  1<<11 аналогично 0.5 у float
     _int_t py = (*y1 << 12) + (1 << 11);
     _int_t ex = (*x2 << 12) + (1 << 11);
     _int_t ey = (*y2 << 12) + (1 << 11);
     if (l != 0)
     {
        dx = (ex - px) / l;
        dy = (ey - py) / l;
     }
     else
     {
        dx = 0;
        dy = 0;
     }
     for (_int_t i = 0; i <= l; ++i)
     {
        putpixel( display,px >> 12, py >> 12,*pixdata);
        px += dx;
        py += dy;
     }

#ifndef DISABLE_SDL_PROTECTION
     /* This is guard for SDL_UpdateRect() function */
     if (*x1 > scrdim.width) *x1 = scrdim.width  ;
     if (*x2 > scrdim.width) *x2 = scrdim.width  ;
     if (*y1 > scrdim.width) *y1 = scrdim.height ;
     if (*y2 > scrdim.width) *y2 = scrdim.height ;
     if (*x1 < 0) *x1 = 0;
     if (*x2 < 0) *x2 = 0;
     if (*y1 < 0) *y1 = 0;
     if (*y2 < 0) *y2 = 0;
     if ( xr > scrdim.width)  xr = scrdim.width  ;
     if ( yr > scrdim.width)  yr = scrdim.height ;
#endif // DISABLE_SDL_PROTECTION

//Uint32 fps_lasttime = SDL_GetTicks(); //the last recorded time.
//Uint32 fps_current; //the current FPS.
//Uint32 fps_frames = 0; //frames passed since the last recorded fps.

// you would initalise these in a main method, really.
     /* Update just the part of the display that we've changed */
     if (*x1 < *x2){
        if (*y1 < *y2) { SDL_UpdateRect(display,*x1,*y1,xr,yr);
                         return;
        }
        else
        if (*y1 > *y2) { SDL_UpdateRect(display,*x1,*y2,xr,yr);
                         return;
        }
        SDL_UpdateRect(display,*x1,*y1,xr,1);
        return;

     }
     else
     if (*x1 > *x2) {
        if (*y1 < *y2) { SDL_UpdateRect(display,*x2,*y1,xr,yr);
                         return;
        }
        else
        if (*y1 > *y2) { SDL_UpdateRect(display,*x2,*y2,xr,yr);
                         return;
        }
        SDL_UpdateRect(display,*x2,*y1,xr,1);
        return;
     }
     // x1 == x2
     if (*y1 < *y2) { SDL_UpdateRect(display,*x1,*y1,1,yr);
                      return;
     }
     else
     if (*y1 > *y2) { SDL_UpdateRect(display,*x1,*y2,1,yr);
                      return;
     }
     SDL_UpdateRect(display,*x1,*y1,1,1);
     return;
  }
/* -------------------------------------------------------------------------- */
  inline void
  putcircle(SDL_Surface *display, _int_t *Xc, _int_t *Yc, _int_t *radius,
                                                             _uint32_t *pixdata)
  {
     _int_t x,y,delta,error;

     delta = 2 - (*radius << 1);
     x = 0;
     y = *radius;

     while (y >= 0)
     {
        putpixel(display, *Xc + x, *Yc + y ,*pixdata);
        putpixel(display, *Xc + x, *Yc - y, *pixdata);
        putpixel(display, *Xc - x, *Yc + y, *pixdata);
        putpixel(display, *Xc - x, *Yc - y, *pixdata);

        error = ((delta + y) << 1) - 1;
        if ((delta < 0) && (error <= 0)) {
           delta += ((++x) << 1) + 1;
           continue;
        }
        error = ((delta - x) << 1) - 1;
        if ((delta > 0) && (error > 0)) {
           delta += 1 - ((--y) << 1);
           continue;
        }
        ++x;
        delta += (x - y) << 1;
        --y;
     }
  }
/* -------------------------------------------------------------------------- */
_ulong_t usec_stopper(void)
{
    static struct timeval last_time    = {0, 0};
    static struct timeval current_time = {0, 0};

    last_time = current_time;
    gettimeofday(&current_time, NULL);

    return (current_time.tv_sec  - last_time.tv_sec) * 1000000 +
           (current_time.tv_usec - last_time.tv_usec);
}
/* -------------------------------------------------------------------------- */
_uint32_t time_left(void)
{
    static Uint32 next_time = 0;
    Uint32 now;

    now = SDL_GetTicks();
    if ( next_time <= now ) {
        next_time = now + TICK_INTERVAL;
        return(0);
    }
    return (next_time - now);
}
/* -------------------------------------------------------------------------- */
#endif // DISABLE_SDL_GRAPHICS
/* -------------------------------------------------------------------------- */
#endif // GRAPHICS_LIB_H_INCLUDED
