#include "menuentry.h"
#include <vector>
#include <fstream>
#include <sstream>

// Menuentry is a class to contain menuentry parameters
menuentry::menuentry(string path_to_entry)
{
    path = path_to_entry;

    // Reading from file
    ifstream fin;
    fin.open(path_to_entry);

    // We need to preset these two because we every arg and class is contained in a new line
    params["grub_arg"] = "";
    params["grub_class"] = "";

    // Line by line reading
    for(string line; getline(fin, line);) {
        string key = line.substr(0, line.find(" "));
        string value = line.substr(line.find(" ")+1);
        if (key == "grub_arg" or key == "grub_class") {
            params[key] += value + ' ';
        }
        else params[key] = value;
    }

    // Stripping grub_arg and grub_class
    if (params["grub_arg"] != "")
        params["grub_arg"].erase(params["grub_arg"].length()-1, 1);
    if (params["grub_class"] != "")
        params["grub_class"].erase(params["grub_class"].length()-1, 1);

    fin.close();
}

// Empty menunetry constructor for File - New
menuentry::menuentry() {}

// Little split function
std::vector<std::string> split(const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

// Exporting menuentry to a file
void menuentry::export_to_file(string export_path) {
    ofstream fout;
    fout.open(export_path);
    for (auto pair : params) {

        // For each grub arg or class we need a new line
        if (pair.first == "grub_arg" or pair.first == "grub_class") {
            vector <string> temp = split(pair.second, ' ');
            for (string arg : temp)
                fout << pair.first << " " << arg << endl;
        }

        // For Everything Else There's...
        else if (pair.second != "") fout << pair.first << " " << pair.second << endl;
    }
    fout.close();
}

// Getting pretty name
string menuentry::get_pretty_name() {
    return get_title() + '\n' + path;
}

// Safe get title
string menuentry::get_title() {
    if (params.count("title"))
        return params["title"];
    return "{NO_TITLE}";
}

// Safe get parameter
string menuentry::get_parameter(string parameter) {
    if (params.count(parameter))
        return params[parameter];
    return "";
}
