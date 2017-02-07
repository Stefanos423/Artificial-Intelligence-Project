#include <SFML/Graphics.hpp>
#include "resources.hpp"
#include "obj.hpp"
#include "group.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define gridColor sf::Color(0, 0, 0, 60)
#define helpColor sf::Color(210, 230, 100, 200)
#define bgGreen sf::Color(0, 50, 32, 200)
#define fgGreen sf::Color(0, 220, 0)
#define gridWidth 2

FILE *r1 = 0, *r2 = 0;  // files for robot 1, 2
int width, height;  // width and height of map in grid squares (*32 to convert to pixels)
int w_width, w_height;  // window width, height
int minimap_width, minimap_height; // minimap width, height
int following = 2;  // robot we are following (2 = None)

bool paused = false;  // is game paused?
bool full_help = false;  // do we show the full game window?
bool grid = true;  // do we show grid?
bool minimap_enabled = true;  // do we show minimap?
bool focus = true;  // is the window focused?
int calculated = 0;
int ai_return = 0;

int argcG;
char **argvG;

/* Clean up and exit */
void quit(sf::RenderWindow *window)
{
    if (r1)
        fclose(r1);
    if (r2)
        fclose(r2);
    window->close();
    paused = true;
}

/* Reads the map file and initializes all game objects */
void readmap()
{
    int x, y, count, ret;
    Obj *obj;
    FILE *f;
    
    // Open files needed
    if (argcG < 2)
        f = fopen("input.txt", "r");
    else
        f = fopen(argvG[1], "r");
    
    if (!f)
    {
        printf("Error. Invalid input file or no file found.\n");
        exit(1);
    }

    r1 = fopen("robot1.txt", "r");
    r2 = fopen("robot2.txt", "r");
    
    ret = fscanf(f, "%d %d", &width, &height);
    
    // Robots and their bases
    ret = fscanf(f, "%d %d", &x, &y);
    obj = new Robot(r1);
    obj->setPosition(32 * (x - 1), 32 * (y - 1));
    
    obj = new Base();
    obj->setPosition(32 * (x - 1), 32 * (y - 1));
    
    ret = fscanf(f, "%d %d", &x, &y);
    obj = new Robot(r2);
    obj->setPosition(32 * (x - 1), 32 * (y - 1));
    
    obj = new Base();
    obj->setPosition(32 * (x - 1), 32 * (y - 1));
    
    // Meeting point
    ret = fscanf(f, "%d %d", &x, &y);
    obj = new MeetingPoint();
    obj->setPosition(32 * (x - 1), 32 * (y - 1));
    
    // Checkpoints
    ret = fscanf(f, "%d", &count);
    for (int i = 0; i < count; i++)
    {
        ret = fscanf(f, "%d %d", &x, &y);
        obj = new Checkpoint();
        obj->setPosition(32 * (x - 1), 32 * (y - 1));
    }
    
    // Walls
    for (int j = 0; j < height; j++)
    {
        char c;
        ret = fscanf(f, "%c", &c);
        for (int i = 0; i < width; i++)
        {
            ret = fscanf(f, "%c", &c);
            if (c == 'X')
            {
                obj = new Wall();
                obj->setPosition(32 * i, 32 * j);
            }
        }
    }
    
    fclose(f);
}

/* Draws a 32x32 sized grid */
void drawGrid(sf::RenderTarget &target, int width, int height)
{
    sf::RectangleShape rect(sf::Vector2f(32 * width + gridWidth, gridWidth));
    rect.setFillColor(gridColor);
    
    for (int i = 0; i <= height; i++)
    {
        rect.setPosition(0, 32 * i);
        target.draw(rect);
    }
    
    rect.setSize(sf::Vector2f(gridWidth, 32 * height));
    
    for (int i = 0; i <= width; i++)
    {
        rect.setPosition(32 * i, 0);
        target.draw(rect);
    }
}

