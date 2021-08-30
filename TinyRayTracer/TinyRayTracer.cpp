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

void render() {
    const int width = 1024;
    const int height = 768;

    int index = 0;
    uint8_t* framebuffer = new uint8_t[width * height * CHANNEL_NUM];

    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            framebuffer[index++] = int(j / float(height) * 255);
            framebuffer[index++] = int(i / float(width) * 255);
            framebuffer[index++] = 0;
        }
    }

    // You have to use 3 comp for complete jpg file. If not, the image will be grayscale or nothing.
    stbi_write_jpg("stbjpg3.jpg", width, height, 3, framebuffer, 100);
}

int main() {
    render();
    return 0;
}
