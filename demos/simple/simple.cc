#include "Courier.h"
#include "Subscriber.h"

#include <cstdio>

using namespace courier;

struct Foo
{
    int a;
    float b;

    Foo (int a, float b)
        : a{a}, b{b} {}
};

struct Bar
{
    double x;

    Bar (double x)
        : x{x} {}
};

class MySubscriber : public Subscriber<Foo, Bar>
{
public:

    void onEvent (const Foo* foo) override
    {
        printf("Received foo: a %d, b %f\n", foo->a, foo->b);
    }

    void onEvent (const Bar* bar)
    {
        printf("Received bar: x %f\n", bar->x);
    }
};

int main ()
{
    Courier courier;

    MySubscriber sub;
    courier.subscribe<Foo>(&sub);
    courier.subscribe<Bar>(&sub);

    courier.send(Foo(2, 5.0f));
    courier.send(Bar(7.0));

    return 0;
}