/* Draws the help panel */
void drawHelp(sf::RenderTarget &target, int x, int y)
{
    sf::Text text;
    sf::RectangleShape rect(sf::Vector2f(200, 30 + full_help * 220));
    rect.setFillColor(helpColor);
    rect.setPosition(x, y);
    target.draw(rect);
    
    text.setFont(Resources::font);
    text.setCharacterSize(18);
    text.setColor(sf::Color::Black);
    
    
    text.setString("[H] Toggle Full Help");
    text.setPosition(x + 10, y + 4);
    target.draw(text);
    
    if (!full_help) return;
    
    text.setString("[P] Toggle Paused");
    text.setPosition(x + 10, y + 26);
    target.draw(text);
    
    text.setString("[G] Toggle Grid");
    text.setPosition(x + 10, y + 48);
    target.draw(text);
    
    text.setString("[M] Toggle Minimap");
    text.setPosition(x + 10, y + 70);
    target.draw(text);
    
    text.setString("[Arrows] Move View");
    text.setPosition(x + 10, y + 92);
    target.draw(text);
    
    text.setString("[=] Zoom In");
    text.setPosition(x + 10, y + 114);
    target.draw(text);
    
    text.setString("[-] Zoom Out");
    text.setPosition(x + 10, y + 136);
    target.draw(text);
    
    text.setString("[Shift] Increase Speed");
    text.setPosition(x + 10, y + 158);
    target.draw(text);
    
    text.setString("[Ctrl] Decrease Speed");
    text.setPosition(x + 10, y + 180);
    target.draw(text);
    
    text.setString("[R] Restart");
    text.setPosition(x + 10, y + 202);
    target.draw(text);
    
    text.setString("[Esc] Exit");
    text.setPosition(x + 10, y + 224);
    target.draw(text);
}

/* Draws the following panel */
void drawFollowing(sf::RenderTarget &target, int x, int y)
{
    sf::Text text;
    sf::RectangleShape rect(sf::Vector2f(150, 30));
    rect.setFillColor(helpColor);
    rect.setPosition(x, y);
    target.draw(rect);
    
    text.setFont(Resources::font);
    text.setCharacterSize(18);
    text.setColor(sf::Color::Black);
    
    switch (following)
    {
        case 0:
            text.setString("[F] Following 1");
            break;
        case 1:
            text.setString("[F] Following 2");
            break;
        case 2:
            text.setString("[F] Free Camera");
    }
    
    text.setPosition(x + 10, y + 4);
    target.draw(text);
}

void drawIntro(sf::RenderTarget &target, int x, int y)
{
    sf::Text text;
    sf::RectangleShape rect(sf::Vector2f(400, 400));
    rect.setFillColor(bgGreen);
    rect.setOutlineThickness(2);
    rect.setOutlineColor(fgGreen);
    rect.setPosition(x, y);
    target.draw(rect);
    
    text.setFont(Resources::font);
    text.setColor(fgGreen);
    
    text.setCharacterSize(50);
    text.setPosition(x + 120, y + 10);
    text.setString("robotAI");
    target.draw(text);
    
    text.setPosition(x + 90, y + 80);
    text.setCharacterSize(30);
    text.setString("          by\nStefanos Argyriou\nThanasis Katsios\nFilippos Konidaris");
    target.draw(text);
    
    text.setPosition(x + 100, y + 316);
    if (calculated == 0)
        text.setString("Thinking...");
    else if (ai_return == 0)
    {
        text.setString("Done!");
        sf::Sprite sprite(Resources::thumbs_up);
        sprite.setPosition(x + 176, y + 264);
        target.draw(sprite);
        calculated--;
    }
    else if (ai_return == 1)
    {
        text.setString("Failure to calculate path.");
        calculated--;
    }
    else if (ai_return == 2)
    {
        text.setString("Input file not found.");
        calculated--;
    }
    else
    {
        text.setString("Unknown ai error.");
        calculated--;
    }
    target.draw(text);
}
/* Clean up and restart */
void restart()
{
    // Close files
    if (r1)
        fclose(r1);
    if (r2)
        fclose(r2);
    
    // delete all objects
    delete Obj::group;
    Obj::group = new Group;
    
    // and empty their groups
    Robot::group->clear();
    Checkpoint::group->clear();
    MeetingPoint::group->clear();
    Wall::group->clear();
    Base::group->clear();
    Bubble::group->clear();

    readmap(); // then make everything again
}

