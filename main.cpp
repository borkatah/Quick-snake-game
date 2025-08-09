#include "./raylib/src/raylib.h"
#include <iostream>
#include <deque>
#include <random>
using namespace std;





//Colors
Color brightgreen = {173, 204, 96, 255};
Color darkgreen = {43, 51, 24, 255};

//Grid parameters - playing field
const int cell_count = 40;
const int cell_size = 25;


//Snake parameters

enum Direction{
    stop, up, down, leftt, rightt
};


//Fruit
class Fruit{
public:

    Vector2 position;

    Fruit(){
        set_random_position();
    }

    ~Fruit(){
        ClearBackground(RED);
    }

    void set_random_position(){
        float x, y;
        x = (float)GetRandomValue(0, cell_count - 1);
        y = (float)GetRandomValue(0, cell_count - 1);
        position = Vector2{x, y};
    }

    void draw(){
        DrawRectangle(position.x * cell_size, position.y * cell_size, cell_size, cell_size, BROWN);
    }
};


//Snake
class Snake{
public:
    //initial position and movement direction of snake:
    deque<Vector2> snake_cells = {{12, 11}, {12, 12}, {12, 13}, {12, 14}, {12, 15}};
    Direction direction = stop;
    

    void draw(){
        for (int i = 0; i < snake_cells.size(); i++){
            DrawRectangle(snake_cells[i].x * cell_size, snake_cells[i].y * cell_size, 
                          cell_size, cell_size, darkgreen);
        }
    }

    void update_position(bool is_eaten){

        int key = GetKeyPressed();        
        //change direction
        switch(key){
            case KEY_UP:
                if(direction != down){  
                    direction = up;
                } 
                break;
            case KEY_DOWN: 
                if(direction != up && direction != stop){
                    direction = down;
                }
                break;
            case KEY_LEFT:
                if(direction != rightt){
                    direction = leftt;
                }
                break;
            case KEY_RIGHT:
                if(direction != leftt){
                    direction = rightt;
                }
                break;
            defult:
                cout << "Player hasn't started moving yet ...";
        }

        if(direction != stop && !is_eaten){
            snake_cells.pop_back();
        }
        
        switch(direction){
            case up:
                snake_cells.push_front({snake_cells[0].x,snake_cells[0].y - 1});
                break;
            case down: 
                snake_cells.push_front({snake_cells[0].x,snake_cells[0].y + 1});
                break;
            case rightt:
                snake_cells.push_front({snake_cells[0].x + 1,snake_cells[0].y});
                break; 
            case leftt:
                snake_cells.push_front({snake_cells[0].x - 1,snake_cells[0].y});
                break;
        }

        

    }

    bool check_collision(){

        const Vector2& head = snake_cells[0];
        
        if(head.x > cell_count-1 || head.y > cell_count-1 || head.x < 0 || head.y < 0)
            return true;

        for(int i = 1; i < snake_cells.size(); i++){

            const Vector2& other = snake_cells[i];

            if(head.x == other.x && head.y == other.y)
                return true;
        }

        return false;
    }

};



bool check_eat(const Vector2& head, const Vector2& fruit){
    if(head.x == fruit.x && head.y == fruit.y)
        return true;
    return false;
}


class Score{
public:

    int value = 0;
    const int horizontal_offset = 5;

    void increment(){
        value++;
    }

    void draw(){
        DrawLine(0, cell_count * cell_size, cell_count * cell_size, cell_count * cell_size, BLACK);
        DrawText(TextFormat("Score: %i", value) , horizontal_offset, cell_count * cell_size + 5, 20, BLACK);
    }

};


int main(){
    
    cout << "Starting the game...";
    InitWindow(cell_count * cell_size, (cell_count + 1 ) * cell_size , "My Snake Game");

    SetTargetFPS(13);
    
    Score score;
    Fruit fruit;
    Snake snake;
    bool GameOver = false;
    while(WindowShouldClose() == false){
        if(GameOver){
            BeginDrawing();

            ClearBackground(brightgreen);

            DrawText("Game Over!", (cell_count * cell_size) / 3, (cell_count * cell_size) / 2, 70, BLACK);

            EndDrawing();
        } else{

            bool is_eaten = check_eat(snake.snake_cells[0], fruit.position);
        
        
            if(is_eaten){
                fruit.set_random_position();
                score.increment();

            }
                

            if(snake.check_collision())
                GameOver = true;

            BeginDrawing();

            ClearBackground(brightgreen);
            
            
                
            fruit.draw();
            snake.draw();
            score.draw();

            EndDrawing();
            snake.update_position(is_eaten);

        }
        
    }


    return 0;
}