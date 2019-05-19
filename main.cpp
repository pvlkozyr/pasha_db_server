#include <iostream>
#include <vector>
#include <fstream>
#include "Database.h"
#include "gen.h"
#include "Query.h"
#include <string>

int main() {
//    gen("name.txt",
//        "fam.txt",
//        "otch.txt",
//        "db.txt",5);
//    db.print("test.txt");
    Database db("test.txt");
    std::string query = "firstName=ШАБАШЕВ";
    Query q(query);
    std::vector<Record> s = db.select_recs(q);
    std::cout << query << " found " << s.size() << "\n";
    query = "date=2016";
    q = Query(query);
    s = db.select_recs(q);
    std::cout << query << " found " << s.size() << "\n";
    return 0;
}