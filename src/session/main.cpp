#include <cstdio>
#include "hello/Hello.hpp"

void hello(const Glib::RefPtr<Gio::DBus::Connection>& connection,
            const Glib::ustring& sender,
            const Glib::ustring& objectPath,
            const Glib::ustring& interfaceName,
            const Glib::ustring& methodName,
            const Glib::VariantContainerBase& args,
            const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation)
{
    const auto var = Glib::Variant<Glib::ustring>::create("hello world");
    Glib::VariantContainerBase response = Glib::VariantContainerBase::create_tuple(var);
    invocation->return_value(response);
}

int main()
{
    Gio::init();

    Hello hello;

    auto loop = Glib::MainLoop::create();
    loop->run();
}