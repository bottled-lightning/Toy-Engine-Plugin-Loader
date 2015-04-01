#ifndef GLPLUGINLOADER_H_
#define GLPLUGINLOADER_H_

class glModule;

#include <boost/program_options.hpp>
#include <vector>
#include <string>

template <template <typename, typename> class T>
class glPluginLoader {
public:
    typedef T<glModule *, std::allocator<glModule *>> containerT;
    
    // Calls each plugin's startup functions
    containerT loadModules(const std::string &modules_ini_path,
                           boost::program_options::variables_map &vm);
    // We call each plugin's individual cleanup functions
    // for client-side safety when working with modules.
    void deleteModules(containerT &modules);
private:
    containerT modules;
};

#endif
