#include <dirent.h>
#include <cstring>
#include <string>

#include "files.hpp"
#include "messages.hpp"

#include <cstdio>

namespace VPanic {
	
	bool find_files(const char* t_path, std::vector<File>* t_out) {
		if(t_out == nullptr) { return false; }
		const unsigned int path_length = strlen(t_path);
		if(path_length <= 0 || t_path == nullptr) {
			message("find_files() : \"no directory specified\"", MsgType::WARNING);
			return false;
		}

		std::string path_str = std::string(t_path, path_length);
		if(path_str.back() != '/') {
			path_str.push_back('/');
		}

		struct dirent *dir = nullptr;
		DIR* d = opendir(t_path);
		if(d) {
			while((dir = readdir(d)) != nullptr) {
				if(dir->d_type == DT_REG) {
					t_out->push_back(File { dir->d_name, path_str + dir->d_name });
				}
			}
			return true;
		}
		return false;
	}

}

