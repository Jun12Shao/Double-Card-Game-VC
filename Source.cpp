#include<iostream>
#include<vector>
#include<list>
#include<unordered_map>
#include<map>
#include<string>
#include<iomanip>
#include<algorithm>
#include<ctime>
#include<fstream>
#include<exception>
#include <sstream>
using namespace std;

class exception1 :public exception {
    virtual const char* what() const throw() {
        return "Illegible move. P2 Overbound!";
    }
} ex1;
class exception2 :public exception {
    virtual const char* what() const throw() {
        return "Illegible move. Overlap existing positions!";
    }
} ex2;
class exception3 :public exception {
    virtual const char* what() const throw() {
        return "Illegible move.Hanging over the empty cell!";
    }
} ex3;
class exception4 :public exception {
    virtual const char* what() const throw() {
        return "Wrong remove segments information.";
    }
} ex4;
class exception5 :public exception {
    virtual const char* what() const throw() {
        return "Wrong remove. The card was placed just by the other player.";
    }
} ex5;

class exception6 :public exception {
    virtual const char* what() const throw() {
        return "Wrong positions of removing card.Two segments are not from the same card.";
    }
} ex6;
class exception7 :public exception {
    virtual const char* what() const throw() {
        return "There is no card in the desired position.";
    }
} ex7;
class exception8 :public exception {
    virtual const char* what() const throw() {
        return "Illegible remove. There are cards hanging above!";
    }
} ex8;
class exception9 :public exception {
    virtual const char* what() const throw() {
        return "Illegible remove. There are cards hanging above!";
    }
} ex9;
class exception10 :public exception {
    virtual const char* what() const throw() {
        return "Remove and move are in the same position!";
    }
} ex10;

class exception11 :public exception {
    virtual const char* what() const throw() {
        return "Remove and move are in the same position! The lenght of letters/numbers should be 4 and separared by space. The first number should be 0 the second number(action) should be between 1 and 8.  Such as: 0 5 A 2 or 0 8 A 11";
    }
} ex11;

class exception12 :public exception {
    virtual const char* what() const throw() {
        return "Remove and move are in the same position! The lenght of letters/numbers should be 7 and separared by space. The fifth number(action) should be between 1 and 8.  Such as: F 2 F 3 3 A 2";
    }
} ex12;


void Calculate_scores(int cell, vector<int>& score);
//void vertical_detect(vector<vector<vector<int>>>& s, int r, int c, int step, vector<vector<int>>& score_board);

vector<string> cells= {"","r/d","r/o","w/d","w/o"};
vector<vector<int>> A= { {},{1,4},{4,1},{4,1},{1,4}, {2,3},{3,2},{3,2}, {2,3} };
map<string, int> dic_col = { {"A",0},{"B",1},{"C",2},{"D",3},{"E",4} ,{"F",5} ,{"G",6} ,{"H",7} };
vector<string> number2col = { "A","B","C","D","E","F","G","H" };
map<string, int> dic_row = { {"1",11},{"2",10},{"3",9},{"4",8},{"5",7},{"6",6},{"7",5},{"8",4},{"9",3},{"10",2},{"11",1} };
map<string, int> dic_a = { {"1",1},{"2",2},{"3",3},{"4",4},{"5",5},{"6",6},{"7",7},{"8",8}};
vector<vector<int>> D = { {-1, 0},{0,1} };

void Visulization(vector<vector<vector<int>>>& s) {
    int i, j;
    vector<vector<string>> table(13,vector<string>(9));
    for (i = 0; i < 12; i++) {
        table[i][0] = std::to_string(12 - i);
        for (j = 0; j < 8; j++) {
            table[i][j + 1] = cells[s[i][j][0]];
        }
    }
    vector<string> temp({"", "A", "B", "C", "D", "E", "F", "G", "H" });
    for (i = 0; i < 9; i++) {
        table[12][i] = temp[i];
    }


    // assume that your data rows are tuples
    string templs = "|{0:2}|{1:4}|{2:4}|{3:4}|{4:4}|{5:4}|{6:4}|{7:4}|{8:4}|"; // column widths : 2, 3, 3, 3, 3, 3, 3, 3, 3

    cout << "———————————————————————" << endl;
    //cout << table[0] << table[1] << table[2], table[3], table[4], table[5], table[6], table[7], table[8];
    for (vector<string> line : table) {
        cout << setw(2) << line[0] << setw(4) << line[1] << setw(4) << line[2] << setw(4) << line[3] << setw(4)
            << line[4] << setw(4) << line[5] << setw(4) << line[6] << setw(4) << line[7] << setw(4) << line[8] << setw(4) << endl;
    }

    cout << "———————————————————————";

}




std::tuple<int,list<vector<int>> > Terminal_checker(vector<vector<vector<int>>> &s, const vector<vector<int>> &last_mv){
    //# Inputs : Current stage s and last move .
    //# Check if the stage is the terminal
    //# Return the scoring boards
    int terminal = 0;
    list <vector<int> > scores;
    vector<int> score;
    vector<vector<int>> m(2);
    int len_mv = last_mv.size();

    //# check if there are four connections due to adding of p1and p2.
    if (len_mv == 5){
        //for (auto it = last_mv.begin() + 3; it != last_mv.end(); it++) m.push_back(move(*it));
        std::copy(std::begin(last_mv)+3, std::end(last_mv), std::begin(m));
    }
    else if (len_mv == 3) {
        //# s_new = s
        //m = last_mv;
        std::copy(std::begin(last_mv)+1, std::end(last_mv), std::begin(m));
    }
    else if (len_mv == 0) {
        return std::make_tuple(terminal, scores);
    }


    for (auto p : m ){
        //# Check the horizontal directionand see if there is a winner
        score = { 0, 0, 0, 0 };  //# store the number of consecutive colors and dots[red, whites, dots, circles]
        int r = p[0];
        int cell;
        for (auto c = p[1] - 3; c < p[1] + 4; c++) {
            //for c in range(p[1] - 3, p[1] + 4, 1) :
            if (c >= 0 && c < 8) {
                cell = s[r][c][0];
                Calculate_scores(cell, score);
                if (*std::max_element(score.begin(),score.end()) >= 4) {
                    scores.push_back(score);
                    terminal = 1;
                    break;
                }
            }
        }
        //# Check the vertical directionand see if there is a winner
        score= {0, 0, 0, 0 };//  # store the number of consecutive colorsand dots[red, whites, dots, circles]
        int c = p[1];
        for (int r = p[0]; r < p[0] + 4; r++) {
            //for r in range(p[0], p[0] + 4, 1) :
            if (r < 12) {
                cell = s[r][c][0];
                Calculate_scores(cell, score);
                if (*std::max_element(score.begin(), score.end()) >= 4) {
                    scores.push_back(score);
                    terminal = 1;
                    break;
                }
            }
        }

        //# Check the left  diagonal(\) and see if there is a winner
        score = {0, 0, 0, 0 }; //  # store the number of consecutive colorsand dots[red, whites, dots, circles]
        r = p[0];
        c = p[1];
        int r0, c0;
        for (int i = -3; i < 4; i++) {
            r0 = r + i;
            c0 = c + i;
            if (r0 >= 0 && r0 < 12 && c0 >= 0 && c0 < 8) {
                cell = s[r0][c0][0];
                Calculate_scores(cell, score);
                if (*std::max_element(score.begin(), score.end()) >= 4) {
                    scores.push_back(score);
                    terminal = 1;
                    break;
                }
            }
        }

        //# Check the right diagonal(/ ) and see if there is a winner
        score= { 0, 0, 0, 0};    //  # store the number of consecutive colorsand dots[red, whites, dots, circles]
        r = p[0];
        c = p[1];
        for (int i = -3; i < 4; i++) {
            r0 = r - i;
            c0 = c + i;
            if (r0 >= 0 && r0 < 12 && c0 >= 0 && c0 < 8) {
                cell = s[r0][c0][0];
                Calculate_scores(cell, score);
                if (*std::max_element(score.begin(), score.end()) >= 4) {
                    scores.push_back(score);
                    terminal = 1;
                    break;
                }
            }
        }
    }

    return std::make_tuple(terminal, scores);
}

