#include <LangtonAnt.h>
LangtonAnt::LangtonAnt(uint32_t winWidth, uint32_t winHeight, uint8_t size) : m_size(size), m_winsize(winWidth, winHeight)
{
    m_total = m_winsize.x * m_winsize.y / (m_size*m_size);
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
        vshad.set(R"(
#version 330 core
layout (location=0) in vec2 pos;
void main()
{
    gl_Position = vec4(pos, 0, 1);
}
        )", false);
        fshad.set(R"(
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
    if (value!=1.)
        outColor=vec4(hsv2rgb(vec3(value*coef,1,1)), 1);
    else
        outColor=vec4(0,0,0,1);
}
        )", false);
        
            m_shader << vshad << fshad << gl::sl::link;
        }
        catch (gl::sl::CompileException exc)
        {
            std::cerr << exc.what();
            throw;
        }
    }

    m_lsDir.clear();
    m_lsDir.resize(rand()%255);
    for(auto& dir : m_lsDir)
        dir=rand()%2?1:-1;
    m_shader 
        << gl::sl::use
        << gl::UniformStatic<int>("tex", 0)
        << gl::UniformStatic<float>("coef", (float)0xFFFF/(float)m_lsDir.size())
        << gl::UniformRef<glm::uvec2>("screenSize", m_winsize);
    reset();
}
void LangtonAnt::update()
{
    m_PBO.bind();
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
    {
        if (rand()%4!=0)
            ptr[i]=rand()%m_lsDir.size();
        else
            ptr[i]=0xFFFF;
    }
    m_PBO.unmap();
    update();
}
