#pragma once
#include "Order.h"
#include "DoublyLinkedList.h"
#include <unordered_map>
#include <tuple>
#include <tuple>
#include <queue>
#include <vector>
#include <algorithm>

// PriceSidePair is a tuple of price and side
using PriceSidePair = std::tuple<double, Side::Enum>;

// ComparePrice is used for bestAsk
struct ComparePrice {
    bool operator()(const std::tuple<double, int>& lhs, const std::tuple<double, int>& rhs) const {
        return std::get<0>(lhs) > std::get<0>(rhs);
    }
};

// ComparePrice2 is used for bestBid
struct ComparePrice2 {
    bool operator()(const std::tuple<double, int>& lhs, const std::tuple<double, int>& rhs) const {
        return std::get<0>(lhs) < std::get<0>(rhs);
    }
};

// Hash function for std::tuple so that it can be used as a key in std::unordered_map
struct MyHash {
    std::size_t operator()(const std::tuple<double, Side::Enum>& k) const {
        // Compute individual hash values for first and second argument (use std::hash<T>{}(item))
        // and combine them using XOR and bit shifting:

        return std::hash<double>{}(std::get<0>(k)) ^ (std::hash<int>{}(std::get<1>(k)) << 1);
    }
};

class OrderBook
{
public:
    OrderBook()
    {
        quantityMap[std::make_tuple(100.0, Side::Buy)] = 10;
    }

    int GetVolumeAtPrice(double price, Side::Enum side)
    {
        if(quantityMap.count(std::make_tuple(price, side)) > 0)
            return quantityMap[std::make_tuple(price, side)];
        return 0;
    }

    void addOrderToBook(Order order)
    {
        
        if(queueMap.count(std::make_tuple(order.getPrice(), order.getSide())) <= 0)
        {
            Node* newNode = new Node();
            newNode->data = order.getOrderId();
            queueMap[std::make_tuple(order.getPrice(), order.getSide())] = *newNode;
            quantityMap[std::make_tuple(order.getPrice(), order.getSide())] = order.getQuantity();
        }
        else
        {
            Node* head = &queueMap.at(std::make_tuple(order.getPrice(), order.getSide()));
            insertEnd(&head, order.getOrderId());
            quantityMap[std::make_tuple(order.getPrice(), order.getSide())] += order.getQuantity();
        }

    }

    void addOrderToBook(Order order, std::priority_queue<std::tuple<double, int>, std::vector<std::tuple<double, int>>, ComparePrice2> book)
    {
        if(queueMap.count(std::make_tuple(order.getPrice(), order.getSide())) <= 0)
        {
            Node* newNode = new Node();
            newNode->data = order.getOrderId();
            queueMap[std::make_tuple(order.getPrice(), order.getSide())] = *newNode;
            quantityMap[std::make_tuple(order.getPrice(), order.getSide())] = order.getQuantity();
        }
        else
        {
            Node* head = &queueMap.at(std::make_tuple(order.getPrice(), order.getSide()));
            insertEnd(&head, order.getOrderId());
            quantityMap[std::make_tuple(order.getPrice(), order.getSide())] += order.getQuantity();
        }
    }

    void CancelOrder(int orderId)
    {
        if(orderMap.count(orderId) > 0){
            Order order = orderMap[orderId];
            Node* head = &queueMap.at(std::make_tuple(order.getPrice(), order.getSide()));
            deleteNode(&head, head);
            auto sameBook = bestBid;
            if(head == NULL){
                if(order.getSide() == Side::Buy)
                {
                    auto sameBook = bestBid;
                }
                else
                {
                    auto sameBook = bestAsk;
                }
                sameBook.pop();
            }
            quantityMap[std::make_tuple(order.getPrice(), order.getSide())] -= order.getQuantity();
            orderMap.erase(orderId);
        }
         
    }

    void PlaceOrder(Order order)
    {
        orderMap[order.getOrderId()] = order;
        auto oppositeBook = bestAsk;
        auto sameBook = bestBid;
        if (order.getSide() == Side::Buy)
        {
            std::priority_queue<std::tuple<double, int>, std::vector<std::tuple<double, int>>, ComparePrice> oppositeBook = bestAsk;
        }
        else
        {
            std::priority_queue<std::tuple<double, int>, std::vector<std::tuple<double, int>>, ComparePrice2> oppositeBook = bestBid;
        }

        if (order.getSide() == Side::Buy)
        {
            std::priority_queue<std::tuple<double, int>, std::vector<std::tuple<double, int>>, ComparePrice2> sameBook = bestBid;
        }
        else
        {
            std::priority_queue<std::tuple<double, int>, std::vector<std::tuple<double, int>>, ComparePrice> sameBook = bestAsk;
        }

        while(order.getQuantity()>0 && oppositeBook.size()>0 && order.getPrice() >= std::get<0>(oppositeBook.top()))
        {
            auto otherOrder = oppositeBook.top();
            double tradePrice = std::get<0>(otherOrder);
            int tradeQuantity = std::min(orderMap.at(std::get<1>(otherOrder)).getQuantity(), order.getQuantity());


            Side::Enum oppositeSide = orderMap.at(std::get<1>(otherOrder)).getSide();
            orderMap.at(std::get<1>(otherOrder)).Fill(tradeQuantity);
            order.Fill(tradeQuantity);
            std::cout<<"Trade by "<<orderMap.at(std::get<1>(otherOrder)).getClient()<<"taken by "<<order.getClient()<<", "<< tradeQuantity<< " shares @ "<<tradePrice<<std::endl;
            if(orderMap.at(std::get<1>(otherOrder)).isFilled())
            {
                CancelOrder(std::get<1>(otherOrder));
            }
        if(order.getQuantity() > 0){
            addOrderToBook(order);
        }
            // if(order.getPrice() >= tradePrice)
            // {
            //     int oppositeQuantity = quantityMap[std::make_tuple(tradePrice, oppositeSide)];
            //     if(order.getQuantity() >= oppositeQuantity)
            //     {
            //         order.Fill(oppositeQuantity);
            //         quantityMap[std::make_tuple(oppositePrice, oppositeSide)] = 0;
            //         oppositeBook.pop();
            //     }
            //     else
            //     {
            //         quantityMap[std::make_tuple(oppositePrice, oppositeSide)] -= order.getQuantity();
            //         order.Fill(order.getQuantity());
            //     }
            // }
            // else
            // {
            //     break;
            // }
        }

        


    }
    void print()
    {
        for(auto it = quantityMap.begin(); it != quantityMap.end(); it++)
        {
            std::cout<<std::get<0>(it->first)<<" "<<std::get<1>(it->first)<<" "<<it->second<<std::endl;
        }
    }

private:
    std::priority_queue<std::tuple<double, int>, std::vector<std::tuple<double, int>>, ComparePrice> bestAsk;
    std::priority_queue<std::tuple<double, int>, std::vector<std::tuple<double, int>>, ComparePrice2> bestBid;
    std::unordered_map<int, Order> orderMap;
    std::unordered_map<std::tuple<double, Side::Enum>, int, MyHash> quantityMap;
    std::unordered_map<std::tuple<double, Side::Enum>, Node, MyHash> queueMap;
};