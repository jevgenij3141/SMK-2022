#include <GL/glew.h>
#include <GL/glut.h>
#include <SDL2/SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    /* set viewing projection */
    glMatrixMode(GL_PROJECTION);
    glFrustum(-0.5F, 0.5F, -0.5F, 0.5F, 1.0F, 3.0F);

    /* position viewer */
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0.0F, 0.0F, -2.0F);

    /* position object */
    glRotatef(30.0F, 1.0F, 0.0F, 0.0F);
    glRotatef(30.0F, 0.0F, 1.0F, 0.0F);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Images
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("box.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    //Object
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

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

          
        //glLoadIdentity();

        // Rotate when user changes rotate_x and rotate_y
        y = 0.8;
        z = 0.2;
        glRotatef( y, 1.0, 0.0, 0.0 );
        glRotatef( z, 0.0, 1.0, 0.0 );

        /* clear color and depth buffers */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);  

        /* draw six faces of a cube */
        glBegin(GL_QUADS);
        glNormal3f( 0.0F, 0.0F, 1.0F);
        glVertex3f( 0.5F, 0.5F, 0.5F); glVertex3f(-0.5F, 0.5F, 0.5F);
        glVertex3f(-0.5F,-0.5F, 0.5F); glVertex3f( 0.5F,-0.5F, 0.5F);

        glNormal3f( 0.0F, 0.0F,-1.0F);
        glVertex3f(-0.5F,-0.5F,-0.5F); glVertex3f(-0.5F, 0.5F,-0.5F);
        glVertex3f( 0.5F, 0.5F,-0.5F); glVertex3f( 0.5F,-0.5F,-0.5F);

        glNormal3f( 0.0F, 1.0F, 0.0F);
        glVertex3f( 0.5F, 0.5F, 0.5F); glVertex3f( 0.5F, 0.5F,-0.5F);
        glVertex3f(-0.5F, 0.5F,-0.5F); glVertex3f(-0.5F, 0.5F, 0.5F);

        glNormal3f( 0.0F,-1.0F, 0.0F);
        glVertex3f(-0.5F,-0.5F,-0.5F); glVertex3f( 0.5F,-0.5F,-0.5F);
        glVertex3f( 0.5F,-0.5F, 0.5F); glVertex3f(-0.5F,-0.5F, 0.5F);

        glNormal3f( 1.0F, 0.0F, 0.0F);
        glVertex3f( 0.5F, 0.5F, 0.5F); glVertex3f( 0.5F,-0.5F, 0.5F);
        glVertex3f( 0.5F,-0.5F,-0.5F); glVertex3f( 0.5F, 0.5F,-0.5F);

        glNormal3f(-1.0F, 0.0F, 0.0F);
        glVertex3f(-0.5F,-0.5F,-0.5F); glVertex3f(-0.5F,-0.5F, 0.5F);
        glVertex3f(-0.5F, 0.5F, 0.5F); glVertex3f(-0.5F, 0.5F,-0.5F);
        glEnd();


        SDL_GL_SwapWindow( window );
    }

    SDL_GL_DeleteContext( ctx );
    SDL_DestroyWindow( window );
    SDL_Quit();

    return 0;
}


