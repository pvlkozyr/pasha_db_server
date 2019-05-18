#ifndef INC_4TERM_DATABASE_H
#define INC_4TERM_DATABASE_H

#include "Query.h"
#include "Record.h"


class Database{
private:
    std::vector<Record> db;
    std::string dbname;
public:
    Database();
    explicit Database(std::string);
    ~Database();
    void delete_recs(Query);
    void add_rec(Record);
    std::vector<Record> select_recs(Query);
    int bill(Query);
};
 
 
#endif //INC_4TERM_DATABASE_H