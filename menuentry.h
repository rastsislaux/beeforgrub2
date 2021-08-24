#ifndef MENUENTRY_H
#define MENUENTRY_H

#include <string>
#include <map>

using namespace std;

class menuentry
{
public:
    menuentry(string path_to_entry);
    menuentry();
    string get_pretty_name();
    string get_title();
    string get_parameter(string parameter);
    void export_to_file(string export_path);
    string path;
    map <string, string> params;
};

#endif // MENUENTRY_H
