#include <iostream>
#include <fstream>
#include <thread>
#include <algorithm>
#include <sstream>
#include <vector>
#include <queue>
#include <iterator>

#define CHUNK_SIZE 1024 * 1024 // * sizeof(uint64_t) = 8 MB
using namespace std;

template <class MinHeap, class Out>
void mergek(MinHeap& heap, Out& out)
{
    while(!heap.empty()) {
        auto range = heap.top(); // pop minimum in O(1)
        heap.pop();
        auto n = *range.first++; // output the minimum, move input
        out.write((char *)(&n), sizeof(uint64_t));
        if (range.first != range.second) // push unless empty range
            heap.emplace(range); // find new minimum in O(log k)
    }
}

int main() {
	
	//create random input file
    cout << "creating input file...\n";
    auto *buf = new uint64_t[CHUNK_SIZE];
    ofstream out1("input", ios::binary);
    for (int i = 0; i < 1000; ++i) { //try 10000000 for many chunks, but comment chunk, input, output file prints
        auto *n = new uint64_t(std::rand() % 100);
        out1.write((char *)(n), sizeof(uint64_t));
    }
    out1.close();
	
	//print input file 
    cout << "input:\n";
    ifstream in("input", ios::binary);
    while (in) {
        uint64_t n;
        in.read((char *)(&n), sizeof(uint64_t));
        cout << n << ' ';
    }
    cout << "\n\n";
    in.close();
	//
	
    in.open("input", ios::binary);
    size_t num_files = 0;
    vector<string> name;
    while (in) {
        in.read((char *)(buf), CHUNK_SIZE);
        auto size_read = in.gcount();
		cout << "reading chunk...\n";
        if (size_read > 0) {
            for (int i = 0; i < 2; ++i) {
                stringstream s;
                s << num_files;
                name.emplace_back();
                s >> name[num_files++];
            }
            std::thread t1([buf, size_read]() { sort(buf, buf + (size_read / (2 * sizeof(uint64_t)))); });
            std::thread t2([buf, size_read]() {
                sort(buf + size_read / (2 * sizeof(uint64_t)),
                     buf + size_read / sizeof(uint64_t));
            });
            t1.join();
            t2.join();
			//print chunk
			cout << "unmerged chunk:\n";
            for (size_t i = 0; i < size_read / sizeof(uint64_t); ++i) {
                cout << buf[i] << ' ';
            }
            cout << "\n";
			//
            ofstream out;
            out.open(name[num_files - 2]);
            for (size_t i = 0; i < size_read / (2 * sizeof(uint64_t)); ++i) {
                out << buf[i] << ' ';
            }
            out.close();
            out.open(name[num_files - 1]);
            for (size_t i = size_read / (2 * sizeof(uint64_t)); i < size_read / sizeof(uint64_t); ++i)  {
                out << buf[i] << ' ';
            }
            out.close();
        }
    }
    in.close();
    delete[] buf;
    ofstream out("output", ios::binary);
    vector <ifstream> files (num_files);
    // merge sorted files
    typedef istream_iterator<uint64_t> It;
    // the heap contains pairs of iterators over files: (current_position, eof)
    typedef pair<It, It> Range;
    auto less_first = [](const Range& lhs, const Range& rhs) {
        return *lhs.first > *rhs.first;
    };
    priority_queue<Range, vector<Range>, decltype(less_first)> heap(less_first);
    auto cur_num = num_files;
    while (cur_num--) {
        files[cur_num].open(name[cur_num]);
        heap.emplace(It(files[cur_num]), It());
    }
    mergek(heap, out);
    for(size_t i = 0; i < num_files; ++i) {
        files[i].close();
        remove(name[i].c_str());
    }
    out.close();
	
	//print output file
    in.open("output");
    cout << "\noutput:\n";
    while(in) {
        uint64_t n;
        in.read((char *)(&n), sizeof(uint64_t));
        cout << n << ' ';
    }
    in.close();
	//
	
    cout << "\n\ndone.\n";
    return 0;
}