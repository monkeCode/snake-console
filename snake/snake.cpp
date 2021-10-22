#include <iostream>
#include <vector>
#include<conio.h>
#include <windows.system.h>

class snake
{
private:
    std::vector<std::pair<int, int>> _blocks;
    bool _takedEat = false;

public:
    snake(std::pair<int,int> startPos)
    {
        _blocks = std::vector<std::pair<int, int>>();
        _blocks.push_back(startPos);
    }
   bool itsSnake(int x, int y)
   {
      auto iter =  std::find_if(_blocks.begin(), _blocks.end(), [x, y](std::pair<int, int> b) {return (b.first == x && b.second == y); });
      return iter != _blocks.end();
   }
    std::pair<int,int> get_head()
    {
        return _blocks[0];
    }
    std::pair<int,int> move(std::pair<int,int> dir)
    {
        auto lastBlock = _blocks[0];
        _blocks[0] = std::pair<int, int>(_blocks[0].first + dir.first, _blocks[0].second - dir.second);
	    for(auto& block : _blocks)
	    {
            auto mid = block;
            block = lastBlock;
            lastBlock = mid;
	    }
        return _blocks[0];
    }
    void eat()
    {
	    
    }
};
class game
{
private:
    int _side;
    bool _food_on_field = false;
    std::pair<int, int> foodCoord;
    snake playerSnake;
    bool _isLife = true;
    std::pair<int, int> dir;
    void draw()
    {
        for(int i = 0; i<_side; i++)
        {
	        for(int k = 0; k < _side; k++)
	        {
		        if(i == 0 || i == _side-1 || k == 0 || k == _side - 1)
		        {
                    std::cout << "#";
		        }
                else if(i == playerSnake.get_head().first && k == playerSnake.get_head().second)
                {
                    std::cout << 'S';
                }
                else if(playerSnake.itsSnake(i,k))
                {
                    std::cout << 's';
                }
                else if (i == foodCoord.first && k == foodCoord.second)
                {
                    std::cout << 'f';
                }
                else
                {
                    std::cout << " ";
                }
                
	        }
            std::cout << std::endl;
        }
    }
    void spawnFood()
    {
        int xPlace = rand() % (_side - 1) + 1;
        int yPlace = rand() % (_side - 1) + 1;
        if(playerSnake.itsSnake(xPlace,yPlace))
        {
            return spawnFood();
        }
        foodCoord = std::pair<int, int>(xPlace, yPlace);
        _food_on_field = true;
    }
	void logic()
    {
        char key =' ';
        if (_kbhit())
        {
            key = _getch();
        }

        switch(key)
        {
        case 'w':
            dir = std::pair<int, int>(0, 1);
            break;
        case 'a':
            dir = std::pair<int, int>(-1, 0);
            break;
        case 'd':
            dir = std::pair<int, int>(1, 0);
            break;
        case 's':
            dir = std::pair<int, int>(0, -1);
            break;
        }
        auto headPos = playerSnake.move(dir);
        if(headPos.first == 0 || headPos.first == _side || headPos.second == 0 || headPos.second == _side)
        {
            _isLife = false;
        }
        if(headPos == foodCoord)
        {
	        
        }
	    if(!_food_on_field)
	    {
            spawnFood();
	    }
    }
public:
    game(int side):playerSnake(snake(std::make_pair(side / 2, side / 2)))
    {
        this->_side = side;
        srand(time(0));
    }

    void play()
    {
	    while(_isLife)
	    {
            std::system("cls");
            logic();
            draw();
	    }
        std::cout << "game over";
    }
};
int main()
{
    game g = game(25);
    g.play();
}

