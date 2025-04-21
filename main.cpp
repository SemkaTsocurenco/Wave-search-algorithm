#include <iostream>
#include <queue>
#include "./Cave_struct.h"

using value_type = int;



bool wave_algorithm(DMatrix<kCave>& cave, std::pair<value_type, value_type> start, std::pair<value_type, value_type> end, std::vector<std::pair<int, int>>& path)
{
		bool found = false;
		std::queue<std::pair<value_type,value_type>> que;
		que.push(start);
    	constexpr int dirs[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
		cave(start.first, start.second) = 0;

		while (!que.empty()){
			auto [r,c] = que.front(); que.pop();
    		if (r==end.first && c==end.second) { found=true; break; }
			for (auto [dr, dc] : dirs){
				int nr = r + dr;
				int nc = c + dc;
				if ((cave(nr, nc)==-1)){
					cave(nr, nc) = cave(r, c) + 1; 
					que.push({nr,nc});
				}
			}
		}
		if (!found) return false;

		int r=end.first, c=end.second;
		while (cave(r , c) != 0) {
			path.push_back({r,c});
			for (auto [dr,dc]: dirs) {
				int nr=r+dr, nc=c+dc;
				if (cave(nr,nc) == cave(r,c) - 1) {
					cave(r, c) = 0;
					r=nr; c=nc;
					break;
				}
			}
		}
		path.push_back(start);
		std::reverse(path.begin(), path.end());

    return true;
};

// void print_path(DMatrix<kCave>& cave, const std::vector<std::pair<int, int>>& path) {
//     for (size_t r = 0; r < cave.rows; ++r) {
//         std::cout << std::setw(3) << r << "  ";
//         for (size_t c = 0; c < cave.cols; ++c) {
//             if (std::find(path.begin(), path.end(), std::make_pair((int)r, (int)c)) != path.end())
//                 std::cout << std::setw(2) << '*';
//             else
//                 std::cout << std::setw(2) << (cave(r, c) ? '#' : ' ');
//         }
//         std::cout << "\n";
//     }
//     std::cout << "\n" << std::setw(5) << "";
//     for (size_t c = 0; c < cave.cols; ++c) std::cout << std::setw(2) << c;
//     std::cout << "\n\n";
// }

int main() {
    DMatrix<kCave> cave; 
    cave.read_cave("../my_cave.txt");
    cave.print();

    std::pair<value_type,value_type> start;
    std::pair<value_type,value_type> end;
    std::cout << "Введите начальную точку: ";
    std::cin >> start.first >> start.second;
    std::cout << "Введите конечную точку: ";
    std::cin >> end.first >> end.second;

    std::vector<std::pair<int, int>> path;
    if (wave_algorithm(cave, start, end, path)) {
        std::cout << "\nПуть найден!\n\n";
        cave.print();
    } else {
        std::cout << "Путь не найден!\n";
    }

    return 0;
}