void Calculate_scores(int cell, vector<int>& score) {
    //# used to check four - consecutive - segment.
    if (cell == 0) {   //# No empty space is allowed.
        //score = [0, 0, 0, 0];
        for (auto it = score.begin(); it != score.end(); it++) {
            *it = 0;
        }
    }
    else if (cell == 1) {
        score[0] += 1;
        score[1] = 0;
        score[2] += 1;
        score[3] = 0;
    }
    else if (cell == 2) {
        score[0] += 1;
        score[1] = 0;
        score[2] = 0;
        score[3] += 1;
    }
    else if (cell == 3) {
        score[0] = 0;
        score[1] += 1;
        score[2] += 1;
        score[3] = 0;
    }
    else if (cell == 4) {
        score[0] = 0;
        score[1] += 1;
        score[2] = 0;
        score[3] += 1;
    }
}

void Move(vector<vector<vector<int>>>& s, const vector<vector<int>>& m, int step) {
    //# Inputs : Move indication m, current stage s.Structure of m : [action, p1, p2]
    //# Move a card to desired location
    //# Returns: new stage after an action.
    int a = m[0][0];  //# get desired action of move, 1~8
    //s[m[1]] = [A[a][0], step];
    int r = m[1][0];
    int c = m[1][1];
    s[r][c][0] = A[a][0];
    s[r][c][1] = step;

    r = m[2][0];
    c = m[2][1];
    s[r][c][0] = A[a][1];
    s[r][c][1] = step;
}

void Remove (vector<vector<vector<int>>>& s, const vector<vector<int>>& rm) {
    //# Inputs : Remove indication rm, current stage s.Structure of rm : [position 1, position 2]
    //# Remove a card from a desired location
    //# Returns: new stage after an action.
    int r = rm[0][0];
    int c = rm[0][1];
    s[r][c][0] = 0;
    s[r][c][1] = 0;

    r = rm[1][0];
    c = rm[1][1];
    s[r][c][0] = 0;
    s[r][c][1] = 0;
    //s[rm[0]] = [0, 0];
    //s[rm[1]] = [0, 0];
    //return s;
}



std::tuple<vector<vector<int>>,vector<vector<int>>> Human_input(int r = 0) {
    vector<vector<int>> rm;
    vector<vector<int>> m;
    string item;
    string command;
    vector<string> char_com;
    int st = 0;

    std::cout << "Please input next move:";
    //std::cin.ignore();
    std::getline(std::cin, command, '\n');
    std::stringstream ss(command);
    while (std::getline(ss, item, ' ')) {
        if (item!="") char_com.push_back(item);
    }

    if (!r) {
        if (char_com.size() != 4 || char_com[0].compare("0")!=0 ) {
            throw ex11;
        }
        else {
            int a = dic_a[char_com[1]];
            int c1 = dic_col[char_com[2]];
            int r1 = dic_row[char_com[3]];
            vector<int> p1={r1, c1};  //# get the position in game board for the base point of our card.
            int r2 = r1 + D[a%2][0];
            int c2 = c1 + D[a%2][1];
            vector<int> p2 = { r2, c2 };  //# get the position in game board for next segment of our card.
            m = {vector<int>({a}), p1, p2 };
        }
    }
    else {
        if (char_com.size() != 7) {
            throw ex12;
        }
        int c1 = dic_col[char_com[0]];
        int r1 = dic_row[char_com[1]];
        vector<int> p1 = { r1, c1 };

        int c2 = dic_col[char_com[2]];
        int r2 = dic_row[char_com[3]];
        vector<int> p2({ r2, c2 });

        int a = dic_a[char_com[4]];
        int c3 = dic_col[char_com[5]];
        int r3 = dic_row[char_com[6]];
        vector<int> p3 = { r3, c3 };  //# get the position in game board for the base point of our card.

        int r4 = r3 + D[a % 2][0];
        int c4 = c3 + D[a % 2][1];
        vector<int> p4 = {r4, c4 };  //# get the position in game board for next segment of our card.
        rm = {p1, p2 };
        m= { vector<int> ({a}), p3, p4};
    }

    return std::make_tuple(rm, m);
}

string get_winner(list<vector<int>>& scores, int step, int c) {
    int k = 1;
    string winner;
    if ((c == 1 && step % 2 == 1) || (c == 0 && step % 2 == 0)) {
        for (auto it = scores.begin(); it!= scores.end(); it++) {
            if ((*it)[0] == 4 || (*it)[1] == 4) {
                winner = "color";
                k = 0;
                break;
            }
        }
        if (k) {
            winner = "dot"; }
    }
    else {
        for (auto it = scores.begin(); it != scores.end(); it++) {
            if ((*it)[2] == 4 || (*it)[3] == 4) {
                winner = "dot";
                k = 0;
                break;
            }
            if (k) {winner = "color";}
        }
    }
    return winner;
}



bool Rule_checker(vector<vector<vector<int>>>& s, vector<int>& p1, vector<int>& p2, int step, int r) {
    //# Inputs : current state s, command, step
    //# Check the legibility of command.If illegible raise error.
    int r1 = p1[0];
    int c1 = p1[1];
    int r2 = p2[0];
    int c2 = p2[1];

    if (!r) {    //# Move legibility check
        //# Check legability of the move
        if (p2[0] < 0 || p2[1]>=8) {
            throw ex1;
        }
        if (s[r1][c1][0] != 0 || s[r2][c2][0] != 0) {//# Automatically check the legality of each move.
            throw ex2;
        }
        if ((p1[0] + 1 < 12 && s[r1 + 1][c1][0] == 0) || (p2[1] - p1[1] == 1 && p2[0] + 1 < 12 && s[r2 + 1][c2][0] == 0)) {
            throw ex3;
            
        }
        return true;
    }

    else {  //                                       # Remove legibility check
        //# Check legability of remove
        if ((p1[0] == p2[0] && p1[1] == p2[1]) || (p2[1] - p1[1] < 0) || (p2[0] - p1[0] > 0)) {
            throw ex4;
        }
        if (s[r1][c1][1] == step - 1 || s[r2][c2][1] == step - 1) {
            throw ex5;
        }

        //# segments retrieved should be from one card.
        if (s[r1][c1][1] != s[r2][c2][1]) {
            throw ex6;
        }

        // # There must be a card in the desired position.
        if (s[r1][c1][0] == 0 || s[r2][c2][0] == 0) {
            throw ex7;
        }

        //# after retrieving, the stage shopuld be legable.
        if (p1[0] - p2[0] == 0) {//  # card removed is horizontal.
            if (p1[0] > 0 && (s[r1 - 1][c1][0] != 0 || s[r2 - 1][c2][0] != 0)) { // # there is a card above p1 0r p2
                throw ex8;
            }
        }
        else {//  # card removed is vertical.
            if (p2[0] > 0 && s[r2- 1][c2][0] != 0) {  //  # there is a card above p2
                throw ex9;
            }
        }
        return true;
    }
}



