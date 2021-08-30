#define _USE_MATH_DEFINES
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "geometry.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define CHANNEL_NUM 3

struct Sphere {
    Vec3f center;
    float radius;

    Sphere(const Vec3f& c, const float& r) : center(c), radius(r) {}

    bool ray_intersect(const Vec3f& orig, const Vec3f& dir, float& t0) const {
        Vec3f L = center - orig;
        float tca = L * dir;
        float d2 = L * L - tca * tca;
        if (d2 > radius * radius) return false;
        float thc = sqrtf(radius * radius - d2);
        t0 = tca - thc;
        float t1 = tca + thc;
        if (t0 < 0) t0 = t1;
        if (t0 < 0) return false;
        return true;
    }
};

Vec3f cast_ray(const Vec3f& orig, const Vec3f& dir, const Sphere& sphere) {
    float sphere_dist = std::numeric_limits<float>::max();
    if (!sphere.ray_intersect(orig, dir, sphere_dist)) {
        return Vec3f(0.2, 0.7, 0.8); // background color
    }

    return Vec3f(0.4, 0.4, 0.3);
}

void render(const Sphere& sphere) {
    const int width = 1024;
    const int height = 768;
    const float fov = M_PI / 2.;

    int index = 0;
    uint8_t* framebuffer = new uint8_t[width * height * CHANNEL_NUM];

    #pragma omp parallel for
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            // We placed the screen at the distance of 1 from the camera
            // aspect ratio: y = 1, x = width / height
            uint8_t distance = 1;
            float x =   ( 2.0 * distance * (i + 0.5) / (float)width  - 1 ) * tan(fov / 2.0) * (width / (float)height);
            float y = - ( 2.0 * distance * (j + 0.5) / (float)height - 1 ) * tan(fov / 2.0);
            Vec3f dir = Vec3f(x, y, -1).normalize();
            Vec3f ray = cast_ray(Vec3f(0, 0, 0), dir, sphere);

            uint8_t r = int(ray.x * 255);
            uint8_t g = int(ray.y * 255);
            uint8_t b = int(ray.z * 255);
            framebuffer[index++] = r;
            framebuffer[index++] = g;
            framebuffer[index++] = b;
        }
    }

    // You have to use 3 comp for complete jpg file. If not, the image will be grayscale or nothing.
    stbi_write_jpg("stbjpg3.jpg", width, height, 3, framebuffer, 100);
}

int main() {
    Sphere sphere(Vec3f(-3, 0, -16), 2);
    render(sphere);

    return 0;
}
