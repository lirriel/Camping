#include <fstream>
#include <vector>
#include "string"

using namespace std;

/**
 * class to solve the task
 */
class CampPlan{
public:
    /**
     * main method for solution
     */
    void solve(){
        sum = 0;
        map = read();
        filllCoasts();
        startCamp();
        write();
    }

private:
    /**
     * enumeration used as a flag on which coast children are
     */
    enum coast {LEFT, RIGHT};

    /**
     * Reading map of rivers from the file
     * @return
     */
    vector<char> read(){
        string path = "input_bridge.txt";
        ifstream ifs(path.c_str(), ios::binary|ios::ate);
        ifstream::pos_type pos = ifs.tellg(); // number of chars inside the vector<char>

        std::vector<char>  result(pos); // vector to put information in

        ifs.seekg(0, ios::beg);
        ifs.read(&result[0], pos); // read all bytes of the file into a vector

        for (int i = 0; i < result.size() - 1; ++i) {
            if (result[i] != 'L' && result[i] != 'B' && result[i] != 'R') //checking file for extra
            {
                result.erase(result.begin() + i);
            }
        }
        return result;
    }

    /**
     * Writing sum of the steps (times of river crossings) in a file
     */
    void write(){
        ofstream output;
        string path = "output_bridge.txt";
        output.open(path.c_str());
        output << sum;
        output.close();
    }

    /**
     * Fill up vectors of coasts
     * if at the same coasts there are the same types of paths in a row, sum them up
     */
    void fillCoasts(){
        int i = 0; // from the beginning
        while (i < map.size()){
            int l = 0; // steps for the left coast
            int r = 0; // steps for the right coast
            if (map[i] == 'L'){
                while (map[i] == 'L'){
                    l++;
                    i++;
                }
            } else if(map[i] == 'R'){
                while (map[i] == 'R'){
                    r++;
                    i++;
                }
            }
            else if (map[i] == 'B'){
                while (map[i] == 'B'){
                    l++;
                    r++;
                    i++;
                }
            }
            // adding steps to vectors
            left.push_back(l);
            right.push_back(r);
        }
    }

    /**
     * If on i step going either way costs equal then
     * there should be held a check up for the next step
     * @param i - index in left / right vector
     * @param current coast children are on
     */
    void checkPath(int i, enum coast &current){
        if (i == left.size() - 1)
        {
            current = RIGHT; // if it is the last step, we should go on th right
            return;
        }
        if (left[i + 1] == right [i + 1]) // if equals again => check up
            checkPath(i + 1, current);
        else
        if (left[i +1] < right [i + 1])
        {
            current = LEFT;
        } else{
            current = RIGHT;
        }
    }

    /**
     * counting sum starting from the left coast and finishing on the right
     */
    void startCamp(){
        coast current = LEFT;
        sum = 0;
        for (int i = 0; i < left.size(); ++i) {
            if (current == LEFT){ // currently on the left
                if (left[i] == 1 + right[i]){ // equals
                    checkPath(i, current);
                    sum += left[i];
                }
                else if (left[i] < 1 + right[i]){ // cheaper stay on the left
                    sum += left[i];
                    current = LEFT;
                }
                else{
                    sum += 1 + right[i]; // cheaper go on the right
                    current = RIGHT;
                }
            }
            else{ // currently on the right
                if (right[i] == left[i] + 1){ // equals
                    checkPath(i, current);
                    sum += right[i];
                }
                else if (right[i]  < 1 + left[i]){ // cheaper stay on the right
                    sum += right[i];
                    current = RIGHT;
                }
                else{
                    sum += 1 + left[i]; // cheaper go on the left
                    current = LEFT;
                }
            }
        }
        if (current == LEFT){ // in case children finished on the left -> cross the main river
            sum++;
        }
    }

    /**
     * count of river crossings
     */
    int sum;
    /**
     * map for the left coast
     */
    vector<int> left;
    /**
     * map for the right coast
     */
    vector<int> right;
    /**
     * map of all rivers from the file
     */
    vector<char> map;
};