#define APPROVALS_OKRA
#include "ApprovalTests.v.1.0.0.hpp"
using namespace std;

class ConsoleDiffReporter : public Reporter
{
	bool Report(std::string received, std::string approved) const override
	{
		cerr << received << " exists " << FileUtils::fileExists(received) << endl;
		cerr << approved << " exists " << FileUtils::fileExists(approved) << endl;
		cerr << received << " size " << FileUtils::fileSize(received) << endl;
		cerr << approved << " size " << FileUtils::fileSize(approved) << endl;

		std::ifstream filer(received);
		std::string str_recevied((std::istreambuf_iterator<char>(filer)), std::istreambuf_iterator<char>());
		cerr << str_recevied << endl;

		std::ifstream filea(approved);
		std::string str_accepted((std::istreambuf_iterator<char>(filea)), std::istreambuf_iterator<char>());
		cerr << str_accepted << endl;

		cerr << "equality" << (str_recevied != str_accepted) << endl;

		return true;
	}
};

TEST("TestNameMayNotContainsSpaces") { Approvals::verify("foo", ConsoleDiffReporter()); }
