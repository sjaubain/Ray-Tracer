#include "screen.h"
#include <cmath>
#include <algorithm>

Screen::Screen(const char* title) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    light = Vec3(200, 200, 600);
    turn = 0;
    quit = false; restart = false; hasSkyImage = false;
}

Screen::~Screen() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyMutex(mutex);
    SDL_DestroyCond(mustRestart);
    SDL_DestroySemaphore(mustRender);
    SDL_DestroySemaphore(nextTurn);
}

void Screen::loadSkyImage(const std::string& filename) {
    skyImage = bitmap_image(filename);
    skyImageSphere = Sphere(Color::sky, 0, 1, Vec3(0, 0, 0), 100000);
    hasSkyImage = true;
}

Color Screen::getSkyPixel(const Vec3& p, const Vec3& dir) const {
    Color ret = skyImageSphere.getColor(p);
    if(!hasSkyImage) {
        return ret;
    } else {
        Vec3 i;
        skyImageSphere.intersect(p, dir, i);
        float theta = i.angle(Vec3(0, 0, 1));
        i.z = 0;
        float phi = i.angle(Vec3(1, 0, 0));

        if(theta < 0) return ret;

        _rgb_t rgb = skyImage.get_pixel((phi + 1) / 2. * skyImage.width(), (1-theta) * skyImage.height());
        return Color(rgb.red, rgb.green, rgb.blue);
    }
}

void Screen::repaint() {
    restart = 1;
    SDL_CondBroadcast(mustRestart);
}

void Screen::start() {

    mutex = SDL_CreateMutex();
    mustRestart = SDL_CreateCond();
    mustRender = SDL_CreateSemaphore(0);
    nextTurn = SDL_CreateSemaphore(0);

    renderThread = SDL_CreateThread(drawShapesMediator, "renderThread", this);
    for(int i = 0; i < N_THREADS; ++i) {

        argsTab[i] = {
            this,
            -1 * SCREEN_WIDTH / 2 + (SCREEN_WIDTH / N_THREADS) * i,
            -1 * SCREEN_WIDTH / 2 + (SCREEN_WIDTH / N_THREADS) * (i + 1)
        };

        workerThreads[i] = SDL_CreateThread(drawShapesPartMediator, "drawingThread", (void*)(&argsTab[i]));
    }

    int time = 0, oldX, oldY, newX, newY, buttonDown = 0;
    SDL_Event event;

    while(!quit) {

        SDL_WaitEvent(&event);
        switch(event.type) {

            case SDL_QUIT:
                quit = true;
                break;

            case SDL_MOUSEBUTTONDOWN:

                SDL_GetMouseState(&oldX, &oldY);
                buttonDown = 1;
                break;

            case SDL_MOUSEMOTION:

                if(buttonDown) {
                    SDL_GetMouseState(&newX, &newY);
                    float theta = asin(10 * (float)(newX - oldX) / SCREEN_WIDTH);
                    cameraPos.rotateZ(theta);
                    lookAt(cameraPos.x, cameraPos.y, cameraPos.z, 0, 0, 0);
                    oldX = newX;
                    oldY = newY;
                    repaint();
                }
                break;

            case SDL_MOUSEBUTTONUP:

                buttonDown = 0;
                break;

            case SDL_MOUSEWHEEL:

                if(event.wheel.y > 0) {
                    cameraPos = cameraPos * 0.85;
                } else {
                    cameraPos = cameraPos * (1 / 0.85);
                }
                lookAt(cameraPos.x, cameraPos.y, cameraPos.z, 0, 0, 0);
                repaint();
                break;
        }
    }
}

void Screen::addShape(Shape* shape) {
    model.push_back(shape);
}

