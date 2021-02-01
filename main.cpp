#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "src/Collada/Collada.hpp"

#include "src/Debug/console.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <istream>

using namespace std;
using namespace xercesc;

GLuint texture;
SDL_Surface *surface;

XercesDOMParser *parser_;

void buildScene();

/*DOMNodeList *getElementsByTagName(const DOMElement *element, string tag)
{
    DEBUG_M("Entering function...");
    XMLCh *xtag = XMLString::transcode(tag.c_str());
    DOMNodeList *nl = element->getElementsByTagName(xtag);
    XMLString::release(&xtag);
    return nl;
}

DOMElement *getElementByTagName(const DOMElement *element, string tag)
{
    DOMNodeList *elements = getElementsByTagName(element, tag);
    if (!elements)
    {
        ERROR("No node with tag '%s' found...", tag.c_str());
        return NULL;
    }

    DOMNode *node = elements->item(0);
    if (!node)
    {
        return NULL;
    }

    return dynamic_cast<xercesc::DOMElement *>(elements->item(0));
}

bool isString_(const XMLCh *tag1, string tag2)
{
    DEBUG_V("Entering function...");
    XMLCh *tag2_c = XMLString::transcode(tag2.c_str());

    int result = XMLString::compareIString(tag1, tag2_c);

    XMLString::release(&tag2_c);
    if (result == 0)
    {
        return true;
    }
    return false;
}

int loadOBJECT()
{
    const string &filename = "./boxes.DAE";

    DEBUG_M("loadColladaDocFile(%s)", filename);
    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException &e)
    {
        char *errMsg = XMLString::transcode(e.getMessage());
        ERROR("Xerces XML ERROR: %s", errMsg);
        XMLString::release(&errMsg);
        return 0;
    }

    parser_ = new XercesDOMParser();

    try
    {
        parser_->parse(filename.c_str());
    }
    catch (const XMLException &e)
    {
        char *errMsg = XMLString::transcode(e.getMessage());
        ERROR("Xerces XML ERROR: %s", errMsg);
        XMLString::release(&errMsg);
        return 0;
    }
    catch (const DOMException &e)
    {
        char *errMsg = XMLString::transcode(e.msg);
        ERROR("Xerces DOM ERROR: %s", errMsg);
        XMLString::release(&errMsg);
        return 0;
    }
    catch (...)
    {
        ERROR("Unknown Xerces ERROR...");
        return 0;
    }

    DOMDocument *xmlDoc = parser_->getDocument();
    DOMElement *elementRoot = xmlDoc->getDocumentElement();
    if (!elementRoot)
    {
        ERROR("Empty XML document...");
        return 0;
    }

    DOMElement *node = getElementByTagName(xmlDoc->getDocumentElement(), "scene");

    DOMNodeList *children = node->getChildNodes();
    int length = children->getLength();
    for (int i = 0; i < length; i++)
    {
        DOMNode *currentNode = children->item(i);
        if (currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE)
        {
            DOMElement *currentElement = dynamic_cast<xercesc::DOMElement *>(currentNode);

            char *tagName = XMLString::transcode(currentElement->getTagName());
            XMLCh *url_x = XMLString::transcode("url");
            char *url = XMLString::transcode(currentElement->getAttribute(url_x));

            if (XMLString::compareIString(tagName, "instance_visual_scene") == 0)
            {
                printf("instance_visual_scene: %s", url);
            }
        }
    }

    return 1;
}*/

// use border state as proxy for fullscreenedness
SDL_Rect ToggleFakeFullscreen(SDL_Window *window, const SDL_Rect &oldBounds)
{
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_BORDERLESS)
    {
        SDL_SetWindowBordered(window, SDL_TRUE);
        SDL_SetWindowSize(window, oldBounds.w, oldBounds.h);
        SDL_SetWindowPosition(window, oldBounds.x, oldBounds.y);
        return oldBounds;
    }
    else
    {
        SDL_Rect curBounds;
        SDL_GetWindowPosition(window, &curBounds.x, &curBounds.y);
        SDL_GetWindowSize(window, &curBounds.w, &curBounds.h);

        int idx = SDL_GetWindowDisplayIndex(window);
        SDL_Rect bounds;
        SDL_GetDisplayBounds(idx, &bounds);
        SDL_SetWindowBordered(window, SDL_FALSE);
        SDL_SetWindowPosition(window, bounds.x, bounds.y);
        SDL_SetWindowSize(window, bounds.w, bounds.h);

        return curBounds;
    }
}

int main(int argc, char **argv)
{
    //loadOBJECT();

    Collada collada;
    collada.load("./boxes.dae");

    SDL_Window *window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 900, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (NULL == window)
    {
        return -1;
    }

    SDL_GLContext ctx = SDL_GL_CreateContext(window);
    if (GLEW_OK != glewInit())
    {
        return -1;
    }

    SDL_Rect curBounds;

    bool running = true;

    /* set viewing projection */
    glMatrixMode(GL_PROJECTION);
    glFrustum(-0.5F, 0.5F, -0.5F, 0.5F, 1.0F, 40.0F);

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
    surface = IMG_Load("./box.png");
    printf("IMG_Load: %s\n", IMG_GetError());
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);

    glTranslated(0, -1, -0.5);

    while (running)
    {
        SDL_Event ev;
        while (SDL_WaitEventTimeout(&ev, 16))
        {
            if (ev.type == SDL_QUIT)
            {
                running = false;
            }

            if (ev.type == SDL_KEYUP && ev.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }

            if (ev.type == SDL_KEYUP && ev.key.keysym.sym == SDLK_f)
            {
                curBounds = ToggleFakeFullscreen(window, curBounds);
            }
        }

        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        glViewport(0, 0, w, h);

        buildScene();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void buildScene()
{

    //glLoadIdentity();

    int y = 0;
    float z = 0;

    // Rotate when user changes rotate_x and rotate_y
    y = 0.8;
    z = 0.2;
    glRotatef(y, 1.0, 0.0, 1);
    //glRotatef( z, 0.0, 1.0, 1.0 );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
    glVertex3f(0, 0, 0);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
    glVertex3f(1, 0, 0);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
    glVertex3f(1, 1, 0);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
    glVertex3f(0, 1, 0);
    glEnd();

    glBegin(GL_QUADS);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
    glVertex3f(0, 0, 0);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
    glVertex3f(0, 1, 0);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
    glVertex3f(0, 1, 1);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
    glVertex3f(0, 0, 1);
    glEnd();

    glBegin(GL_QUADS);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
    glVertex3f(0, 0, 0);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
    glVertex3f(1, 0, 0);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
    glVertex3f(1, 0, 1);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
    glVertex3f(0, 0, 1);
    glEnd();

    glBegin(GL_QUADS);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
    glVertex3f(1, 0, 0);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
    glVertex3f(1, 1, 0);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
    glVertex3f(1, 1, 1);
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
    glVertex3f(1, 0, 1);
    glEnd();
}
