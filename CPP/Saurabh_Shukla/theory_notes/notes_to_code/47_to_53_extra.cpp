/*
    ================================================================
    C++ MASTER (ADVANCED): Templates + Streams + RAII + mmap
    ================================================================

    Added:
    - RAII wrappers for files
    - Exception-safe file handling
    - mmap vs fstream comparison
    - Systems-level caveats

    This is now INTERVIEW READY for:
    Apple / Arm / Low-level systems roles
*/

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <cstring>

// POSIX for mmap
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

/* ============================================================
   1. FUNCTION TEMPLATE (same as before)
   ============================================================ */

template <typename T>
T big(T a, T b)
{
    return (a > b) ? a : b;
}

/* ============================================================
   2. RAII FILE WRAPPER (CRITICAL FOR INTERVIEWS)
   ============================================================

   Problem:
   - Manual open/close → resource leaks
   - Exception between open & close → file left open

   Solution:
   - RAII (Resource Acquisition Is Initialization)
   - Constructor acquires
   - Destructor releases
*/

class FileWriter
{
private:
    ofstream fout;

public:
    FileWriter(const string& filename, ios::openmode mode = ios::out)
    {
        fout.open(filename, mode);

        if (!fout.is_open())
            throw runtime_error("Failed to open file for writing");
    }

    /*
        Disable copy (important!)
        Avoid double close / shared ownership bugs
    */
    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    /*
        Move allowed (modern C++)
    */
    FileWriter(FileWriter&& other) noexcept
        : fout(std::move(other.fout)) {}

    void writeLine(const string& s)
    {
        fout << s << '\n';

        /*
            Exception safety:
            Check stream state after write
        */
        if (!fout)
            throw runtime_error("Write failed");
    }

    ~FileWriter()
    {
        // Destructor auto closes file
        // NEVER throw from destructor
        if (fout.is_open())
            fout.close();
    }
};


/* ============================================================
   3. RAII FILE READER
   ============================================================ */

class FileReader
{
private:
    ifstream fin;

public:
    FileReader(const string& filename)
    {
        fin.open(filename);

        if (!fin.is_open())
            throw runtime_error("Failed to open file for reading");
    }

    string readAll()
    {
        /*
            Strong exception safety:
            No partial state exposure
        */
        string result;
        string line;

        while (getline(fin, line))
        {
            result += line + '\n';
        }

        return result;
    }

    char readChar()
    {
        char ch;
        if (!fin.get(ch))
            throw runtime_error("Read failed / EOF");

        return ch;
    }

    streampos position()
    {
        return fin.tellg();
    }

    void seek(streampos pos)
    {
        fin.clear();  // IMPORTANT before seek
        fin.seekg(pos);

        if (!fin)
            throw runtime_error("Seek failed");
    }

    ~FileReader()
    {
        if (fin.is_open())
            fin.close();
    }
};


/* ============================================================
   4. EXCEPTION SAFE FILE USAGE
   ============================================================ */

void safeFileDemo()
{
    try
    {
        FileWriter writer("safe.txt");
        writer.writeLine("Hello RAII");
        writer.writeLine("Exception-safe writing");

        FileReader reader("safe.txt");

        cout << "\n[Safe Read]\n";
        cout << reader.readAll();

        cout << "Current pos: " << reader.position() << endl;
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << endl;
    }
}


/* ============================================================
   5. MMAP (SYSTEMS LEVEL FILE ACCESS)
   ============================================================

   mmap:
   - Maps file directly into virtual memory
   - No explicit read/write syscalls per access
   - Kernel handles paging

   Interview Insight:
   - Used in databases, drivers, high-perf systems
*/

class MMapFile
{
private:
    int fd;
    size_t length;
    void* addr;

public:
    MMapFile(const string& filename)
    {
        fd = open(filename.c_str(), O_RDONLY);
        if (fd < 0)
            throw runtime_error("open() failed");

        struct stat st;
        if (fstat(fd, &st) < 0)
            throw runtime_error("fstat failed");

        length = st.st_size;

        addr = mmap(nullptr, length, PROT_READ, MAP_PRIVATE, fd, 0);
        if (addr == MAP_FAILED)
            throw runtime_error("mmap failed");
    }

    const char* data() const
    {
        return static_cast<const char*>(addr);
    }

    size_t size() const
    {
        return length;
    }

    ~MMapFile()
    {
        if (addr && addr != MAP_FAILED)
            munmap(addr, length);

        if (fd >= 0)
            close(fd);
    }
};


/* ============================================================
   6. MMAP DEMO
   ============================================================ */

void mmapDemo()
{
    try
    {
        MMapFile file("safe.txt");

        cout << "\n[MMAP CONTENT]\n";

        /*
            Direct memory access
            No read() loop needed
        */
        for (size_t i = 0; i < file.size(); ++i)
        {
            cout << file.data()[i];
        }
    }
    catch (const exception& e)
    {
        cerr << "mmap error: " << e.what() << endl;
    }
}


/* ============================================================
   7. MMAP vs FSTREAM (INTERVIEW GOLD)
   ============================================================ */

/*
    fstream:
    --------
    + Portable (C++ standard)
    + Easy to use
    - Multiple syscalls (read/write)
    - Copies data from kernel → user buffer

    mmap:
    -----
    + Zero-copy (kernel maps pages directly)
    + Faster for large files / random access
    + Used in OS, DBs, drivers

    - Platform dependent (POSIX)
    - Page faults instead of read()
    - Must handle alignment/page issues

    KEY INTERVIEW LINE:
    "mmap trades syscall overhead for page fault cost"
*/


/* ============================================================
   MAIN
   ============================================================ */

int main()
{
    cout << "=== TEMPLATE ===\n";
    cout << big(10, 20) << endl;

    cout << "\n=== RAII FILE ===\n";
    safeFileDemo();

    cout << "\n=== MMAP ===\n";
    mmapDemo();

    cout << "\n=== DONE ===\n";

    return 0;
}
