#include <iostream>
#include "SDL2/SDL.h"
#include <vector>
#include "shape.h"
#include "sphere.h"

#ifndef SCREEN_H
#define SCREEN_H

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400
#define CAMERA_DEPTH 1000
#define SCALE 1
#define REFRESH_DELAY_MILLIS 20
#define N_THREADS 4
#define N_TURN 6

class Screen {

public:

    typedef struct args {
        Screen* screen;
        int startX;
        int endX;
    } threadArgs;

    Screen(const char*);
    ~Screen();
    void addShape(Shape*);

    static int drawShapesMediator(void *p) { // wrapper needed for SDL thread
        return (static_cast<Screen*>(p))->drawShapes();
    }
    int drawShapes();

    static int drawShapesPartMediator(void *p) { // wrapper needed for SDL thread
        threadArgs* args = (threadArgs*)p;
        return (static_cast<Screen*>(args->screen))->drawShapesPart(p);
    }
    int drawShapesPart(void*);

    void repaint();
    void render();
    void start();
    void lookAt(float, float, float, float, float, float);
    Color rayTrace(const Vec3&, const Vec3&, int);

private:

    SDL_Window* window;
    SDL_Renderer* renderer;
    rgb_t viewPort[SCREEN_WIDTH][SCREEN_HEIGHT];
    std::vector<Shape*> model;
    bool quit, restart;

    Vec3 cameraPos;
    Vec3 cameraLookAt;
    Vec3 light;
    Vec3 p, e1, e2; // base vectors for the plane representing the view port
                    // p is the center point of the view port

    SDL_Thread* renderThread;
    SDL_Thread* workerThreads[N_THREADS];
    threadArgs argsTab[N_THREADS];
    SDL_mutex* mutex;
    SDL_cond* mustRestart;
    SDL_sem* mustRender;
    SDL_sem* nextTurn;
    int turn;
};

#endif
