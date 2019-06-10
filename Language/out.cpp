/*
 * Star+
 * Eric Diskin
 * V0.5.2
 */


//////////////
// BUILTINS //
//////////////
#include <iostream>
#include <string>

std::string print(std::string a)
{
	// print put a string.
	std::cout << a << std::endl;
	return a;
}

int cmd(std::string a)
{
	// execute a system command.
	return system(a.c_str());
}

////////////
// SCRIPT //
////////////

int main()
{
	cmd("COLOR 1c"/*@string*/);
	print("Hello World!"/*@string*/);
	print("The answer to life, the universe and everything is 42."/*@string*/);
	cmd("PAUSE"/*@string*/);
}


