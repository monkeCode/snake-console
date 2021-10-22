#include <iostream>
#include <vector>
#include<conio.h>
#include <windows.system.h>
#include <thread>
//коды ошибок
enum class errorCode { hitSnake, hitWall };

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
    //определяет, есть ли змея по этим координатам
   bool itsSnake(int x, int y)
   {
      auto iter =  std::find_if(_blocks.begin(), _blocks.end(), [x, y](std::pair<int, int> b) {return (b.first == x && b.second == y); });
      return iter != _blocks.end();
   }
    //метод получения положения головы
    std::pair<int,int> get_head()
    {
        return _blocks[0];
    }
    std::pair<int,int> move(std::pair<int,int> dir)
    {
        auto lastBlock = _blocks[0];
        _blocks[0] = std::pair<int, int>(_blocks[0].first - dir.second, _blocks[0].second + dir.first);
	    for(int i =1 ; i < _blocks.size() - _takedEat; i++)
	    {
            auto mid = _blocks[i];
            _blocks[i] = lastBlock;
            lastBlock = mid;
	    }
        //если змеюка въехала сама в себя, бросит исключение
        if(std::find_if(_blocks.begin()+1, _blocks.end(), [&](std::pair<int,int>b){return _blocks[0] == b;})!= _blocks.end())
        {
            throw errorCode::hitSnake;
        }
        _takedEat = false;
        return _blocks[0];
    }
    void eat()
    {
        _blocks.push_back(std::make_pair(_blocks[_blocks.size() - 1].first, _blocks[_blocks.size() - 1].second));
        _takedEat = true;

    }
};
class game
{
private:
    int _side;
    bool _food_on_field = false;
    std::pair<int, int> foodCoord;
    snake _playerSnake;
    bool _isGameLife = true;
    int _score = 0;
    std::pair<int, int> _dir;

    //метод отрисовки поля и вывода в консоль
    void draw()
    {
        for(int i = 0; i<_side+1; i++)
        {
	        for(int k = 0; k < _side+1; k++)
	        {
		        if(i == 0 || i == _side || k == 0 || k == _side )
		        {
                    std::cout << "#";
		        }
                else if(i == _playerSnake.get_head().first && k == _playerSnake.get_head().second)
                {
                    std::cout << 'S';
                }
                else if(_playerSnake.itsSnake(i,k))
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
        std::cout << "direction: " << _dir.first << " " << _dir.second <<" Score: "<<_score;
    }

    //метод спавна еды, будет вызываться рекурсивно, пока не найдет случайное свободное место на поле
    void spawnFood()
    {
        int xPlace = rand() % (_side - 1) + 1;
        int yPlace = rand() % (_side - 1) + 1;
        if(_playerSnake.itsSnake(xPlace,yPlace))
        {
            return spawnFood();
        }
        foodCoord = std::pair<int, int>(xPlace, yPlace);
        _food_on_field = true;
    }

    //логика всей игры
	void logic()
    {
        //отлавливает нажатие на клавиатуре
        char key =' ';
        if (_kbhit())
        {
            key = _getch();
        }
        //определяет направление движения
        switch (key)
        {
        case 'w':
            if (_dir.second != -1)
                _dir = std::pair<int, int>(0, 1);
            break;
        case 'a':
            if (_dir.first != 1)
                _dir = std::pair<int, int>(-1, 0);
            break;
        case 'd':
            if (_dir.first != -1)
                _dir = std::pair<int, int>(1, 0);
            break;
        case 's':
            if (_dir.second != 1)
                _dir = std::pair<int, int>(0, -1);
            break;
        }

        //пытается подвинуть змеюку
        std::pair<int, int> headPos;
        try { headPos = _playerSnake.move(_dir); }
        catch (errorCode except)
        {
            if(except == errorCode::hitSnake)
            _isGameLife = false;
        }
        //проверка на въезд в стену
        if(headPos.first == 0 || headPos.first == _side || headPos.second == 0 || headPos.second == _side)
        {
            _isGameLife = false;
        }
        //проверка на еду
        if(headPos == foodCoord)
        {
            _playerSnake.eat();
            _food_on_field = false;
            _score++;
        }
        //спавн еды, если ее нет на поле
	    if(!_food_on_field)
	    {
            spawnFood();
	    }
    }
public:
    game(int side):_playerSnake(snake(std::make_pair(side / 2, side / 2)))
    {
        this->_side = side;
        srand(time(0));
    }

    void play()
    {
	    while(_isGameLife)
	    {
            std::system("cls");
            logic();
            draw();
            Sleep(500);
	    }
        std::cout << "\ngame over";
    }
};
int main()
{
    game g = game(25);
    g.play();
}

