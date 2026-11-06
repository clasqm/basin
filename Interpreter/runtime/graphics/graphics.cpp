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
#include <cassert>
#include <map>
   using std::map;
#include <algorithm>
   using std::min;

#include "types.h"
#include "error_handler.h"
#include "graphics.h"
#include "graphics_lib.h"
/* -------------------------------------------------------------------------- */
#ifndef DISABLE_SDL_GRAPHICS
/* -------------------------------------------------------------------------- */
  enum ScreenModeW { // Screen width
     x320  = 320,
     x640  = 640,
     x800  = 800,
     x1400 = 1400
  };
/* -------------------------------------------------------------------------- */
  enum ScreenModeH { // Screen height
     x240 = 240,
     x480 = 480,
     x600 = 600,
     x900 = 900
  };
/* -------------------------------------------------------------------------- */
  enum BitDepth {
     _4bit  = 4,
     _8bit  = 8,
     _16bit = 16,
     _24bit = 24,
     _32bit = 32
  };
/* -------------------------------------------------------------------------- */
  struct RGBColors {
     _uint32_t  black,          dark_blue,
                dark_green,     dark_cyan,
                dark_red,       dark_magenta,
                dark_yellow,    light_gray,
                dark_gray,      blue,
                green,          cyan,
                red,            magenta,
                yellow,         white;
  };
/* -------------------------------------------------------------------------- */
  class Graphics {
     private:_bool_t initialized;
             _bool_t last_display_locked;
             _bool_t display_created;
             SDL_Surface *last_display;
             ScreenModeW width;
             ScreenModeH height;
             BitDepth depth;
             _uint32_t flags;
             _uint32_t last_color;
             _uint32_t default_color;
             //SDL_Thread *upd_screen_thread;

     public: Graphics ();
             ~Graphics ();
             SDL_Surface *get_last_display () {return last_display;};
             void set_last_display (SDL_Surface *display)
                                                      {last_display = display;};
             _bool_t isinit () const {return initialized;}
             void    set_init(_bool_t flag) {initialized = flag;};
             void SetRGBColors();
             _bool_t IsDispLocked() const {return last_display_locked;};
             void SetIsDispLocked(_bool_t mode) {last_display_locked = mode;};
             _bool_t is_created()const {return display_created;};

             void set_created(_bool_t c){
                display_created = c;
                //upd_screen_thread = SDL_CreateThread(upd_screen_func, last_display);
                //if ( upd_screen_thread == NULL ) {
                //   fprintf(stderr, "Unable to create thread: %s\n", SDL_GetError());
                //}
             };

             void set_width(ScreenModeW w){width = w;};
             void set_height(ScreenModeH h){height = h;};
             void set_depth(BitDepth d) {depth = d;};
             void set_flags(_uint32_t f){flags = f;};
             void set_last_color(_uint32_t c){last_color = c;};
             void set_def_color(_uint32_t c){default_color = c;};

             ScreenModeW get_width() const {return width;};
             ScreenModeH get_height() const {return height;};
             BitDepth get_depth() const {return depth;};
             _uint32_t get_flags() const {return flags;};
             _uint32_t get_last_color() const {return last_color;};
             _uint32_t get_def_color() const {return default_color;};
             void upd_screen_func (SDL_Surface *screen, int x,int y,  int w,
                                                                       int h);
  };
/* -------------------------------------------------------------------------- */
  inline Graphics::Graphics ()
  {
     initialized = false;
     last_display_locked = false;
     display_created = false;
  }
/* -------------------------------------------------------------------------- */
  inline Graphics::~Graphics ()
  {
     bool flag = false;
     if (initialized)
     {// FIXME !
        SDL_Event event;
        cout << "\nPress any key to continue.\n";
        while (true)
        {
           SDL_PollEvent(&event);
           switch (event.type)
           {
              case SDL_KEYDOWN:
                 //SDL_WaitThread(upd_screen_thread, NULL);
                 SDL_Quit();flag = true;
              break;
              default:;
           }
           SDL_Delay(1);
           if (flag) break;
        }
     }
  }
/* -------------------------------------------------------------------------- */
  RGBColors colors;
