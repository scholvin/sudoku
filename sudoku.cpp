#include <iostream>
#include <stdexcept>

class Sudoku
{
    int m_b[9][9] = {0};    // the board, initialize to 0's across

public:
    static int s_iteration; // how many iterations
    static int s_solutions; // how many solutions
    static int s_depth;     // current stack depth
    static int s_maxdepth;  // how deep did the stack get

    // is this a valid solution so far?
    bool valid() const
    {
        // the trick: we use a bit field initialized to 0. when we see n 
        // in the range to check (row, column, box), we see if that bit
        // is set. if it is, that number has already been used and we're
        // done. else, we set it. note that we don't care about 0's, since
        // that means it's not set yet.

        // rows
        for (auto j = 0; j < 9; j++)
        {
            int bits = 0;
            for (auto i = 0; i < 9; i++)
            {
                if (m_b[i][j] == 0) continue;
                if (bits & (1 << m_b[i][j])) return false;
                bits |= (1 << m_b[i][j]);
            }
        }
        // columns
        for (auto i = 0; i < 9; i++)
        {
            int bits = 0;
            for (auto j = 0; j < 9; j++)
            {
                if (m_b[i][j] == 0) continue;
                if (bits & (1 << m_b[i][j])) return false;
                bits |= (1 << m_b[i][j]);
            }
        }
        // sub-squares
        for (auto x = 0; x < 3; x++)
        {
            for (auto y = 0; y < 3; y++)
            {
                int bits = 0;
                for (auto i = x * 3; i < x * 3 + 3; i++)
                {
                    for (auto j = y * 3; j < y * 3 + 3; j++)
                    {
                        if (m_b[i][j] == 0) continue;
                        if (bits & (1 << m_b[i][j])) return false;
                        bits |= (1 << m_b[i][j]);
                    }
                }
            }
        }

        // must be ok if we made it this far
        return true;
    }

    // is this a complete solution? (no 0's)
    bool complete() const
    {
        for (auto i = 0; i < 9; i++) 
            for (auto j = 0; j < 9; j++)
                if (m_b[i][j] == 0)
                    return false;
        return true;
    }

    // solve this thing via recursive backtracking
    void solve()
    {
        s_iteration++;
        s_depth++;
        if (s_depth > s_maxdepth) s_maxdepth = s_depth;
        if (complete() && valid())
        {
            std::cout << "solution at iteration " << s_iteration << std::endl << *this << std::endl;
            s_solutions++;
            s_depth--;
            return;
        }
        for (auto i = 0; i < 9; i++) 
        {
            for (auto j = 0; j < 9; j++)
            {
                if (m_b[i][j] != 0)
                {
                    // skip until we find a blank space
                    continue;
                }
               Sudoku candidate(*this);
               for (int c = 1; c <= 9; c++)
               {
                    candidate.m_b[i][j] = c; // encapsulation fail
                    if (!candidate.valid())
                    {
                        continue;
                    }
                    else
                    {
                        // try it
                        candidate.solve();
                    }
                }
                // backtrack - set the square we were working on to 0 and head back up
                candidate.m_b[i][j] = 0;
                s_depth--;
                return;
            }
        }
        throw std::logic_error("unexpected code point reached in Sudoku::solve");
    }

    friend std::ostream& operator << (std::ostream& os, const Sudoku& b);
    friend std::istream& operator >> (std::istream& is, Sudoku& b);
};

int Sudoku::s_iteration = 0;
int Sudoku::s_solutions = 0;
int Sudoku::s_depth = 0;
int Sudoku::s_maxdepth = 0;

// print the board to stdout
std::ostream& operator << (std::ostream& os, const Sudoku& s)
{
    for (auto i = 0; i < 9; i++) 
    {
        for (auto j = 0; j < 9; j++)
        {
            os << s.m_b[i][j];
        }
        os << std::endl;
    }
    return os;
}

// take the board from stdin
// any non-digit character will be interpreted as an empty space
// there's no error checking here...
std::istream& operator >> (std::istream& is, Sudoku& s)
{
    std::string line;
    for (auto i = 0; i < 9; i++) 
    {
        std::getline(is, line);
        for (auto j = 0; j < 9; j++)
        {
            if (line[j] >= '0' && line[j] <= '9')
            {
                s.m_b[i][j] = line[j] - '0';
            }
        }
    }
}

int main(int, char**)
{
    Sudoku s;

    std::cin >> s;
    if (!s.valid())
    {
        std::cout << "invalid input" << std::endl;
        return 1;
    }
    else
    {
        s.solve();
    }

    std::cout << "total solutions: " << Sudoku::s_solutions << std::endl
              << "max stack depth: " << Sudoku::s_maxdepth << std::endl;
    return 0;
}
