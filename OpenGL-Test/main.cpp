/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
 and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Texture wrapper class
class LTexture
{
    public:
        //Initializes variables
        LTexture();
    
        //Deallocates memory
        ~LTexture();
    
        //Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
    
        //Deallocates texture
        void free();
    
        //Renders texture at given point
        void render( int x, int y );
    
        //Gets image dimensions
        int getWidth();
        int getHeight();
    
    private:
        //The actual hardware texture
        SDL_Texture* mTexture;
    
        //Image dimensions
        int mWidth;
        int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Scene textures
LTexture gFooTexture;
LTexture gBackgroundTexture;
//Rendered texture
LTexture gTextTexture;


LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor ) {
    //Get rid of preexisting texture
    free();
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL ) {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        
    } else {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL ) {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            
        } else {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
            
        } //Get rid of old surface
        SDL_FreeSurface( textSurface );
        
    } //Return success
    return mTexture != NULL;
    
}

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render( int x, int y )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

bool init()
{
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
        
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            }
        }
    }
    
     //Initialize SDL_ttf
    if( TTF_Init() == -1 ) {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    
    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;
    
    //Open the font
    gFont = TTF_OpenFont( "/Library/Fonts/Arial.ttf", 28 );
    if( gFont == NULL ) {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
        
    } else {
        //Render text
        SDL_Color textColor = { 0, 0, 0 };
        if( !gTextTexture.loadFromRenderedText( "The quick brown fox jumps over the lazy dog", textColor ) ){
            printf( "Failed to render text texture!\n" );
            success = false;
        }
    }
    
    return success;
}

void close()
{
    //Free loaded images
    gFooTexture.free();
    gBackgroundTexture.free();
    
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;
    
    //Quit SDL subsystems
    SDL_Quit();
}

int lazy_main( int argc, char* args[] )
{
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Main loop flag
            bool quit = false;
            
            //Event handler
            SDL_Event e;
            
            //While application is running
            while( !quit )
            {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                }
                
                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );
                
                //Render background texture to screen
//                gBackgroundTexture.render( 0, 0 );
                
                //Render Foo' to the screen
//                gFooTexture.render( 240, 190 );
                
                gTextTexture.render(0, 0);
                
                //Update screen
                SDL_RenderPresent( gRenderer );
            }
        }
    }
    
    //Free resources and close SDL
    close();
    
    return 0;
}

