#include <iostream>
#include "world_type.h"
#include <fstream>
#include <cstdlib>
#include "simulation.h"
using namespace std;

string split(string &str){
	//EFFECTS:cut the first word of str(words are split by a space), 
	//			original str will be modified, return the first word
	//MODIFIES: str becomes the original string without its first word
	int len = str.length();
	string temp = "";
	for(int i=0; i<len; i++){
		if (str[i] != ' ')
			temp += str[i]; 
		else{
			str = (i==len-1) ? "" : str.substr(i+1,len); 
			break;
		}
		if(i==len-1)
			str = "";
	}
	return temp;
} 

int give_index(string tar, const string str_array[], const int array_size){
	//EFFECTS: return the index of parameter tar in str_array, if can't find tar, return -1
	for(int i=0; i<array_size; i++){
		if(str_array[i] == tar)
			return i;
	}
	return -1;
}

species_t read_species(string species_name, string file_name){
	//EFFECTS: read the file regarded with species, and construct in the format as species_t type
	species_t result;
	result.name = species_name;
	result.programSize = 0;
	string temp = "";
	ifstream f_stream(file_name.c_str());
	//check status of file
	if (!f_stream){
		file_exception err;
		err.file_path = file_name;
		throw err;
	}
	while (!f_stream.eof()){
		getline(f_stream, temp);
		if(temp.length()){
			//check if there is too many program
			if(result.programSize >= MAXPROGRAM){
				instruction_num_exception err;
				err.species = species_name;
				throw err;
			}
			string instruction_name = split(temp);
			int index = give_index(instruction_name, opName, OP_SIZE);
			//check invalid instruction
			if(index == -1){
				instruction_exception err;
				err.invalid_instruction = instruction_name;
				throw err;
			}
			opcode_t t1 = opcode_t(index);
			string temp2 = split(temp);
			if(temp2 != ""){
				result.program[result.programSize].op = t1;
				result.program[result.programSize].address = (unsigned int)atoi(temp2.c_str());
			}
			else{
				result.program[result.programSize].op = t1;
				result.program[result.programSize].address = 0;
			}
			result.programSize += 1;
		}
		else 
			break;
	}
	f_stream.close();
	return result;
}

void read_path(string file_name, world_t &world){
//EFFECTS: read the path file, write the content into the world
//MODIFIES: the species, numSpecies of world
	ifstream f_stream(file_name.c_str());
	//check the file status
	if (!f_stream){
		file_exception err;
		err.file_path = file_name;
		throw err;
	}
	string path_name = "";
	getline(f_stream, path_name);
	string temp = "";
	world.numSpecies = 0;
	while (!f_stream.eof()){
		getline(f_stream, temp);
		if(temp != ""){
			world.species[world.numSpecies] = read_species(temp, path_name + "/" + temp);
			world.numSpecies += 1;
			//check if there is too many species
			if(world.numSpecies >= MAXSPECIES){
				species_num_exception err;
				throw err;
			}
		}
	}
	f_stream.close();
}

string itos(int i){
//REQUIRES: lengths of i can't exceed 10
//EFFECTS: turn an integer to string type
	char ss[10];
	sprintf(ss, "%d", i);
	string i_str = ss;
	return i_str;
};

