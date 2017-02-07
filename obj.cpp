#include "obj.hpp"
#include "group.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern int width;
extern int height;
extern int minimap_width;
extern int minimap_height;

// Obj Base Class
Group *Obj::group = new Group;
Obj::Obj()
{
    this->group->push_back(this);
}

Obj *Obj::check_same_pos(Group *other)
{
    for(std::vector<Obj*>::const_iterator i = other->begin(); i != other->end(); ++i)
    {
        sf::Vector2f mypos = this->getPosition(), otherpos = (*i)->getPosition();
        if (mypos.x == otherpos.x && mypos.y == otherpos.y)
            return *i;
    }
    return NULL;
}

void Obj::draw(sf::RenderTarget& target, bool minimap)
{
    target.draw(*this);
}


// Robot
Group *Robot::group = new Group;
int Robot::move_speeds[5] = {1, 2, 4, 8, 16};
int Robot::move_frames[5] = {60, 30, 15, 10, 5};
int Robot::change_skips[5] = {2, 1, 0, 0, 0};
int Robot::current_conf = 2;

Robot::Robot(FILE *input) : Obj(),
id(this->group->size()),
input(input),
update_checked(false),
direction(3),
image(0),
skip_frame(0),
frame(Robot::move_frames[current_conf])
{
    char str[6];
    int n;
    this->setOrigin(sf::Vector2f(4, 9));
    this->setTexture(Resources::robot[this->id][3][0]);
    this->reply[0] = 0;
    
    // make minimap text
    n = sprintf(str, "%d", this->id + 1);
    this->text.setFont(Resources::font);
    this->text.setString(str);
    this->text.setCharacterSize(12);
    this->text.setColor(sf::Color(0, 220, 0));
    sf::FloatRect textRect = this->text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    
    this->group->push_back(this);
}

void Robot::update()
{
    this->setPosition(this->getPosition() + this->speed);  // update position
    sf::Vector2f pos = this->getPosition();
    if ((int)pos.x % 32 == 0 && (int)pos.y % 32 == 0)  // if we are on the grid
    {
        if (!update_checked)
        {
            this->update_checked = 1;
            this->speed.x = 0;
            this->speed.y = 0;
            
            // check for checkpoint
            Checkpoint *other = (Checkpoint *)this->check_same_pos(Checkpoint::group);
            if (other)
            {
                other->increase_state(this->id);
            }
            
            // and set image to the not moving one
            this->image = 0;
            this->setTexture(Resources::robot[this->id][this->direction][this->image]);
        }
        
        // check for meeting point collision
        MeetingPoint *other = (MeetingPoint *)this->check_same_pos(MeetingPoint::group);
        if (other)
        {
            Robot *other_robot; // and other robot next to me
            for(std::vector<Obj*>::const_iterator i = this->group->begin(); i != this->group->end(); ++i)
            {
                if ((*i) != (Obj *)this)
                {
                    other_robot = (Robot *)(*i);
                    break;
                }
            }
            sf::Vector2f other_pos = other_robot->getPosition();
            float x_dst = (other_pos.x - pos.x);
            float y_dst = (other_pos.y - pos.y);
            if ( x_dst * x_dst + y_dst * y_dst < 1025.0)
            { // we are next to each other
                if (other->increase_state())
                {
                    // set directions of robots
                    if (pos.x < other_pos.x)
                    {
                        this->direction = 2;
                        other_robot->direction = 0;
                    }
                    else if (pos.x > other_pos.x)
                    {
                        this->direction = 0;
                        other_robot->direction = 2;
                    }
                    else if (pos.y < other_pos.y)
                    {
                        this->direction = 3;
                        other_robot->direction = 1;
                    }
                    else
                    {
                        this->direction = 1;
                        other_robot->direction = 3;
                    }
                    this->setTexture(Resources::robot[this->id][this->direction][this->image]);
                    other_robot->setTexture(Resources::robot[other_robot->id][other_robot->direction][other_robot->image]);
                    
                    // and talk!
                    this->talk("Wazzuuuup");
                    strcpy(other_robot->reply, "Wazzuuuuuuup");
                }
            }
        }
    }
    else
    {
        this->update_checked = false;
        if (this->skip_frame-- <= 0)
        {  // next texture
            if (this->image < 8)
                this->image++;
            this->setTexture(Resources::robot[this->id][this->direction][this->image]);
            this->skip_frame = Robot::change_skips[Robot::current_conf];
        }
    }
    
    if (this->frame <= 1)
        this->check_reply();
    
    if (this->frame-- <= 0 && this->input)  // time to move!
    {
        int x, y, new_x, new_y;
        int ret;
        ret = fscanf(this->input, "%d %d", &new_y, &new_x);
        if (ret == 2)
        {
            this->frame = Robot::move_frames[current_conf];
            x = (this->getPosition().x) / 32;
            y = (this->getPosition().y) / 32;
            this->speed.x = (new_x - x) * Robot::move_speeds[current_conf];
            this->speed.y = (new_y - y) * Robot::move_speeds[current_conf];
            
            if (this->speed.x < 0)
                this->direction = 0;
            else if (this->speed.x > 0)
                this->direction = 2;
            else if (this->speed.y < 0)
                this->direction = 1;
            else if (this->speed.y > 0)
                this->direction = 3;
            
            this->setTexture(Resources::robot[this->id][this->direction][this->image]);
        }
    }
}

