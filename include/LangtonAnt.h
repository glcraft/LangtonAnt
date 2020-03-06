#pragma once
#include <libglw/GLWrapper.h>
#include <libglw/Shaders.h>
#include <glm/glm.hpp>
class LangtonAnt
{
public:
    LangtonAnt(uint32_t winWidth, uint32_t winHeight, uint8_t size);
    void init();
    void update();
    void draw();
private:
    void reset();
    void move(uint16_t* grid, glm::uvec2& pos, int8_t& dir);
    gl::Texture m_tex;
    gl::Buffer<GL_PIXEL_UNPACK_BUFFER, uint16_t> m_PBO;
    gl::ArrayBuffer<glm::vec2> m_VBO;
    gl::sl::Program m_shader;
    uint8_t m_size;
    glm::uvec2 m_winsize;
    size_t m_total;

    std::vector<char> m_lsDir;
    glm::uvec2 m_pos;
    int8_t m_dir;
};