void read_world(string file_name, world_t &world){
//EFFECTS: construct the world according to the given file
//MODIFIES: the world's grid, creatures, numCreature
	ifstream f_stream(file_name.c_str());
	//check status of file
	if (!f_stream){
		file_exception err;
		err.file_path = file_name;
		throw err;
	}
	string temp = "";
	getline(f_stream, temp);
	world.grid.height = atoi(temp.c_str());

	//check height
	if(world.grid.height <= 0 || world.grid.height > MAXHEIGHT){
		grid_height_exception err;
		throw err;
	}
	getline(f_stream, temp);
	world.grid.width = atoi(temp.c_str());
	//check width
	if(world.grid.width <= 0 || world.grid.width > MAXWIDTH){
		grid_width_exception err;
		throw err;
	}
	for(unsigned int i=0; i<world.grid.height; i++){
		getline(f_stream, temp);
		for(unsigned int j=0; j<world.grid.width; j++){
			string temp_str = " ";
			world.grid.squares[i][j] = NULL;
			temp_str[0] = temp[j];
			int index = give_index(temp_str, terrainShortName, TERRAIN_SIZE);
			//check if there is invalid terrain
			if(index == -1){
				terrain_exception err;				
				err.terrain = temp_str + " " + itos(i) + " " + itos(j);
				throw err; 
			}
			terrain_t t = terrain_t(index);
			world.grid.terrain[i][j] = t; 
		}
	}
	world.numCreatures = 0;

	while (!f_stream.eof()){
		getline(f_stream, temp);
		if(temp != ""){
			static creature_t c;
			string c_name = split(temp); 
			
			string temp_1 = split(temp);
			int index = give_index(temp_1, directName, DIRECT_SIZE);
			//check invalid direction
			if(index == -1){
				direction_exception err;
				err.unknown_direction = temp_1;
				throw err;
			}

			c.direction = direction_t(index);
			string world_species_name[world.numSpecies];
			for(unsigned int j=0; j<world.numSpecies; j++)
				world_species_name[j] = world.species[j].name;

			//check invalid species
			index = give_index(c_name, world_species_name, world.numSpecies);
			if(index == -1){
				unknown_species_exception err;
				err.unknown_species = c_name;
				throw err;
			}
			c.species = &world.species[index];
	
			c.programID = 0;
			int c_y = atoi(split(temp).c_str());
			int c_x = atoi(split(temp).c_str());
			//check invalid placement of creature(out of bound)
			if(c_x>=(int)world.grid.width || c_x<0 || c_y>=(int)world.grid.height || c_y<0){
				boundary_exception err;
				err.creature = c_name + " " + directName[c.direction] + " " + itos(c_y) + " " + itos(c_x);
				err.size =  itos(world.grid.height) +"-by-"+ itos(world.grid.width);
				throw err;
			}
			point_t location = {c_y, c_x};
			c.location = location;

			string ability_one = split(temp);
			string ability_two = split(temp);
			//check invalid ability
			if (ability_one != "" && ability_one != "f" && ability_one != "a"){
				ability_exception err;
				err.creature = c_name+ " " + directName[c.direction] + " " + itos(c_y) + " " + itos(c_x);
				err.ability = ability_one;
				throw err;
			}
			if (ability_two != "" && ability_two != "f" && ability_two != "a"){
				ability_exception err;
				err.creature = c_name+ " " + directName[c.direction] + " " + itos(c_y) + " " + itos(c_x);
				err.ability = ability_two;
				throw err;
			}
			c.ability[FLY] = (ability_one == "f" || ability_two == "f") ? true : false;
			c.ability[ARCH] = (ability_one == "a" || ability_two == "a") ? true : false;
			c.hillActive = false;
			world.creatures[world.numCreatures] = c;
			//check the overlap
			if(world.grid.squares[location.r][location.c]!=NULL){
				overlap_exception err;
				err.creature_1 = c_name+ " " + directName[c.direction] + " " + itos(c_y) + " " + itos(c_x);
				creature_t* temp_c = world.grid.squares[location.r][location.c];
				err.creature_2 = temp_c->species->name + " " + directName[temp_c->direction] + " " + itos(c_y) + " " + itos(c_x); 
				throw err;
			}
			//check if a creature will be drown(invalid placement on lake)
			if(!c.ability[FLY] && world.grid.terrain[c_y][c_x] == LAKE){
				drown_exception err;
				err.creature = c_name+ " " + directName[c.direction] + " " + itos(c_y) + " " + itos(c_x);
				throw err;
			}

			world.grid.squares[location.r][location.c] = &world.creatures[world.numCreatures];

			world.numCreatures += 1;
			//check if there is too many creature
			if(world.numCreatures > MAXCREATURES){
				creature_num_exception err;
				throw err;
			}
		}
	}
	f_stream.close();	
}

void tell_world(const world_t world){
//EFFECTS: print the current situation of world
	int row = world.grid.height;
	int col = world.grid.width;
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			string info = (j==col) ?  "____"  : "____ ";
			if (world.grid.squares[i][j] != NULL){
				creature_t* c = world.grid.squares[i][j];
				string short_name = c->species->name;
				info[0] = short_name[0];
				info[1] = short_name[1];
				char direction = directName[c->direction][0];
				info[3] = direction;
			}
			cout << info;
		}
		cout << endl; 
	}
}

