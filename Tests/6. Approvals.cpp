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

		{
			std::ifstream file(received);
			std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                        cerr << str << endl;
		}
		{
			std::ifstream file(approved);
			std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                        cerr << str << endl;
		}
                return true;
	}
};

TEST("TestNameMayNotContainsSpaces") { Approvals::verify("foo", ConsoleDiffReporter()); }
