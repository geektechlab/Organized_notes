/*
    ================================================================
    C++ MASTER REFERENCE: Templates + File Streams (Interview Level)
    ================================================================

    Topics Covered:
    1. Function Templates (generic functions)
    2. Class Templates (generic classes)
    3. iostream hierarchy
    4. ifstream / ofstream / fstream usage
    5. File open modes (ios::in, ios::out, ios::app, ios::binary, etc.)
    6. << and >> operators
    7. get(), getline(), EOF handling
    8. tellg(), tellp(), seekg(), seekp()
    9. Binary vs Text mode differences
    10. Common pitfalls asked in interviews

    RULE:
    Every line here is intentional. Don't skip comments.
*/

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

/* ============================================================
   1. FUNCTION TEMPLATE (Generic Function)
   ============================================================

   Key Idea:
   - Avoid writing multiple overloads for different types
   - Compiler generates code based on usage

   Interview Trap:
   - Templates are compile-time, not runtime polymorphism
*/

template <typename T>
T big(T a, T b)
{
    return (a > b) ? a : b;
}

/*
    Multiple type template
    Useful when arguments are of different types
*/
template <typename T, typename U>
auto bigMixed(T a, U b) -> decltype(a > b ? a : b)
{
    return (a > b) ? a : b;
}


/* ============================================================
   2. CLASS TEMPLATE (Generic Class)
   ============================================================

   Equivalent to your "arraylist" example but modernized
*/

template <typename T>
class ArrayList
{
private:
    struct ControlBlock
    {
        size_t capacity;
        T* arr;
    };

    ControlBlock* cb;

public:
    explicit ArrayList(size_t cap)
    {
        cb = new ControlBlock;
        cb->capacity = cap;
        cb->arr = new T[cap];

        cout << "[ArrayList] Allocated for type\n";
    }

    ~ArrayList()
    {
        delete[] cb->arr;
        delete cb;
    }

    void set(size_t idx, T val)
    {
        if (idx >= cb->capacity)
            throw out_of_range("Index out of bounds");
        cb->arr[idx] = val;
    }

    T get(size_t idx) const
    {
        if (idx >= cb->capacity)
            throw out_of_range("Index out of bounds");
        return cb->arr[idx];
    }
};


/* ============================================================
   3. STREAM HIERARCHY (IMPORTANT FOR INTERVIEW)
   ============================================================

   ios (base class)
     ├── istream  (input)
     ├── ostream  (output)
     └── iostream (input + output)

   File streams:
     ifstream → input from file
     ofstream → output to file
     fstream  → both

   cin  → istream
   cout → ostream
*/


/* ============================================================
   4. FILE WRITING (ofstream)
   ============================================================ */

void writeFile()
{
    ofstream fout;

    /*
        open modes:
        ios::out   → write (default for ofstream)
        ios::app   → append
        ios::binary→ binary mode
        ios::ate   → go to end immediately
    */

    fout.open("test.txt", ios::out);  // overwrite

    if (!fout)
    {
        cerr << "File open failed\n";
        return;
    }

    // << operator writes to file (ostream behavior)
    fout << "Hello World\n";
    fout << "Line 2\n";

    fout.close();
}


/* ============================================================
   5. FILE READING (ifstream)
   ============================================================ */

void readFile()
{
    ifstream fin("test.txt");

    if (!fin)
    {
        cerr << "File open failed\n";
        return;
    }

    /*
        >> operator:
        - Stops at whitespace (IMPORTANT PITFALL)
    */

    string word;
    fin >> word;  // reads only "Hello"
    cout << "Using >> : " << word << endl;

    /*
        get() → reads character INCLUDING whitespace
    */

    fin.clear();          // clear EOF flag
    fin.seekg(0);         // go back to beginning

    char ch;
    cout << "Using get(): ";
    while (fin.get(ch))   // CORRECT EOF handling
    {
        cout << ch;
    }

    fin.close();
}


/* ============================================================
   6. getline() vs >> (VERY IMPORTANT INTERVIEW POINT)
   ============================================================ */

void readLines()
{
    ifstream fin("test.txt");

    string line;

    cout << "\nUsing getline():\n";

    while (getline(fin, line))
    {
        cout << line << endl;
    }

    fin.close();
}


/* ============================================================
   7. tellg() and seekg() (INPUT POINTER)
   ============================================================ */

void filePointerDemo()
{
    ifstream fin("test.txt");

    cout << "\nInitial position: " << fin.tellg() << endl;

    fin.seekg(6); // move to index 6
    cout << "After seekg(6): " << fin.tellg() << endl;

    char ch;
    fin.get(ch);
    cout << "Character at pos 6: " << ch << endl;

    /*
        Relative movement:
        ios::beg → beginning
        ios::cur → current
        ios::end → end
    */

    fin.seekg(-2, ios::end);
    cout << "Near end pos: " << fin.tellg() << endl;

    fin.close();
}


/* ============================================================
   8. tellp() and seekp() (OUTPUT POINTER)
   ============================================================ */

void outputPointerDemo()
{
    ofstream fout("test.txt", ios::in | ios::out);

    cout << "\nOutput pointer: " << fout.tellp() << endl;

    fout.seekp(5);
    fout << "X"; // overwrite at position 5

    fout.close();
}


/* ============================================================
   9. BINARY vs TEXT MODE (CRITICAL DIFFERENCE)
   ============================================================ */

void binaryDemo()
{
    ofstream fout("bin.dat", ios::binary);

    int x = 12345;

    /*
        write raw bytes
        reinterpret_cast required
    */
    fout.write(reinterpret_cast<char*>(&x), sizeof(x));
    fout.close();

    ifstream fin("bin.dat", ios::binary);

    int y;
    fin.read(reinterpret_cast<char*>(&y), sizeof(y));

    cout << "\nBinary read value: " << y << endl;

    fin.close();
}


/* ============================================================
   10. COMMON PITFALLS (READ CAREFULLY)
   ============================================================ */

/*
    ❌ WRONG EOF handling:
    while (!fin.eof())   ← classic bug

    ✅ CORRECT:
    while (fin.get(ch))

    -------------------------------------

    ❌ >> ignores spaces
    "Hello World" → only "Hello"

    ✅ use getline()

    -------------------------------------

    ❌ Mixing >> and getline without handling newline

    -------------------------------------

    ❌ Not clearing stream state:
    fin.clear()

    -------------------------------------

    ❌ Not resetting pointer:
    fin.seekg(0)

    -------------------------------------

    ❌ Binary vs text confusion:
    - Text mode may modify line endings (\n → \r\n)
    - Binary mode preserves exact bytes
*/


/* ============================================================
   MAIN DRIVER
   ============================================================ */

int main()
{
    cout << "=== TEMPLATE DEMO ===\n";

    cout << big(4, 5) << endl;
    cout << big(5.6, 3.4) << endl;
    cout << bigMixed(5, 5.5) << endl;

    ArrayList<int> list(3);
    list.set(0, 10);
    list.set(1, 20);

    cout << "ArrayList[1]: " << list.get(1) << endl;

    cout << "\n=== FILE OPERATIONS ===\n";

    writeFile();
    readFile();
    readLines();

    filePointerDemo();
    outputPointerDemo();

    binaryDemo();

    cout << "\n=== DONE ===\n";

    return 0;
}