void process_location(direction_t d, int step, int &loc_x, int &loc_y){
//EFFECTS: process the location according to the direction and step
//MODIFIES: the location loc_x and loc_y
	switch(d){
		case EAST:
			loc_x = loc_x + step;
			break;
		case WEST:
			loc_x = loc_x - step;
			break;	
		case SOUTH:			
			loc_y = loc_y + step;
			break;
		case NORTH:
			loc_y = loc_y - step;
			break;
		default:
			break;
	}
}

void to_front_location(const world_t &world, int &start_y, int &start_x){
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: process the current location, change it to the front location
//		according to the direction that the creature is facing to
//MODIFIES: the location loc_y, loc_x
	creature_t* target = world.grid.squares[start_y][start_x];
	process_location(target->direction, 1, start_x, start_y);
}



bool front_out_of_bound(world_t &world, int loc_y, int loc_x){
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: test whether the front location is out of bound of the world
	to_front_location(world, loc_y, loc_x);
	//cout<<"INFO:" << loc_y << " " <<loc_x<<endl;
	if(loc_x>=(int)world.grid.width || loc_x<0 || loc_y>=(int)world.grid.height || loc_y<0)
		return true;
	else
		return false;	
}

bool front_is_empty(world_t &world, int loc_y, int loc_x){
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: test whether the front location is not occupied by a creature
	if(!front_out_of_bound(world, loc_y, loc_x)){
		to_front_location(world, loc_y, loc_x);
		if(world.grid.squares[loc_y][loc_x])
			return false;
	}
	return true;
}

bool front_is_enemy(world_t &world, int loc_y, int loc_x){
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: test whether the front location is occupied by an enemy
	if(!front_is_empty(world, loc_y, loc_x)){
		string species_name = world.grid.squares[loc_y][loc_x]->species->name;
		to_front_location(world, loc_y, loc_x);
		string front_species_name = world.grid.squares[loc_y][loc_x]->species->name;
		if(!(species_name==front_species_name))
			return true;
	}
	return false;
}

bool front_is_friend(world_t &world, int loc_y, int loc_x){
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: test whether the front location is occupied by a friend(the same kind of creature)
	if(!front_is_empty(world, loc_y, loc_x) && !front_is_enemy(world, loc_y, loc_x))
		return true;
	return false;
}

bool front_not_plain(world_t &world, int loc_y, int loc_x, string terrain){
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: test whether the front terrain is the specified type of terrain
	if(!front_out_of_bound(world, loc_y, loc_x)){
		to_front_location(world, loc_y, loc_x);
		terrain_t front_t = terrain_t(give_index(terrain, terrainName, TERRAIN_SIZE));
		if (world.grid.terrain[loc_y][loc_x] == front_t)
			return true;
	}
	return false;
}

bool front_is_wall(world_t &world, int loc_y, int loc_x){
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: test whether the front position is a wall(out of bound or being lake to creature not able to fly)
	bool facing_wall = false;
	bool front_is_bound = front_out_of_bound(world, loc_y, loc_x);
	bool can_fly = world.grid.squares[loc_y][loc_x]->ability[FLY];
	if(!front_out_of_bound(world, loc_y, loc_x)){
		to_front_location(world, loc_y, loc_x);
		if(world.grid.terrain[loc_y][loc_x] == LAKE)
			facing_wall = !can_fly;
	}
	return (front_is_bound || facing_wall);
}

