#include <iostream>
#include <vector>
#include <fstream>
#include "/home/pavel/pasha_db_server/Database.h"
#include "/home/pavel/pasha_db_server/gen.h"
#include "/home/pavel/pasha_db_server/Query.h"
#include <string>

int main() {
    /*gen("/home/pavel/pasha_db_server/name.txt",
        "/home/pavel/pasha_db_server/fam.txt",
        "/home/pavel/pasha_db_server/otch.txt",
        "/home/pavel/pasha_db_server/db.txt",5);
        */
    Database db("/home/pavel/pasha_db_server/db.txt");
    std::string query = "firstName=ЯРУНОВ\tdate=2015";
    Query q(query);
    std::vector<Record> s = db.select_recs(q);
    std::cout << query << " found " << s.size() << "\n";
    query = "date=2016";
    q = Query(query);
    s = db.select_recs(q);
    std::cout << query << " found " << s.size() << "\n";
    return 0;
}