void Robot::draw(sf::RenderTarget &target, bool minimap)
{
    if (minimap)
    {
        this->text.setPosition((this->getPosition().x + 16.0f) * minimap_width / (width * 32.0f), (this->getPosition().y + 16.0f) * minimap_height / (height * 32.0f));
        target.draw(this->text);
    }
    else
        Obj::draw(target);
}

void Robot::talk(const char *str)
{
    Obj *obj = new Bubble(str);
    obj->setPosition(this->getPosition().x + 10, this->getPosition().y - 90);
    for(std::vector<Obj*>::const_iterator i = this->group->begin(); i != this->group->end(); ++i)
        ((Robot *)(*i))->frame = 60;
}

void Robot::check_reply()
{
    if (this->reply[0] != 0)
    {
        this->talk(this->reply);
        this->reply[0] = 0;
    }
}

void Robot::increase_speed()
{
    Robot::current_conf = (Robot::current_conf < 4 ? Robot::current_conf + 1 : 4);
}

void Robot::decrease_speed()
{
    Robot::current_conf = (Robot::current_conf > 0 ? Robot::current_conf - 1 : 0);
}

Group *Wall::group = new Group;
Wall::Wall() : Obj()
{
    this->setTexture(Resources::wall);
    this->group->push_back(this);
}

// Checkpoint
Group *Checkpoint::group = new Group;
Checkpoint::Checkpoint() : Obj(), state(0)
{   
    this->setTexture(Resources::checkpoint[0]);
    this->group->push_back(this);
    
    // text for minimap
    this->text.setFont(Resources::font);
    this->text.setString("c");
    this->text.setCharacterSize(12);
    this->text.setColor(sf::Color(0, 220, 0));
    sf::FloatRect textRect = this->text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
}

void Checkpoint::draw(sf::RenderTarget &target, bool minimap)
{
    if (minimap)
    {
        this->text.setPosition((this->getPosition().x + 16.0f) * minimap_width / (width * 32.0f), (this->getPosition().y + 16.0f) * minimap_height / (height * 32.0f));
        target.draw(this->text);
    }
    else
        Obj::draw(target);
}

void Checkpoint::increase_state(int r_id)
{
    this->state |= 1 << r_id;
    if (this->state == 3)
        this->setTexture(Resources::checkpoint[2]);
    else
    {
        this->setTexture(Resources::checkpoint[1]);
    }
}

// Meeting Point
Group *MeetingPoint::group = new Group;
MeetingPoint::MeetingPoint() : Obj(), state(0), frame(0), texture(0)
{
    this->setTexture(Resources::meeting[0]);
    this->group->push_back(this);
    
    // text for minimap
    this->text.setFont(Resources::font);
    this->text.setString("m");
    this->text.setCharacterSize(12);
    this->text.setColor(sf::Color(0, 220, 0));
    sf::FloatRect textRect = this->text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
}

void MeetingPoint::update()
{
    if (!this->state && this->frame++ == 30)
    {
        this->frame = 0;
        this->texture = (this->texture + 1) % 2;
        this->setTexture(Resources::meeting[this->texture]);
    }
}

void MeetingPoint::draw(sf::RenderTarget &target, bool minimap)
{
    if (minimap)
    {
        this->text.setPosition((this->getPosition().x + 16.0f) * minimap_width / (width * 32.0f), (this->getPosition().y + 16.0f) * minimap_height / (height * 32.0f));
        target.draw(this->text);
    }
    else
        Obj::draw(target);
}

bool MeetingPoint::increase_state()
{
    this->setTexture(Resources::meeting[2]);
    if (this->state == 0)
    {
        this->state = 1;
        return true;
    }
    return false;
}

// Bubble
Group *Bubble::group = new Group;
Bubble *Bubble::marked_for_delete = 0;

Bubble::Bubble(const char *str) : Obj(), frame(0), str(str)
{
    this->setTexture(Resources::bubble);
    this->group->push_back(this);
    
    this->text.setFont(Resources::font);
    this->text.setString(str);
    this->text.setCharacterSize(24);
    this->text.setColor(sf::Color::Black);
}

void Bubble::update()
{
    if (this->frame++ >= 60)
    {
        if (!Bubble::marked_for_delete)
            Bubble::marked_for_delete = this;  // set current bubble to be deleted
            // this is essential because it cant be deleted directly in an update call
            // or it will cause problems in the group and make the app crash
    }
}

void Bubble::draw(sf::RenderTarget &target, bool minimap)
{
    if (minimap) return;
    this->text.setPosition(this->getPosition().x + 32, this->getPosition().y + 25);
    Obj::draw(target);
    target.draw(this->text);
}

/* Delete marked object */
void Bubble::delete_marked()
{
    Bubble *marked = Bubble::marked_for_delete;
    if (!marked) return;
    
    // remove it from both its groups
    for(std::vector<Obj*>::iterator i = Obj::group->begin(); i != Obj::group->end(); ++i)
        if (*i == (Obj *)marked)
        {
            Obj::group->erase(i);
            break;
        }
        
    for(std::vector<Obj*>::iterator i = marked->group->begin(); i != marked->group->end(); ++i)
        if (*i == (Obj *)marked)
        {
            marked->group->erase(i);
            break;
        }
        
    Bubble::marked_for_delete = 0;
    delete marked;  // and finally delete it
}


Group *Base::group = new Group;
Base::Base() : Obj(), id(this->group->size())
{
    this->setTexture(Resources::base[this->id]);
    this->group->push_back(this);
}