/* -------------------------------------------------------------------------- */
  void Graphics::SetRGBColors()
  {
     colors.black        = SDL_MapRGB(last_display->format,0x00,0x00,0x00);
     colors.dark_blue    = SDL_MapRGB(last_display->format,0x00,0x00,0x80);
     colors.dark_green   = SDL_MapRGB(last_display->format,0x00,0x80,0x00);
     colors.dark_cyan    = SDL_MapRGB(last_display->format,0x00,0x80,0x80);
     colors.dark_red     = SDL_MapRGB(last_display->format,0x80,0x00,0x00);
     colors.dark_magenta = SDL_MapRGB(last_display->format,0x80,0x00,0x80);
     colors.dark_yellow  = SDL_MapRGB(last_display->format,0x80,0x80,0x00);
     colors.light_gray   = SDL_MapRGB(last_display->format,0xc0,0xc0,0xc0);
     colors.dark_gray    = SDL_MapRGB(last_display->format,0x80,0x80,0x80);
     colors.blue         = SDL_MapRGB(last_display->format,0x00,0x00,0xff);
     colors.green        = SDL_MapRGB(last_display->format,0x00,0xff,0x00);
     colors.cyan         = SDL_MapRGB(last_display->format,0x00,0xff,0xff);
     colors.red          = SDL_MapRGB(last_display->format,0xff,0x00,0x00);
     colors.magenta      = SDL_MapRGB(last_display->format,0xff,0x00,0xff);
     colors.yellow       = SDL_MapRGB(last_display->format,0xff,0xff,0x00);
     colors.white        = SDL_MapRGB(last_display->format,0xff,0xff,0xff);
     default_color = colors.black;
     last_color    = colors.white;
  }
/* -------------------------------------------------------------------------- */
  void Graphics::upd_screen_func (SDL_Surface *screen, int x,int y,  int w,
                                                                     int h) {

     if (time_left() == TICK_INTERVAL) {

        if (SDL_Flip(screen) != 0) {
            std::cerr << "Failed to swap the buffers: "
                      << SDL_GetError()
                      << std::endl;
        }
        else SDL_UpdateRect((SDL_Surface *) screen, x, y, w, h);
     }
  }
/* -------------------------------------------------------------------------- */
  Graphics graphics;
/* -------------------------------------------------------------------------- */
  void graphics_SCREEN (val_Arr_t *params)
  {
     switch (params->at(0)->Get_ival())
     {
        case 12: // SCREEN 12
           if (!graphics.isinit()) {
              if ( SDL_Init( SDL_INIT_VIDEO ) < 0)
                 error(CANNOT_INIT_VIDEO_SUBSYS);
              graphics.set_init(true);

           }
           if ( SDL_VideoModeOK (ScreenModeW(x640),
                                 ScreenModeH(x480),
                                 BitDepth(_16bit),
                                 SDL_HWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF))
           {
              graphics.set_width(x640);
              graphics.set_height(x480);
              graphics.set_depth(_16bit);
              graphics.set_flags(SDL_HWSURFACE | SDL_ANYFORMAT |
                                 SDL_DOUBLEBUF);
#ifndef DISABLE_SDL_PROTECTION
              // This numbers are passed to putpixel() and putline() to check
              // that pixel is on screen before drawing.
              scrdim.width  = x640;
              scrdim.height = x480;
#endif // DISABLE_SDL_PROTECTION
           }
           else error(CANNOT_SET_DISPLAY);
        break;
        default: error(UNKNOWN_VIDEO_MODE);
     }
     if (!graphics.is_created())
     {
        SDL_Surface *display;
        display = SDL_SetVideoMode (graphics.get_width(),graphics.get_height(),
                                    graphics.get_depth(),graphics.get_flags() );
        assert (display);
        graphics.set_last_display(display);
        graphics.SetRGBColors();
        graphics.set_created(true);
        return;
     }
  }
