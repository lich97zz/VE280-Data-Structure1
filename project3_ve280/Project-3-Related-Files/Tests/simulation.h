#ifndef SIMULATION_H
#define SIMULATION_H

//define some exception type for error checking
struct argc_exception{};
struct round_limit_exception{};
struct file_exception{
	string file_path;
};
struct species_num_exception{};
struct instruction_num_exception{ //5
	string species;
};
struct instruction_exception{
	string invalid_instruction;
};
struct grid_height_exception{};
struct grid_width_exception{};
struct terrain_exception{
	string terrain;
};
struct creature_num_exception{}; //10
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

string split(string &str);
//EFFECTS:cut the first word of str(words are split by a space), 
//			original str will be modified, return the first word
//MODIFIES: str becomes the original string without its first word

string itos(int i);
//REQUIRES: lengths of i can't exceed 10
//EFFECTS: turn an integer to string type

int give_index(string tar, const string str_array[], const int array_size);
//EFFECTS: return the index of parameter tar in str_array, if can't find tar, return -1

species_t read_species(string species_name, string file_name);
//EFFECTS: read the file regarded with species, and construct in the format as species_t type

void read_path(string file_name, world_t &world);
//EFFECTS: read the path file, write the content into the world
//MODIFIES: the species, numSpecies of world

void read_world(string file_name, world_t &world);
//EFFECTS: construct the world according to the given file
//MODIFIES: the world's grid, creatures, numCreature

void tell_world(const world_t world);
//EFFECTS: print the current situation of world

void process_location(direction_t d, int step, int &loc_x, int &loc_y);
//EFFECTS: process the location according to the direction and step
//MODIFIES: the location loc_x and loc_y

void to_front_location(const world_t &world, int &start_y, int &start_x);
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: process the current location, change it to the front location
//		according to the direction that the creature is facing to
//MODIFIES: the location loc_y, loc_x

bool front_out_of_bound(world_t &world, int loc_y, int loc_x);
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: test whether the front location is out of bound of the world

bool front_is_empty(world_t &world, int loc_y, int loc_x);
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: test whether the front location is not occupied by a creature

bool front_is_enemy(world_t &world, int loc_y, int loc_x);
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: test whether the front location is occupied by an enemy

bool front_is_friend(world_t &world, int loc_y, int loc_x);
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: test whether the front location is occupied by a friend(the same kind of creature)

bool front_not_plain(world_t &world, int loc_y, int loc_x, string terrain);
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: test whether the front terrain is the specified type of terrain

bool front_is_wall(world_t &world, int loc_y, int loc_x);
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: test whether the front position is a wall(out of bound or being lake to creature not able to fly)

bool action_hop(world_t &world, int loc_y, int loc_x);
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: make the creature hop, it goes to the front by 1 step
//		if the front is not out of bound/ not facing a wall/ the front is not occupied
//MODIFIES: change the infomation of that creature in the world consistently

bool action_turn(world_t &world, int start_y, int start_x, string turn_to);
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: make the creature change its direction
//MODIFIES: change the infomation of that creature in the world consistently

bool action_infect(world_t &world, int loc_y, int loc_x);
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: infect the creature in the front if possible
//MODIFIES: change the infomation of the creature in the world consistently

void creature_action(world_t &world, creature_t &target);
//EFFECTS: make the target do action according to its instruction list
//MODIFIES: change the infomation of the creature in the world consistently

bool end_turn_action(creature_t &target);
//EFFECTS: check if some action can ends the turn

void creature_round_action(world_t &world, creature_t &target, bool verbose);
//EFFECTS: simulate the behavior of a creature in one round
//MODIFIES: change the infomation of the creature in the world consistently

void go_round(world_t &world, int &round_num, bool verbose);
//EFFECTS: simulate the behavior of all creatures in one round
//MODIFIES: change the infomation of the creature in the world consistently

void world_simulation(int argc, char* argv[]);
//EFFECTS: simulate the world without exception handle

void project3(int argc, char* argv[]);
//EFFECT: add exception handle to the world_simulation function


#endif