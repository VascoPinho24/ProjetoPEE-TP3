#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <unistd.h>
#include <thread>

#define DB_REFRESH_MS 5000

void connectDB();
void disconnectDB();
unsigned long long int hubKey(unsigned int);