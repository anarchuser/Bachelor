#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <array>

using namespace std;

vector <string> const headers     {"protocol", "peers", "duration", "frequency", "average", "maximum"};
vector <string> const protocols   {"naive", "voting"};
vector <int>         const peers       { 5, 10 };
vector <int>         const durations   { 10, 60 };
vector <int>         const frequencies { 1, 30, 60 };

string build_cmd (string const & prgm, string const & protocol, int peer, int duration, int frequency);

int main (int argc, char * argv[]) {
    if (argc != 3) return EXIT_FAILURE;
    string prgm (argv[1]);
    bool writeHeader = !filesystem::exists (argv[2]);

    ofstream file;
    file.open (argv[2], ios_base::app);
    if (writeHeader) {
        for (auto const & header : headers) file << header << '\t';
        file << '\n';
    }

    array <char, 128> buffer;
    for (auto const & protocol : protocols) {
        for (auto peer : peers) {
            for (auto duration : durations) {
                for (auto frequency : frequencies) {
                    file << protocol << '\t' << peer << '\t' << duration << '\t' << frequency << '\t';
                    auto cmd = build_cmd (prgm, protocol, peer, duration, frequency);
                    (std::cout << cmd << "\t>> ").flush();
                    unique_ptr<FILE, decltype (& pclose)> pipe (popen (cmd.c_str(), "r"), pclose);
                    if (!pipe) throw runtime_error ("popen() failed!");
                    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
                        file << buffer.data();
                        std::cout << buffer.data();
                    }
                    std::cout.flush();
                }
            }
        }
    }
}

string build_cmd (string const & prgm, string const & protocol, int peer, int duration, int frequency) {
    std::stringstream ss;
    ss << prgm;
    ss << " -t " << protocol;
    ss << " -p " << peer;
    ss << " -d " << duration;
    ss << " -f " << frequency;
    ss << " -m " << 0;
    return ss.str();
}
