#include <cstdio>
#include "hello/Hello.hpp"
#include "mime/Mime.hpp"

int main()
{
    Gio::init();

    Hello hello;
    Mime mime;

    auto loop = Glib::MainLoop::create();
    loop->run();
}