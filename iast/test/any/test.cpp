#include "../../any.hpp"

#include <iostream>
#include <memory>
using namespace std;

class G
    {
public:
    ~G() {cout << "Im done" << endl;}
    };

int main()
    {
    Any a;

    a = make_shared<G>();

    shared_ptr<G> b = a.getAs< shared_ptr<G> >();

    return 0;
    }