void callai()
{
    char str[256];
    int n;
    if (argcG < 2)
        sprintf(str, "./ai");
    else if (argcG == 2)
        sprintf(str, "./ai %s", argvG[1]);
    else
        sprintf(str, "./ai %s %s", argvG[1], argvG[2]);
    
    ai_return = system(str);
    ai_return = WEXITSTATUS(ai_return);
    printf("Ai completed with return code %d.\n", ai_return);
    calculated = 50;
}

int main(int argc, char **argv)
{
    int current_zoom;
    sf::Thread aithread(&callai);
    aithread.launch();
    argcG = argc;
    argvG = argv;
    Resources::init();  // inits all resource files
    
    readmap();
    
    // make the window
    w_width = 1024;
    w_height = 768;
    sf::RenderWindow window(sf::VideoMode(w_width, w_height), "robotAI");
    window.setFramerateLimit(30);
    window.setKeyRepeatEnabled(false);
    
    // and the corresponding view
    sf::View default_view = window.getDefaultView();
    sf::View view(sf::FloatRect((width * 32 - w_width) / 2, (height * 32 - w_height) / 2,  w_width, w_height));

    // make the minimap and its texture, sprite
    sf::RenderTexture minimap;
    minimap_width = 100;
    minimap_height = 100 * height / width;
    minimap.create(100, 100 * height / width);
    const sf::Texture &minimap_texture = minimap.getTexture();
    sf::Sprite minimap_sprite(minimap_texture);
    minimap_sprite.setPosition(w_width - minimap_width - 5, w_height - minimap_height - 5);
    // make a rectangle representing the minimap's boundaries
    sf::RectangleShape minimap_rect(sf::Vector2f(minimap_width, minimap_height));
    minimap_rect.setFillColor(sf::Color(0, 0, 0, 0));
    minimap_rect.setOutlineThickness(2);
    minimap_rect.setOutlineColor(fgGreen);
    minimap_rect.setPosition(minimap_sprite.getPosition());
    int minimap_frame = 45;
    // and a rectangle representing the portion of current view on the minimap
    sf::RectangleShape view_rect(sf::Vector2f(view.getSize().x * minimap_width / (width * 32.0f), view.getSize().y * minimap_height / (height * 32.0f)));
    view_rect.setOrigin(sf::Vector2f(view_rect.getSize().x / 2, view_rect.getSize().y / 2));
    view_rect.setPosition(view.getCenter().x * minimap_width / (width * 32.0f), view.getCenter().y * minimap_height / (height * 32.0f));
    view_rect.setFillColor(sf::Color(0, 0, 0, 0));
    view_rect.setOutlineThickness(2);
    view_rect.setOutlineColor(fgGreen);

    init:
    
    while (window.isOpen())
    {
        // EVENT HANDLING
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    quit(&window);  // c ya!
                    break;
                case sf::Event::LostFocus:
                    focus = false;
                    break;
                case sf::Event::GainedFocus:
                    focus = true;
                    break;
                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Escape:
                            quit(&window);
                            break;
                        case sf::Keyboard::P:
                            paused = !paused;
                            break;
                        case sf::Keyboard::G:
                            grid = !grid;
                            break;
                        case sf::Keyboard::M:
                            minimap_enabled = !minimap_enabled;
                            minimap_frame = 45;
                            break;
                        case sf::Keyboard::H:
                            full_help = !full_help;
                            break;
                        case sf::Keyboard::F:
                            following = (following + 1) % 3;
                            break;
                        case sf::Keyboard::LShift:
                        case sf::Keyboard::RShift:
                            Robot::increase_speed();
                            break;
                        case sf::Keyboard::LControl:
                        case sf::Keyboard::RControl:
                            Robot::decrease_speed();
                            break;
                        case sf::Keyboard::Equal:
                            if (current_zoom > 0)
                            {
                                view.zoom(0.70710678);
                                view_rect.scale(0.70710678, 0.70710678);
                                view_rect.setOutlineThickness(view_rect.getOutlineThickness() * 1.4142136);
                                current_zoom--;
                            }
                            break;
                        case sf::Keyboard::Dash:
                            if (current_zoom < 4)
                            {
                                view.zoom(1.4142136);
                                view_rect.scale(1.4142136, 1.4142136);
                                view_rect.setOutlineThickness(view_rect.getOutlineThickness() * 0.70710678);
                                current_zoom++;
                            }
                            break;
                        case sf::Keyboard::R:
                            restart();
                            goto init;  // goto rocks :)
                            break;
                    }
                    break;
                case sf::Event::Resized:
                    // fix views
                    w_width = event.size.width;
                    w_height = event.size.height;
                    sf::Vector2f center = view.getCenter();
                    view.reset(sf::FloatRect(0, 0, w_width, w_height));
                    default_view.reset(sf::FloatRect(0, 0, w_width, w_height));
                    view.setCenter(center);
                    
                    view_rect.setSize(sf::Vector2f(view.getSize().x * minimap_width / (width * 32.0f), view.getSize().y * minimap_height / (height * 32.0f)));
                    view_rect.setOrigin(sf::Vector2f(view_rect.getSize().x / 2, view_rect.getSize().y / 2));
                    
                    minimap_sprite.setPosition(w_width - minimap_width - 5, w_height - minimap_height - 5);
                    minimap_rect.setPosition(minimap_sprite.getPosition());
                    break;
            }
        }
        
        if (focus)
        {
            // move views
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                view.move(sf::Vector2f(-32, 0));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                view.move(sf::Vector2f(0, -32));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                view.move(sf::Vector2f(32, 0));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                view.move(sf::Vector2f(0, 32));
        }

        // UPDATE
        if (!paused && calculated == 1 && ai_return == 0)
        {
            // update all objects
            MeetingPoint::group->update();
            Robot::group->update();
            Bubble::group->update();
            Bubble::delete_marked();
        }
        if (calculated == 1 && ai_return != 0 && ai_return != 1)
            quit(&window);
            
        if (following < Robot::group->size())
            view.setCenter((*Robot::group)[following]->getPosition() + sf::Vector2f(16, 16));
            
            view_rect.setPosition(view.getCenter().x * minimap_width / (width * 32.0f), view.getCenter().y * minimap_height / (height * 32.0f));
        
        // DRAW
        window.clear(sf::Color(200, 200, 200));
        
        // draw all objects
        window.setView(view);
        Wall::group->draw(window);
        Base::group->draw(window);
        Checkpoint::group->draw(window);
        MeetingPoint::group->draw(window);
        Robot::group->draw(window);
        Bubble::group->draw(window);
        
        if (grid)
            drawGrid(window, width, height);
        
        window.setView(default_view);
        
        // draw the panels
        drawHelp(window, w_width - 200, 0);
        drawFollowing(window, 0, w_height - 30);
        
        if (minimap_enabled)  // draw minimap
        {
            minimap.clear(bgGreen);
            
            if (minimap_frame > 0)
            {
                minimap_frame--;
                sf::Text text;
                text.setFont(Resources::font);
                text.setCharacterSize(26);
                text.setColor(fgGreen);
                text.setPosition(8, 16);
                text.setString("Pip-Bot\n  2000");
                minimap.draw(text);
            }
            else
            {
                Checkpoint::group->draw(minimap, true);
                MeetingPoint::group->draw(minimap, true);
                Robot::group->draw(minimap, true);
                minimap.draw(view_rect);
            }
            minimap.display();
            window.draw(minimap_sprite);
            window.draw(minimap_rect);
        }
        if (calculated != 1)
            drawIntro(window, w_width / 2 - 200, w_height / 2 - 200);
        window.display();
    }

    return 0;
} 
