#pragma once
#include <vector>
#include <string>

// TODO: add writing files and reading them!

namespace VPanic {


	struct File {
		const std::string name;
		const std::string fullpath;
	};

	bool find_files(const char* t_path, std::vector<File>* t_out);
	// TODO: bool find_files_with_extension(
	//		const char* t_path, const char* t_ext, std::vector<File>* t_out);

}


