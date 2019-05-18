#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "Database.h"
#include <cstring>
#define PRICE 5
Database::Database() {
    std::cout << "no database file specified" << std::endl;
}
 
void intarr(std::string s,std::vector<int> a,char ptr)
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
 
Database::Database(std::string fname) {
    dbname = fname;
    std::ifstream file(fname);
    if (!file.is_open()) {
        std::cout << "file doesn't exist" << std::endl;
        return;
    }
    record t;
    int c;
    std::string z;
    while (!file.eof()) {
        std::getline(file,t.fio,'\t');
        std::cout << t.fio << std::endl;
        std::getline(file,t.ip,'\t');
        std::cout <<  t.ip <<std::endl;
        intarr(t.ip,t.clip,'.');
        std::getline(file,t.date,'\t');
        std::cout << t.date << std::endl;
        intarr(t.date,t.cldate,'-');
        std::getline(file,t.tin,'\t');
        std::cout << t.tin << std::endl;
        intarr(t.tin,t.cltin,' ');
        std::getline(file,t.tout,'\t');
        std::cout << t.tout << std::endl;
        intarr(t.tout,t.cltout,' ');
        c++;
        t.id = c;
        std::getline(file,z,'\n');
        db.push_back(t);
    }
    std::cout << "added: " << db.size() << std::endl;
    file.close();
 
}
 
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
}
void Database::delete_recs(std::vector<int> s) {
    std::sort(s.begin(), s.end());
    while (!s.empty()) {
        //std::cout << s.back() << std::endl;
        db.erase(db.begin() + s.back());
        s.pop_back();
    }
    std::cout << "current size is : " << db.size() << std::endl;
}
void Database::add_rec(const std::string &fio, const std::string &ip,
                       const std::string &date, const std::string &tin, const std::string &tout) {
    record t;
    t.fio = fio;
    t.ip = ip;
    t.date = date;
    t.tin = tin;
    t.tout = tout;
    intarr(ip,t.clip,'.');
    intarr(date,t.cldate,'-');
    intarr(tin,t.cltin,' ');
    intarr(tout,t.cltout,' ');
    db.push_back(t);
    std::cout << "Record added successfully" << std::endl;
    std::cout << db.size() << std::endl;
}
 
void Database::add_rec(record t) {
    std::cout << "Record added successfully" << std::endl;
    db.push_back(t);
}
 
std::vector<int> Database::select_recs(const int &id_min, const int &id_max,
                                       const int &nip, const int &ip_min, const int &ip_max,
                                       const int &ndate, const int &date_min, const int &date_max)
{
    std::vector<int> s;
    record t;
    for (size_t i = 0; i < db.size(); i++) {
        t = db[i];
        if (((id_min == -1 && id_max == -1) || (t.id>= id_min && t.id <= id_max))
            &&((nip == -1) || (t.clip[nip] >= ip_min && t.clip[nip] <= ip_max))
            && ((ndate == -1) || (t.cldate[ndate] >= date_min && t.cldate[ndate] <= date_max)))
            s.push_back(i);
    }
    return s;
    }
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