string Rev_translation(vector<vector<int>> m) {
    //# translate the action of machine to human command
    string new_m;

    if (m.size() == 3) {
        vector<int> p1 = m[1];
        vector<int> p2 = m[2];
        new_m = to_string(0) + " " + to_string(m[0][0]) + " " + number2col[p1[1]] +" " + to_string(12 - p1[0]);
    }
    else if (m.size()== 5) {
        vector<int> p1 = m[0];
        vector<int> p2 = m[1];
        vector<int> p3 = m[3];
        vector<int> p4 = m[4];
        new_m = number2col[p1[1]] + " " + to_string(12 - p1[0]) + " " + number2col[p2[1]] + " " + to_string(12 - p2[0]) + " " + 
            to_string(m[2][0]) + " " + number2col[p3[1]] + " " + to_string(12 - p3[0]);
    }
    return new_m;
}


void Calculate_scores2(int cell, vector<int>& score){
   // # Used to check three - consecutive - segment or two - consecutive - segment
    if (cell == 0) {    //# Empty space is allowed.
        score = score;
    }
    else if (cell == 1) {
        score[0] += 1;
        score[1] = 0;
        score[2] += 1;
        score[3] = 0;
    }
    else if (cell == 2) {
        score[0] += 1;
        score[1] = 0;
        score[2] = 0;
        score[3] += 1;
    }
    else if (cell == 3) {
        score[0] = 0;
        score[1] += 1;
        score[2] += 1;
        score[3] = 0;
    }
    else if (cell == 4) {
        score[0] = 0;
        score[1] += 1;
        score[2] = 0;;
        score[3] += 1;
    }
 }


