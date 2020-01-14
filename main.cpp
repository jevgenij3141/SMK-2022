#include <GL/glew.h>
#include <GL/glut.h>
#include <SDL2/SDL.h>

///usr/lib/x86_64-linux-gnu/libGLU.so - not included

// use border state as proxy for fullscreenedness
SDL_Rect ToggleFakeFullscreen( SDL_Window* window, const SDL_Rect& oldBounds )
{
    if( SDL_GetWindowFlags( window ) & SDL_WINDOW_BORDERLESS )
    {
        SDL_SetWindowBordered( window, SDL_TRUE );
        SDL_SetWindowSize( window, oldBounds.w, oldBounds.h );
        SDL_SetWindowPosition( window, oldBounds.x, oldBounds.y );
        return oldBounds;
    }
    else
    {
        SDL_Rect curBounds;
        SDL_GetWindowPosition( window, &curBounds.x, &curBounds.y );
        SDL_GetWindowSize( window, &curBounds.w, &curBounds.h );

        int idx = SDL_GetWindowDisplayIndex( window );
        SDL_Rect bounds;
        SDL_GetDisplayBounds( idx, &bounds );
        SDL_SetWindowBordered( window, SDL_FALSE );
        SDL_SetWindowPosition( window, bounds.x, bounds.y );
        SDL_SetWindowSize( window, bounds.w, bounds.h );

        return curBounds;
    }
}

int main( int argc, char **argv )
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
        return -1;

    SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    
    if( NULL == window ){
        return -1;
    }

    SDL_GLContext ctx = SDL_GL_CreateContext( window );
    if( GLEW_OK != glewInit() ){
        return -1;
    }

    SDL_Rect curBounds;

    bool running = true;

    int y = 0;
    float z = 0;

    while( running )
    {
        SDL_Event ev;
        while( SDL_WaitEventTimeout( &ev, 16 ) )
        {
            if( ev.type == SDL_QUIT ){
                running = false;
            }

            if( ev.type == SDL_KEYUP && ev.key.keysym.sym == SDLK_ESCAPE ){
                running = false;
            }

            if( ev.type == SDL_KEYUP && ev.key.keysym.sym == SDLK_f ){
                curBounds = ToggleFakeFullscreen( window, curBounds );
            }
        }

        int w, h;
        SDL_GetWindowSize( window, &w, &h );
        glViewport( 0, 0, w, h );

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Rotate when user changes rotate_x and rotate_y
        y += 1;
        z += 0.2;
        glRotatef( y, 1.0, 0.0, 0.0 );
        glRotatef( z, 0.0, 1.0, 0.0 );


        // White side - BACK
        glBegin(GL_POLYGON);
        glColor3f(   1.0,  1.0, 1.0 );
        glVertex3f(  0.5, -0.5, 0.5 );
        glVertex3f(  0.5,  0.5, 0.5 );
        glVertex3f( -0.5,  0.5, 0.5 );
        glVertex3f( -0.5, -0.5, 0.5 );
        glEnd();

        // Purple side - RIGHT
        glBegin(GL_POLYGON);
        glColor3f(  1.0,  0.0,  1.0 );
        glVertex3f( 0.5, -0.5, -0.5 );
        glVertex3f( 0.5,  0.5, -0.5 );
        glVertex3f( 0.5,  0.5,  0.5 );
        glVertex3f( 0.5, -0.5,  0.5 );
        glEnd();

        // Green side - LEFT
        glBegin(GL_POLYGON);
        glColor3f(   0.0,  1.0,  0.0 );
        glVertex3f( -0.5, -0.5,  0.5 );
        glVertex3f( -0.5,  0.5,  0.5 );
        glVertex3f( -0.5,  0.5, -0.5 );
        glVertex3f( -0.5, -0.5, -0.5 );
        glEnd();

        // Blue side - TOP
        glBegin(GL_POLYGON);
        glColor3f(   0.0,  0.0,  1.0 );
        glVertex3f(  0.5,  0.5,  0.5 );
        glVertex3f(  0.5,  0.5, -0.5 );
        glVertex3f( -0.5,  0.5, -0.5 );
        glVertex3f( -0.5,  0.5,  0.5 );
        glEnd();

        // Red side - BOTTOM
        glBegin(GL_POLYGON);
        glColor3f(   1.0,  0.0,  0.0 );
        glVertex3f(  0.5, -0.5, -0.5 );
        glVertex3f(  0.5, -0.5,  0.5 );
        glVertex3f( -0.5, -0.5,  0.5 );
        glVertex3f( -0.5, -0.5, -0.5 );
        glEnd();

        SDL_GL_SwapWindow( window );
    }

    SDL_GL_DeleteContext( ctx );
    SDL_DestroyWindow( window );
    SDL_Quit();

    return 0;
}


