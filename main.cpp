#include<iostream>
#include<vector>
#include<algorithm>
#include<deque>
#include<mutex>
#include<thread>
#include<windows.h>
#include<chrono>

std::mutex mtx;
char destination = '-';

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
    TSnake(size_t headX, size_t headY, size_t frame_height, size_t frame_width)
        : _points(std::deque<TPoint>(0)), frame_height(frame_height), frame_width(frame_width)
    {
        _points.push_front(TPoint{headX, headY});
    }

    bool CheckForCollision(size_t x, size_t y){
        return std::any_of(_points.begin(), _points.end(), [x, y](const TPoint& a) {
            return a.x == x && a.y == y;
        });
    }

    bool CheckForHead(size_t x, size_t y){
        return x == _points.front().x && y == _points.front().y;
    }

    bool CheckForApple(size_t x, size_t y){//////////////////////////////////////////////////////////////////////////////////////////////////////////
        return false;
    }

    void AddToTail(const TPoint& point) {
        _points.push_back(point);
    }

    void GameOver(){/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::cout << "GAME OVER" << "\n";
    }

    void SetFrameHeight(size_t _height){
        frame_height = _height;
    }

    void SetFrameWidth(size_t _width){
        frame_width = _width;
    }

    void Update(){
        mtx.lock();
        bool up = (GetAsyncKeyState(VK_UP) & 0x80000000), down = (GetAsyncKeyState(VK_DOWN) & 0x80000000), 
            right = (GetAsyncKeyState(VK_RIGHT) & 0x80000000), left = (GetAsyncKeyState(VK_LEFT) & 0x80000000);
        if (!(up | down | right | left)){
            if (destination == 'u'){
                up = true;
            } else if (destination == 'd'){
                down = true;
            } else if (destination == 'r'){
                right = true;
            } else if (destination == 'l'){
                left = true;
            }
        }    
        if (up){
            if (destination == 'u'){
                for (auto point = _points.begin(); point != _points.end(); ++point){
                    point->x--;
                }
            } else if (destination != 'd'){
                _points.push_front(TPoint{_points.front().x - 1, _points.front().y});
                _points.pop_back();
                destination = 'u';
            }
            if (CheckForApple(_points.front().x, _points.front().y)){
                _points.push_front(TPoint{_points.front().x - 1, _points.front().y});
            }
        } else if (down){
            if (destination == 'd'){
                for (auto point = _points.begin(); point != _points.end(); ++point){
                    point->x++;
                }
            } else if (destination != 'u'){
                _points.push_front(TPoint{_points.front().x + 1, _points.front().y});
                _points.pop_back();
                destination = 'd';
            }
            if (CheckForApple(_points.front().x, _points.front().y)){
                _points.push_front(TPoint{_points.front().x + 1, _points.front().y});
            }
        } else if (right){
            if (destination == 'r'){
                for (auto point = _points.begin(); point != _points.end(); ++point){
                    point->y++;
                }
            } else if (destination != 'l'){
                _points.push_front(TPoint{_points.front().x, _points.front().y + 1});
                _points.pop_back();
                destination = 'r';
            }
            if (CheckForApple(_points.front().x, _points.front().y)){
                _points.push_front(TPoint{_points.front().x, _points.front().y + 1});
            }
        } else if (left){
            if (destination == 'l'){
                for (auto point = _points.begin(); point != _points.end(); ++point){
                    point->y--;
                }
            } else if (destination != 'r'){
                _points.push_front(TPoint{_points.front().x, _points.front().y - 1});
                _points.pop_back();
                destination = 'l';
            }
            if (CheckForApple(_points.front().x, _points.front().y)){
                _points.push_front(TPoint{_points.front().x, _points.front().y - 1});
            }
        }
        for (auto i = _points.begin(); i != _points.end(); ++i){
            if (i->x == _points.front().x && i->y == _points.front().y && i != _points.begin()){
                GameOver();
            }
            if (i->x == 0){
                i->x = frame_height - 2;
            } else if (i->x == frame_height - 1){
                i->x = 1;
            } else if (i->y == 0){
                i->y = frame_width - 2;
            } else if (i->y == frame_width - 1){
                i->y = 1;
            }
        }
        mtx.unlock();
    }

private:
    std::deque<TPoint> _points;
    size_t frame_height;
    size_t frame_width;
};

class TField {
public:
    TField(size_t _height, size_t _width, TSnake _snake)
            : _height(_height), _width(_width), _snake(_snake)
    {}

    size_t GetHeight() const {
        return _height;
    }

    size_t GetWidth() const {
        return _width;
    }

    char GetSymbolByCoord(size_t x, size_t y){
        if (_snake.CheckForHead(x, y)){
            return '0';
        }
        if (_snake.CheckForCollision(x, y)){
            return 'o';
        }
        if (x == 0 || x == _height - 1 || y == 0 || y == _width - 1) {
            return '#';
        }
        return ' ';
    }
private:
    TSnake _snake;
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
    TSnake snake(8, 1, 10, 15);
    snake.AddToTail({7, 1});
    snake.AddToTail({6, 1});
    TField field(10, 15, snake);
    TGameFrame frame(field, 10, 15);
    frame.DrawFrame();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    snake.Update();
    TField field1(10, 15, snake);
    TGameFrame frame1(field1, 10, 15);
    frame1.DrawFrame();
}