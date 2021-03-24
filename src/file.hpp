#pragma once
#include <vector>
#include <string>
#include <functional>


namespace vpanic {

	struct File {
		const std::string name;
		const std::string fullpath;
	};

	void find_all(const char* t_directory, std::vector<File>* t_out, const int t_settings = 0);
	bool read_file_lines(const char* t_filename, std::function<void(const std::string& line)> t_lmbd);
	bool read_file(const char* t_filename, std::string* t_out);

}

