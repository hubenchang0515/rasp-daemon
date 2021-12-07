#include <cstdio>
#include "hello/Hello.h"

using namespace Raspd;

int main()
{
    Gio::init();

    Hello hello;

    auto loop = Glib::MainLoop::create();
    loop->run();
}