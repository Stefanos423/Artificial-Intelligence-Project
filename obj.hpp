#ifndef OBJ_HPP
#define OBJ_HPP

#include <SFML/Graphics.hpp>
#include "resources.hpp"

class Group;

/* Base class for all the objects used in the game */
class Obj : public sf::Sprite
{
public:
    Obj();
    static Group *group;  // to contain all objects of the type
    Obj *check_same_pos(Group *other);  // checks if there is an object of the specified group that has the same position as self and returns it
    virtual void update() {}  // inherit this and implement game logic update for self
    virtual void draw(sf::RenderTarget& target, bool minimap=false);  // draws object
};

/* Class for both Robots */
class Robot : public Obj
{
    static int move_speeds[5];  // possible move speeds
    static int move_frames[5];  // possible frames till next move
    static int change_skips[5];  // possible frames till next surface change
    static int current_conf;  // current choice of move_speeds, move_frames, change_skips
    int update_checked;
    sf::Text text;  // minimap representation
    FILE *input;  // file to read from
public:
    unsigned int id; // id of robot (0 or 1)
    int direction; // current direction (0 = left, clockwise)
    int image;  // index of current image
    int frame;
    int skip_frame;
    char reply[24];  // contains a reply if the robot should reply
    static Group *group;
    sf::Vector2f speed;
    Robot(FILE * input);
    void talk(const char *str);
    void check_reply();
    void update();
    void draw(sf::RenderTarget& target, bool minimap=false);
    static void increase_speed();
    static void decrease_speed();
};

/* Class for all walls */
class Wall : public Obj
{
public:
    static Group *group;
    Wall();
};

/* Class for the starting point of the robots */
class Base : public Obj
{
public:
    unsigned int id;
    static Group *group;
    Base();
};

/* Class for all the checkpoints */
class Checkpoint : public Obj
{
    unsigned int state;
    sf::Text text;  // minimap representation
public:
    static Group *group;
    Checkpoint();
    void draw(sf::RenderTarget& target, bool minimap=false);
    void increase_state(int r_id);
};

/* Class for the meeting point */
class MeetingPoint : public Obj
{
private:
    unsigned int state;
    int texture;
    int frame;
    sf::Text text;  // minimap representation
public:
    static Group *group;
    MeetingPoint();
    void draw(sf::RenderTarget& target, bool minimap=false);
    bool increase_state();
    void update();
};

/* Class for the speech bubbles */
class Bubble : public Obj
{
private:
    int frame;
    static Bubble *marked_for_delete;
    const char *str;
    sf::Text text;
public:
    static Group *group;
    Bubble(const char *str);
    void update();
    void draw(sf::RenderTarget& target, bool minimap=false);
    static void delete_marked();
};
#endif