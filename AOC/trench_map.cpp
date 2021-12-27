#include "open_gl.hpp"
#include "types.hpp"

std::istringstream test{
    R"(..#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..###..######.###...####..#..#####..##..#.#####...##.#.#..#.##..#.#......#.###.######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#..#..#..##..#...##.######.####.####.#.#...#.......#..#.#.#...####.##.#......#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.....####.#..#..#.##.#....##..#.####....##...##..#...#......#.#.......#.......##..####..#...#.#.#...##..#.#..###..#####........#..####......#..#

#..#.
#....
##..#
..#..
..###
)"};

int main() {
    OpenGL::InitContext("Trench Map", 0, 0);
    OpenGL::Program program;
    {
        std::ifstream shader_file{"C:\\Users\\Marshall\\source\\repos\\AOC\\AOC\\trench_map.comp"};
        std::string shader_source{std::istreambuf_iterator<char>{shader_file}, {}};
        OpenGL::Shader shader{shader_source, GL_COMPUTE_SHADER};
        program.Create({shader});
    }
    glUseProgram(program);

    std::ifstream input_file{"input.txt"};
    std::istream& input = test;

    OpenGL::Texture rule_texture;
    glCreateTextures(GL_TEXTURE_1D, 1, &rule_texture.handle);
    glTextureStorage1D(rule_texture, 1, GL_R8I, 1 << 9);
    {
        std::string rules;
        input >> rules;
        glTextureSubImage1D(rule_texture, 0, 0, rules.size(), GL_RED_INTEGER, GL_BYTE,
                            rules.data());
    }
    glBindImageTexture(2, rule_texture, 0, 0, 0, GL_READ_ONLY, GL_R8I);

    input.ignore(2);
    std::string image_data(std::istreambuf_iterator<char>{input}, {});

    const unsigned width = image_data.find('\n') + 1;
    std::ranges::replace(image_data, '\n', '.');
    const unsigned height = image_data.size() / width;

    constexpr GLsizei image_dimensions = 256;
    const unsigned x_offset = (image_dimensions - width) / 2;
    const unsigned y_offset = (image_dimensions - height) / 2;
    std::array<OpenGL::Texture, 2> compute_images;
    for (auto& image : compute_images) {
        glCreateTextures(GL_TEXTURE_2D, 1, &image.handle);
        glTextureStorage2D(image, 1, GL_R8I, image_dimensions, image_dimensions);
        glTextureParameteri(image, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(image, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    unsigned source_image = 0;
    std::string result(image_dimensions * image_dimensions, '.');
    for (int y = 0; y < height; ++y) {
        std::copy_n(image_data.begin() + width * y, width - 1,
                    result.begin() + (y_offset + y) * image_dimensions + x_offset);
    }
    glTextureSubImage2D(compute_images[source_image], 0, 0, 0, image_dimensions, image_dimensions,
                        GL_RED_INTEGER, GL_BYTE, result.data());

    constexpr unsigned cycles = 50;
    for (unsigned i{0}; i < cycles; ++i) {
        glBindTextureUnit(0, compute_images[source_image]);
        // glBindImageTexture(0, compute_images[source_image], 0, 0, 0, GL_READ_WRITE, GL_R8I);
        glBindImageTexture(1, compute_images[!source_image], 0, 0, 0, GL_READ_WRITE, GL_R8I);
        glDispatchCompute(image_dimensions / 8, image_dimensions / 8, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
        source_image = !source_image;
    }

    glGetTextureImage(compute_images[source_image], 0, GL_RED_INTEGER, GL_BYTE, result.size(),
                      result.data());
    std::ofstream result_file{"result.txt"};
    for (int y = 0; y < image_dimensions; ++y)
        fmt::print(result_file, "{:03}: {} |\n", y,
                   std::string_view{result.data() + image_dimensions * y, image_dimensions});
    unsigned active_count = std::ranges::count(result, '#');
    fmt::print("Active: {}\n", active_count);
}
