#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "objloader.h" // Your object loader header file

const int screenWidth = 800;
const int screenHeight = 600;
const int fontGridSize = 50; // Adjust to control spacing of characters in grid
const float scaleFactor = 20.0f; // Increase this to make objects larger
const char* fontPath = "data/font/"; // Path to your font .obj files

// Initialize SDL and OpenGL
bool initSDL(SDL_Window*& window, SDL_GLContext& context) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("OBJ Font Rendering", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("Failed to create SDL window: %s\n", SDL_GetError());
        return false;
    }

    context = SDL_GL_CreateContext(window);
    if (!context) {
        printf("Failed to create OpenGL context: %s\n", SDL_GetError());
        return false;
    }

    // Set up orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, 0, screenHeight); // Flip y-axis

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return true;
}

// Render each character in a grid pattern on the screen
void renderFont(objloader& loader, const std::vector<unsigned int>& characters) {
    int x = 10, y = screenHeight-30; // Starting position for text rendering
    for (size_t i = 0; i < characters.size(); ++i) {
        glPushMatrix();
          glTranslatef(x, y, 0);
          glScalef(scaleFactor, scaleFactor, scaleFactor); // Scale up objects

          // Set color for objects if they lack textures
          glColor3f(1.0f, 1.0f, 1.0f); // White color for debugging
          glCallList(characters[i]); // Draw each character’s display list

          x += fontGridSize;
          if (x > screenWidth - fontGridSize) {
              x = 10;
              y -= fontGridSize;
          }
        glPopMatrix();
    }
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_GLContext context;

    if (!initSDL(window, context)) {
        return -1;
    }
    objloader loader;
    std::vector<unsigned int> characters;
    char c[50];
    for(int i=0;i<94;i++)
    {
      sprintf(c,"data/font/%d.obj",i);
      unsigned int tmp=loader.load(c,NULL);
      characters.push_back(tmp);
    }

//    std::vector<unsigned int> characters;
//    // Load each .obj character
//    for (int i = 1; i <= 93; ++i) {
//        std::string objFile = fontPath + std::to_string(i) + ".obj";
//        int listId = loader.load(objFile, nullptr); // Use display lists
//        if (listId >= 0) {
//            characters.push_back(listId);
//        }
//    }

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render font in 2D orthographic projection
        renderFont(loader, characters);

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
