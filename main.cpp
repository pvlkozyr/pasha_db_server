#include <iostream>
#include <vector>
#include <fstream>
#include "Database.h"
#include "gen.h"
#include <string>
int main() {
    gen("/users/2/207/Kozyr_PV/name.txt",
        "/users/2/207/Kozyr_PV/fam.txt",
        "/users/2/207/Kozyr_PV/otch.txt",
        "/users/2/207/Kozyr_PV/db.txt",100);
    Database db("/users/2/207/Kozyr_PV/db.txt");
    //db.add_rec("������ ����� �������������","255.255.255.255","20.04.2019",
     //"(1231 1234 12345 1235 1234 1234 1234 1234 1234 1234)","(1231 1234 12345 1235 1234 1234 1234 1234 1234 1234)");
     //db.select_recs("0.0.0.0","100.100.100.100","0","0","0","0","0","0");
    return 0;
}