#include "resources.hpp"
sf::Texture Resources::robot[2][4][9], Resources::checkpoint[3], Resources::meeting[3], 
            Resources::wall, Resources::bubble, Resources::base[2], Resources::thumbs_up;
sf::Font Resources::font;            
    
int Resources::init()
{
    Resources::robot[0][0][0].loadFromFile("resources/robot0/left/Frame0.png");
    Resources::robot[0][0][1].loadFromFile("resources/robot0/left/Frame1.png");
    Resources::robot[0][0][2].loadFromFile("resources/robot0/left/Frame2.png");
    Resources::robot[0][0][3].loadFromFile("resources/robot0/left/Frame3.png");
    Resources::robot[0][0][4].loadFromFile("resources/robot0/left/Frame4.png");
    Resources::robot[0][0][5].loadFromFile("resources/robot0/left/Frame5.png");
    Resources::robot[0][0][6].loadFromFile("resources/robot0/left/Frame6.png");
    Resources::robot[0][0][7].loadFromFile("resources/robot0/left/Frame7.png");
    Resources::robot[0][0][8].loadFromFile("resources/robot0/left/Frame8.png");
    
    Resources::robot[0][1][0].loadFromFile("resources/robot0/up/Frame0.png");
    Resources::robot[0][1][1].loadFromFile("resources/robot0/up/Frame1.png");
    Resources::robot[0][1][2].loadFromFile("resources/robot0/up/Frame2.png");
    Resources::robot[0][1][3].loadFromFile("resources/robot0/up/Frame3.png");
    Resources::robot[0][1][4].loadFromFile("resources/robot0/up/Frame4.png");
    Resources::robot[0][1][5].loadFromFile("resources/robot0/up/Frame5.png");
    Resources::robot[0][1][6].loadFromFile("resources/robot0/up/Frame6.png");
    Resources::robot[0][1][7].loadFromFile("resources/robot0/up/Frame7.png");
    Resources::robot[0][1][8].loadFromFile("resources/robot0/up/Frame8.png");
    
    Resources::robot[0][2][0].loadFromFile("resources/robot0/right/Frame0.png");
    Resources::robot[0][2][1].loadFromFile("resources/robot0/right/Frame1.png");
    Resources::robot[0][2][2].loadFromFile("resources/robot0/right/Frame2.png");
    Resources::robot[0][2][3].loadFromFile("resources/robot0/right/Frame3.png");
    Resources::robot[0][2][4].loadFromFile("resources/robot0/right/Frame4.png");
    Resources::robot[0][2][5].loadFromFile("resources/robot0/right/Frame5.png");
    Resources::robot[0][2][6].loadFromFile("resources/robot0/right/Frame6.png");
    Resources::robot[0][2][7].loadFromFile("resources/robot0/right/Frame7.png");
    Resources::robot[0][2][8].loadFromFile("resources/robot0/right/Frame8.png");
    
    Resources::robot[0][3][0].loadFromFile("resources/robot0/down/Frame0.png");
    Resources::robot[0][3][1].loadFromFile("resources/robot0/down/Frame1.png");
    Resources::robot[0][3][2].loadFromFile("resources/robot0/down/Frame2.png");
    Resources::robot[0][3][3].loadFromFile("resources/robot0/down/Frame3.png");
    Resources::robot[0][3][4].loadFromFile("resources/robot0/down/Frame4.png");
    Resources::robot[0][3][5].loadFromFile("resources/robot0/down/Frame5.png");
    Resources::robot[0][3][6].loadFromFile("resources/robot0/down/Frame6.png");
    Resources::robot[0][3][7].loadFromFile("resources/robot0/down/Frame7.png");
    Resources::robot[0][3][8].loadFromFile("resources/robot0/down/Frame8.png");
    
    
    Resources::robot[1][0][0].loadFromFile("resources/robot1/left/Frame0.png");
    Resources::robot[1][0][1].loadFromFile("resources/robot1/left/Frame1.png");
    Resources::robot[1][0][2].loadFromFile("resources/robot1/left/Frame2.png");
    Resources::robot[1][0][3].loadFromFile("resources/robot1/left/Frame3.png");
    Resources::robot[1][0][4].loadFromFile("resources/robot1/left/Frame4.png");
    Resources::robot[1][0][5].loadFromFile("resources/robot1/left/Frame5.png");
    Resources::robot[1][0][6].loadFromFile("resources/robot1/left/Frame6.png");
    Resources::robot[1][0][7].loadFromFile("resources/robot1/left/Frame7.png");
    Resources::robot[1][0][8].loadFromFile("resources/robot1/left/Frame8.png");
    
    Resources::robot[1][1][0].loadFromFile("resources/robot1/up/Frame0.png");
    Resources::robot[1][1][1].loadFromFile("resources/robot1/up/Frame1.png");
    Resources::robot[1][1][2].loadFromFile("resources/robot1/up/Frame2.png");
    Resources::robot[1][1][3].loadFromFile("resources/robot1/up/Frame3.png");
    Resources::robot[1][1][4].loadFromFile("resources/robot1/up/Frame4.png");
    Resources::robot[1][1][5].loadFromFile("resources/robot1/up/Frame5.png");
    Resources::robot[1][1][6].loadFromFile("resources/robot1/up/Frame6.png");
    Resources::robot[1][1][7].loadFromFile("resources/robot1/up/Frame7.png");
    Resources::robot[1][1][8].loadFromFile("resources/robot1/up/Frame8.png");
    
    Resources::robot[1][2][0].loadFromFile("resources/robot1/right/Frame0.png");
    Resources::robot[1][2][1].loadFromFile("resources/robot1/right/Frame1.png");
    Resources::robot[1][2][2].loadFromFile("resources/robot1/right/Frame2.png");
    Resources::robot[1][2][3].loadFromFile("resources/robot1/right/Frame3.png");
    Resources::robot[1][2][4].loadFromFile("resources/robot1/right/Frame4.png");
    Resources::robot[1][2][5].loadFromFile("resources/robot1/right/Frame5.png");
    Resources::robot[1][2][6].loadFromFile("resources/robot1/right/Frame6.png");
    Resources::robot[1][2][7].loadFromFile("resources/robot1/right/Frame7.png");
    Resources::robot[1][2][8].loadFromFile("resources/robot1/right/Frame8.png");
    
    Resources::robot[1][3][0].loadFromFile("resources/robot1/down/Frame0.png");
    Resources::robot[1][3][1].loadFromFile("resources/robot1/down/Frame1.png");
    Resources::robot[1][3][2].loadFromFile("resources/robot1/down/Frame2.png");
    Resources::robot[1][3][3].loadFromFile("resources/robot1/down/Frame3.png");
    Resources::robot[1][3][4].loadFromFile("resources/robot1/down/Frame4.png");
    Resources::robot[1][3][5].loadFromFile("resources/robot1/down/Frame5.png");
    Resources::robot[1][3][6].loadFromFile("resources/robot1/down/Frame6.png");
    Resources::robot[1][3][7].loadFromFile("resources/robot1/down/Frame7.png");
    Resources::robot[1][3][8].loadFromFile("resources/robot1/down/Frame8.png");
    
    
    Resources::checkpoint[0].loadFromFile("resources/checkpoint0.png");
    Resources::checkpoint[1].loadFromFile("resources/checkpoint1.png");
    Resources::checkpoint[2].loadFromFile("resources/checkpoint2.png");
    
    Resources::meeting[0].loadFromFile("resources/meeting0.png");
    Resources::meeting[1].loadFromFile("resources/meeting1.png");
    Resources::meeting[2].loadFromFile("resources/meeting2.png");
    
    Resources::base[0].loadFromFile("resources/base1.png");
    Resources::base[1].loadFromFile("resources/base2.png");
    
    Resources::wall.loadFromFile("resources/wall.png");
    
    Resources::bubble.loadFromFile("resources/bubble.png");
    
    Resources::font.loadFromFile("resources/HappyTime.otf");
    Resources::thumbs_up.loadFromFile("resources/thumbs_up.png");
    return 0;
}
