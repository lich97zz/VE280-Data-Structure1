#include <iostream>
#include "dlist.h"
#include <cstdlib>
using namespace std;

// string split(string &str){
//     //EFFECTS:cut the first word of str(words are split by a space), 
//     //          original str will be modified, return the first word
//     //MODIFIES: str becomes the original string without its first word
//     int len = str.length();
//     string temp = "";
//     for(int i=0; i<len; i++){
//         if (str[i] != ' ')
//             temp += str[i]; 
//         else{
//             str = (i==len-1) ? "" : str.substr(i+1,len); 
//             break;
//         }
//         if(i==len-1)
//             str = "";
//     }
//     return temp;
// } 

class Call{ //a call, with its info
    public:
        Call(){};
        void set_call(int _in_time, string _name, string _level, int _duration){
            name = _name;
            in_time = _in_time;
            duration = _duration;
            level = _level;                         
        }
        int give_time() {return in_time;}
        int give_duration() {return duration;}
        string give_level() {return level;}
        string give_name() {return name;}
    private:
        string name;
        int in_time;
        int duration;
        string level;
};

void print(Dlist<Call> stack){ //没什么卵用
    int count = 1;
    while(!stack.isEmpty()){
        Call* c = stack.removeFront();
        cout<< count<<": "<<c->give_time()<<" "<<\
            c->give_name()<<" "<<c->give_level()<<\
            " "<<c->give_duration()<<endl;
        count++;
    }
}

void set_total(Dlist<Call> &total){
//EFFECTS: at the beginning of the simulation, set the queue with all calls
//MODIFIES: total
    string content;

    // cout<<"give info here:"<<endl;
    // getline(cin,content); //???
    // cout << "------------------------------"<<endl;
    int num; /*= atoi(split(content).c_str());*/ //num of calls
    cin>>num;
    for(int i=0; i<num; i++){
        int in_time;/* = atoi(split(content).c_str());*/
        cin>>in_time;
        string name;/* = split(content);*/
        cin>>name;
        string level;/* = split(content);*/
        cin>>level;
        int duration;/* = atoi(split(content).c_str());*/
        cin>>duration;

        Call* call = new Call;
        call->set_call(in_time, name, level, duration);
        total.insertBack(call);
    }
}

void set_stacks(int time, Dlist<Call> &total, Dlist<Call> &platinum_calls,\
    Dlist<Call> &gold_calls, Dlist<Call> &silver_calls, \
    Dlist<Call> &regular_calls){
//EFFECTS: at each tick, if there is some incoming call at this tick, remove
//         it from total, add it to corresponding stack
//MODIFIES: the five queues storing calls of different type
    cout<<"Starting tick #"<<time<<endl;
    Dlist<Call> temp_stack = total;
    int num = 0; //num of calls in current time
    while(!temp_stack.isEmpty()) //check how many items need to be removed
        if(temp_stack.removeFront()->give_time() == time)
            num += 1;
    for(int i=0; i<num; i++){
        Call* temp = total.removeFront();
        Call* target = new Call; // set the info of the call
        target->set_call(temp->give_time(), temp->give_name(),\
             temp->give_level(), temp->give_duration());
        string type = temp->give_level();
        if(type == "platinum")//add it to corresponding queue
            platinum_calls.insertBack(target);
        else if(type == "gold")
            gold_calls.insertBack(target);
        else if(type == "silver")
            silver_calls.insertBack(target);
        else if(type == "regular")
            regular_calls.insertBack(target);
        cout<<"Call from "<< temp->give_name()<< //prompt
            " a "<<temp->give_level()<<" member"<<endl;
    }
}

class end_of_simu{}; //Exception type, means all calls have been answered

void answer_calls(Dlist<Call> total, Dlist<Call> &platinum, Dlist<Call> &gold, \
    Dlist<Call> &silver, Dlist<Call> &regular, int time, int &busy_time){
    //EFFECTS: at each tick, check if the agent is busy, if not busy, answer
    //         a call according to priority, and delete it from its queue
    //MODIFIES: platinum, gold, silver, regular, busy_time
    if(busy_time > time) //already answering a call
        return;
    if(!platinum.isEmpty()){ //if there is a platinum_call
        Call* temp = platinum.removeFront();
        busy_time = time + temp->give_duration();
        cout<<"Answering call from "<<temp->give_name()<<endl;
    }
    else if(!gold.isEmpty()){       
        Call* temp = gold.removeFront();
        busy_time = time + temp->give_duration();
        cout<<"Answering call from "<<temp->give_name()<<endl;
    }
    else if(!silver.isEmpty()){
        Call* temp = silver.removeFront();
        busy_time = time + temp->give_duration();
        cout<<"Answering call from "<<temp->give_name()<<endl;
    }
    else if(!regular.isEmpty()){
        Call* temp = regular.removeFront();
        busy_time = time + temp->give_duration();
        cout<<"Answering call from "<<temp->give_name()<<endl;
    }
    else if(total.isEmpty()){ // there is no call to answer, end
        end_of_simu end;
        throw end;
    }
}

void call_simu(){
    Dlist<Call> total;
    Dlist<Call> platinum_calls;
    Dlist<Call> gold_calls;
    Dlist<Call> silver_calls;
    Dlist<Call> regular_calls;

    set_total(total); //set the queue for total, add all the calls
    int time = 0;
    int busy_time = 0; //busy until time <busy_time>
    while(1){
        set_stacks(time, total, platinum_calls, gold_calls, silver_calls, regular_calls);
        try{
            answer_calls(total, platinum_calls, gold_calls, \
                silver_calls, regular_calls, time, busy_time);
            time += 1;
        }
        catch(end_of_simu e){break;}
    }


}
int main(int argc, char *argv[]){
    call_simu();
}
