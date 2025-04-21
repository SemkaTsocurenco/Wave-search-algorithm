#include <iostream>


int main (){
	for (int i = 30 ; ; i++ ){
		std::cout<<"\r\33["<<std::to_string(i)<<"mWave search algorithm!"<<std::flush;
		i = (i>36) ? (30) : (i + 1);
	}
	return 0;
}
