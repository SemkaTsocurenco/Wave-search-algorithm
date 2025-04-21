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

constexpr char kRed[]   = "\x1B[31m";
constexpr char kGreen[]   = "\x1B[33m";
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
        for (std::size_t r = 0; r < rows; ++r) {
            std::cout <<std::setw(3)<< r;
            std::cout <<std::setw(2)<<"";
            for (std::size_t c = 0; c < cols; ++c) {
                int v = (*this)(r, c);

                if (v == -2)
                    std::cout << kGreen << std::setw(2) << '#'<< kReset;

                else if (v == -1)
                    std::cout << std::setw(2) << ' '<< kReset;

                else if (v == 0)
                    std::cout << kRed<< std::setw(2) << '*'<< kReset;

                else 
                    std::cout << std::setw(2) << std::to_string(v)<< kReset;

            }
            std::cout << "\n";
        }
        std::cout << "\n";
        std::cout <<std::setw(5)<< "";
        for (std::size_t c = 0; c < cols; ++c) {
            std::cout <<std::setw(2)<< c;
        }
        std::cout << "\n\n\n";

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
