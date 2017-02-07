#ifndef GROUP_INCLUDED_HPP
#define GROUP_INCLUDED_HPP

#include <SFML/Graphics.hpp>

class Obj;

/* A class that inherits from std::vector and gets used to group Obj and call similar functions of them */
class Group : public std::vector<Obj*> {
    public:
        Group();
        ~Group();

        void update();
        void draw(sf::RenderTarget&, bool minimap=false) const;
};

#endif