list<vector<vector<int>>> get_children(vector<vector<vector<int>>>& s, int step, vector<vector<int>> rm = {}, int rc = 1) {
    //# Input： state of the game s ϵ S, depth of the game
    //# Return : list of next states and last_move after a legal move
    //# Notation : algorithm for forward moving and recycling moving is different
    list<vector<vector<int>>> s_next;
    vector<int> p1,p2;
    vector<vector<int>> m;
    bool result;
    //list<int> visit_card = {step - 1};
    int visit_card = step - 1;
    int card_number;
    int r, c, r2, c2;

    if (step <= 24 || rc == 0){
        for (c = 0; c < 8; c++) {
            for (r = 0; r < 12; r++) { 
                if (s[r][c][0] == 0) {
                    if (r == 11 || (r <= 10 && s[r + 1][c][0] != 0)){
                        p1 = { r, c };
                        for (int a=1; a<9;a++) {      //# 8 kinds of cards
                            r2 = r + D[a % 2][0];
                            c2 = c + D[a % 2][1];
                            p2 = {r2, c2};           //# get the position in game board for next segment of our card.
                            if (rm.size() != 2 || p1 != rm[0] || p2 != rm[1]){
                                try {
                                    result = Rule_checker(s, p1, p2, step, 0);
                                }
                                catch (exception& e) {
                                    //std::cout << e.what() << endl;
                                    result = false;
                                }
                                if (result == true){
                                    if (step <= 24) {
                                        m = { {a}, p1, p2 };
                                    }
                                    else {
                                        m = { rm[0], rm[1], {a}, p1, p2 };
                                    }
                                    s_next.push_back(m);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (step <= 40 && rc){ // # Remove cards from board
        for (c = 0; c < 8; c++) {
            for (r = 0; r < 12; r++) {
                if (s[r][c][0] != 0) {
                    if (r == 0 || s[r - 1][c][0] == 0) {
                        card_number = s[r][c][1];
                        if (card_number!= visit_card){
                        //if (std::find(visit_card.begin(), visit_card.end(), card_number) == visit_card.end()){
                        //if (card_number not in visit_card){ 
                            if (r < 11 && s[r + 1][c][1] == card_number) {
                                p1 = { r + 1, c };
                                p2 = { r, c };
                            }
                            if (c < 7 && s[r][c + 1][1] == card_number) {
                                p1 = { r, c };
                                p2 = { r, c + 1 };
                            }
                            if (c >0 && s[r][c - 1][1] == card_number) {
                                p1 = { r, c-1 };
                                p2 = { r, c};
                            }

                            try {
                                result = Rule_checker(s, p1, p2, step, 1);
                            }
                            catch (exception& e) {
                                //std::cout << e.what() << endl;
                                result = false;
                            }

                            if (result) {
                                //visit_card.push_back(card_number);
                                rm = { p1, p2 };
                                vector<vector<vector<int>>> s_rm(s);
                                Remove(s_rm, rm);

                                for (auto it : get_children(s_rm, step, rm, 0)) {
                                    s_next.push_back(it);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return s_next;
}

void vertical_detect(vector<vector<vector<int>>>& s, int r, int c, int step, vector<vector<float>>& score_board) {
    //# calculate consecutive colorsand dots
    vector<int> score(4, 0);    //  # store the number of consecutive colorsand dots[red, whites, dots, circles]

    for (int r1 = r + 4; r1 > r; r1--) {
        if (r1 < 12) {
            int cell = s[r1][c][0];
            Calculate_scores2(cell, score);
            if ((step > 24 && r1 == r + 2 && s[r][c][1] != step - 1) || r1 == r + 1) {
                for (int i = 0; i < 4; i++) {
                    if (score[i] == 2 && r >= 1) {
                        score_board[i][0] += 1;
                    }
                    if (score[i] == 3 && r >= 0) {
                        score_board[i][1] += 1;
                    }

                }
            }
        }
    }
}


    

void left_diag_detect(vector<vector<vector<int>>>&s, int r, int c, int step, vector<vector<float>>& score_board) {
    //# Check the left diagonal(/ ) and see if there is a winner
    vector<int> score = { 0, 0, 0, 0 };//  # store the number of consecutive colorsand dots[red, whites, dots, circles]
    for (int k = 3; k > 0; k--) {
        int r0 = r + k;
        int c0 = c - k;
        if (r0 < 12 && c0 >= 0) {
            int cell = s[r0][c0][0];
            Calculate_scores2(cell, score);
            for (int i = 0; i < 4; i++) {
                if (score[i] == 2) {    //# it is possible for this line to combine 4 consecutive colors or dots
                    score_board[i][0] += 1;
                }
                if (score[i] == 3) {
                    score_board[i][1] += 1;
                }
            }
        }
    }
}


void right_diag_detect(vector<vector<vector<int>>>& s, int r, int c, int step, vector<vector<float>>& score_board) {
    //# Check the right diagonal(\) and see if there is a winner
    vector<int> score = { 0, 0, 0, 0 }; //  # store the number of consecutive colorsand dots[red, whites, dots, circles]
    for (int k = 3; k > 0; k--) {
        int r0 = r + k;
        int c0 = c + k;
        if (r0 < 12 && c0 < 8) {
            int cell = s[r0][c0][0];
            Calculate_scores2(cell, score);

            for (int i = 0; i < 4; i++) {
                if (score[i] == 2) {// # it is possible for this line to combine 4 consecutive colors or dots
                    score_board[i][0] += 1;
                }
                if (score[i] == 3) {
                    score_board[i][1] += 1;
                }
            }
        }
    }
}

void horizontal_detect(vector<vector<vector<int>>>& s, int r, int c, int step, vector<vector<float>>& score_board) {
    //# calculate consecutive colorsand dots
    vector<int> score(4, 0);    //  # store the number of consecutive colorsand dots[red, whites, dots, circles]
    int cell;
    for (int k = 3; k > -1; k--) {
        if (c - k < 8 && c-k>=0) {
            cell = s[r][c - k][0];
            Calculate_scores2(cell, score);
        }
    }
    for (int i = 0; i < 4; i++) {
        if (score[i] == 2) { // # it is possible for this line to combine 4 consecutive colors or dots
            score_board[i][0] += 1.0 / 2;
        }
        if (score[i] == 3) {
            score_board[i][1] += 1.0 / 2;
        }
    }

    score = { 0, 0, 0, 0 };     //  # store the number of consecutive colorsand dots[red, whites, dots, circles]
    for (int k = 2; k > -2; k--) {
        if (c - k < 8 && c-k>=0) {
            cell = s[r][c - k][0];
            Calculate_scores2(cell, score);
        }
    }
    for (int i = 0; i < 4; i++) {
        if (score[i] == 2) { //: # it is possible for this line to combine 4 consecutive colors or dots
            score_board[i][0] += 1.0 / 2;
        }
        if (score[i] == 3) {
            score_board[i][1] += 1.0 / 2;
        }
    }

    score = { 0, 0, 0, 0 };//  # store the number of consecutive colors and dots[red, whites, dots, circles]
    for (int k = 3; k > -1; k--) {
        if (c + k < 8) {
            cell = s[r][c + k][0];
            Calculate_scores2(cell, score);
        }
        for (int i = 0; i < 4; i++) {
            if (score[i] == 2) {        // # it is possible for this line to combine 4 consecutive colors or dots
                score_board[i][0] += 1 / 2;
            }
            if (score[i] == 3) {
                score_board[i][1] += 1 / 2;
            }
        }

        score = { 0, 0, 0, 0 };         //  # store the number of consecutive colorsand dots[red, whites, dots, circles]
        for (int k = 2; k > -2; k--) {
            if (c + k < 8 && c+k>=0) {
                cell = s[r][c + k][0];
                Calculate_scores2(cell, score);
            }
        }
        for (int i = 0; i < 4; i++) {
            if (score[i] == 2) {     // # it is possible for this line to combine 4 consecutive colors or dots
                score_board[i][0] += 1 / 2;
            }
            if (score[i] == 3) {
                score_board[i][1] += 1 / 2;
            }
        }
    }
}

float  h1(vector<vector<vector<int>>>& s, int step) {

    // # h1 win over h2
    //# Inputs: state of the game s ϵ S
    //# without rule.
    //# Search surface cellsand calculate the number of three - continual colors, two continual colorsand three - continual dots and two - continual dots
    //# v(three - colors) = 1; v(two - colors) = 0.2; v(three - dots) = -1; v(two - dots) = -0.2;
    //# h(s) = nb(three - colors) + 0.2 * nb(two - colors) - nb(three - dots) - 0.2 * nb(two - dots)
    //# Reurn: heuristic function value of a state.

    vector<vector<float>> score_board(4,vector<float>(2,0));                            //# Initiate score board of size 4 * 2 with zero.[(nb_2reds, nb_3reds), (nb_2whites, nb_3whites), (nb_2dots, nb_3dots), (nb_2cricles, nb_3circles)]
    vector<vector<int>> surface = { {12, 0},{12, 1},{12, 2},{12, 3},{12, 4},{12, 5},{12, 6},{12, 7} };
    vector<int> score(4);
    int cell;
    //# if step<=24:
 
    //# search vertically
    for (int c = 0; c < 8; c++) {
        for (int r = 0; r < 12; r++) {
            if (s[r][c][0] != 0) { // # s[r][c] is the surface
                surface[c] = { r, c };
                if (r == 0 || s[r - 1][c][0] == 0) { //# and s[r - 1][c] is empty
                   // # calculate consecutive colors and dots
                    score = { 0, 0, 0, 0 };   //# store the number of consecutive colors and dots[red, whites, dots, circles]
                    for (int r1 = r + 3; r1 > r - 1; r1--) {
                        if (r1 < 12) {
                            cell = s[r1][c][0];
                            Calculate_scores2(cell, score);
                            if ((step > 24 && r1 == r + 1 && s[r][c][1] != step - 1) || r1 == r) {
                                for (int i = 0; i < 4; i++) {
                                    if (score[i] == 2 && r1 >= 2) { //# it is possible for this line to combine 4 consecutive colors or dots
                                        score_board[i][0] += 1;
                                    }
                                    if (score[i] == 3 && r1 >= 1) {
                                        score_board[i][1] += 1;
                                    }
                                }
                            }
                        }
                    }

                    //# Check the right  diagonal(\)
                    score = { 0, 0, 0, 0 };  //# store the number of consecutive colors and dots[red, whites, dots, circles]
                    for (int k = 2; k > -3; k--) {
                        int r0 = r + k;
                        int c0 = c + k;
                        if ((r0 >= 0 && r0 < 12) && (c0 >= 0 && c0 < 8)) {
                            cell = s[r0][c0][0];
                            Calculate_scores2(cell, score);
                            if (r0 - 1 > 0 && c0 - 1 > 0 && cell != 0 && s[r0 - 1][c0 - 1][0] == 0 && k <= 0) {
                                for (int i = 0; i < 4; i++) {
                                    if (score[i] == 2) {// # it is possible for this line to combine 4 consecutive colors or dots
                                        score_board[i][0] += 1;
                                    }
                                    if (score[i] == 3) {
                                        score_board[i][1] += 1;
                                    }
                                }
                            }
                        }
                    }

                    //# Check the left diagonal(/ ) and see if there is a winner
                    score = { 0, 0, 0, 0 }; // # store the number of consecutive colorsand dots[red, whites, dots, circles]
                    for (int k = 2; k > -3; k--) {
                        int r0 = r + k;
                        int c0 = c - k;
                        if (r0 >= 0 && r0 < 12 && c0 >= 0 && c0 < 8) {
                            cell = s[r0][c0][0];
                            Calculate_scores2(cell, score);
                            if (r0 - 1 >= 0 && c0 + 1 < 8 && cell != 0 && s[r0 - 1][c0 + 1][0] == 0 && k <= 0) {
                                for (int i = 0; i < 4; i++) {
                                    if (score[i] == 2) {// # it is possible for this line to combine 4 consecutive colors or dots
                                        score_board[i][0] += 1;
                                    }
                                    if (score[i] == 3) {
                                        score_board[i][1] += 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
        }
    }
    
    //# Check horizontally
    score = { 0, 0, 0, 0 };   //# store the number of consecutive colors and dots[red, whites, dots, circles]
    for (int c = 0; c < 7; c++) {
        int r1 = surface[c][0];
        int r2 = surface[c+1][0];
        if (r1 < r2) {
            if (c > 0) {
                for (int i = r1; i < r2; i++) {
                    if (i < 10) {
                        left_diag_detect(s, i, c + 1, step, score_board);
                    }
                    horizontal_detect(s, i, c + 1, step, score_board);
                }
            }
        }
        else if (r1 > r2) {
            if (c < 6) {
                for (int i = r1 - 1; i > r2; i--) {
                    if (i < 10) {
                        right_diag_detect(s, i, c, step, score_board);
                    }
                    horizontal_detect(s, i, c, step, score_board);
                }
            }
        }
    }


    float h = 5.0 * (score_board[0][1] + score_board[1][1]) + 0.2 * (score_board[0][0] + score_board[1][0]) - 10 * (score_board[2][1] + score_board[3][1]) - 0.2 * (score_board[2][0] + score_board[3][0]);

    float h_min = 0;
    int v,t;
    int r0, c0, r1, c1;
    vector<int> p1, p2;
    bool result;
    vector<vector<int>> rm;
    vector<int> visit_card;
    if (step > 24 && step <= 40) {
        //# zhaochu bianhua
        visit_card = { step - 1 };
        for (int c = 0; c < 8; c++) {
            vector<vector<float>> score_board2(4, vector<float>(2, 0));
            int r = surface[c][0];
            if (r < 12) {
                int card_number = s[r][c][1];
                if (std::find(visit_card.begin(), visit_card.end(), card_number) == visit_card.end()) {
                    if (r < 11 && s[r + 1][c][1] == card_number) { //# vertical card
                        p1 = { r + 1, c };
                        p2 = { r, c };
                        v = 1;
                    }
                    if (c < 7 && s[r][c + 1][1] == card_number) {  //# horizontal card
                        p1 = { r, c };
                        p2 = { r, c + 1 };
                        v = 0;
                    }
                    try {
                        result = Rule_checker(s, p1, p2, step, 1);
                    }
                    catch (exception& e) {
                        result = false;
                        cout << e.what() << endl;
                    }

                    if (result) {
                        visit_card.push_back(card_number);
                        rm = { p1, p2 };
                        t = -1;
                        for (auto p : rm) {
                            t += 1;
                            //# calculate consecutive colors and dots
                            score = { 0, 0, 0,0 };//  # store the number of consecutive colorsand dots[red, whites, dots, circles]
                            r0 = p[0];
                            c0 = p[1];

                            if (v == 0 || t == 1) {  //  # Check the vertical direction
                                for (r1 = r0 + 3; r1 > r0; r1--) {
                                    if (r1 < 12) {
                                        cell = s[r1][c][0];
                                        Calculate_scores2(cell, score);
                                    }
                                }
                                for (int i = 0; i < 4; i++) {
                                    if (score[i] == 2 && r1 >= 2) {// # it is possible for this line to combine 4 consecutive colors or dots
                                        score_board2[i][0] += 1;
                                    }
                                    if (score[i] == 3 && r1 >= 1) {
                                        score_board2[i][1] += 1;
                                    }
                                }
                            }

                            //# Check the right  diagonal(\)
                            score = { 0, 0, 0, 0 };  //# store the number of consecutive colors and dots[red, whites, dots, circles]
                            for (int k = 3; k > 0; k--) {
                                r1 = r0 + k;
                                c1 = c0 + k;

                                if (r1 >= 0 && r1 < 12 && c1 >= 0 && c1 < 8) {
                                    cell = s[r1][c1][0];
                                    Calculate_scores2(cell, score);
                                }
                            }
                            for (int i = 1; i < 4; i++) {
                                if (score[i] == 2) {// # it is possible for this line to combine 4 consecutive colors or dots
                                    score_board2[i][0] += 1;
                                }
                                if (score[i] == 3) {
                                    score_board2[i][1] += 1;
                                }
                            }

                            //# Check the left diagonal(/ )
                            score = { 0, 0, 0,0 };  //# store the number of consecutive colors and dots[red, whites, dots, circles]
                            for (int k = 3; k < 0; k--) {
                                r1 = r0 + k;
                                c1 = c0 - k;
                                if (r1 >= 0 && r1 < 12 && c1 >= 0 && c1 < 8) {
                                    int cell = s[r1][c1][0];
                                    Calculate_scores2(cell, score);
                                }
                            }
                            for (int i = 1; i < 4; i++) {
                                if (score[i] == 2) {// # it is possible for this line to combine 4 consecutive colors or dots
                                    score_board[i][0] += 1;
                                }
                                if (score[i] == 3) {
                                    score_board[i][1] += 1;
                                }
                            }

                            //# Check the horizontal direction
                            score = { 0, 0, 0, 0 };
                            if (v == 0) {
                                if (t == 0) {
                                    for (int i = 3; i > 0; i--) {
                                        c1 = c0 - i;
                                        if (c1 > 0) {
                                            cell = s[r0][c1][0];
                                            Calculate_scores2(cell, score);
                                        }
                                    }
                                }
                                else if (t == 1) {
                                    for (int i = 3; i > 0; i--) {
                                        c1 = c0 - i;
                                        if (c1 > 0) {
                                            cell = s[r0][c1][0];
                                            Calculate_scores2(cell, score);
                                        }
                                    }
                                }
                            }
                            else if (v == 1) {
                                vector<int> score1(4, 0);  //# store the number of consecutive colors and dots[red, whites, dots, circles]
                                vector<int> score2(4, 0);  //# store the number of consecutive colors and dots[red, whites, dots, circles]
                                for (int i = 3; i > 0; i--) {
                                    c1 = c0 - i;
                                    if (c1 > 0) {
                                        cell = s[r][c1][0];
                                        Calculate_scores2(cell, score1);
                                    }
                                    for (int i = -3; i < 0; i++) {
                                        c1 = c0 - i;
                                        if (c1 < 8) {
                                            cell = s[r][c1][0];
                                            Calculate_scores2(cell, score2);
                                        }
                                    }
                                    for (int i = 0; i < 4; i++) {
                                        score[i] = score1[i] + score2[i];
                                    }
                                }
                                for (int i = 0; i < 4; i++) {
                                    if (score[i] == 2) { //# it is possible for this line to combine 4 consecutive colors or dots
                                        score_board2[i][0] += 1;
                                    }
                                    if (score[i] == 3) {
                                        score_board2[i][1] += 1;
                                    }
                                }
                                float h2 = 5 * (score_board2[0][1] + score_board2[1][1]) + 0.2 * (score_board2[0][0] + score_board2[1][0]) - 10 * (score_board2[2][1] + score_board2[3][1]) - 0.2 * (score_board2[2][0] + score_board2[3][0]);

                                if (h2 < h_min) {
                                    h_min = h2;
                                }
                            }
                        }
                    }
                }
            }
        }
        
    }
    return h + h_min;
}

float h2(vector<vector<vector<int>>>& s, int step, vector<vector<int>>& last_move) {
    //# calcaulate the relative change of heuristic value caused by last_move
    vector<vector<float>> score_board(4, vector<float>(2, 0));                            //# Initiate score board of size 4 * 2 with zero.[(nb_2reds, nb_3reds), (nb_2whites, nb_3whites), (nb_2dots, nb_3dots), (nb_2cricles, nb_3circles)]
    vector<int> score(4), score1(4),score2(4);
    int cell;
    vector<vector<int>> m, rm;
    int c0, r0, a, i, r, c,empt;

    //# check if there are four connections due to adding of p1and p2.
    if (last_move.size() == 5) {
        for (auto it = last_move.begin(); it != last_move.begin() + 2; it++) {
            rm.push_back(*it);
        }
        for (auto it = last_move.begin() + 3; it != last_move.end(); it++) {
            m.push_back(*it);
        }
        a = (*(last_move.begin() + 2))[0];
    }
    else if (last_move.size() == 3) {
        for (auto it = last_move.begin() + 1; it != last_move.end(); it++) {
            m.push_back(*it);
        }
        a = (*(last_move.begin()))[0];
    }
    else if (last_move.size() == 0) {
        return 0;
    }

    for (auto p : m) {
        //# Check the vertical directionand see the change
        score = { 0, 0, 0, 0 };  //# store the number of consecutive colors and dots[red, whites, dots, circles]
        c0 = p[1];
        r0 = p[0];
        if (a % 2 == 1) {
            for (r = r0 + 3; r > r0 - 1; r--) {
                if (r < 12) {
                    cell = s[r][c0][0];
                    Calculate_scores2(cell, score);
                    if (r == r0 + 1) {
                        for (i = 0; i < 4; i++) {
                            if (score[i] == 2) { //# it is possible for this line to combine 4 consecutive colors or dots
                                score_board[i][0] -= 1;
                            }
                            if (score[i] == 3) {
                                score_board[i][1] -= 1;
                            }
                        }
                    }
                    if (r == r0) {
                        for (i = 0; i < 4; i++) {
                            if (score[i] == 2) { //# it is possible for this line to combine 4 consecutive colors or dots
                                score_board[i][0] += 1;
                            }
                            if (score[i] == 3) {
                                score_board[i][1] += 1;
                            }
                        }
                    }
                }
            }
        }
        else if (p[0] == m[1][0] && p[1] == m[1][1]) {
            for (r = r0 + 4; r > r0 - 1; r--) {
                if (r < 12) {
                    cell = s[r][c0][0];
                    Calculate_scores2(cell, score);
                    if (r == r0 + 2) {
                        for (i = 0; i < 4; i++) {
                            if (score[i] == 2) { //# it is possible for this line to combine 4 consecutive colors or dots
                                score_board[i][0] -= 1;
                            }
                            if (score[i] == 3) {
                                score_board[i][1] -= 1;
                            }
                        }
                    }
                    if (r == r0) {
                        for (i = 0; i < 4; i++) {
                            if (score[i] == 2) { //# it is possible for this line to combine 4 consecutive colors or dots
                                score_board[i][0] += 1;
                            }
                            if (score[i] == 3) {
                                score_board[i][1] += 1;
                            }
                        }
                    }
                }
            }
        }

        //# Check the right diagonal(\) and see the change
        score1 = { 0, 0, 0, 0 };  //# store the number of consecutive colors and dots[red, whites, dots, circles]
        score2 = { 0, 0, 0, 0 };  //# store the number of consecutive colors and dots[red, whites, dots, circles]
        empt = 0;
        for (i = 3; i > 0; i--) {
            r = r0 + i;
            c = c0 + i;
            if (r < 12 && c < 8) {
                cell = s[r][c][0];
                if (cell == 0) {
                    empt += 1;
                }
                else {
                    Calculate_scores2(cell, score1);
                }
            }
        }
        for (i = -3; i < 0; i++) {
            r = r0 + i;
            c = c0 + i;
            if (r >= 0 && c >= 0) {
                cell = s[r0][c0][0];
                if (cell == 0) {
                    empt += 1;
                }
                else {
                    Calculate_scores2(cell, score2);
                }
            }
        }
        for (i = 0; i < 4; i++) {
            score[i] = score1[i] + score2[i];
        }
        for (i = 0; i < 4; i++) {
            if (score[i] == 2) { //# it is possible for this line to combine 4 consecutive colors or dots
                score_board[i][0] -= 1;
            }
            if (score[i] >= 3) {
                score_board[i][1] -= 1;
            }
        }
        cell = s[r0][c0][0];
        Calculate_scores2(cell, score);

        for (i = 0; i < 4; i++) {
            if (score[i] == 2 && empt > 1) {  //# it is possible for this line to combine 4 consecutive colors or dots
                score_board[i][0] += 1;
            }
            if (score[i] >= 3 && empt > 0) {
                score_board[i][1] += 1;
            }
        }


        //# Check the left diagonal(/ ) and see if there is a winner
        score1 = { 0, 0, 0, 0 };  //# store the number of consecutive colors and dots[red, whites, dots, circles]
        score2 = { 0, 0, 0, 0 };
        empt = 0;
        for (i = 3; i > 0; i--) {
            r = r0 + i;
            c = c0 - i;
            if (r < 12 && c >0) {
                cell = s[r][c][0];
                if (cell == 0) {
                    empt += 1;
                }
                else {
                    Calculate_scores2(cell, score1);
                }
            }
        }
        for (i = -3; i < 0; i++) {
            r = r0 + i;
            c = c0 - i;
            if (r >= 0 && c < 8) {
                cell = s[r][c][0];
                if (cell == 0) {
                    empt += 1;
                }
                else {
                    Calculate_scores2(cell, score2);
                }
            }
        }
        for (i = 0; i < 4; i++) {
            score[i] = score1[i] + score2[i];
        }
        for (i = 0; i < 4; i++) {
            if (score[i] == 2) { //# it is possible for this line to combine 4 consecutive colors or dots
                score_board[i][0] -= 1;
            }
            if (score[i] >= 3) {
                score_board[i][1] -= 1;
            }
        }
        cell = s[r0][c0][0];
        Calculate_scores2(cell, score);

        for (i = 0; i < 4; i++) {
            if (score[i] == 2 && empt > 1) {  //# it is possible for this line to combine 4 consecutive colors or dots
                score_board[i][0] += 1;
            }
            if (score[i] >= 3 && empt > 0) {
                score_board[i][1] += 1;
            }
        }

        //# Check the horizontal direction and see the change of consecutive cards
        score1 = { 0, 0, 0, 0 };  //# store the number of consecutive colors and dots[red, whites, dots, circles]
        score2 = { 0, 0, 0, 0 };
        empt = 0;
        for (i = 3; i > 0; i--) {
            c = c0 - i;
            if (c > 0) {
                cell = s[r0][c][0];
                if (cell == 0) {
                    empt += 1;
                }
                else {
                    Calculate_scores2(cell, score1);
                }
            }
        }
        for (i = -3; i < 0; i++) {
            c = c0 - i;
            if (c < 8) {
                cell = s[r0][c][0];
                if (cell == 0) {
                    empt += 1;
                }
                else {
                    Calculate_scores2(cell, score2);
                }
            }
        }
        for (i = 0; i < 4; i++) {
            score[i] = score1[i] + score2[i];
        }
        for (i = 0; i < 4; i++) {
            if (score[i] == 2) { //# it is possible for this line to combine 4 consecutive colors or dots
                score_board[i][0] -= 1;
            }
            if (score[i] >= 3) {
                score_board[i][1] -= 1;
            }
        }
        cell = s[r0][c0][0];
        Calculate_scores2(cell, score);

        for (i = 0; i < 4; i++) {
            if (score[i] == 2 && empt > 1) { // # it is possible for this line to combine 4 consecutive colors or dots
                score_board[i][0] += 1;
            }
            if (score[i] >= 3 && empt > 0) {
                score_board[i][1] += 1;
            }
        }
    }

    float h = 5 * (score_board[0][1] + score_board[1][1]) + 0.2 * (score_board[0][0] + score_board[1][0]) - 10 * (score_board[2][1] + score_board[3][1]) - 0.2 * (score_board[2][0] + score_board[3][0]);
    return  h;
}


//# Minimax + alpha - beata pruning search with heuristic
std::tuple<float, list<vector<vector<int>>>> Minimax_ab(vector<vector<vector<int>>>& s, int depth, float alpha, float beta, int maximizingPlayer, int step, int ab, float (*h)(vector<vector<vector<int>>>&, int,vector<vector<int>>&), int color, float ph, vector<vector<int>>& last_move) {
    //# Inputs : state of the game s ϵ S, depth of the algorithm, alpha, beta and indicator for whose turn
    //# Inputs : step, last_move, heuristic functionand max_color
    //# Actions : according to current stage, find out the 'optimal' action(position + placement)
    //# Return : action to next step

 
    list<vector<vector<int>>> offspring;
    list<vector<vector<int>>> children;
    int terminal;
    list<vector<int>> scores;
    string winner;
    list<vector<vector<int>>> optimal_nodes;
    vector<vector<int>> rm, m;
    float v;

    std::tie(terminal, scores) = Terminal_checker(s, last_move);

    if (depth == 0 || terminal || step > 40){
        optimal_nodes.push_back(last_move);
        if (terminal){
            winner = get_winner(scores, step, color);
            if (winner == "color"){
                v = 1e6;
            }
            else {
                v = -1e6;
            }
        }
        else {
            v = h(s, step,last_move);
        }
        return std::make_tuple(v, optimal_nodes);
    }
    
    children = get_children(s, step);  //# Data structure of children : [[m1, 0, []], [m2, 0, []], , , , [mn, 0, []]] .m : next_move[a, p1, p2] or [p1, p2, a, p3, p4]
    
    list<vector<vector<int>>> next_node;
    float v1;

    float v0 = h(s, depth, last_move) + ph;
    if (maximizingPlayer) {
        v = -1e8;
        
        //int t = 0;

        for (auto child : children) {
            //t++;
            //if (t > 4) { break; }
            //m = child;
            vector<vector<vector<int>>> next_s(s);
            if (child.size() == 3) {
                Move(next_s, child, step);
            }
            else if (child.size() == 5) {
                if (rm.size() != 0) { rm.clear(); }
                if (m.size() != 0) { m.clear(); }
                for (auto it = child.begin(); it != child.begin()+2;it++) {
                    rm.push_back(*it);
                }
                for (auto it = child.begin()+2; it != child.end();it++) {
                    m.push_back(*it);
                }
                //std::copy(child.begin(), child.begin() + 1, rm.begin());
                //std::copy(child.begin()+2, child.end(), m.begin());
                Remove(next_s, rm);
                Move(next_s, m, step);
            }
            
            std::tie(v1, next_node) = Minimax_ab(next_s, depth - 1, alpha, beta, 0, step + 1, ab,  h, color, v0, child);
            // free the memory of next_s;
            next_s.clear();
            next_s.shrink_to_fit();

            //if trace:
            //offspring += [[v1, m, child_offspring]];

            if (v1 > v) {
                v = v1;
                optimal_nodes = { last_move };//  # optimal_nodes only record nodes of the optimal child
                for (auto it : next_node) {
                    optimal_nodes.push_back(it);
                }
            }

            //# cost_tree[2][i] = subtree
            if (ab) {
                alpha = max(alpha, v);
                if (beta <= alpha) {
                    break;
                }  //# beta pruning
                 
            }
        }
        return std::make_tuple(v, optimal_nodes);
    }

    else {
        v = 1e8;
        //int t2 = 0;
        for (auto child : children) {
            //t2++;
            //if (t2 > 4) { break; }
            vector<vector<vector<int>>> next_s(s);
            if (child.size() == 3) {
                Move(next_s, child, step);
            }
            else if (child.size() == 5) {
                //std::copy(child.begin(), child.begin() + 1, rm.begin());
                //std::copy(child.begin() + 2, child.end(), m.begin());

                if (rm.size() != 0) { rm.clear(); }
                if (m.size() != 0) { m.clear(); }
                for (auto it = child.begin(); it != child.begin() + 2; it++) {
                    rm.push_back(*it);
                }
                for (auto it = child.begin()+2; it != child.end(); it++) {
                    m.push_back(*it);
                }
                Remove(next_s, rm);
                Move(next_s, m, step);
            }

            std::tie(v1, next_node) = Minimax_ab(next_s, depth - 1, alpha, beta, 1, step + 1, ab, h, color, v0, child);
            // free the memory of next_s;
            next_s.clear();
            next_s.shrink_to_fit();


            if (v1 < v) {
                v = v1;
                optimal_nodes = { last_move };
                for (auto it : next_node) {
                    optimal_nodes.push_back(it);
                }
            }
            if (ab) {
                beta = min(beta, v);
                if (beta <= alpha) { //# alpha pruning
                    break;
                }
            }
        }
        return std::make_tuple(v, optimal_nodes);
    }
}

/*
std::vector<std::string> split(const std::string& s) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item,' ')) {
        elems.push_back(item);
        // elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
    }
    return elems;
}*/

void Game_Frame(vector<vector<vector<int>>>& s) {
    //******************************** Flow Frame of the Game**********************************************************
    // automatic = int(input("If human-to-huam: input 0,else machine-to-human: input 1: "))
    std::cout << "In the machine-to-human mode!" << endl;
    int automatic = 1;
    int firstplayer;
    int machine_first;
    int ab;
    int terminal;
    string player;
    int c;

    list<int> times, numb;
    vector<vector<int>> lm, m, rm;
    int dep, step;
    time_t start, end;
    float v1;
    list<vector<vector<int>>> optimal_path;

    while (true) {
        std::cout << "AI palys as the 1th player or 2nd palyer?(1 or 2):";
        cin >> firstplayer;
        if (firstplayer == 1) {
            machine_first = 1;
            break;
        }
        else if (firstplayer == 2) {
            machine_first = 0;
            break;
        }
        else {
            std::cout << "Wrong input,try again.Ipnut 1 or 2.";
        }
    }

    std::cout << "Machine will adopt MiniMax search algorithm!" << endl;
    while (true) {
        std::cout << "Activate alpha-beta or not.(activate: 1;not activate:0):";
        cin >> ab;
        std::cin.ignore();
        if (ab == 1) {
            ab = 1;
            break;
        }
        else if (ab == 0) {
            ab = 0;
            break;
        }
        else {
            std::cout << "Wrong input,try again." << endl;
        }
    }

    

    while (true) {
        std::cout << "Input which role the first player wants to paly(color or dot?): ";
        std::getline(std::cin, player,'\n');
        std::transform(player.begin(), player.end(), player.begin(), [](unsigned char new_p) { return std::tolower(new_p); });
        if (player == "color") {
            std::cout << "Color first.";
            c = 1;
            break;
        }
        else if (player == "dot") {
            std::cout << "dot first.";
            c = 0;
            break;
        }
        else {
            std::cout << "Wrong input,try again.";
        }
    }

    int machine_max;
    if ((machine_first && c) || (!machine_first && !c)) {
        machine_max = 1;
    }
    else {
        machine_max = 0;
    }

    // Start the main for loop of our game
   
    for (step = 1; step < 41; step++) {
        std::cout << "step:" << step << endl;
        if (step <= 24) {                            // Period for regular moves
            while (true) {
                try {
                    if (step % 2 == c) {
                        std::cout << "It is color's turn." << endl;
                    }
                    else {
                        std::cout << "It is dot/circle's turn." << endl;
                    }

                    // if automatic:
                    if (step % 2 == machine_first) {
                        start = time(NULL);
                        if (step <= 2) {
                            dep = 2;
                        }
                        else if (step <= 10) {
                            dep = 3;
                        }
                        else if (step <= 21) {
                            dep = 4;
                        }
                        else {
                            dep = 5;
                        }

                        //# dep = 2
                        //if (step<2) vector<vector<int>> lm(0);
                        if (step >= 2) {
                            //std::copy(std::begin(m), std::end(m), std::begin(lm));
                            if (lm.size() != 0) { lm.clear(); }
                            for (auto it =m.begin(); it!= m.end(); it++) {
                                lm.push_back(*it);
                            }
                        }

                        std::tie(v1, optimal_path) = Minimax_ab(s, dep, -1e8, 1e8, machine_max, step, ab, h2, c, 0, lm);

                        //m = optimal_path[1];
                        list< vector<vector<int>>>::iterator it = optimal_path.begin();
                        std::advance(it, 1);

                        //std::copy(std::begin(*it), std::end(*it), std::begin(m));
                        if (m.size() != 0) { m.clear(); }
                        for (auto it2 = (*it).begin(); it2 != (*it).end(); it2++) {
                            m.push_back(*it2);
                        }

                        std::cout << Rev_translation(*it) << endl;
                    }

                    else {
                        /*
                        vector<vector<int>> _;
                        std::tie(_, m) = Human_input();
                        */

                        if (step >= 2) {
                            //std::copy(std::begin(m), std::end(m), std::begin(lm));
                            if (lm.size() != 0) { lm.clear(); }
                            for (auto it = m.begin(); it != m.end(); it++) {
                                lm.push_back(*it);
                            }
                        }

                        std::tie(v1, optimal_path) = Minimax_ab(s, dep, -1e8, 1e8, machine_max, step, ab, h2, c, 0, lm);

                        //m = optimal_path[1];
                        list< vector<vector<int>>>::iterator it = optimal_path.begin();
                        std::advance(it, 1);

                        //std::copy(std::begin(*it), std::end(*it), std::begin(m));
                        if (m.size() != 0) { m.clear(); }
                        for (auto it2 = (*it).begin(); it2 != (*it).end(); it2++) {
                            m.push_back(*it2);
                        }
                        std::cout << Rev_translation(*it) << endl;

                    }

                    bool res;
                    res = Rule_checker(s, m[1], m[2], step, 0);

                    Move(s, m, step);         //# Move to the desired position

                    Visulization(s);

                    //# if step % 2 == machine_first:
                    end = time(NULL);
                    std::cout << "time consume:" << end - start << endl;
                    times.push_back(end - start);
                    break;
                }

                catch (exception& e) {
                    std::cout << e.what() << endl;
                    if (step % 2 == machine_first) {
                        std::cout << "AI put an illigebal move. AI failed!!!  Game Over.";
                        return;
                    }
                    else {
                        std::cout << "Please try again:";
                    }
                }
            }

            if (step == 24) {
                std::cout << "End of regular move. Enter recycling moving stage!!!!" << endl;
            }
        }

        //# Period for recycling moves
        else {
            while (true) {
                try {
                    if (step % 2 == c) {
                        std::cout << "It is color's turn.";
                    }
                    else {
                        std::cout << "It is dot/circle's turn.";
                    }
                    if (!automatic) {
                        std::tie(rm, m) = Human_input(1);
                    }
                    else {
                        if (step % 2 == machine_first) {
                            start = time(NULL);

                            if (step == 25) {
                                //std::copy(std::begin(m), std::end(m), std::begin(lm));
                                if (lm.size() != 0) { lm.clear(); }
                                for (auto it = m.begin(); it != m.end(); it++) {
                                    lm.push_back(*it);
                                }
                            }
                            else {
                                list< vector<vector<int>>>::iterator it = optimal_path.begin();
                                std::advance(it, 1);
                                //std::copy(std::begin(*it), std::end(*it), std::begin(lm));
                                if (lm.size() != 0) { lm.clear(); }
                                for (auto it2 = (*it).begin(); it2 != (*it).end(); it2++) {
                                    lm.push_back(*it2);
                                }
                            }

                            std::tie(v1, optimal_path) = Minimax_ab(s, dep, -1e8, 1e8, machine_max, step, ab, h2, c, 0, lm);
                            std::cout << Rev_translation(lm) << endl;

                            list< vector<vector<int>>>::iterator it = optimal_path.begin();
                            std::advance(it, 1);
                            

                            if (rm.size() != 0) { rm.clear(); }
                            if (m.size() != 0) { m.clear(); }
                            for (auto it2 = (*it).begin(); it2!= (*it).begin()+2; it2++) {
                                    rm.push_back(*it2);
                                }
                            for (auto it2 = (*it).begin()+2; it2!= (*it).end(); it2++) {
                                    m.push_back(*it2);
                                }
                            
                            

                            //std::copy(std::begin(*it), std::begin(*it) + 2, std::begin(rm));
                            //std::copy(std::begin(*it) + 2, std::end(*it), std::begin(m));


                        }
                        else {
                            std::tie(rm, m) = Human_input(1);
                        }
                    }

                    if (rm[0] == m[1] && rm[1] == m[2]) {
                        throw ex10;
                    }

                    bool result = Rule_checker(s, rm[0], rm[1], step, 1);
                    vector<vector<vector<int>>> s1(s);
                    Remove(s1, rm);
                    result = Rule_checker(s1, m[1], m[2], step, 0);

                    Remove(s, rm);
                    Move(s, m, step);
                    Visulization(s);

                    //# if step % 2 == machine_first:
                    end = time(NULL);
                    std::cout << "time consume:" << end - start;
                    times.push_back(end - start);
                    break;
                }

                catch (exception& e) {
                    std::cout << e.what() << endl;
                    if (step % 2 == machine_first) {
                        std::cout << "AI put an illigebal move. AI failed!!! Game Over." << endl;
                        return;

                    }
                    std::cout << "Please try again:";
                }
            }
        }

        
        list<vector<int>> scores;
        string winner;
        std::tie(terminal, scores) = Terminal_checker(s, m);

        if (terminal == 1) {
            winner = get_winner(scores, step, c);
            std::cout << "Congratalations! The winner is:" << winner;
            break;
        }
    }

    if (terminal == 0) {
        std::cout << "No winner, Draw" << endl;
    }
    return;
}


int main(void) {
    vector<vector<vector<int>>> s(12, vector<vector<int>>(8, vector<int>(2, 0)));
    //vector<vector<int>> m, rm;

   //std::tie(rm, m) = Human_input(0);
    //std::tie(rm, m) = Human_input(1);


	Visulization(s);
	Game_Frame(s);
}