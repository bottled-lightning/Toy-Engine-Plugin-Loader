#ifndef GLMODULE_H_
#define GLMODULE_H_

#include <string>

/* Simple API that will allow us to retrieve the  
 * modules from the shared library via pointer.
 * This opacity is safer.
 */
#define EXPORT_MODULE(module) \
    extern "C" glModule *export_##module() { return new module(#module); }
// note: module argument is a string
#define OBTAIN_MODULE(module) ("export_" + module)

#ifdef _WIN32
#define DEFINE_MODULE(module) \
    extern "C" __declspec(dllexport) glModule * __cdecl export_##module();
#elif __unix
#define DEFINE_MODULE(module) \
    extern "C" glModule *export_##module();
#endif

class glModule {
public:
    glModule(const char *name)
        : _module_name(name) {}
    virtual ~glModule() {}
    virtual void startup() { /* entering */ }
    virtual void cleanup() { /* exiting  */ }
    
    const std::string &getName();

private:
    std::string _module_name;
};

#endif