Color Screen::rayTrace(const Vec3& p, const Vec3& dir, int recursiveDepth) {

    Vec3 i, j;
    Color c; // default color for sky
    float minDist = 10000000, curDist;

    // first sort all objects by distance from ray origin p to intersection
    // point j, so we can proceed further computation on the nearest object only
    std::vector<std::pair<Shape*, float>> sortedByDist;
    for(Shape* s : model) {
        if(s->intersect(p + EPSILON * dir, dir, j)) {
            sortedByDist.push_back(std::make_pair(s, curDist = (j - p).length()));
            if(curDist < minDist) {
                minDist = curDist;
                i = j;
            }
        }
    }
    sort(sortedByDist.begin(), sortedByDist.end(),
    [](const std::pair<Shape*, float>& a, const std::pair<Shape*, float>& b) {
        return a.second < b.second;
    });

    Shape* s = sortedByDist.size() ? sortedByDist.at(0).first : nullptr;

    if(s) {

        // intersection with object pointing towards light
        bool shadowed = false;

        if(recursiveDepth != 0) {
            for(Shape* t : model) {
                if(s != t // must not consider itself for shadowing effects
                   && !s->intersect(i + EPSILON * (light - i), light - i, j)
                   && t->intersect(i, light - i, j) && ((j - i).length() < (light - i).length()))
                    shadowed = true;
            }

            Vec3 n = s->normal(i);
            float alpha = (n.angle(light - i) + 1) / 2;
            // some "black magic" to modulate color with light
            c = alpha * alpha * Color::white + (1 - alpha * alpha) * alpha * s->getColor(i);

            if(shadowed) {
                c = 0.6 * c + 0.4 * Color::black;
            }

            shadowed = false;

            // reflected vector (Descartes formula)
            Vec3 refl = dir - 2 * (dir * n.normalized()) * n.normalized();
            float reflCoeff = s->getReflCoeff();

            // recursive call with reflected vector
            // use of epsilon value to avoid to intersect the new refl ray
            // with the same shape (possibly due to rounding errors..)
            c = (1 - reflCoeff) * c + reflCoeff * rayTrace(i + EPSILON * refl, refl, recursiveDepth - 1);

            float op;
            if((op = s->getOpacity()) != 1) // throw a new ray through the object if it has a transparency
                c = op * c + (1-op) * rayTrace(i + EPSILON * dir, dir, recursiveDepth - 1);

        } else {

            // end of recursion
            float reflCoeff = s->getReflCoeff();
            c = (1 - reflCoeff) * s->getColor(i);
        }

    } else {

        // if the ray doesn't hit any shape
        c = getSkyPixel(p, dir);
    }

    return c;
}

int Screen::drawShapes() {

    while(1) {

        turn = 0;
        while(turn < N_TURN) {

            if(restart)
                break;
            if(quit)
                return 0;

            int time = SDL_GetTicks();

            // wake all threads
            for(int t = 0; t < N_THREADS; ++t)
                SDL_SemPost(nextTurn);

            // wait for all threads to render image
            for(int t = 0; t < N_THREADS; ++t) {
                SDL_SemWait(mustRender);
            }

            std::cout << "render time with pixel size " << N_TURN - turn << " : " << SDL_GetTicks() - time << " ms\n";

            if(!restart)
                render();

            turn++;
        }

        SDL_LockMutex(mutex);
        if(!restart)
            SDL_CondWait(mustRestart, mutex);
        SDL_UnlockMutex(mutex);

        restart = 0;
    }

    return 0;
}

int Screen::drawShapesPart(void* data) {

    threadArgs* args = (threadArgs*)data;
    while(1) {

        SDL_SemWait(nextTurn);

        Vec3 currLoc, ray;

        for(int w = (int)args -> startX; w < (int)args -> endX ; w += (N_TURN - turn)) {

            if(restart)
                break;
            if(quit)
                return 0;

            for(int h = -1 * SCREEN_HEIGHT / 2; h < SCREEN_HEIGHT / 2; h += (N_TURN - turn)) {

                currLoc = p + w * SCALE * e1 + h * SCALE * e2;
                ray = currLoc - cameraPos;

                rgb_t rgb = rayTrace(cameraPos, ray, MAX_RECURSIVE_DEPTH).getRgbFormat();
                for(int i = w; i < w + (N_TURN - turn) && i < (int)args -> endX; ++i) {
                    for(int j = h; j < h + (N_TURN - turn) && j < SCREEN_HEIGHT / 2; ++j) {
                        viewPort[i + SCREEN_WIDTH / 2][j + SCREEN_HEIGHT / 2] = rgb;
                    }
                }

            }
        }
        SDL_SemPost(mustRender);
    }
    return 0;
}

void Screen::render() {
    SDL_RenderClear(renderer);
    rgb_t rgb;
    for(int w = 0; w < SCREEN_WIDTH; ++w) {
        for(int h = 0; h < SCREEN_HEIGHT; ++h) {
            rgb = viewPort[w][h];
            SDL_SetRenderDrawColor(renderer, rgb.r, rgb.g, rgb.b, 255);
            SDL_RenderDrawPoint(renderer, w, h);
        }
    }
    SDL_RenderPresent(renderer);
}

void Screen::lookAt(float cam_x, float cam_y, float cam_z, float focus_x, float focus_y, float focus_z) {

    cameraPos = Vec3(cam_x, cam_y, cam_z);
    cameraLookAt = Vec3(focus_x, focus_y, focus_z);

    Vec3 normal = (cameraLookAt - cameraPos).normalized();

    p = cameraPos + normal * CAMERA_DEPTH;
    e1 = Vec3(p.y, -1 * p.x, 0).normalized();
    e2 = e1 ^ normal;
}