bool action_hop(world_t &world, int loc_y, int loc_x){
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: make the creature hop, it goes to the front by 1 step
//		if the front is not out of bound/ not facing a wall/ the front is not occupied
//MODIFIES: change the infomation of that creature in the world consistently
	int start_x = loc_x;
	int start_y = loc_y;
	// cout<< "x:" << loc_x << " " << "y:" << loc_y <<endl;
	creature_t* target = world.grid.squares[start_y][start_x];
	to_front_location(world, loc_y, loc_x);
	int new_x = loc_x;
	int new_y = loc_y;
	//case 1:out of bound
	if (front_out_of_bound(world, start_y, start_x)){
		//cout << "going to a place out of bound! " <<endl;
		return false;
	}
	//case 2:can't fly but going to lake
	bool can_fly = target->ability[FLY];
	if(!can_fly && front_not_plain(world, start_y, start_x, "lake")){
		//cout << "can't fly but going to lake!" <<endl;
		return false;
	}
	//case 3:place is occupied
	if(!front_is_empty(world, start_y, start_x)){
		//cout << "going to an occupied place!" <<endl;
		return false;
	}
	//can hop
	world.grid.squares[new_y][new_x] = target;
	world.grid.squares[new_y][new_x]->location.r = new_y;
	world.grid.squares[new_y][new_x]->location.c = new_x;
	world.grid.squares[start_y][start_x] = NULL;
	return true;
}

bool action_turn(world_t &world, int start_y, int start_x, string turn_to){
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: make the creature change its direction
//MODIFIES: change the infomation of that creature in the world consistently
	direction_t old_d = world.grid.squares[start_y][start_x]->direction;
	direction_t new_d;
	if(turn_to == "right"){ 
		new_d = (old_d==DIRECT_SIZE-1) ? direction_t(0) : direction_t(old_d+1);
		world.grid.squares[start_y][start_x]->direction = new_d;
		return true;
	}
	else if(turn_to == "left"){
		new_d = (old_d==0) ? direction_t(DIRECT_SIZE-1) : direction_t(old_d-1);
		world.grid.squares[start_y][start_x]->direction = new_d;
		return true;
	}
	return false;
}

bool action_infect(world_t &world, int loc_y, int loc_x){
//REQUIRE: the world has a creature on the grid(start_y, start_x)
//EFFECTS: infect the creature in the front if possible
//MODIFIES: change the infomation of the creature in the world consistently
	int start_x = loc_x;
	int start_y = loc_y;
	creature_t* target = world.grid.squares[start_y][start_x];
	bool target_archery = target->ability[ARCH];
	if(!target_archery){
		to_front_location(world, loc_y, loc_x);
		int new_x = loc_x;
		int new_y = loc_y;
		bool condition1 = front_is_enemy(world, start_y, start_x);
		bool condition2 = !front_not_plain(world, start_y, start_x, "forest");
		if(condition1 && condition2){
			world.grid.squares[new_y][new_x]->programID = 0;
			//change the pointer to species
			string species_name = world.grid.squares[start_y][start_x]->species->name;
			string world_species_name[world.numSpecies];
			for(unsigned int j=0; j<world.numSpecies; j++)
				world_species_name[j] = world.species[j].name;
			int index = give_index(species_name, world_species_name, world.numSpecies);
			world.grid.squares[new_y][new_x]->species = &world.species[index];		
			return true;
		}
	}//end if(!target_archery)
	else{
		while(loc_x<(int)world.grid.width && loc_x>=0 && loc_y<(int)world.grid.height && loc_y>=0){
			string species_name = world.grid.squares[start_y][start_x]->species->name;
			bool front_enemy = false;
			if(world.grid.squares[loc_y][loc_x]!=NULL){
				string front_species_name = world.grid.squares[loc_y][loc_x]->species->name;
				front_enemy = !(species_name==front_species_name);
			}	
			if(front_enemy){
				int new_x = loc_x;
				int new_y = loc_y;
				world.grid.squares[new_y][new_x]->programID = 0;
				string species_name = world.grid.squares[start_y][start_x]->species->name;
				string world_species_name[world.numSpecies];
				for(unsigned int j=0; j<world.numSpecies; j++)
					world_species_name[j] = world.species[j].name;
				int index = give_index(species_name, world_species_name, world.numSpecies);
				world.grid.squares[new_y][new_x]->species = &world.species[index];		
				return true;
			}
			else
				process_location(target->direction, 1, loc_x, loc_y);
		}//endof while
	}//endof if(target_archery)
	return false;
}

