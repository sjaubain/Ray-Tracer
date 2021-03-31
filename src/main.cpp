#include <exception>
#include <string>
#include <iostream>
#include <SDL2/SDL.h>

#include "sphere.h"
#include "screen.h"
#include "plane.h"
#include "floor.h"

int main(int argc, char * argv[])
{
    // init some shapes
    Sphere s1(Color(215, 255, 200), 0.3, Vec3(1300, 1500, 230), 220);
    Sphere s2(Color(25, 255, 240), 0.8, Vec3(1800, 800, 430), 420);
    Sphere s4(Color(255, 255, 210), 0.1, Vec3(3000, 3000, 630), 620);
    Sphere s3(Color::red, 0, Vec3(1700, 2900, 430), 420);
    Sphere s5(Color::blue, 0.1, Vec3(3000, 1000, 330), 320);

    Floor p1(Color(30, 220, 25), 0.1, Vec3(0, 0, 0), Vec3(1, 0, 0), Vec3(0, 1, 0), 4500, 4500);
    Plane p2(Color::white, 0.05, Vec3(0, 0, 0), Vec3(0, 0, 1), Vec3(1, 0, 0), 4000, 4500);
    Plane p3(Color::white, 0.05, Vec3(0, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1), 4500, 4000);

    Screen sc("Ray Tracer");
    sc.addShape(&p1);
    sc.addShape(&p2);
    sc.addShape(&p3);
    sc.addShape(&s1);
    sc.addShape(&s2);
    sc.addShape(&s3);
    sc.addShape(&s4);
    sc.addShape(&s5);
    sc.lookAt(8000, 8000, 5000, 0, 0, 0);
    sc.start();

    return EXIT_SUCCESS;
}