/* -------------------------------------------------------------------------- */
  void graphics_PRESET (val_Arr_t *params)
  {
     if (!graphics.isinit())
        error(MUST_SET_DISPLAY);
     _int_t x = params->at(0)->Get_ival();
     _int_t y = params->at(1)->Get_ival();
     size3 size = params->size();
     if (size == 3)
     {
        _uint32_t color(0);
        switch (params->at(2)->Get_ival())
        {
           case 0: color = colors.black;break;
           case 1: color = colors.dark_blue;break;
           case 2: color = colors.dark_green;break;
           case 3: color = colors.dark_cyan;break;
           case 4: color = colors.dark_red;break;
           case 5: color = colors.dark_magenta;break;
           case 6: color = colors.dark_yellow;break;
           case 7: color = colors.light_gray;break;
           case 8: color = colors.dark_gray;break;
           case 9: color = colors.blue;break;
           case 10:color = colors.green;break;
           case 11:color = colors.cyan;break;
           case 12:color = colors.red;break;
           case 13:color = colors.magenta;break;
           case 14:color = colors.yellow;break;
           case 15:color = colors.white;break;
           default: error(UNKNOWN_COLOR_NUMBER);
        }
        SDL_Surface *display = graphics.get_last_display();
        /* Lock the screen for direct access to the pixels */
        if ( SDL_MUSTLOCK(display) ) {
           if ( SDL_LockSurface(display) < 0 ) {
               fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
               return;
           }
        }

        putpixel(display, x, y, color);

        if ( SDL_MUSTLOCK(display) ) {
           SDL_UnlockSurface(display);
        }

#ifndef DISABLE_SDL_PROTECTION
        /* This is guard for SDL_UpdateRect() function */
        if (x > scrdim.width) x = scrdim.width  ;
        if (y > scrdim.width) y = scrdim.height ;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
#endif // DISABLE_SDL_PROTECTION

        graphics.upd_screen_func(display ,x,y,1,1);
        /* Update just the part of the display that we've changed */
        //SDL_UpdateRect(display, x, y, 1, 1);
        graphics.set_last_color(color);
        return;
     }
     SDL_Surface *display = graphics.get_last_display();
     /* Lock the screen for direct access to the pixels */
     if ( SDL_MUSTLOCK(display) ) {
        if ( SDL_LockSurface(display) < 0 ) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
     }

     putpixel(display, x, y, graphics.get_def_color());

     if ( SDL_MUSTLOCK(display) ) {
        SDL_UnlockSurface(display);
     }

#ifndef DISABLE_SDL_PROTECTION
     /* This is guard for SDL_UpdateRect() function */
     if (x > scrdim.width) x = scrdim.width  ;
     if (y > scrdim.width) y = scrdim.height ;
     if (x < 0) x = 0;
     if (y < 0) y = 0;
#endif // DISABLE_SDL_PROTECTION

     graphics.upd_screen_func(display,x,y,1,1);
     /* Update just the part of the display that we've changed */
     //SDL_UpdateRect(display,x,y,1,1);
  }