void creature_action(world_t &world, creature_t &target){
//EFFECTS: make the target do action according to its instruction list
//MODIFIES: change the infomation of the creature in the world consistently
	unsigned int cur_id = target.programID;
	opcode_t cur_instruction = target.species->program[cur_id].op;
	unsigned int cur_address = target.species->program[cur_id].address;
	int loc_x = target.location.c;
	int loc_y = target.location.r;

	bool final_id = (target.programID == (target.species->programSize)-1);
	bool front_forest = front_not_plain(world, loc_y, loc_x, "forest");
	switch(cur_instruction){
		case HOP: 
			action_hop(world, loc_y, loc_x);
			target.programID = (final_id)? 0 : (target.programID+1);
			break; 
		case LEFT:
			action_turn(world, loc_y, loc_x, "left");
			target.programID = (final_id)? 0 : (target.programID+1);
			break;
		case RIGHT:
			action_turn(world, loc_y, loc_x, "right");
			target.programID = (final_id)? 0 : (target.programID+1);
			break;
		case INFECT:
			action_infect(world, loc_y, loc_x);
			target.programID = (final_id)? 0 : (target.programID+1);
			break;
		case IFEMPTY:
			if(front_is_empty(world, loc_y, loc_x))
				target.programID = cur_address-1;
			else
				target.programID = (final_id)? 0 : (target.programID+1);
			break;
		case IFENEMY:
			if(front_forest)
				target.programID = (final_id)? 0 : (target.programID+1);
			else{				
				if(front_is_enemy(world, loc_y, loc_x))
					target.programID = cur_address-1;
				else
					target.programID = (final_id)? 0 : (target.programID+1);
			}
			break;
		case IFSAME:
			if(front_forest)
				target.programID = (final_id)? 0 : (target.programID+1);
			else{
				if(front_is_friend(world, loc_y, loc_x))
					target.programID = cur_address-1;
				else
					target.programID = (final_id)? 0 : (target.programID+1);
			}
			break;
		case IFWALL:
			if(front_is_wall(world, loc_y, loc_x))
				{target.programID = cur_address-1; }
			else
				target.programID = (final_id)? 0 : (target.programID+1);
			break;
		case GO:
			target.programID = cur_address-1;
			break;
		default:
			cout << "ERROR: current instruction is skipped..." << endl;
			break;
	}
}

bool end_turn_action(creature_t &target){
	unsigned int cur_id = target.programID;
	opcode_t cur_instruction = target.species->program[cur_id].op;
	switch(cur_instruction){
		case HOP:
		case LEFT:
		case RIGHT:
		case INFECT:
			return true;
			break;
		default:
			return false;
			break;
	}
	return false;
}

void creature_round_action(world_t &world, creature_t &target, bool verbose){
//EFFECTS: simulate the behavior of a creature in one round
//MODIFIES: change the infomation of the creature in the world consistently
	terrain_t front_t = HILL;
	int loc_x = target.location.c;
	int loc_y = target.location.r;
	int can_fly = target.ability[FLY];
	if(!can_fly){
		if ((world.grid.terrain[loc_y][loc_x]==front_t) && (target.hillActive==false)){
			target.hillActive = true;
			return;
		}
		if ((world.grid.terrain[loc_y][loc_x]==front_t) && (target.hillActive==true)){
			target.hillActive = false;
		}
	}
	
	int limit = 0;//avoid dead loop
	if(verbose){
		//print infomation with verbose mode
		cout << "Creature (" << target.species->name <<" "\
			<< directName[target.direction] <<" "<< target.location.r\
			<<" "<< target.location.c<< ") takes action: "<<endl;
		while(1){
			if(!end_turn_action(target)){
				instruction_t ins = target.species->program[target.programID];
				cout << "Instruction "<< target.programID+1 << ": " << opName[ins.op] << " " << ins.address << endl;
				creature_action(world, target);
			}
			else{
				instruction_t ins = target.species->program[target.programID];
				cout << "Instruction "<< target.programID+1 << ": " << opName[ins.op] <<endl;
				creature_action(world, target);
				tell_world(world);
				break;
			}
			limit++;
			if(limit == 50)
				{cout<<"ERR, dead loop with creature_round_action"<<endl; break;} //some dead loop is happening
		}
	}
	else{
		//print infomation with concise mode
		while(1){
			if(end_turn_action(target)){
				instruction_t ins = target.species->program[target.programID];
				cout << "Creature (" << target.species->name <<" "\
					<< directName[target.direction] <<" "<< target.location.r\
					<<" "<< target.location.c<< ") takes action: " << opName[ins.op]<<endl;
				creature_action(world, target);
				break;
			}
			else
				creature_action(world, target);
		}
	}
}

