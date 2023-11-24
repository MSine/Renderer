#include "renderer.hpp"

Mesh::Mesh() {
    //Room
        this->vertexes.push_back(new Vec3D(0, 0, 0));
        this->vertexes.push_back(new Vec3D(10000, 0, 0));
        this->vertexes.push_back(new Vec3D(10000, 5000, 0));
        this->vertexes.push_back(new Vec3D(0, 5000, 0));
        this->vertexes.push_back(new Vec3D(0, 0, 10000));
        this->vertexes.push_back(new Vec3D(10000, 0, 10000));
        this->vertexes.push_back(new Vec3D(10000, 5000, 10000));
        this->vertexes.push_back(new Vec3D(0, 5000, 10000));

        this->triangles.push_back(new Triangle(this, 0, 1, 2));
        this->triangles.push_back(new Triangle(this, 0, 2, 3));

        //Ceiling
        this->triangles.push_back(new Triangle(this, 0, 1, 5));
        this->triangles.back()->surface.brightness = 100;
        this->triangles.push_back(new Triangle(this, 0, 5, 4));
        this->triangles.back()->surface.brightness = 100;

        this->triangles.push_back(new Triangle(this, 0, 4, 7));
        this->triangles.push_back(new Triangle(this, 0, 7, 3));

        this->triangles.push_back(new Triangle(this, 1, 5, 6));
        this->triangles.push_back(new Triangle(this, 1, 6, 2));

        this->triangles.push_back(new Triangle(this, 2, 6, 7));
        this->triangles.push_back(new Triangle(this, 2, 7, 3));

        this->triangles.push_back(new Triangle(this, 5, 6, 7));
        this->triangles.push_back(new Triangle(this, 5, 7, 4));

    

}