#include <iostream>
#include <vector>
#include <any>
#include <tuple>
#include <SFML/Graphics.hpp>
#include <random>



class Game
{
private:
    

public:
    sf::Sprite sprite;
    sf::Text text;
    sf::Font font;
    std::vector<std::vector<std::vector<std::vector<std::any>>>> q_table;
    std::vector<std::string> actions = {"flap", "glide"};

    Game(const sf::Texture& texture, const sf::Vector2f& position)
    {   
        if (!font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf")){
            throw std::runtime_error("font failed to load");
        }

        sprite.setTexture(texture);
        sprite.setScale(1.8, 1.5);
        sprite.setPosition(position);
    }

    void moveSprite(const sf::Vector2f& offset)
    {
        sf::Vector2f newPosition = sprite.getPosition() - offset;
        sprite.setPosition(newPosition);

        if (sprite.getPosition().x + 510 <= 0){
            sf::Vector2f newPosition = sf::Vector2f(479.f, 0.f);
            sprite.setPosition(newPosition);
        }

    }

    void draw(sf::RenderWindow& window)

    {   
        window.draw(sprite);
               
    }


};


class Bird{

    private:
        float drop_speed = 0.005f;
        float decay = 1.f;
        float rotate = 0.f;
        sf::Texture birdu1;
        std::vector<sf::Texture> txtures;

    public:
        float y = 640.f/2.f;
        float x = 50.f;
        float og_x = x;
        float og_y = y;
        sf::Sprite sprite;
        std::random_device rd;
        int choice;
        

        
        Bird(){
            txtures = load_birds();
            sprite.setTexture(txtures[1]);
            y -= (sprite.getLocalBounds().height*1.4);
            sprite.setPosition(sf::Vector2f(x, y));
            sprite.setScale(sf::Vector2f(1.4,1.4));
            sprite.setRotation(rotate);
                       

        }

        void draw(sf::RenderWindow& window){
            window.draw(sprite);
        }

        void fly(int fly){
            if (fly == 1){
                decay = 0;
                y -= 25.f; //was 25.f for non ai.
                rotate = -20.f;
                sprite.setTexture(txtures[2]);
                sprite.setPosition(sf::Vector2f(x, y));
            } else if (fly == 0){
                y += drop_speed * decay;
                decay += 0.0015f;

                if (rotate < 50){
                   rotate += 0.008;     
                }
                else { 
                    rotate = 50;
                }
                
                if (decay >= 1.5 && decay <= 2.5){
                    sprite.setTexture(txtures[1]);
                } else if (decay > 2.5){
                    sprite.setTexture(txtures[0]);
                }
                sprite.setRotation(rotate);
                sprite.setPosition(sf::Vector2f(x, y));
            }


        }

        std::vector<sf::Texture> load_birds(){

            std::vector<sf::Texture> txt;

            sf::Texture texture2;
            if (!texture2.loadFromFile("/home/kali/Desktop/machine_learning/reinforcement_learning/imgs/bird1.png")){
                throw std::runtime_error("pipe image not found");
            }
            txt.push_back(texture2);

            sf::Texture texture3;
            if (!texture3.loadFromFile("/home/kali/Desktop/machine_learning/reinforcement_learning/imgs/bird2.png")){
                throw std::runtime_error("pipe image not found");
            }
            txt.push_back(texture3);

            sf::Texture texture4;
            if (!texture4.loadFromFile("/home/kali/Desktop/machine_learning/reinforcement_learning/imgs/bird3.png")){
                throw std::runtime_error("pipe image not found");
            }
            txt.push_back(texture4);

            return txt;
            
        } 

        void reset(){
            x = og_x;
            y = og_y;
            rotate = 0.f;
            decay = 1.f;
            sprite.setRotation(rotate);
            
        }
        
};




class Pipe{

    private:
        std::random_device rd;  // Obtain a random seed from the system
        sf::Texture pipeimg;
        
        
        
    public:
        std::vector<float> closest_pipe;
        sf::Sprite pos2dieU;
        sf::Sprite pos2dieB;
        sf::Texture diepos;
        sf::Sprite pipex1U;
        sf::Sprite pipex1B;
        sf::Sprite pipex2U;
        sf::Sprite pipex2B;
        float x1;
        float x2;
        float y1;
        float y2; 
        float og_x1;
        float og_x2; 
        
