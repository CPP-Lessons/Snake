#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<thread>
#include<mutex>
#include<chrono>
#include<windows.h>

struct TOffset {
    size_t x;
    size_t y;
};

struct TPoint {
    size_t x;
    size_t y;
    char dest;
};

struct TConfig {
    size_t fps;
};

TConfig config = {
        10
};

bool isOver = false;
std::mutex mtx;
char destination = 'u';

class TSnake {
public:
    TSnake()
    {}

    bool CheckForCollision(size_t x, size_t y) const {
        return std::any_of(_points.begin(), _points.end(), [x, y](const TPoint& a) {
            return a.x == x && a.y == y;
        });
    }

    void AddToTail(const TPoint& point) {
        _points.push_back(point);
    }
    
    TPoint& GetHead() {
    	return _points[0];
	}
	
	std::vector<TPoint>& GetBody() {
		return _points;
	}
	
	char GetSymbolByCoord(size_t x, size_t y) const {
		if (_points[0].x == x && _points[0].y == y) {
			return '0';
		}
		return 'o';
	} 
	
private:
    std::vector<TPoint> _points;
};

class TField {
public:
    TField(size_t _height, size_t _width, TSnake& snake)
            : _height(_height), _width(_width), _snake(snake)
    {}

    size_t GetHeight() const {
        return _height;
    }

    size_t GetWidth() const {
        return _width;
    }

    char GetSymbolByCoord(size_t x, size_t y) const {
        if (x == 0 || x == _height - 1 || y == 0 || y == _width - 1) {
            return '#';
        }
        if (_snake.CheckForCollision(x, y)) {
        	return _snake.GetSymbolByCoord(x, y);
		}
        return ' ';
    }
private:
    size_t 	_height;
    size_t 	_width;
    TSnake& _snake;
};

class TScore
{
public:

    TScore()
    {
        _score = 0;
        _time = 0;
    }

    void Add(int score)
    {
        _score+= score;
    }

    std::string GetScoreString()
    {
        return ("Score: " + std::to_string(_score));
    }

    void AddTime(float deltaTime)
    {
        _time += deltaTime;
    }

    std::string GetTimeString()
    {
        return ("Time: " + std::to_string(_time));
    }
private:
    int _score;
    unsigned _time;
};

class TScoreBoard
{
public:
    void AddScore(TScore score)
    {
        _scores.push_back(score);
    }

private:
    std::vector<TScore> _scores;
};


class THeadVisualisation
{
public:
    char getSymbolByCoord(size_t x, size_t y)
    {
        std::string currentScoreString = _score.GetScoreString();
        if (y < currentScoreString.size() &&  x == 0)
            return currentScoreString[y];
        currentScoreString = _score.GetTimeString();
        if(y < currentScoreString.size() && x == 1)
            return currentScoreString[y];
        return ' ';
    }
private:
    TScore _score;
};

class TGameFrame {
public:
    TGameFrame(TField& field, size_t height, size_t width)
            :
            _field_offset({0, 0}),
            _main_field(field),
            _height(height),
            _width(width)
    {}

    void SetFieldOffset(TOffset offset) {
        _field_offset = offset;
    }

    void DrawFrame() {
        for (size_t i = 0; i < _height; ++i) {
            for (size_t j = 0; j < _width; ++j) {
                std::cout << getSymbolByCoord(i, j);
            }
            std::cout << std::endl;
        }
    }
private:
    THeadVisualisation _head;
    TField& _main_field;
    TOffset _field_offset;
    size_t _height;
    size_t _width;

private:
    char getSymbolByCoord(size_t x, size_t y) {
        size_t minFieldH = _field_offset.x;
        size_t maxFieldH = minFieldH + _main_field.GetHeight();
        size_t minFieldW = _field_offset.y;
        size_t maxFieldW = minFieldW + _main_field.GetWidth();
        
		if (x >= minFieldH && x < maxFieldH && y >= minFieldW && y < maxFieldW) {
            return _main_field.GetSymbolByCoord(x - _field_offset.x, y - _field_offset.y);
        }
        return _head.getSymbolByCoord(x, y);
    }
};

