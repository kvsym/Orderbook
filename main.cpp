
#include "OrderBook.h"
#include <iostream>


int main()
{
    DateTime newdate(2020,  1, 1, 12, 0, 0);
    int c = 1;
    Order order1(c, newdate, Side::Buy,  100.0, 10, "GOOGL");
    std::cout<<order1.getClient()<<std::endl;
    OrderBook ob;
    ob.addOrderToBook(order1);
    return 0;
}