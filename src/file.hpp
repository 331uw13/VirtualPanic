#pragma once
#include <vector>
#include <string>


namespace vpanic {

	struct File {
		const std::string name;
		const std::string fullpath;
	};

	void find_all(const char* t_directory, std::vector<File>* t_out, const int t_settings = 0);


}



