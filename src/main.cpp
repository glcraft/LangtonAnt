#include <MainGame.h>
int main()
{
    srand(time(nullptr));
    MainGame m_main;
    m_main.init();
    m_main.display();

    return 0;
}