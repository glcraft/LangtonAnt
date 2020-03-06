#include <MainGame.h>
#include <gl/glew.h>
#include <MyException.h>
#include <LangtonAnt.h>
void MainGame::init()
{
    sf::ContextSettings();
    m_window = std::unique_ptr<sf::Window>(new sf::Window(sf::VideoMode(1280, 720, 8), "Langton Ant", 7, sf::ContextSettings(0,0,0,3,3,sf::ContextSettings::Core)));
    glewExperimental = true;
    if (int code=glewInit(); code!=GLEW_OK)
        throw MyException("glew did not init properly.", code, "GLEW");
}
void MainGame::display()
{
    LangtonAnt lant(m_window->getSize().x, m_window->getSize().y, 8);
    lant.init();
    while(m_window->isOpen())
    {
        sf::Event ev;
        while (m_window->isOpen() && m_window->pollEvent(ev))
        {
            if ((ev.type==sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) || ev.type == sf::Event::Closed)
                m_window->close();
        }
        glClear(GL_COLOR_BUFFER_BIT);
        lant.update();
        lant.draw();
        m_window->display();
    }
}