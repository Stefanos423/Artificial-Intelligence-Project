#ifndef RESOURCES_HPP
#define RESOURCES_HPP
#include <SFML/Graphics.hpp>
class Resources
{
public:
    static sf::Texture robot[2][4][9], checkpoint[3], meeting[3], wall, bubble, base[2], thumbs_up;
    static sf::Font font;
    static int init();
};
#endif