#include "log.hh"

void status(std::ostream& out, const string& name, bool success)
{
	out
		<< name
		<< ": "
		<< (success ? "\033[32m" : "\033[91m")
		<< (success ? "ok" : "x")
		<< "\033[0m"
		<< "\n";
}
