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
		return DiffReporter().Report(received, approved);
	}
};

TEST("TestNameMayNotContainsSpaces") { Approvals::verify("foo", ConsoleDiffReporter()); }
