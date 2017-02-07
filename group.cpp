#include "group.hpp"
#include "obj.hpp"

Group::Group() :
    std::vector<Obj*>() {
}

Group::~Group() {
    for(std::vector<Obj*>::iterator i = begin(); i != end(); ++i) {
        delete *i;
    }
}

void Group::update() {
    for(std::vector<Obj*>::const_iterator i = begin(); i != end(); ++i) {
        (*i)->update();
    }
}

void Group::draw(sf::RenderTarget& Tar, bool minimap) const {
    for(std::vector<Obj*>::const_iterator i = begin(); i != end(); ++i) {
        (*i)->draw(Tar, minimap);
    }
}