void go_round(world_t &world, int &round_num, bool verbose){
//EFFECTS: simulate the behavior of all creatures in one round
//MODIFIES: change the infomation of the creature in the world consistently
	cout << "Round " << round_num <<endl;
	for(unsigned int i=0; i<world.numCreatures; i++){
		creature_round_action(world, world.creatures[i], verbose);
	}
	if(!verbose)
		tell_world(world);
	round_num += 1;
}

void world_simulation(int argc, char* argv[]){
	//check invalid input(num of argument is not enough)
	if(argc < 4){
		argc_exception err;
		throw err;
	}
	string species_file = argv[1];
	string world_file = argv[2];
	int round_limit = atoi(argv[3]);
	//check invalid input(negative round)
	if(round_limit < 0){
		round_limit_exception err;
		throw err;
	}

	string verbose_str = "";
	if(argv[4])
		verbose_str = argv[4];
	bool verbose = (verbose_str == "v") ? true : false;

	world_t world;
	read_path(species_file, world);
	read_world(world_file, world);
	cout << "Initial state" << endl;
	tell_world(world);

	for (int round = 1; round <= round_limit;)
		go_round(world, round, verbose);
}

void project3(int argc, char* argv[]){
//EFFECT: add exception handle to the world_simulation function
	try{
		world_simulation(argc, argv);
	}
	catch (argc_exception err){
		cout << "Error: Missing arguments!" << endl;
		cout << "Usage: ./p3 <species-summary> <world-file> <rounds> [v|verbose]" <<endl;
	}
	catch (round_limit_exception err){
		cout << "Error: Number of simulation rounds is negative!" << endl;
	}
	catch (file_exception err){
		cout << "Error: Cannot open file " << err.file_path << "!" << endl;
	}
	catch (species_num_exception err){
		cout << "Error: Too many species!" <<endl;
		cout << "Maximal number of species is "	<< MAXSPECIES << "." << endl;
	}
	catch (instruction_num_exception err){
		cout << "Error: Too many instructions for species "<< err.species << "!" <<endl;
		cout << "Maximal number of instructions is "<< MAXPROGRAM << "." << endl;		
	}
	catch (instruction_exception err){
		cout << "Error: Instruction "<<err.invalid_instruction<<" is not recognized!"<<endl;
	}
	catch (grid_height_exception err){
		cout << "Error: The grid height is illegal!" << endl;
	}
	catch (grid_width_exception err){
		cout << "Error: The grid width is illegal!" << endl;
	}
	catch (terrain_exception err){
		cout << "Error: Terrain square ("<<err.terrain<<") is invalid!"<< endl;
	}
	catch (creature_num_exception err){
		cout << "Error: Too many creatures!" <<endl;
		cout << "Maximal number of creatures is "<< MAXCREATURES << "." << endl;
	}
	catch (unknown_species_exception err){
		cout << "Error: Species "<< err.unknown_species <<" not found!" <<endl;
	}
	catch (direction_exception err){
		cout << "Error: Direction "<< err.unknown_direction <<" is not recognized!" <<endl;
	}
	catch (boundary_exception err){
		cout << "Error: Creature ("<< err.creature <<") is out of bound!"<<endl;
		cout << "The grid size is " << err.size<<"."<<endl;
	}
	catch (ability_exception err){
		cout << "Error: Creature ("<< err.creature <<") has an invalid ability " << err.ability << "!"<<endl;
	}
	catch (overlap_exception err){
		cout << "Error: Creature ("<< err.creature_1 <<") overlaps with creature ("<< err.creature_2 << ")!";
	}
	catch (drown_exception err){
		cout << "Error: Creature ("<< err.creature <<") is in a lake square!"<<endl;
		cout << "The creature cannot fly!"<<endl;
	}
}
