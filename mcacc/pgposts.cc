#include <fstream>

#include "common.h"
#include "pgposts.h"
#include "types.h"

using namespace std;

void make_pgposts(const etran_cs& the_etrans, const ntran_ts& the_ntrans)
{
	string line;
	strings fields;

	string fname = s3("pgposts.csv");
	ofstream ofs;
	ofs.open(fname.c_str(), ofstream::out);
	auto out = [&](const strings& strs) { 
		ofs << supo::intercalate(",", strs) << endl; 
	};

	for(const auto& n:the_ntrans) {
		out({ n.dstamp, n.dr, n.amount.stra(), n.desc });
		out({ n.dstamp, n.cr, "-"s + n.amount.stra(), n.desc });
	}


	ofs.close();

}