/* -------------------------------------------------------------------------- */
  void graphics_LINE (val_Arr_t *params)
  {
     if (!graphics.isinit())
        error(MUST_SET_DISPLAY);
     _int_t x1 = (*params)[0]->Get_ival();
     _int_t y1 = (*params)[1]->Get_ival();
     _int_t x2 = (*params)[2]->Get_ival();
     _int_t y2 = (*params)[3]->Get_ival();

#ifndef DISABLE_SDL_PROTECTION
     /* This is guard for SDL_UpdateRect() function */
     if (x1 > scrdim.width) x1 = scrdim.width  ;
     if (x2 > scrdim.width) x2 = scrdim.width  ;
     if (y1 > scrdim.width) y1 = scrdim.height ;
     if (y2 > scrdim.width) y2 = scrdim.height ;
     if (x1 < 0) x1 = 0;
     if (x2 < 0) x2 = 0;
     if (y1 < 0) y1 = 0;
     if (y2 < 0) y2 = 0;
#endif // DISABLE_SDL_PROTECTION

     _uint32_t color = graphics.get_last_color();
     if (params->size() == 5)
     {
        Types t(VOID);
        if ((t = (*params)[4]->GetType()) == INT)
        {
           switch ((*params)[4]->Get_ival())
           {
              case 0: color = colors.black;break;
              case 1: color = colors.dark_blue;break;
              case 2: color = colors.dark_green;break;
              case 3: color = colors.dark_cyan;break;
              case 4: color = colors.dark_red;break;
              case 5: color = colors.dark_magenta;break;
              case 6: color = colors.dark_yellow;break;
              case 7: color = colors.light_gray;break;
              case 8: color = colors.dark_gray;break;
              case 9: color = colors.blue;break;
              case 10:color = colors.green;break;
              case 11:color = colors.cyan;break;
              case 12:color = colors.red;break;
              case 13:color = colors.magenta;break;
              case 14:color = colors.yellow;break;
              case 15:color = colors.white;break;
              default:color = graphics.get_last_color();
           }
        }
        else
        if (t == STRING && (*params)[4]->Get_sval() == "B") // "BOX"
        {
           color = graphics.get_last_color();
           SDL_Surface *display = graphics.get_last_display();
           /* Lock the screen for direct access to the pixels */
           if ( SDL_MUSTLOCK(display) ) {
              if ( SDL_LockSurface(display) < 0 ) {
                  fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
                  return;
              }
           }
           putline(display,&x1,&y1,&x1,&y2,&color);
           putline(display,&x1,&y1,&x2,&y1,&color);
           putline(display,&x2,&y2,&x1,&y2,&color);
           putline(display,&x2,&y2,&x2,&y1,&color);
           if ( SDL_MUSTLOCK(display) ) {
              SDL_UnlockSurface(display);
           }
           return;
        }
        else
        if (t == STRING && (*params)[4]->Get_sval() == "BF") // "BOX FILLED"
        {
           color = graphics.get_last_color();
           SDL_Surface *display = graphics.get_last_display();
           /* Lock the screen for direct access to the pixels */
           if ( SDL_MUSTLOCK(display) ) {
              if ( SDL_LockSurface(display) < 0 ) {
                  fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
                  return;
              }
           }
           putline(display,&x1,&y1,&x1,&y2,&color);
           putline(display,&x1,&y1,&x2,&y1,&color);
           putline(display,&x2,&y2,&x1,&y2,&color);
           putline(display,&x2,&y2,&x2,&y1,&color);
           SDL_Rect *box = new SDL_Rect;
           box->x = min(x1,x2);
           box->y = min(y1,y2);
           box->h = abs(y2-y1);
           box->w = abs(x2-x1);
           if (SDL_FillRect(display,box,color)) {
              fprintf(stderr, "Can't colour rectangle: %s\n", SDL_GetError());
           }
           if ( SDL_MUSTLOCK(display) ) {
              SDL_UnlockSurface(display);
           }
           //SDL_UpdateRects(display,1,box);
           delete box;
           return;
        }
     }
     if (params->size() == 6)
     {
        if ((*params)[4]->GetType() != INT || (*params)[5]->GetType() != STRING)
           error(WRONG_PARAMETER_TYPE);
        switch ((*params)[4]->Get_ival())
        {
           case 0: color = colors.black;break;
           case 1: color = colors.dark_blue;break;
           case 2: color = colors.dark_green;break;
           case 3: color = colors.dark_cyan;break;
           case 4: color = colors.dark_red;break;
           case 5: color = colors.dark_magenta;break;
           case 6: color = colors.dark_yellow;break;
           case 7: color = colors.light_gray;break;
           case 8: color = colors.dark_gray;break;
           case 9: color = colors.blue;break;
           case 10:color = colors.green;break;
           case 11:color = colors.cyan;break;
           case 12:color = colors.red;break;
           case 13:color = colors.magenta;break;
           case 14:color = colors.yellow;break;
           case 15:color = colors.white;break;
           default:color = graphics.get_last_color();
        }
        if ((*params)[5]->Get_sval() == "B") // "BOX"
        {
           SDL_Surface *display = graphics.get_last_display();
           /* Lock the screen for direct access to the pixels */
           if ( SDL_MUSTLOCK(display) ) {
              if ( SDL_LockSurface(display) < 0 ) {
                  fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
                  return;
              }
           }
           putline(display,&x1,&y1,&x1,&y2,&color);
           putline(display,&x1,&y1,&x2,&y1,&color);
           putline(display,&x2,&y2,&x1,&y2,&color);
           putline(display,&x2,&y2,&x2,&y1,&color);
           if ( SDL_MUSTLOCK(display) ) {
              SDL_UnlockSurface(display);
           }
           return;
        }
        else
        if ((*params)[5]->Get_sval() == "BF") // "BOX FILLED"
        {
           SDL_Surface *display = graphics.get_last_display();
           /* Lock the screen for direct access to the pixels */
           if ( SDL_MUSTLOCK(display) ) {
              if ( SDL_LockSurface(display) < 0 ) {
                  fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
                  return;
              }
           }
           putline(display,&x1,&y1,&x1,&y2,&color);
           putline(display,&x1,&y1,&x2,&y1,&color);
           putline(display,&x2,&y2,&x1,&y2,&color);
           putline(display,&x2,&y2,&x2,&y1,&color);
           SDL_Rect *box = new SDL_Rect;
           box->x = min(x1,x2);
           box->y = min(y1,y2);
           box->h = abs(y2-y1);
           box->w = abs(x2-x1);
           if (SDL_FillRect(display,box,color)) {
              fprintf(stderr, "Can't colour rectangle: %s\n", SDL_GetError());
           }
           if ( SDL_MUSTLOCK(display) ) {
              SDL_UnlockSurface(display);
           }
           //SDL_UpdateRects(display,1,box);
           delete box;
           return;
        }
        else error(WRONG_PARAMETERS_LIST);
     }
     SDL_Surface *display = graphics.get_last_display();
     /* Lock the screen for direct access to the pixels */
     if ( SDL_MUSTLOCK(display) ) {
        if ( SDL_LockSurface(display) < 0 ) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
     }
     putline(display,&x1,&y1,&x2,&y2,&color);
     if ( SDL_MUSTLOCK(display) ) {
        SDL_UnlockSurface(display);
     }
  }
