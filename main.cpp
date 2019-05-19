#include <iostream>
#include <vector>
#include <fstream>
#include "Database.h"
#include "gen.h"
#include "Query.h"
#include <string>
int main() {
    /*gen("C:\\Users\\nagat\\Desktop\\name.txt",
        "C:\\Users\\nagat\\Desktop\\fam.txt",
        "C:\\Users\\nagat\\Desktop\\otch.txt",
        "C:\\Users\\nagat\\Desktop\\db.txt",5);
        */
    Database db("C:\\Users\\nagat\\Desktop\\db.txt");
    db.print("C:\\Users\\nagat\\Desktop\\test.txt");
    Query q("firstName=ГАЛАКТИОНОВ");
    db.select_recs(q);
    return 0;
}