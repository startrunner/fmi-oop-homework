/////////////////////////////////////////////////////////////
//
// Author: Александър Иванов Иванов
// FN: 81841
// Date: 27.03.2019
//
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <algorithm>

char *reallocate(const char *str)
{
    char *reallocated = new char[strlen(str) + 1];
    strcpy(reallocated, str);
    return reallocated;
}

template<typename T> class ArrayList
{
    int itemCount = 0;
    int capacity = 0;
    T *buffer = nullptr;
public:

    ArrayList(int initialCapacity)
    {
        if (initialCapacity != 0)
        {
            buffer = new T[initialCapacity]();
            capacity = initialCapacity;
        }
    }
    ArrayList() :ArrayList<T>(0) {}

    ArrayList(const ArrayList &toCopy) { assign(toCopy); }

    ArrayList<T> &operator = (const ArrayList<T> &toCopy)
    {
        destroy();
        assign(toCopy);
        return *this;
    }

    ~ArrayList() { destroy(); }

    int count() const { return itemCount; }

    T& operator [] (int index)const
    {
        checkIndex(index);
        return buffer[index];
    }

    void add(const T &item)
    {
        makeSpace();
        buffer[itemCount++] = item;
    }

private:

    void checkIndex(int index)
    {
        if (index < 0 || index >= itemCount) throw std::runtime_error("nada");
    }

    void makeSpace()
    {
        if (itemCount < capacity)return;

        T *newBuffer = new T[capacity * 2]();

        for (int i = 0; i < itemCount; i++)newBuffer[i] = buffer[i];
        delete[] buffer;

        buffer = newBuffer;
        capacity *= 2;
    }

    void assign(const ArrayList<T> &toCopy)
    {
        capacity = toCopy.capacity;
        itemCount = toCopy.itemCount;
        buffer = new T[toCopy.capacity];
        for (int i = 0; i < itemCount; i++)buffer[i] = toCopy.buffer[i];
    }

    void destroy()
    {
        if (buffer != nullptr)delete[] buffer;
    }
};

class Measurement {
    char const* location;
    char date[9]; //DD_MM_YY + zero
    double temperatureC;

public:
    Measurement(const Measurement &toCopy) { assign(toCopy); }

    Measurement(char const* location, char const* date, double temperature)
    {
        this->location = reallocate(location);
        strcpy(this->date, date);
        temperatureC = temperature;
    }

    Measurement() :Measurement("Sofia", "01/01/1999", 12) {}

    Measurement& operator = (const Measurement &toAssign)
    {
        destroy();
        assign(toAssign);
        return *this;
    }

    ~Measurement() { destroy(); }
    // Ако е необходимо: деструктор, конструктор за копиране, оператор за присвояване

    char const* getLocation() const { return location; }
    char const* getDate() const { return date; }
    double getTemperature() const { return temperatureC; }

private:
    void assign(const Measurement &other)
    {
        location = reallocate(other.location);
        strcpy(date, other.date);
        temperatureC = other.temperatureC;
    }

    void destroy()
    {
        if (location != nullptr)delete[] location;
    }
};

class MaxTemperatures {
    ArrayList<Measurement> measurements;
    double temperatureSum = 0;
    Measurement mHighestMeasurement;

public:
    MaxTemperatures(unsigned N) : measurements(N) {}
    MaxTemperatures() :MaxTemperatures(32) {}

    void addMeasurement(Measurement const& measurement)
    {
        temperatureSum += measurement.getTemperature();
        if (measurements.count() == 0 || mHighestMeasurement.getTemperature() < measurement.getTemperature())
        {
            mHighestMeasurement = measurement;
        }
        measurements.add(measurement);

    }

    double findAverage() const { return temperatureSum / measurements.count(); }

    Measurement highestMeasurement() const { return mHighestMeasurement; }
};

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include "var3-tests.h"

int main() 
{
    doctest::Context().run();
    std::cin.get();

    return 0;
}