/* -------------------------------------------------------------------------- */
  void graphics_CIRCLE (val_Arr_t *params)
  {
     if (!graphics.isinit())
        error(MUST_SET_DISPLAY);
     _int_t Xc = (*params)[0]->Get_ival();
     _int_t Yc = (*params)[1]->Get_ival();
     _int_t R  = (*params)[2]->Get_ival();

     _uint32_t color = graphics.get_last_color();
     if (params->size() == 4)
     {
        switch ((*params)[3]->Get_ival())
        {
           case 0: color = colors.black;break;
           case 1: color = colors.dark_blue;break;
           case 2: color = colors.dark_green;break;
           case 3: color = colors.dark_cyan;break;
           case 4: color = colors.dark_red;break;
           case 5: color = colors.dark_magenta;break;
           case 6: color = colors.dark_yellow;break;
           case 7: color = colors.light_gray;break;
           case 8: color = colors.dark_gray;break;
           case 9: color = colors.blue;break;
           case 10:color = colors.green;break;
           case 11:color = colors.cyan;break;
           case 12:color = colors.red;break;
           case 13:color = colors.magenta;break;
           case 14:color = colors.yellow;break;
           case 15:color = colors.white;break;
           default:;// Do nothing.Already passed default initialization.
        }
     }
     SDL_Surface *display = graphics.get_last_display();
     /* Lock the screen for direct access to the pixels */
     if ( SDL_MUSTLOCK(display) ) {
        if ( SDL_LockSurface(display) < 0 ) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
     }
     // Draw circle direct to display
     putcircle(display, &Xc, &Yc, &R, &color);

     if ( SDL_MUSTLOCK(display) ) {
        SDL_UnlockSurface(display);
     }

#ifndef DISABLE_SDL_PROTECTION
     /* This is guard for SDL_UpdateRect() function */
     _int_t x = Xc-R-1,
            y = Yc-R-1;
     if (x > scrdim.width) x = scrdim.width ;
     if (y > scrdim.height) y = scrdim.height;
     if (x < 0) x = 0;
     if (y < 0) y = 0;
     /* Update just the part of the display that we've changed */
     //SDL_UpdateRect(display, x, y, (R << 1)+3, (R << 1)+3);
#else

     /* Update just the part of the display that we've changed */
     //SDL_UpdateRect(display, Xc-R-1, Yc-R-1, (R << 1)+3, (R << 1)+3);

#endif // DISABLE_SDL_PROTECTION
  }
/* -------------------------------------------------------------------------- */
#endif // DISABLE_SDL_GRAPHICS