        Pipe(){

            if (!pipeimg.loadFromFile("/home/kali/Desktop/machine_learning/reinforcement_learning/imgs/pipe.png")){
                throw std::runtime_error("pipe image not found");
            }

            if (!diepos.loadFromFile("/home/kali/Pictures/red_dot.png")){
                throw std::runtime_error("dot image not found");
            }

            pos2dieU.setTexture(diepos); 
            pos2dieB.setTexture(diepos);          
            pipex1U.setTexture(pipeimg);
            pipex1B.setTexture(pipeimg);
            pipex2U.setTexture(pipeimg);
            pipex2B.setTexture(pipeimg);
            pipex1U.setScale(sf::Vector2f(1.f,1.3f));
            pipex2U.setScale(sf::Vector2f(1.f,1.3f));
            pipex1B.setScale(sf::Vector2f(1.f,1.3f));
            pipex2B.setScale(sf::Vector2f(1.f,1.3f));

            //std::cout << "pipex1U.getGlobalBounds().height = " << pipex1U.getGlobalBounds().height;
                       
            
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float>why1(pipex1U.getGlobalBounds().height -130.f,pipex1U.getGlobalBounds().height);
            std::uniform_real_distribution<float>why2(pipex1U.getGlobalBounds().height - 80.f,pipex1U.getGlobalBounds().height);
            x1 = 275.f;
            x2 = 475.f;
            float og_x1 = x1;
            float og_x2 = x2; 
            y1 = why1(gen);
            y2 = why2(gen);
            closest_pipe.push_back(x1);
            closest_pipe.push_back(y1);
            closest_pipe.push_back(y1 + 120.f);

            //pos2dieU.setPosition(sf::Vector2f(closest_pipe[0], closest_pipe[1]));
            //pos2dieB.setPosition(sf::Vector2f(closest_pipe[0], closest_pipe[2]));
            pipex1U.setPosition(sf::Vector2f(x1, y1));
            pipex1U.setRotation(180.f);
            pipex1B.setPosition(sf::Vector2f(x1 - pipex1B.getGlobalBounds().width, y1 + 120.f));
            
            pipex2U.setPosition(sf::Vector2f(x2, y2));
            pipex2U.setRotation(180.f);
            pipex2B.setPosition(sf::Vector2f(x2 - pipex2B.getGlobalBounds().width, y2 + 120.f));

            //std::cout << "pipex1U.getGlobalBounds().height is: " << pipex1U.getGlobalBounds().height << std::endl;
            //std::cout << "pipex1U.getGlobalBounds().height is: " << pipex1U.getGlobalBounds().height - 130 << std::endl;

        }

        void draw(sf::RenderWindow& window){
            window.draw(pipex1U);
            window.draw(pipex1B);
            window.draw(pipex2U);
            window.draw(pipex2B);
            //window.draw(pos2dieU);
            //window.draw(pos2dieB);
        }

        void move(){
            x1 -= 0.02f;
            x2 -= 0.02f;

            pipex1U.setPosition(sf::Vector2f(x1, y1));
            pipex1B.setPosition(sf::Vector2f(x1 - pipex1B.getGlobalBounds().width, y1 + 120.f));
            
            pipex2U.setPosition(sf::Vector2f(x2, y2));
            pipex2B.setPosition(sf::Vector2f(x2 - pipex2B.getGlobalBounds().width, y2 + 120.f));

            spawn();    
            //pos2dieU.setPosition(sf::Vector2f(closest_pipe[0], closest_pipe[1]));
            //pos2dieB.setPosition(sf::Vector2f(closest_pipe[0], closest_pipe[2]));
        }

        void spawn(){
            
            if (x1 - pipex1U.getGlobalBounds().width <= 0.f){
                std::mt19937 gen(rd());
                std::uniform_real_distribution<float>why1(pipex1U.getGlobalBounds().height -130.f,pipex1U.getGlobalBounds().height);
                x1 += 400.f;
                y1 = why1(gen);
                pipex1U.setPosition(sf::Vector2f(x1, y1));
                pipex1B.setPosition(sf::Vector2f(x1 - pipex1B.getGlobalBounds().width, y1 + 120.f));  
            }

            if (x2 - pipex1U.getGlobalBounds().width <= 0.f){
                std::mt19937 gen(rd());
                std::uniform_real_distribution<float>why2(pipex1U.getGlobalBounds().height - 80.f,pipex1U.getGlobalBounds().height);
                x2 += 400.f;
                y2 = why2(gen);
                pipex2U.setPosition(sf::Vector2f(x2, y2));
                pipex2B.setPosition(sf::Vector2f(x2 - pipex2B.getGlobalBounds().width, y2 + 120.f));
            }

            if (x1 < x2){
                closest_pipe[0] = x1;
                closest_pipe[1] = y1;
                closest_pipe[2] = y1+120.f;

            } else if (x2 < x1){
                closest_pipe[0] = x2;
                closest_pipe[1] = y2;
                closest_pipe[2] = y2+120.f;    
            }
        }

