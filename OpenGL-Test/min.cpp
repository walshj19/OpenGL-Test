#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>
#include <string>


int main( int argc, char* args[] )
{
    SDL_Init( SDL_INIT_VIDEO );
    SDL_Window * window = SDL_CreateWindow( "SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN );
    SDL_Renderer * renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    TTF_Init();
    TTF_Font * font = TTF_OpenFont( "/Library/Fonts/Arial.ttf", 28 );

    SDL_Color TextColor = { 255, 255, 255 };
    SDL_Rect TextLocation;
    SDL_Texture* TextTexture;

    // generate a surface with the given text string
    SDL_Surface* TextSurface = TTF_RenderText_Solid( font, "test", TextColor );

    //Create texture from surface pixels
    TextTexture = SDL_CreateTextureFromSurface( renderer, TextSurface );

    //Set rendering space and render to screen
    TextLocation = { 20, 20, TextSurface->w, TextSurface->w };
    SDL_RenderCopy( renderer, TextTexture, NULL, &TextLocation );
    SDL_RenderPresent( renderer );
    
    SDL_Delay( 5000 );
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();
    return 0;
}
