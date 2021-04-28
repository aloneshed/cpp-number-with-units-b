#include <iostream>
// #include <string>

namespace ariel
{
    class NumberWithUnits
    {
    private:
        double _number;
        std::string _unit;

    public:
        NumberWithUnits(double number, const std::string &unit);
        static void read_units(std::ifstream &units_file);
        friend int compare(const NumberWithUnits &A, const NumberWithUnits &B);
        double get_number() const { return _number; }
        std::string get_unit() const { return _unit; }

        NumberWithUnits operator+(const NumberWithUnits &other);
        NumberWithUnits &operator+=(const NumberWithUnits &other);
        NumberWithUnits operator+();

        NumberWithUnits operator-(const NumberWithUnits &other);
        NumberWithUnits &operator-=(const NumberWithUnits &other);
        NumberWithUnits operator-();

        friend bool operator==(const NumberWithUnits &A, const NumberWithUnits &B);
        friend bool operator!=(const NumberWithUnits &A, const NumberWithUnits &B);
        friend bool operator>(const NumberWithUnits &A, const NumberWithUnits &B);
        friend bool operator>=(const NumberWithUnits &A, const NumberWithUnits &B);
        friend bool operator<(const NumberWithUnits &A, const NumberWithUnits &B);
        friend bool operator<=(const NumberWithUnits &A, const NumberWithUnits &B);

        NumberWithUnits &operator++();        //prefix
        NumberWithUnits operator++(int flag); //postfix
        NumberWithUnits &operator--();        //prefix
        NumberWithUnits operator--(int flag); //postfix

        friend NumberWithUnits operator*(double size, const NumberWithUnits &A);
        friend NumberWithUnits operator*(const NumberWithUnits &A, double size);

        friend std::istream &operator>>(std::istream &is, NumberWithUnits &A);
        friend std::ostream &operator<<(std::ostream &os, const NumberWithUnits &A);
    };

}