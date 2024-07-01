#pragma once

#include <iostream>
#include <string>

/**
 * This file has Side, Date and Time, and Order classes.
 */

struct Side
{
    enum Enum
    {
        Buy,
        Sell
    };
};

class DateTime
{
public:
    DateTime() { }
    DateTime(int year, int month, int day, int hour, int minute, int second)
        : year(year), month(month), day(day), hour(hour), minute(minute), second(second)
    {
    }

    bool operator<(const DateTime& other) const
    {
        if (year < other.year)
            return true;
        if (year > other.year)
            return false;

        if (month < other.month)
            return true;
        if (month > other.month)
            return false;

        if (day < other.day)
            return true;
        if (day > other.day)
            return false;

        if (hour < other.hour)
            return true;
        if (hour > other.hour)
            return false;

        if (minute < other.minute)
            return true;
        if (minute > other.minute)
            return false;

        if (second < other.second)
            return true;
        if (second > other.second)
            return false;

        return false;
    }

private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};

class Order
{
public:
    Order() { }
    Order(int orderId, DateTime timestamp, Side::Enum side, double price, int initialQuantity, std::string client)
        
    {
        orderId_ = orderId;
        timestamp_ = timestamp;
        side_ = side;
        price_ = price;
        quantity_ = initialQuantity;
        client_ = client;
    }

    int getOrderId() const
    {
        return orderId_;
    }

    DateTime getTimestamp() const
    {
        return timestamp_;
    }

    Side::Enum getSide() const
    {
        return side_;
    }

    double getPrice() const
    {
        return price_;
    }

    int getQuantity() const
    {
        return quantity_;
    }

    std::string getClient() const
    {
         return client_;
    }

    bool isFilled() const
    {
        return quantity_ == 0;
    }

    void Fill(int quantity)
    {
        if(quantity > quantity_)
        {
            throw std::runtime_error("Not enough quantity in order");
        }

        quantity_ -= quantity;
    }

private:
    int orderId_;
    DateTime timestamp_;
    Side::Enum side_;
    double price_;
    int quantity_;
    std::string client_;
};