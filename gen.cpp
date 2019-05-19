#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include "gen.h"
#include "stdlib.h"
 
 
void gen(std::string finname, std::string finfam, std::string finotch, std::string foutname, int SIZE) {
    srand(unsigned(std::time(0)));
    int y, m, d, t;
    std::string fio, ip, date, tin, tout, p;
    std::vector<std::string> trin, trout, fim,ffam,fotch;
    std::ifstream file(finname);
    while (!file.eof()){
        file >> p;
        fim.push_back(p);
    }
    file.close();
    std::ifstream file1(finfam);
    while (!file1.eof()){
        file1 >> p;
        ffam.push_back(p);
    }
    file1.close();
    std::ifstream file2(finotch);
    while (!file2.eof()){
        file2 >> p;
        fotch.push_back(p);
    }
    file2.close();
    std::ofstream file3(foutname);
    for (int i = 0;i<SIZE;i++)
    {
        fio = ffam[rand() % ffam.size()] + " " + fim[rand() % fim.size()] + " " + fotch[rand() % fotch.size()];
        file3 << fio + "\t";
        ip = std::to_string(rand() % 255) + "." +  std::to_string(rand() % 255) + "." +  std::to_string(rand() % 255) + "." +  std::to_string(rand() % 255);
        file3 << ip + "\t";
        y = 2015 + rand() % 4;
        m = 1 + rand() % 12;
       // std::cout << "Record added successfully loop" << std::endl;
        switch (m) {
            case 1:
                d = 1 + rand() % 31;
                break;
            case 3:
                d = 1 + rand() % 31;
                break;
            case 5:
                d = 1 + rand() % 31;
                break;
            case 7:
                d = 1 + rand() % 31;
                break;
            case 8:
                d = 1 + rand() % 31;
                break;
            case 10:
                d = 1 + rand() % 31;
                break;
            case 12:
                d = 1 + rand() % 31;
                break;
 
 
            case 4:
                d = 1 + rand() % 30;
                break;
            case 6:
                d = 1 + rand() % 30;
                break;
            case 9:
                d = 1 + rand() % 30;
                break;
            case 11:
                d = 1 + rand() % 30;
                break;
 
 
            default:
                if (y % 4 == 0) {
                    d = 1 + rand() % 29;
                } else {
                    d = 1 + rand() % 28;
                }
        }
        date = std::to_string(y) + "-";
        date += std::string(2 - std::to_string(m).length(), '0') + std::to_string(m) + '-';
        date += std::string(2 - std::to_string(d).length(), '0') + std::to_string(d);
        file3 << date;
        /*
        tin = "";
        tout = "";
        for (int j = 0; j < 24; j++) {
            t = rand() % 10000;
            tin += std::to_string(t) + " ";
            t = rand() % 10000;
            tout += std::to_string(t) + " ";
        }
        file3 << tin + "\t";
        file3 << tout + "\t";
         */
        if (i < SIZE - 1) file3 << '\n';
 
      //  std::cout << "Record added successfully loop2" << std::endl;
    }
    file3.close();
}