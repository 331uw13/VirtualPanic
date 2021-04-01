#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <libgen.h>
#include <dirent.h>
#include <cstring>
#include <fstream>

#include "file.hpp"
#include "messages.hpp"
#include "settings.hpp"


namespace vpanic {
	
	void find_all(const char* t_directory, std::vector<File>& t_out, const int t_settings) {
		struct dirent* dir;
		DIR* d = opendir(t_directory);
		if(!d) { 
			message(MSG_ERROR, "Failed to open directory \"%s\"", t_directory);
			return;
	   	}
		std::string full = t_directory;	
		if(full.back() != 0x2F) {
			full.push_back(0x2F);
		}

		while((dir = readdir(d)) != NULL) {
			if(dir->d_type == DT_REG || t_settings == INCLUDE_SUB_DIRECTORIES) {
				t_out.push_back(File { dir->d_name, full+dir->d_name });
			}
		}
	}

	bool read_file_lines(const char* t_filename, std::function<void(const std::string& line)> t_lmbd) {
		std::fstream f(t_filename, std::fstream::in);
		if(!f.is_open()) {
			message(MSG_ERROR, "Failed to open file \"%s\"", t_filename);
			return false;
		}
		std::string l;
		while(getline(f, l)) { t_lmbd(l); }
		f.close();
		return true;
	}

	bool read_file(const char* t_filename, std::string& t_out) {
		struct stat buf;
		const int fd = open(t_filename, O_RDONLY);
		if(fd < 0) {
			message(MSG_ERROR, "[%i] Failed to open file \"%s\"", fd, t_filename);
			perror("open()");
			return false;
		}

		const int fs = fstat(fd, &buf);
		if(fs < 0) {
			message(MSG_ERROR, "[%i] Failed to get information about file \"%s\"", fs, t_filename);
			perror("stat()");
			close(fd);
			return false;
		}
		
		const size_t length = buf.st_size;
		if(length == 0) {
			message(MSG_WARNING, "File is empty or failed to get file length from \"%s\"", t_filename);
			return false;
		}

		char* addr = nullptr;
		addr = (char*)mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
		
		if(addr == nullptr) {
			close(fd);
			message(MSG_ERROR, "Failed to map file \"%s\"", t_filename);
			return false;
		}
	
		t_out.append(addr);
		munmap(addr, length);
		close(fd);
		return true;
	}

}


