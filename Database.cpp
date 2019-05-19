#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "Database.h"
#include "Record.h"
#include <cstring>
#define PRICE 5
Database::Database() {
    std::cout << "no database file specified" << std::endl;
}
/*
void traffic(std::string s,std::vector<int> a,char ptr)
{
    std::string tmp = "";
    for (size_t i=0;i<strlen(s.c_str());i++)
      if (s[i]!=ptr)
      {
           tmp+=s[i];
      } else
      {
          a.push_back(std::stoi(tmp));
          tmp = "";
      }
 
}
 */
Database::Database(std::string fname) {
    dbname = fname;
    std::ifstream file(fname);
    if (!file.is_open()) {
        std::cout << "file doesn't exist" << std::endl;
        return;
    }
    Record t;
    int c = 0;
    while (!file.eof()) {
        std::getline(file,t.fio.firstName,' ');
        std::getline(file,t.fio.lastName,' ');
        std::getline(file,t.fio.patronimic,'\t');
        std::string tmp;
        std::getline(file,tmp,'.');
        t.ip.o1 = std::stoi(tmp);
        std::getline(file,tmp,'.');
        t.ip.o2 = std::stoi(tmp);
        std::getline(file,tmp,'.');
        t.ip.o3 = std::stoi(tmp);
        std::getline(file,tmp,'\t');
        t.ip.o4 = std::stoi(tmp);
        std::getline(file,tmp,'-');
        t.date.year = std::stoi(tmp);
        std::getline(file,tmp,'-');
        t.date.month = std::stoi(tmp);
        std::getline(file,tmp,'\n');
        t.date.day = std::stoi(tmp);
        c++;
        t.id = c;
        db.push_back(t);
    }
    std::cout << "added: " << db.size() << std::endl;
    file.close();
 
}

void Database::print(std::string fname)
{
    std::ofstream file(fname);
    for (int i = 0; i < db.size();i++)
    {
        file<<db[i].print();
    }
    file.close();
}

 /*
Database::~Database() {
    std::ofstream temp;
    temp.open("temp");
    for (size_t i = 0; i < db.size(); i++) {
        temp << db[i].fio + '\t' + db[i].ip + '\t' +
                db[i].date + '\t' + db[i].tin + '\t' +
                db[i].tout;
        if (i < db.size() - 1) temp << '\n';
    }
    temp.close();
    std::remove(dbname.c_str());
    std::rename("temp", dbname.c_str());
}*/
void Database::add_rec(Record t) {
    std::cout << "Record added successfully" << std::endl;
    db.push_back(t);
}
std::vector<Record> Database::select_recs(Query q)
{
    std::vector<Record> s;
    for (size_t i = 0; i < db.size(); i++) {
        if (q.check(db[i])) {
            s.push_back(db[i]);
        }
    }
    return s;
}
void Database::delete_recs(Query q) {
    for (size_t i = 0;i < db.size();i++)
    {
        if (q.check(db[i])) db.erase(db.begin() + i);
    }
    std::cout << "current size is : " << db.size() << std::endl;
}
    /*
 int Database::traffic(const int &id_min, const int &id_max,
                       const int &nip, const int &ip_min, const int &ip_max,
                       const int &ndate, const int &date_min, const int &date_max,
                       std::vector<int> nums){
    record t ;
    int sum = 0;
     for (size_t i = 0; i < db.size(); i++) {
       t = db[i];
        if (((id_min == -1 && id_max == -1) || (t.id>= id_min && t.id <= id_max))
            &&((nip == -1) || (t.clip[nip] >= ip_min && t.clip[nip] <= ip_max))
            && ((ndate == -1) || (t.cldate[ndate] >= date_min && t.cldate[ndate] <= date_max))) {
            for (int i = 0; i < 24;i++){
 
                    if (nums[i] == 1){
                        sum+=t.cltin[i]*PRICE;
                        }
                }
            }
 }
    return sum;
}
  */