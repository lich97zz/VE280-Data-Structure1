#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
using namespace std;
struct argc_exception{};
struct round_limit_exception{};
struct file_exception{
	string file_path;
};
struct species_num_exception{
	string max;
};
struct instruction_num_exception{
	string species;
	string max;
};//5
struct instruction_exception{
	string invalid_instruction;
};
struct grid_height_exception{};
struct grid_width_exception{};
struct terrain_exception{
	string terrain;
};
struct creature_num_exception{
	string max;
}; //10
struct unknown_species_exception{
	string unknown_species;
};
struct direction_exception{
	string unknown_direction;
};
struct boundary_exception{
	string creature;
	string size;
};
struct ability_exception{
	string creature;
	string ability;
};
struct overlap_exception{ //15
	string creature_1;
	string creature_2;
};
struct drown_exception{
	string creature;
};

int main(){
	int i = 12;
	char ss[20];
	sprintf(ss, "%d", i);
	string s = ss;
}