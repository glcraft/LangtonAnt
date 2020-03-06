#include <LangtonAnt.h>
#include <limits>

const std::string_view strVertShad=R"(
#version 330 core
layout (location=0) in vec2 pos;
void main()
{
    gl_Position = vec4(pos, 0, 1);
}
)";
const std::string_view strFragShad=R"(
#version 330 core
uniform sampler2D tex;
uniform uvec2 screenSize;
uniform float coef;
out vec4 outColor;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}
void main()
{
    float value=texture(tex, gl_FragCoord.xy/vec2(screenSize)).r;
    if (value!=0.)
        outColor=vec4(hsv2rgb(vec3(value*coef,1,1)), 1);
    else
        outColor=vec4(0,0,0,1);
}
)";
LangtonAnt::LangtonAnt(uint32_t winWidth, uint32_t winHeight, uint8_t size) : m_size(size), m_winsize(winWidth, winHeight), m_dur(0)
{
    m_total = m_winsize.x * m_winsize.y / (m_size*m_size);
}
void LangtonAnt::setWindowSize(uint32_t winWidth, uint32_t winHeight)
{
    m_winsize = {winWidth,winHeight};
    m_shader << gl::UniformRef<glm::uvec2>("screenSize", m_winsize);
}
void LangtonAnt::setDuration(float duration)
{
    m_dur=std::chrono::duration<float>(duration);
}
void LangtonAnt::init()
{
    if (!m_tex.id())
    {
        m_tex.instantiate();
        gl::Sampler::sptr s = std::shared_ptr<gl::Sampler>(new gl::Sampler);
        s->setFiltering(gl::Sampler::Nearest);
        s->setWrap(gl::Sampler::ClampToEdge);
        m_tex.setSampler(s);
        m_tex.setTarget(GL_TEXTURE_2D);
        m_tex.setFormat(GL_R16);
        m_tex.setSize({m_winsize.x/m_size, m_winsize.y/m_size});
        m_tex.init_null(GL_RED, GL_UNSIGNED_SHORT);
    }
    if (!m_PBO.id())
    {
        m_PBO.instantiate();
        m_PBO.reserve(m_tex.getSize().x*m_tex.getSize().y);
    }
    if (!m_VBO.id())
    {
        m_VBO.instantiate();
        auto vao = std::make_shared<gl::VertexArray>();
        vao->instantiate();
        m_VBO.attachVertexArray(vao);
        m_VBO.set_attrib(gl::ArrayBuffer<glm::vec2>::Attrib<0>(0, 2));
        m_VBO.reserve(4);
        auto ptr = m_VBO.map_write();
        ptr[0] = {-1,-1};
        ptr[1] = { 1,-1};
        ptr[2] = { 1, 1};
        ptr[3] = {-1, 1};
        m_VBO.unmap();
    }
    if (!m_shader.id())
    {
        gl::sl::Shader<gl::sl::Vertex> vshad;
        gl::sl::Shader<gl::sl::Fragment> fshad;
        try{
            vshad.set(strVertShad.data(), false);
            fshad.set(strFragShad.data(), false);
            m_shader << vshad << fshad << gl::sl::link;
        }
        catch (gl::sl::CompileException exc)
        {
            std::cerr << exc.what();
            throw;
        }
    }

    m_lsDir.clear();
    m_lsDir.resize(rand()%1000);
    for(auto& dir : m_lsDir)
        dir=rand()%2?1:-1;
    m_shader 
        << gl::sl::use
        << gl::UniformStatic<int>("tex", 0)
        << gl::UniformStatic<float>("coef", (float)0xFFFF/(float)m_lsDir.size())
        << gl::UniformRef<glm::uvec2>("screenSize", m_winsize);
    m_pos=m_tex.getSize()/2.f;
    m_t0=std::chrono::steady_clock::now();
    reset();
}
void LangtonAnt::update()
{
    m_PBO.bind();
    auto ptr = m_PBO.map_readwrite();
    auto t = std::chrono::steady_clock::now();
    if ((t-m_t0)>m_dur)
    {
        if (m_dur.count()<0)
        {
            size_t n=-m_dur.count()/0.01f;
            for(int i=0;i<n;i++)
                move(ptr, m_pos, m_dir);
        }
        else
            move(ptr, m_pos, m_dir);
        m_t0=std::chrono::steady_clock::now();
    }
    m_PBO.unmap();
    m_tex.bind();
    // m_tex.init_null(GL_R, GL_UNSIGNED_SHORT);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_tex.getSize().x, m_tex.getSize().y, GL_RED, GL_UNSIGNED_SHORT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}
void LangtonAnt::draw()
{
    m_shader << gl::sl::use;
    gl::Framebuffer::BindScreen();
    m_tex.bindTo(0);
    m_VBO.draw(GL_TRIANGLE_FAN);
}
void LangtonAnt::reset()
{
    m_PBO.bind();
    auto ptr = m_PBO.map_write();
    for(size_t i=0;i<m_total;++i)
        ptr[i]=0;
    m_PBO.unmap();
    update();
}

void LangtonAnt::move(uint16_t* grid, glm::uvec2& pos, int8_t& dir)
{
    switch(dir)
    {
        case 0:
            --pos.x;
            break;
        case 1:
            --pos.y;
            break;
        case 2:
            ++pos.x;
            break;
        case 3:
            ++pos.y;
            break;
    }
    auto tmax = std::numeric_limits<uint32_t>::max();
    for(int i=0; i<pos.length();i++)
    {
        if (pos[i]==tmax)
            pos[i]=m_tex.getSize()[i]-1;
        else if (pos[i]>=m_tex.getSize()[i])
            pos[i]=0;
    }
    uint16_t& t=grid[pos.x+pos.y*(uint32_t)m_tex.getSize().x];
    dir+=m_lsDir[t++];
    if (t==m_lsDir.size()-1)
        t=1;
    if (dir>3)
        dir=0;
    else if (dir<0)
        dir=3;
}
