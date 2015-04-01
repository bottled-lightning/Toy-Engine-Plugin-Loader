#include "glModule.h"

#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>

#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#elif __unix
#   include <dlfcn.h>
#endif

class glPluginLoader.h {
    public:
        std::vector<glModule*> loadModules(const std::string &modules_ini_path,
                                           boost::program_options::variables_map &vm);
        void deleteModules(std::vector<glModule*> &modules);
    private:
};