        void reset(){
            x1 = 275.f;
            x2 = 475.f;
            float og_x1 = x1;
            float og_x2 = x2; 
            closest_pipe.push_back(x1);
            closest_pipe.push_back(y1);
            closest_pipe.push_back(y1 + 120.f);

            //pos2dieU.setPosition(sf::Vector2f(closest_pipe[0], closest_pipe[1]));
            //pos2dieB.setPosition(sf::Vector2f(closest_pipe[0], closest_pipe[2]));
            pipex1U.setPosition(sf::Vector2f(x1, y1));
            pipex1U.setRotation(180.f);
            pipex1B.setPosition(sf::Vector2f(x1 - pipex1B.getGlobalBounds().width, y1 + 120.f));
            
            pipex2U.setPosition(sf::Vector2f(x2, y2));
            pipex2U.setRotation(180.f);
            pipex2B.setPosition(sf::Vector2f(x2 - pipex2B.getGlobalBounds().width, y2 + 120.f));
        }    


};



std::vector<sf::Texture> init_textures(){
    std::vector<sf::Texture> textures;
    //std::cout << "textures.size() = " << textures.size();

    sf::Texture texture;
    if (!texture.loadFromFile("/home/kali/Desktop/machine_learning/reinforcement_learning/imgs/bg.png")){
        throw std::runtime_error("background image not found");
    }
    textures.push_back(texture);

    sf::Texture texture1;
    if (!texture1.loadFromFile("/home/kali/Desktop/machine_learning/reinforcement_learning/imgs/pipe.png")){
        throw std::runtime_error("pipe image not found");
    }
    textures.push_back(texture1);

    return textures;
}; 




int main()

{
    std::vector<sf::Texture> textures = init_textures();
    Game Background(textures[0], sf::Vector2f(0.f, 0.f));
    Game Background2(textures[0], sf::Vector2f(479.f,0.f));
    sf::RenderWindow window(sf::VideoMode(480, 640), "SFML Window");
    Bird bird;
    Pipe pipe;
    sf::Text text;
    sf::Text ep_rate;
    sf::Font font;
    int score = 0;
    float epsilon = 1.f;
    float alpha = 1.f;
    float gamma = 0.5f; 


    if (!font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf")){
            throw std::runtime_error("font failed to load");
    }

    //SET THE TEXTS. 
    text.setString("Score: " + std::to_string(score));
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);
    text.setPosition(30,15);
    text.setFont(font);

    ep_rate.setString("Epsilon: " + std::to_string(epsilon));
    ep_rate.setCharacterSize(24);
    ep_rate.setFillColor(sf::Color::Black);
    ep_rate.setPosition(30,35);
    ep_rate.setFont(font);


    while (window.isOpen())

    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space){
                bird.fly(1);
            } 
        }
       
        // RESET CHECK / IM REWARD CHECK.
        if (bird.y < pipe.closest_pipe[1] && bird.x + bird.sprite.getGlobalBounds().width > pipe.closest_pipe[0] - pipe.pipex2B.getGlobalBounds().width && bird.x < pipe.closest_pipe[0] || bird.y < 0 || bird.y > 640 || bird.y + bird.sprite.getGlobalBounds().height > pipe.closest_pipe[2] && bird.x + bird.sprite.getGlobalBounds().width > pipe.closest_pipe[0] - pipe.pipex2B.getGlobalBounds().width && bird.x < pipe.closest_pipe[0]){
            bird.reset();
            pipe.reset();
        }

        // MOVE ENVIRONMENT
        bird.fly(0);
        pipe.move();
        Background.moveSprite(sf::Vector2f(0.05, 0.f));
        Background2.moveSprite(sf::Vector2f(0.05, 0.f));
        

        //DRAW
        text.setString("Score: " + std::to_string(score));
        ep_rate.setString("Epsilon: " + std::to_string(epsilon));
        window.clear();
        Background.draw(window);
        Background2.draw(window);
        pipe.draw(window);
        bird.draw(window);
        window.draw(text);
        window.draw(ep_rate);
        window.display();

            
        
    }

    return 0;
}