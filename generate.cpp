#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <array>

using namespace std;

#define DEFAULT_ITERATIONS 20

vector <string> const headers     {"protocol", "peers", "duration", "frequency", "average", "maximum", "faults"};
vector <string> const protocols   {"naive", "voting"};
vector <int>    const peers       { 10 };
vector <int>    const durations   { 60 };
vector <int>    const frequencies { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70 };

string build_cmd (string const & prgm, string const & protocol, int peer, int duration, int frequency);

int main (int argc, char * argv[]) {
    if (argc < 3) return EXIT_FAILURE;
    string prgm (argv[1]);

    auto tsv_name = string (argv[2]) + ".tsv";
    auto data_name = string (argv[2]) + ".map";
    ofstream tsv, data;
    tsv.open (tsv_name, ios_base::app);
    data.open (data_name, ios_base::app);
    for (auto i = headers.begin(); i != headers.end();) tsv << * i << ((++i != headers.end()) ? '\t' : '\n');

    auto const iterations = argc == 4 ? std::stoi (argv[3]) : DEFAULT_ITERATIONS;
    array <char, 128> buffer;
    for (auto const & protocol : protocols) {
        for (auto peer : peers) {
            for (auto duration : durations) {
                for (auto frequency : frequencies) {
                    data << "#" << protocol << '\t' << peer << '\t' << duration << '\t' << frequency << '\n';
                    for (int i = 1; i <= iterations; i++) {
                        tsv << protocol << '\t' << peer << '\t' << duration << '\t' << frequency << '\t';
                        auto cmd = build_cmd (prgm, protocol, peer, duration, frequency);
                        (std::cout << i << "/" << iterations << ":\t" << cmd << "\t").flush();
                        unique_ptr<FILE, decltype (& pclose)> pipe (popen (cmd.c_str(), "r"), pclose);
                        if (!pipe) throw runtime_error ("popen() failed!");
                        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
                            tsv << buffer.data();
                            data << buffer.data();
                            std::cout << buffer.data();
                        }
                        cout << endl;
                        tsv << endl;
                        data << endl;
                    }
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
    ss << " -r ";
//    ss << " -a beelzebub ";
    return ss.str();
}
