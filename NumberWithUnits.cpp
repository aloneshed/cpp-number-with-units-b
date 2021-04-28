#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <string>
using namespace std;

#include "NumberWithUnits.hpp"
using namespace ariel;
const double EPSILON = 0.00001;

static map<string, map<string, double>> units;

NumberWithUnits::NumberWithUnits(double number, const string &unit): _number(number), _unit(unit){
    if(units.count(unit) == 0){
        throw invalid_argument{"Unit doesn't exist in the units list"};
    }
}

void update_units(string &unit1, string &unit2){
    for(auto &pair : units[unit2]) {
        double value = units[unit1][unit2] * pair.second;
        units[unit1][pair.first] = value;
        units[pair.first][unit1] = 1/value;
    }
}

void NumberWithUnits::read_units(ifstream &units_file){
    string line;
    string unit1, unit2, eq;
    double num1 = 0;
    double num2 = 0;
    while(getline(units_file, line))
    {
        istringstream ist(line);
        ist >> num1 >> unit1 >> eq >> num2 >> unit2;
        units[unit1][unit2] = num2;
        units[unit2][unit1] = 1/num2;
        update_units(unit1, unit2);
        update_units(unit2, unit1);
    }
}

double convert(const string& unit_from, const string& unit_to, double number){
    if(unit_from == unit_to){
        return number;
    }
    try{
        return units.at(unit_from).at(unit_to) * number;
    }
    catch(const exception& e){
        throw invalid_argument{"different dimensions, can not make convert from " + unit_from + " to " + unit_to};
    }
}


int ariel::compare(const NumberWithUnits& A, const NumberWithUnits& B){
    double converted_unit = convert(B.get_unit(), A.get_unit(), B.get_number());
    double expression1 = A.get_number() - converted_unit;
    double expression2 = -expression1;
    if(expression1 > EPSILON) {return 1;}
    if(expression2 > EPSILON) {return -1;}
    return 0;
}

NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits& other) {
    double converted = convert(other._unit, this->_unit, other._number);
    return NumberWithUnits(this->_number + converted, this->_unit);
}

NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& other) {
    double converted = convert(other._unit, this->_unit, other._number);
    this->_number += converted;
    return *this;
}

NumberWithUnits NumberWithUnits::operator+() {
    return *this;
}

NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits& other) {
    double converted = convert(other._unit, this->_unit, other._number);
    return NumberWithUnits(this->_number - converted, this->_unit);
}

NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits& other) {
    double converted = convert(other._unit, this->_unit, other._number);
    this->_number -= converted;
    return *this;
}

NumberWithUnits NumberWithUnits::operator-() {
    return NumberWithUnits(-this->_number, this->_unit);
}

bool ariel::operator==(const NumberWithUnits& A, const NumberWithUnits& B){
    return compare(A, B) == 0;
}

bool ariel::operator!=(const NumberWithUnits& A, const NumberWithUnits& B){
    return compare(A, B) != 0;
}

bool ariel::operator>(const NumberWithUnits& A, const NumberWithUnits& B){
    return compare(A, B) > 0;
}

bool ariel::operator>=(const NumberWithUnits& A, const NumberWithUnits& B){
    return compare(A, B) >= 0;
}

bool ariel::operator<(const NumberWithUnits& A, const NumberWithUnits& B){
    return compare(A, B) < 0;
}

bool ariel::operator<=(const NumberWithUnits& A, const NumberWithUnits& B){
    return compare(A, B) <= 0;
}

NumberWithUnits& NumberWithUnits::operator++(){ //prefix
    this->_number++;
    return *this;
} 

NumberWithUnits NumberWithUnits::operator++(int flag) { //postfix
    NumberWithUnits copy = *this;
    this->_number++;
    return copy;
}

NumberWithUnits& NumberWithUnits::operator--(){ //prefix
    this->_number--;
    return *this;
}

NumberWithUnits NumberWithUnits::operator--(int flag){ //postfix
    NumberWithUnits copy = *this;
    this->_number--;
    return copy;
}

NumberWithUnits ariel::operator*(double size, const NumberWithUnits& A){
    return NumberWithUnits(A._number * size, A._unit);
}

NumberWithUnits ariel::operator*(const NumberWithUnits& A, double size){
    return size * A;
}

istream& ariel::operator>>(istream& is, NumberWithUnits& A){
    string unit;
    char temp1 = 0;
    char temp2 = 0;
    double number = 0;
    is>>number>>temp1>>unit;
    if(unit.find(']')!=string::npos)//check if "]" is in the string
    {
        size_t index = unit.find(']');
        unit = unit.substr(0,index);
    }
    else{
        is>>temp2;
    } 
        
    if(units.count(unit)!=0){
        A._unit = unit;
        A._number = number;
    }
    else{
        throw invalid_argument {"unidentified unit."};
    }
    return is;
}  

ostream& ariel::operator<<(ostream& os, const NumberWithUnits& A){
    return os << A._number << "[" << A._unit << "]";
}
