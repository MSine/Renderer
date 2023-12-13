#include "renderer.hpp"

__device__ Mesh mesh;
__device__ Vec3D camPos(5000, 2500, 9000);

__global__ sf::Color rayCast(Vec3D rayDir) {
    uint x = (blockIdx.x * blockDim.x) + threadIdx.x;
    uint y = (blockIdx.y * blockDim.y) + threadIdx.y;
    Vec3D intersection;
    Ray ray(camPos.pos, x, y, -FDP);

    bool rayActive;

    do {
        rayActive = false;
        for (auto trig: mesh.triangles) {
            if (trig->rayIntersectsTriangle(ray, intersection)) {
                //TODO  make surface brightness effect the color, arctan(energy*brightness) maybe?
                if (trig->surface.brightness > 0) {
                    ray.distTrav += ray.rayStart.distance(intersection);
                    return ray.adjustedColor();
                }
                else if (ray.reflect(intersection, *trig)) {
                    rayActive = true;
                    continue;
                }
            }
        }
    } while (rayActive);

    return sf::Color::Black;
}