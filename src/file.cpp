#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <libgen.h>
#include <dirent.h>
#include <cstring>

#include "file.hpp"
#include "messages.hpp"
#include "settings.hpp"

namespace vpanic {
	
	void find_all(const char* t_directory, std::vector<File>* t_out, const int t_settings) {
		if(t_out == nullptr) { return; }
		struct dirent* dir;
		DIR* d = opendir(t_directory);
		if(!d) { 
			message(MType::ERROR, "Cannot open directory \"%s\"", t_directory);
			return;
	   	}

		std::string full = t_directory;	
		if(full.back() != 0x2F) {
			full.push_back(0x2F);
		}

		while((dir = readdir(d)) != NULL) {
			if(dir->d_type == DT_REG || t_settings == INCLUDE_SUB_DIRECTORIES) {
				t_out->push_back(File { dir->d_name, [dir, full]() { return full+dir->d_name; }() });
			}
		}
	}

}
