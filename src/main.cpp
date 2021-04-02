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
    Sphere s1(Color(215, 255, 200), 0.4, 0.9, Vec3(1300, 1500, 230), 220);
    Sphere s2(Color(25, 255, 240), 0.2, 0.9, Vec3(1800, 800, 430), 420);
    Sphere s4(Color(255, 255, 210), 0.3, 0.9, Vec3(3000, 3000, 630), 620);
    Sphere s3(Color::red, 0.1, 1, Vec3(1700, 2900, 430), 420);
    Sphere s5(Color::blue, 0.1, 1, Vec3(3000, 1000, 330), 320);
    Sphere s6(Color::green, 0.6, 0.7, Vec3(-1700, -2900, 430), 420);
    Sphere s7(Color::black, 0.1, 0.7, Vec3(-3000, -1000, 330), 320);
    Sphere s8(Color::blue, 0.1, 1, Vec3(-800, 100, 730), 720);
    Sphere s9(Color::green, 0.2, 0.7, Vec3(1500, -490, 930), 920);
    Sphere s10(Color::black, 0.6, 0.7, Vec3(-200, 1700, 530), 520);



    Floor p1(Color::white, 0.6, 1., Vec3(-45000, -45000, 0), Vec3(1, 0, 0), Vec3(0, 1, 0), 90000, 90000);
    Plane p2(Color(125, 120, 120), 0, 1, Vec3(-4500, -4500, 0), Vec3(0, 0, 1), Vec3(1, 0, 0), 5000, 9000);
    Plane p3(Color::white, 0.1, 1., Vec3(-4500, -4500, 0), Vec3(0, 1, 0), Vec3(0, 0, 1), 9000, 5000);

    Screen sc("Ray Tracer");
    sc.addShape(&p1);
    //sc.addShape(&p2);
    //sc.addShape(&p3);
    sc.addShape(&s1);
    sc.addShape(&s2);
    sc.addShape(&s3);
    sc.addShape(&s4);
    sc.addShape(&s5);
    sc.addShape(&s6);
    sc.addShape(&s7);
    sc.addShape(&s8);
    sc.addShape(&s9);
    sc.addShape(&s10);
    sc.lookAt(8000, 8000, 3000, 0, 0, 0);
    sc.start();

    Color c = Color::black;
    c = c.mixWith(Color::white, 0.5);
    c.print();
    return EXIT_SUCCESS;
}
