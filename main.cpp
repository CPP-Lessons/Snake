#include<iostream>
#include<vector>
#include<algorithm>

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

    char GetSymbolByCoord(size_t x, size_t y) {
        if (x == 0 || x == _height - 1 || y == 0 || y == _width - 1) {
            return '#';
        }
        if (_matrix[3][_count][0] == x && _matrix[3][_count][1] == y){
            _count += 1;
            return '#';
        }
        return ' ';
    }
private:
    int _matrix[10][10][2] = {{},
                            {{3, 5}, {4, 3}},
                            {{3, 2}, {3, 7}, {6, 3}},
                            {{3, 3}, {3, 7}, {5, 5}, {7, 7}},
                            {{3, 3}, {3, 4}, {4, 3}},
                            {{3, 3}, {3, 4}, {4, 3}},
                            {{3, 3}, {3, 4}, {4, 3}},
                            {{3, 3}, {3, 4}, {4, 3}},
                            {{3, 3}, {3, 4}, {4, 3}},
                            {{3, 3}, {3, 4}, {4, 3}}};
    int _count = 0;
    size_t _height;
    size_t _width;
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
            return _main_field.GetSymbolByCoord(x, y);
        }
        std::cout << "Ooops!" << std::endl;
        return ' ';
    }
};

int main() {
    TField field(10, 10);
    TGameFrame frame(field, 10, 10);
    TSnake snake(0, 0);
    snake.AddToTail({1, 1});
    std::cout << snake.CheckForCollision(1, 0) << std::endl;
    frame.DrawFrame();
    system("pause");
}