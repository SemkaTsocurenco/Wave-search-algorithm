#ifndef CAVE_STRUCT_H
#define CAVE_STRUCT_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>
#include <fstream>    

enum Kind {
    kCave,
    kForest
};

constexpr char kRed[]   = "\x1B[30;44m";
constexpr char kGreen[]   = "\x1B[36m";
constexpr char kReset[] = "\x1B[0m";

template <Kind>
struct DMatrix { };

template<>
struct DMatrix<kCave> {


    using value_type  = int;
    using data_type   = std::vector<value_type>;
    using return_type = DMatrix<kCave>;

    data_type   data;
    value_type  rows = 0, cols = 0;

    int &operator()(std::size_t row, std::size_t col) {
        return data[row * cols + col];
    }

    int operator()(std::size_t row, std::size_t col) const {
        if (row >= rows || col >= cols)
            return 1;
        return data[row * cols + col];
    }


    void print() const {
    // 1) вычисляем min и max среди всех ненулевых значений
    int vmin = std::numeric_limits<int>::max();
    int vmax = std::numeric_limits<int>::min();
    for (int x : data) {
        if (x > 0) {
            vmin = std::min(vmin, x);
            vmax = std::max(vmax, x);
        }
    }
    // на случай, если все v<=0
    if (vmin > vmax) { vmin = 0; vmax = 1; }

    auto color_for = [&](int v) {
        // нормальзуем t в [0,1]
        double t = double(v - vmin) / double(vmax - vmin);
        if (t < 0) t = 0;
        if (t > 1) t = 1;
        int R = int(t * 255);
        int G = int((1 - t) * 255);
        // escape‑код для установки цвета текста: \x1B[38;2;<r>;<g>;<b>m
        std::ostringstream ss;
        ss << "\x1B[30m"<<"\33[48;2;" << R << ";" << G <<";0m";
        return ss.str();
    };

    std::cout <<"\n\n\x1B[46m"<<std::setw(5)<< "";
    for (std::size_t c = 0; c < cols+1; ++c) {
        std::cout <<"\x1B[46m"<<std::setw(2)<< c;
    }
    std::cout << "\x1B[0m\n";
    for (std::size_t r = 0; r < rows; ++r) {
        std::cout<< "\x1B[46m"<< std::setw(3) << r << "  \x1B[0m";
        for (std::size_t c = 0; c < cols; ++c) {
            int v = (*this)(r, c);
            if (v == -2) {
                std::cout << kGreen<< std::setw(2) << "██" << kReset;
            }
            else if (v == -1) {
                std::cout << std::setw(2) << ' ' << kReset;
            }
            else if (v == 0) {
                std::cout << kRed << std::setw(2) << "▒▒" << kReset;
            }
            else {
                // здесь всё, что >0, красим градиентом
                std::string col = color_for(v);
                std::cout << col << std::setw(2) << v << kReset;
            }
        }
        std::cout << "\x1B[36m██\n\x1B[0m";
    }
    
    for (std::size_t c = 0; c < cols+3; ++c) {
        std::cout<<"\x1B[36m"<<"██";
    }
    std::cout << "█\x1B[0m\n\n\n";

    }



    bool read_cave(const std::string &fileName){
        std::ifstream in(fileName);
        if (!in.is_open()) {
            return false;
        }

        data.clear();      
        rows = 0;
        cols = 0;
        std::vector<std::string> lines;
        std::string line;

        while (std::getline(in, line)) {

            if (cols == 0) {
                cols = static_cast<value_type>(line.size());
            } else if (line.size() != cols) {
                std::cerr << "Ошибка: inconsistent line length at row "
                        << rows << "\n";
                return false;
            }
            lines.push_back(line);
            ++rows;
        }
        in.close(); 

        data.reserve(rows * cols);
        for (const auto &l : lines) {
            for (char c : l) {
                if (c == '#')  data.push_back(-2);
                else /* любой другой */ data.push_back(-1);
            }
        }

        return true;
        
    }



};

#endif // CAVE_STRUCT_H
