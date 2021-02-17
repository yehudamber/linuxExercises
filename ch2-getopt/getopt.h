#pragma once

inline namespace MyUtils
{

int getopt(int argc, char* const argv[], const char* optstring);

extern char* optarg;

extern int optind;
extern int optopt;

} // inline namespace MyUtils
