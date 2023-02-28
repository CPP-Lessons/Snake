#include<iostream>
#include<vector>
#include<algorithm>
#include<string>

struct TOffset {
    size_t x;
    size_t y;
};

struct TPoint {
    size_t x;
    size_t y;
};

class TSnake {
public:
    TSnake(size_t headX, size_t headY)
        : _points(std::vector<TPoint>(1))
    {
        _points[0].x = headX;
        _points[0].y = headY;
    }

    bool CheckForCollision(size_t x, size_t y) {
        return std::any_of(_points.begin(), _points.end(), [x, y](const TPoint& a) {
            return a.x == x && a.y == y;
        });
    }

    void AddToTail(const TPoint& point) {
        _points.push_back(point);
    }
private:
    std::vector<TPoint> _points;
};

class TField {
public:
    TField(size_t _height, size_t _width)
            : _height(_height), _width(_width)
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
        return ' ';
    }
private:
    size_t _height;
    size_t _width;
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
	
	std::string GetStringScores()
	{
		for(int i = 0; i < _scores.size(); i++)
		{
			std::cout << "Game: " << i << std::endl;
			std::cout << _scores[i].GetScoreString() << std::endl;
			std::cout << _scores[i].GetTimeString() << std::endl << std::endl;
		}
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
    TGameFrame(TField field, size_t height, size_t width)
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
    TField _main_field;
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
        _head.getSymbolByCoord(x, y);
    }
};

int main() {
	
	TField field(10, 10);
	TGameFrame frame(field, 20, 20);
	frame.SetFieldOffset({10, 0});
//    TSnake snake(0, 0);
//    snake.AddToTail({1, 1});
//    std::cout << snake.CheckForCollision(1, 0) << std::endl;
    frame.DrawFrame();
}
