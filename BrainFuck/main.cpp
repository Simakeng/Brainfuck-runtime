#include <iostream>
#include <iomanip>
#include <string>
#include <deque>
class BrainFuck
{
private:
	std::deque<int64_t> mem;
	std::deque<int64_t>::iterator currentPtr;
	int64_t startIndex;
public:
	BrainFuck() :startIndex(0) { mem.push_back(0); currentPtr = mem.begin(); };
	~BrainFuck() {};
public:
	void operator+(int64_t rhs)
	{
		*currentPtr += rhs;
	}
	void operator-(int64_t rhs)
	{
		*currentPtr -= rhs;
	}
	void operator>(int64_t rhs)
	{
		auto cnt = std::distance(currentPtr, mem.end()) - 1;
		if (cnt < rhs)
		{
			mem.insert(mem.end(), rhs - cnt, 0);
			currentPtr = mem.end() - 1;
		}
		else {
			currentPtr += rhs;
		}
	}
	void operator<(int64_t rhs)
	{
		auto cnt = std::distance(mem.begin(), currentPtr);
		if (cnt < rhs)
		{
			startIndex -= rhs - cnt;
			mem.insert(mem.begin(), rhs - cnt, 0);
			currentPtr = mem.begin();
		}
		else {
			currentPtr -= rhs;
		}
	}
	void output()
	{
		putchar(*currentPtr);
	}
	void input()
	{
		*currentPtr = getchar();
	}
	bool test() { return *currentPtr == 0; };
	void dispRam()
	{
		for (auto it = mem.begin(); it != mem.end(); it++)
		{
			std::cout << std::setiosflags(std::ios::left) << std::setw(log(mem.size()) + 1) << std::distance(mem.begin(), it) + startIndex;
			std::cout << " : " << *it;
			if (it == currentPtr)std::cout << " <-";
			std::cout << std::endl;
		}
	}
};
void fuckit(std::deque<char> cmdbuf, BrainFuck& fuck)
{
	auto ps = cmdbuf.begin();
	if (*ps == '[')
		if (fuck.test())
			return;
	while (ps != cmdbuf.end())
	{
		switch (*ps)
		{
		case '+': fuck + 1; ++ps; break;
		case '-': fuck - 1; ++ps; break;
		case '>': fuck > 1; ++ps; break;
		case '<': fuck < 1; ++ps; break;
		case ',': fuck.input();  ++ps; break;
		case '.': fuck.output(); ++ps; break;
		case ']':
			if (std::distance(ps, cmdbuf.end()) != 1)
				std::cout << "Synatax Error : Unmatched ] \n";
			else if (!fuck.test())
			{
				ps = cmdbuf.begin() + 1;
				break;
			}
			else
				return;
		case '[':
		{
			if (ps == cmdbuf.begin())
			{
				ps++;
				break;
			}
			auto start = ps;
			int um = 0;
			do
			{
				if (*ps == '[') um++;
				if (*ps == ']') um--;
				ps++;
			} while (um != 0);
			auto subbuf = std::deque<char>();
			subbuf.insert(subbuf.end(), start, ps);
			ps = cmdbuf.erase(start, ps);
			fuckit(subbuf, fuck);
		}
		default:
			break;
		}
	}
}
int main()
{
	using namespace std;
	cout << "Brainfuck 0.0.1" << endl;
	string linebuf;
	BrainFuck fuck;
	deque<char> cmdbuf;
	while (getline(cin, linebuf), linebuf.find("exit()") != 0)
	{
		if (linebuf == "ram()")
		{
			fuck.dispRam();
			continue;
		}
		for (auto ch : linebuf)
		{
			switch (ch)
			{
			case '+':
			case '-':
			case '>':
			case '<':
			case '.':
			case ',':
			case '[':
			case ']':	cmdbuf.push_back(ch); break;
			default:	cout << "unsupported char : " << ch << endl;
			case '\n':
			case '\r':
			case ' ':	break;
			}
		}
		auto ps = cmdbuf.begin();

		while (ps != cmdbuf.end())
		{
			switch (*ps)
			{
			case '+': fuck + 1; ps = cmdbuf.erase(ps); break;
			case '-': fuck - 1; ps = cmdbuf.erase(ps); break;
			case '>': fuck > 1; ps = cmdbuf.erase(ps); break;
			case '<': fuck < 1; ps = cmdbuf.erase(ps); break;
			case ',': fuck.input();  ps = cmdbuf.erase(ps); break;
			case '.': fuck.output(); ps = cmdbuf.erase(ps); break;
			case '[':
			{
				auto start = ps;
				int um = 0;
				do
				{
					if (*ps == '[') um++;
					if (*ps == ']') um--;
					ps++;
					if (ps == cmdbuf.end())
						break;
				} while (um != 0);
				if (um != 0)
					break;
				auto subbuf = std::deque<char>();
				subbuf.insert(subbuf.end(), start, ps);
				ps = cmdbuf.erase(start, ps);
				fuckit(subbuf, fuck);
			}
			default:
				break;
			}
		}
	}
}