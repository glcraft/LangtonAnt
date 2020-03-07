#include <MainGame.h>
#include <nlohmann/json.hpp>
int main(int argc, char** argv)
{
    using namespace std::string_view_literals;
    std::string pathConf = "./conf.json";
    if (argc!=1)
    {
        for(int i=1;i<argc;i++)
        {
            if (argv[i]=="--conf"sv)
            {
                pathConf = argv[++i];
            }
        }
    }
    Configuration conf;
    std::ifstream file(pathConf);
    if (file)
    {
        auto beg = file.tellg();
        file.seekg(0, std::ios::end);
        auto end = file.tellg();
        file.seekg(0, std::ios::beg);
        std::string str(end-beg, ' ');
        file.read(str.data(), end-beg);
        nlohmann::json f = nlohmann::json::parse(str);
        if (f.find("winSize")!=f.end())
            conf.winSize = {f["winSize"]["x"], f["winSize"]["y"]};
        if (f.find("duration")!=f.end())
            conf.duration = f["duration"];
        if (f.find("maxColor")!=f.end())
            conf.maxColor = f["maxColor"];
        if (f.find("sizePixel")!=f.end())
            conf.sizePixel = f["sizePixel"];
    }
    srand(time(nullptr));
    MainGame m_main;
    m_main.init(conf);
    m_main.display();

    return 0;
}