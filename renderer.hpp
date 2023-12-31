#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

#define WINDOWSIZEX 1200.
#define WINDOWSIZEY 1000.
#define WINDOWSIZEX2 600
#define WINDOWSIZEY2 500

#define SCSZX 1920      //Screen size on x axis
#define SCSZX_2 960
#define SCSZY 1080      //Screen size on y axis
#define SCSZY_2 540
#define PPCM 54         //Pixel per centimeter, depends on res
#define MPP 0.0001/PPCM   //Meter per pixel, depends on res
#define FDCM 50/2         //Focal distance as centimeter, eye from screen
#define FDP FDCM*PPCM   //Focal distance as pixels 

#define EPSILON 0.0000001

#define MINBRIGHTNESS 0.05
#define P2TOBRIGHTCONST 0.2*MPP*MPP
#define RAYDISAMOUNT 10

class Mesh;
class Triangle;
class Ray;

typedef struct {
    sf::Color color = sf::Color(180, 255, 255);
    float opacity =  1;        //How much reflection vs refraction, 1 full reflection, 0 full refraction
    float notAbsorbed = 1;   //How much energy will be retained
    bool specular = true;      //Mirror like clean reflection
    float refIndex = 1;        //Refraction index
    float brightness = 0;      //How much light the surface produces
} Surface;

typedef struct {
    sf::Color color = sf::Color(255, 255, 255);
    float brightness = 0;
} RayCasted;


class Vec3D {
    public:
        union {
            struct {
                float x, y, z;
            };
            float pos[3];
        };
        Vec3D();
        Vec3D(float setPos);
        Vec3D(float setPos[3]);
        Vec3D(float x, float y, float z);
        Vec3D(const Vec3D& v1, const Vec3D& v2);
        Vec3D(const Vec3D* v1, const Vec3D* v2);

        Vec3D operator+(const Vec3D& v);
        void operator+=(const Vec3D& v);
        Vec3D operator-(const Vec3D& v);
        void operator-=(const Vec3D& v);
        Vec3D operator*(const float s);
        void operator*=(const float s);

        static double dotProduct(Vec3D& a, Vec3D& b);
        void crossProduct(Vec3D& v1, Vec3D& v2);
        void normalize();
        float distance(Vec3D& v);

        static Vec3D randomVec();
};

class Ray {
    private:
        void cleanReflect(Vec3D& interPoint, Triangle& trig);
        void randomReflect(Vec3D& interPoint, Triangle& trig);
    public:
        Vec3D rayStart;
        Vec3D rayDir;
        sf::Color color = sf::Color::White;
        float energy = 1;
        float distTrav = 1;
        Ray(Vec3D& start, Vec3D& dir);
        Ray(float start[3], float dx, float dy, float dz);
        int reflect(Vec3D& interPoint, Triangle& trig);
        int refract(Vec3D& interPoint, Triangle& trig);        //TODO
        sf::Color adjustedColor();
        void diminishColor(Triangle &trig);
};


class Triangle {
    public:
        Mesh* mesh;
        Vec3D* vertexes[3];
        Vec3D norm;
        Surface surface;
        Triangle(Mesh* mesh, int vec1, int vec2, int vec3);
        void calcNorm();
        bool rayParallel(Ray& ray);
        bool rayIntersectsTriangle(Ray& ray, Vec3D& outIntersectionPoint);
};

class Mesh {
    public:
        std::vector<Vec3D*> vertexes;
        std::vector<Triangle*> triangles;
        Mesh();
};
