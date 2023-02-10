#include <embedded/lua/lvm.h>
#include <embedded/js/jsvm.h>
#include <embedded/ruby/rvm.h>
#include <embedded/python/pvm.h>

int main()
{
    python_vm pvm;
    pvm.preload.push_back("print(\" hello world !\")");
    pvm.initialize();

    /*lua_vm lvm;
    lvm.preload_trunks = {
        "scripts\\lua\\Main.lua"
    };

    lvm.init();
    lvm.tick();*/

}