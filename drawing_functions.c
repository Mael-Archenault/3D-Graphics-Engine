#include "src/include/SDL2/SDL.h"
#include <stdio.h>

void drawTriangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3){
    SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
    SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
}

void fillTriangle(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int x3, int y3, float* color){
    SDL_Vertex triangleVertex[3]=
                {
                    {
                        { x1, y1 }, /* first point location */ 
                        {*(color), *(color+1), *(color+2),*(color+3)}, /* first color */ 
                        { 0.f, 0.f }
                    },
                    {
                        { x2, y2 }, /* second point location */ 
                        {*(color), *(color+1), *(color+2),*(color+3)}, /* second color */
                        { 0.f, 0.f }
                    },
                    {
                        {x3, y3 }, /* third point location */ 
                        {*(color), *(color+1), *(color+2),*(color+3)}, /* third color */
                        { 0.f, 0.f }
                    }
                };
    SDL_RenderGeometry(renderer, NULL,triangleVertex, 6,NULL, 0);
}