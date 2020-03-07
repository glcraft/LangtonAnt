#pragma once
#include <libglw/GLWrapper.h>
#include <libglw/Shaders.h>
#include <random>
#include <glm/glm.hpp>
class LangtonAnt
{
public:
    LangtonAnt(uint32_t winWidth, uint32_t winHeight, uint8_t size);
    void init();
    void update();
    void draw();

    void setWindowSize(uint32_t winWidth, uint32_t winHeight);
    void setDuration(float duration);
    void setMaxColor(uint32_t duration);
private:
    void reset();
    void move(uint16_t* grid, glm::uvec2& pos, int8_t& dir);
    uint32_t random();
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
    std::chrono::duration<float> m_dur;
    std::chrono::steady_clock::time_point m_t0;

    std::random_device m_rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 m_gen; //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<uint32_t> m_dis;
    
};