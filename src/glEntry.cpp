#include "glEventChannel.h"
#include "glTaskManager.h"
#include "glEventQueue.h"
#include "glModule.h"

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <string>

#ifdef _WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#elif __unix
#	include <dlfcn.h>
#endif

const std::string VERSION = "0.0.1";
const std::string DEFAULT_DEBUG = "debug.log";
const std::string DEFAULT_INI = "module_configuration.ini";
const int NUMTHREAD = 1;

// TODO: Just make it return an int, take modules by reference
std::vector<std::unique_ptr<glModule>> loadModules(const std::string &modules_ini_path,
												   boost::property_tree::ptree &pt);
void startupModules(std::vector<std::unique_ptr<glModule>> &modules);
void cleanupModules(std::vector<std::unique_ptr<glModule>> &modules);

bool handleCommandLineArguments(int argc, char **argv, boost::program_options::variables_map &vm);

int main(int argc, char **argv) {

	std::ios_base::sync_with_stdio(false);

	// populate variable map with command line arguments
	boost::program_options::variables_map vm;
	if(handleCommandLineArguments(argc, argv, vm) == false)
		return 1;

	// dynamically load objects from modules folder
	boost::property_tree::ptree pt;
	std::vector<std::unique_ptr<glModule>> modules =
		loadModules(vm["module_ini"].as<std::string>(), pt);
	if(modules.empty())
		return 1;

	// global classes must interact at this level
	glTaskManager manager(glModule::channel);
	glEventQueue queue(glModule::channel);

	startupModules(modules);
	// TODO: spawn appropriate number of threads?
	manager.sendStartSignal();
	while(!manager.shouldClose()) {
		manager.pollEvents();
		// TODO: regulate to keep frame rate at 60 FPS?
		// NOTE: make this the glVideo module's job
		// so that only glVideo's latency suffers
		// rather than the main loop's!
	}
	cleanupModules(modules);

	return 0;
}

bool handleCommandLineArguments(int argc, char **argv, boost::program_options::variables_map &vm) {
	try {
		namespace po = boost::program_options;
		po::options_description description("Command line options");
		description.add_options()
			("help,h", "lists options")
			("version,V", "current version of Toy Engine")
			("num_threads,t", po::value<int>()->default_value(NUMTHREAD),
				"set number of threads")
			("debug_log,D", po::value<std::string>()->default_value(DEFAULT_DEBUG),
				"set location of debug log")
			("module_ini,I", po::value<std::string>()->default_value(DEFAULT_INI),
				"set path to the module configuration file")
		;

		po::store(po::parse_command_line(argc, argv, description), vm);
		if(vm.count("help")) {
			std::cout << description << std::endl;
			return false;
		} else if(vm.count("version")) {
			std::cout << "Toy Engine version " << VERSION << std::endl;
			return false;
		}
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
		return false;
	} catch(...) {
		std::cerr << "Unknown Failure" << std::endl;
		return false;
	}

	return true;
}

std::vector<std::unique_ptr<glModule>> loadModules(const std::string &modules_ini_path,
												   boost::property_tree::ptree &pt) {
	std::vector<std::unique_ptr<glModule>> modules;
	std::vector<std::string> module_classes;
	std::string module_dl_path;

	try {
		boost::property_tree::ini_parser::read_ini(modules_ini_path, pt);
		module_dl_path = pt.get<std::string>("module_import.module_lib_path");

		std::stringstream ss;
		ss << pt.get<std::string>("module_import.module_classes");
		std::string temp;
		while(std::getline(ss, temp, ','))
			module_classes.push_back(std::move(temp));
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
		return modules;
	}

	typedef glModule * (*export_module_func_t)(void);

#ifdef _WIN32
	HMODULE modules_library = LoadLibrary((LPCTSTR) module_dl_path.c_str());
	if(modules_library == NULL) {
		std::cerr << "Could not find " << module_dl_path << std::endl;
		return modules;
	}
	for(unsigned int i = 0; i < module_classes.size(); ++i) {
		FARPROC export_module_func =
			GetProcAddress(modules_library, (LPCSTR) OBTAIN_MODULE(module_classes[i]).c_str());
		if(export_module_func == NULL) {
			std::cerr << "Could not export module " << module_classes[i] << std::endl;
			modules.clear();
			return modules;
		}
		modules.push_back(
			std::unique_ptr<glModule>(
				((export_module_func_t) export_module_func)()
			)
		);
	}
#elif __unix
	void *modules_library = dlopen(module_dl_path.c_str(), RTLD_LAZY);
	if(modules_library == NULL) {
		std::cerr << "Could not find " << module_dl_path << std::endl;
		return modules;
	}
	for(unsigned int i = 0; i < module_classes.size(); ++i) {
		void *export_module_func =
			dlsym(modules_library, OBTAIN_MODULE(module_classes[i]).c_str());
		if(export_module_func == NULL) {
			std::cerr << "Could not find " << OBTAIN_MODULE(module_classes[i]) << std::endl;
			modules.clear();
			return modules;
		}
		modules.push_back(
			std::unique_ptr<glModule>(
				((export_module_func_t) export_module_func)()
			)
		);
	}
#else
	std::cerr << "Unsupported platform" << std::endl;
#endif

	return modules;
}

void startupModules(std::vector<std::unique_ptr<glModule>> &modules) {
	for(auto &module : modules)
		module->startup();
}

void cleanupModules(std::vector<std::unique_ptr<glModule>> &modules) {
	for(auto &module : modules)
		module->cleanup();
}
