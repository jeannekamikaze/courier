#include "BufferedCourier.h"
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

    void onEvent (const Bar* bar) override
    {
        printf("Received bar: x %f\n", bar->x);
    }
};

int main ()
{
    BufferedCourier courier;

    MySubscriber sub;
    courier.subscribe<Foo>(&sub);
    courier.subscribe<Bar>(&sub);

    courier.send(new Foo(2, 5.0f));
    courier.send(new Bar(7.0));

    courier.dispatch();

    return 0;
}
