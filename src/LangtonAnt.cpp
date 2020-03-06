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
        m_tex.setSampler(s);
        m_tex.setTarget(GL_TEXTURE_2D);
        m_tex.setFormat(GL_R);
        m_tex.setSize({m_winsize.x/m_size, m_winsize.y/m_size});
        m_tex.init_null(GL_R, GL_UNSIGNED_SHORT);
    }
    if (!m_PBO.id())
    {
        m_PBO.instantiate();
        m_PBO.reserve(m_winsize.x * m_winsize.y / (m_size*m_size));
    }
    if (!m_FBO.id())
    {
        m_FBO.instantiate();
        m_FBO.attachTexture(gl::Framebuffer::AttachColor0, m_tex, 0);
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
out vec4 outColor;
void main()
{
    outColor=vec4(1.);
}
        )", false);
        m_shader << vshad << fshad << gl::sl::link;
    }
    reset();
}
void LangtonAnt::update()
{
    m_PBO.bind();
    m_tex.bind();
    m_tex.init_null(GL_R, GL_UNSIGNED_SHORT);
}
void LangtonAnt::draw()
{
    m_shader.use();
    // m_FBO.bindTo(gl::Framebuffer::TargetDraw);
    gl::Framebuffer::BindScreen();
    m_VBO.draw(GL_TRIANGLE_FAN);
    // m_FBO.blitToScreen({0,0,m_winsize/(uint32_t)m_size},{0,0,m_winsize},GL_COLOR_BUFFER_BIT, gl::Sampler::Nearest);
}
void LangtonAnt::reset()
{
    auto ptr = m_PBO.map_write();
    for(size_t i=0;i<m_total;++i)
        ptr[i]=0;
    m_PBO.unmap();
}
