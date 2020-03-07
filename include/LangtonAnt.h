#pragma once
#include <libglw/GLWrapper.h>
#include <libglw/Shaders.h>
#include <random>
#include <glm/glm.hpp>

struct Configuration
{
    glm::uvec2 winSize={1280,720};
    float duration=0.f;
    uint32_t maxColor=250;
    uint8_t sizePixel=4; 
};
class LangtonAnt
{
public:
    LangtonAnt();
    void init();
    void update();
    void draw();

    void setConfiguration(Configuration conf);
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
    Configuration m_config;

    std::vector<char> m_lsDir;
    glm::uvec2 m_pos;
    int8_t m_dir;
    std::chrono::steady_clock::time_point m_t0;

    std::random_device m_rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 m_gen; //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<uint32_t> m_dis;
    
};