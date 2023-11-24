#include "renderer.hpp"


Vec3D::Vec3D() {
    for (int i = 0; i < 3; i++)
        this->pos[i] = 0.;    
}
Vec3D::Vec3D(double setPos) {
    for (int i = 0; i < 3; i++)
        this->pos[i] = setPos;
}
Vec3D::Vec3D(double setPos[3]) {
    for (int i = 0; i < 3; i++)
        this->pos[i] = setPos[i];
}
Vec3D::Vec3D(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
Vec3D::Vec3D(const Vec3D& v1, const Vec3D& v2) {
    this->x = v1.x - v2.x;
    this->y = v1.y - v2.y;
    this->z = v1.z - v2.z;
}
Vec3D::Vec3D(const Vec3D* v1, const Vec3D* v2) {
    this->x = v1->x - v2->x;
    this->y = v1->y - v2->y;
    this->z = v1->z - v2->z;
}

Vec3D Vec3D::operator+(const Vec3D& v) {
    Vec3D newVec(this->x + v.x, this->y + v.y, this->z + v.z);
    return newVec;
}
void Vec3D::operator+=(const Vec3D& v) {
    for(int i = 0; i < 3; i++)
        this->pos[i] += v.pos[i]; 
}
Vec3D Vec3D::operator-(const Vec3D& v) {
    Vec3D newVec(*this, v);
    return newVec;
}
void Vec3D::operator-=(const Vec3D& v) {
    for(int i = 0; i < 3; i++)
        this->pos[i] -= v.pos[i]; 
}
Vec3D Vec3D::operator*(const double s) {
    Vec3D newVec(this->x*s, this->y*s, this->z*s);
    return newVec;
}
void Vec3D::operator*=(const double s) {
    for(int i = 0; i < 3; i++)
        this->pos[i] *= s; 
}

void Vec3D::crossProduct(Vec3D& v1, Vec3D& v2) {
    this->x = v1.y * v2.z - v2.y * v1.z;
    this->y = -v1.x * v2.z + v2.x * v1.z;
    this->z = v1.x * v2.y - v2.x * v1.y;
}

void Vec3D::normalize() {
    double _dist = 1/sqrt(x*x + y*y + z*z);
    x *= _dist;
    y *= _dist;
    z *= _dist;
}

double Vec3D::distance(Vec3D& v) {
    return sqrt((this->x - v.x)*(this->x - v.x) + (this->y - v.y)*(this->y - v.y) + (this->z - v.z)*(this->z - v.z))*MPP;
}

double Vec3D::dotProduct(Vec3D& a, Vec3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}


Triangle::Triangle(Mesh* mesh, int vec1, int vec2, int vec3) {
    this->vertexes[0] = mesh->vertexes[vec1];
    this->vertexes[1] = mesh->vertexes[vec2];
    this->vertexes[2] = mesh->vertexes[vec3];
    this->calcNorm();
}

void Triangle::calcNorm() {
    Vec3D v1(this->vertexes[1], this->vertexes[0]);
    Vec3D v2(this->vertexes[2], this->vertexes[0]);
    this->norm.crossProduct(v1, v2);
    this->norm.normalize();
}

bool Triangle::rayParallel(Ray& ray) {
    double dot = Vec3D::dotProduct(this->norm, ray.rayDir);
    return (dot < EPSILON) && (dot > -EPSILON);    
}

Ray::Ray(Vec3D& start, Vec3D& dir) {
    this->rayStart = Vec3D(start.pos);
    this->rayDir = Vec3D(dir.pos);
}

int Ray::reflect(Vec3D& interPoint, Triangle& trig) {
    this->energy *= trig.surface.notAbsorbed;
    if (this->energy / (this->distTrav * this->distTrav) < MINBRIGHTNESS)
        return 0;
    this->distTrav += this->rayStart.distance(interPoint);

    if (trig.surface.specular) {
        cleanReflect(interPoint, trig);
    }
    else {
        randomReflect(interPoint, trig);
    }

    diminishColor(trig);

    return 1;
}

void Ray::cleanReflect(Vec3D& interPoint, Triangle& trig) {

    double dot = -Vec3D::dotProduct(trig.norm, this->rayDir);

    this->rayDir += trig.norm * (2 * dot);
    this->rayStart = interPoint;
}

void Ray::randomReflect(Vec3D& interPoint, Triangle& trig) {}

int Ray::refract(Vec3D& interPoint, Triangle& trig) {
    //TODO
    return 0;
}

sf::Color Ray::adjustedColor() {
    sf::Color color(this->color);
    double tmp = this->energy / (this->distTrav * this->distTrav);
    color.r *= tmp;
    color.g *= tmp;
    color.b *= tmp;
    return color;
}

void Ray::diminishColor(Triangle &trig) {
    this->color.r = trig.surface.color.r < this->color.r ? trig.surface.color.r : this->color.r;
    this->color.g = trig.surface.color.g < this->color.g ? trig.surface.color.g : this->color.g;
    this->color.b = trig.surface.color.b < this->color.b ? trig.surface.color.b : this->color.b;
}


bool Triangle::rayIntersectsTriangle(Ray& ray, Vec3D& interPoint) {
    Vec3D h, q;
    double det, u, v;
    Vec3D edge1(this->vertexes[1], this->vertexes[0]);
    Vec3D edge2(this->vertexes[2], this->vertexes[0]);
    h.crossProduct(ray.rayDir, edge2);
    det = Vec3D::dotProduct(edge1, h);

    // This ray is parallel to this triangle
    if (det > -EPSILON && det < EPSILON)
        return false;

    double invDet = 1.0 / det;
    Vec3D s(&ray.rayStart, this->vertexes[0]);
    u = invDet * Vec3D::dotProduct(s, h);

    if (u < 0.0 || u > 1.0)
        return false;

    q.crossProduct(s, edge1);
    v = invDet * Vec3D::dotProduct(ray.rayDir, q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line
    double t = invDet * Vec3D::dotProduct(edge2, q);

    if (t > EPSILON) // ray intersection
    {
        interPoint = ray.rayStart + ray.rayDir * t;
        return true;
    }
    // This means that there is a line intersection but not a ray intersection
    else
        return false;
}

