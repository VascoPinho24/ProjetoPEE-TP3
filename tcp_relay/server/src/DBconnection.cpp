#include "../include/DBconnection.hpp"
#include "../include/tcpHandler.hpp"

sql::Driver *driver;
sql::Connection *con;
sql::Statement *stmt;
sql::ResultSet *res;

int connectedState = 0;

void connectDB(){
    if(!connectedState){
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "peeserver", "peeserver123@");
        stmt = con->createStatement();
        stmt->execute("USE test");
        connectedState = 1;
    } 
}

unsigned long long int hubKey(unsigned int hubID){
    res = stmt->executeQuery("SELECT hubkey FROM hubs WHERE ID=" + std::to_string(hubID));

    if(res->next()){
        return res->getInt("hubkey");
    }else{
        return 0;
    }
}

void disconnectDB(){
    if(connectedState){
        delete con;
        delete stmt;
        delete res;
        connectedState = 0;
    }
}
