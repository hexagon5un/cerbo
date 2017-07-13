#include <assert.h>
#include <cfenv>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <string>
#include <vector>
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>

#include <supo.hpp>

#include "args.h"
#include "common.h"
#include "oven.h"
#include "tests.h"
#include <supo_general.hpp>
#include "show.h"


using namespace std;
using namespace supo;

// http://www.linuxquestions.org/questions/programming-9/deleting-a-directory-using-c-in-linux-248696/
// remove directory recursively
// include dirent.h sys/types.h
int rmdir(const char *dirname)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    if (path == NULL) {
        fprintf(stderr, "Out of memory error\n");
        return 0;
    }
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("Error opendir()");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(path, (size_t) PATH_MAX, "%s/%s", dirname, entry->d_name);
            if (entry->d_type == DT_DIR) {
                rmdir(path);
            }

	    remove(path);
        }

    }
    closedir(dir);
    remove(dirname);

    return 1;
}

int rmdir(const string& dirname)
{
	return rmdir(dirname.c_str());
}

void clean()
{
	rmdir(sndir(1));
	rmdir(sndir(2));
	rmdir(sndir(3));
}


/* Perform user preprocessing
 *
 * It doesn't do much, but it has been separated out to aid profiling
 */
void preprocess(const char* command)
{
	supo::ssystem(command, true);
}


int 
main(int argc, char *argv[])
{
	feenableexcept(FE_OVERFLOW);
	const vm_t vm = parse_args(argc, argv);

	if(vm.count("clean")) clean();

	if(vm.count("pre")>0) {
		string pre = vm.at("pre");
		preprocess(pre.c_str());
	}

	string wiegley_str = vm.at("wiegley");
	bool do_wiegley = wiegley_str == "on";
	if(! do_wiegley && wiegley_str != "off") {
		cerr << "ERR: Option wiegley error. Must be on|off, but given`"
			<< wiegley_str 
			<< "'. Continuing anyway." << endl;
	}

	oven ove;
	ove.m_vm = vm;
	ove.load_inputs();
	if(vm.at("snap") == "on") ove.fetch();
	ove.process(do_wiegley);

	//supo::ssystem("mcacc-reports.sh", true);
	if(vm.count("show") > 0) show(vm.at("show"));
	return EXIT_SUCCESS;
}