void FakeConsole() {
    mtx.lock();
    std::cout << "FakeConsole";
    mtx.unlock();
}

void UpdateFrame() {

    while(true) {
//        TSnake.isPressed();

        FakeConsole();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void GameOver(){
    std::vector<std::string> gm = {
        "  ####   ####  ##   ## ##### ",
        " ##     ##  ## ### ### ## ",
        " ## ### ###### ## # ## #### ",
        " ##  ## ##  ## ##   ## ## ",
        "  ####  ##  ## ##   ## ##### ",
        " ",
        "  ####  ##  ## ##### ##### ",
        " ##  ## ##  ## ##    ## ## ",
        " ##  ## ##  ## ####  ##### ",
        " ##  ##  ####  ##    ##  ## ",
        "  ####    ##   ##### ##  ## "
    };
    system("cls");
    mtx.lock();

    for(int i = gm.size() - 1; i >= 0; i--) {
        for(int j = i; j < gm.size(); j++){
            std::cout << gm[j] << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(i > 0){
            system("cls");
        }
    }

    mtx.unlock();
}

void Update(TSnake& snake){
    bool up     = (GetAsyncKeyState(VK_UP) & 0x80000000);
    bool down   = (GetAsyncKeyState(VK_DOWN) & 0x80000000);
    bool right  = (GetAsyncKeyState(VK_RIGHT) & 0x80000000);
    bool left   = (GetAsyncKeyState(VK_LEFT) & 0x80000000);

	
    mtx.lock();
    if (up && destination != 'd') {
		destination = 'u';
	} else if (down && destination != 'u') {
		destination = 'd';
	} else if (right && destination != 'l') {
		destination = 'r';
	} else if (left && destination != 'r') {
		destination = 'l';
	}

    
    decltype(snake.GetBody()) body = snake.GetBody(); 
	for (size_t i = body.size() - 1; i >= 0; i--) {
		size_t dx = 0;
    	size_t dy = 0;
		 
		char currDest;
		if (i == 0) {
			currDest = destination;
		} else {
			currDest = body[i - 1].dest;
		}
		switch (currDest) {
			case 'u':
				dx = -1;
			break;
			case 'd':
				dx = 1;
			break;
			case 'r':
				dy = 1;
			break;
			case 'l':
				dy = -1;
			break;	
		}
		body[i].dest = currDest;
		body[i].x += dx;
		body[i].y += dy;
		if (i == 0) {
			break;
		}
	}

    mtx.unlock();
}

void DrawFrameLoop(TGameFrame& frame) {
	while(true) {
		frame.DrawFrame();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		system("cls");
		mtx.lock();
		if (isOver) {
			mtx.unlock();
			break;
		}
		mtx.unlock();
	}
	GameOver();
}

void UpdateLoop(TSnake& snake) {
	while(true) {
		Update(snake);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		decltype(snake.GetHead()) head = snake.GetHead();
		system("cls");
		mtx.lock();
		if (head.x == 0 || head.y == 0) {
			isOver = true;
			mtx.unlock();
			break;
		}
		mtx.unlock();
	}
}

int main() {
	TSnake snake;
	snake.AddToTail({4, 4, 'l'});
    snake.AddToTail({4, 5, 'l'});
    snake.AddToTail({4, 6, 'l'});
    snake.AddToTail({4, 7, 'l'});
    snake.AddToTail({4, 8, 'l'});
    
	TField field(10, 10, snake);
    
	TGameFrame frame(field, 20, 20);
    
	frame.SetFieldOffset({10, 0});

	std::thread thr2(DrawFrameLoop, std::reference_wrapper<TGameFrame>(frame));
	std::thread thr1(UpdateLoop, std::reference_wrapper<TSnake>(snake));
//    thr.join();
	thr1.join();
	thr2.join();
//    return 0;
}
