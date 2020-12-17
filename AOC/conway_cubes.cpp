#include <algorithm>
#include <fstream>
#include <thread>

#include <fmt/ostream.h>

#include "open_gl.hpp"
#include "types.hpp"

int main() {
    OpenGL::InitContext("Conway Cubes", 0, 0);
    OpenGL::Program program;
    {
        std::ifstream shader_file{"conway_cubes.comp"};
        std::string shader_source{std::istreambuf_iterator<char>{shader_file}, {}};
        OpenGL::Shader shader{shader_source, GL_COMPUTE_SHADER};
        program.Create({shader});
    }
    glUseProgram(program);

    std::ifstream input_file{"input.txt"};

    std::string line;
    std::getline(input_file, line);
    unsigned width = line.size();
    std::string input = line;
    while (std::getline(input_file, line)) input += line;
    unsigned height = input.size() / width;

    constexpr GLsizei image_dimensions = 32;
    unsigned x_offset = (image_dimensions - width) / 2;
    unsigned y_offset = (image_dimensions - height) / 2;
    unsigned z_offset = image_dimensions / 2;
    std::array<OpenGL::Texture, 2> compute_images{{{true}, {true}}};

    for (const auto& image : compute_images) {
        glBindTexture(GL_TEXTURE_3D, image);
        glTextureStorage3D(image, 1, GL_R8I, image_dimensions, image_dimensions, image_dimensions);
    }

    unsigned source_image = 0;
    std::vector<char> result(image_dimensions * image_dimensions * image_dimensions, '.');
    glTextureSubImage3D(compute_images[source_image], 0, 0, 0, 0, image_dimensions,
                        image_dimensions, image_dimensions, GL_RED_INTEGER, GL_BYTE, result.data());
    glTextureSubImage3D(compute_images[source_image], 0, x_offset, y_offset, z_offset, width,
                        height, 1, GL_RED_INTEGER, GL_BYTE, input.data());

    constexpr unsigned cycles = 6;
    for (unsigned i{0}; i < cycles; ++i) {
        glBindImageTexture(0, compute_images[source_image], 0, 0, 0, GL_READ_WRITE, GL_R8I);
        glBindImageTexture(1, compute_images[!source_image], 0, 0, 0, GL_READ_WRITE, GL_R8I);
        glDispatchCompute(image_dimensions, image_dimensions, image_dimensions);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        source_image = !source_image;
    }

    glGetTextureImage(compute_images[source_image], 0, GL_RED_INTEGER, GL_BYTE, result.size(),
                      result.data());
    unsigned active_count = std::ranges::count(result, '#');
    fmt::print("Active: {}\n", active_count);
}
