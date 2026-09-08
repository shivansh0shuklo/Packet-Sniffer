#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include <string>

using namespace std;
#define payloadmax 1000;
#define max_str[100];


struct packet_info{
    string packet_header[100];
    int packet_number;
    bool packet_drop = false;

};