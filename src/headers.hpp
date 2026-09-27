#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include <unistd.h> //used to like talk to teh keranal for reciveng data like network inforamtion or read a file.txt fromt the hardware .
#include <sys/socket.h>
#include <iomanip> //for like foramtting the hex outut(MAC address cleanly)
#include <arpa/inet.h>//for conversion of like ip --> to like string or binaryies
#include <netinet/ip.h>//for the ipv4 header niformation ttl , port, protocal 
#include <netinet/tcp.h>//for th tcp protocal inforamtion its port
#include <netinet/udp.h>//for the protocal udp
#include <net/ethernet.h>//for mac addresses 
#include <string>

using namespace std;
#define payloadmax 1000;
#define max_str[100];


struct packet_info{
    string packet_header[100];
    string src_ip;
    string des_ip;
    uint16_t src_port;
    uint16_t des_port;//exactly allocates 2 byte or 16bits in the memory allocation
    string flags{'-'};
    string protocal{"other"};
    size_t length{0};//mesure the memory byte size data 

    //to like convert this struct to json for python 
    string tojson() const {
        // this / tells the c++ to like take next " as a text charater  
        return "{"
            "\"src_ip\":\"" + src_ip + "\","
            "\"src_port\":" + to_string(src_port) + ","
            "\"des_ip\":\"" + des_ip + "\","
            "\"des_port\":" + to_string(des_port) + ","
            "\"protocal\":\"" + protocal  + "\","
            "\"flags\":\"" + flags + "\","
            "\"length\":" + to_string(length) + 
        "}";
    }

};
//configuration for the sniffer 
struct snifferconfig{// -1 is just a sentinal value(just as a placeholder like a condition boundary)
    string interface{""};
    int target_port{-1};//the network port can be like 0 - 60k something s it tells do not filter by port 
    bool filter_tcp{false};//same
    bool filter_udp{false};//tells to do not filter for only udp protocal
    int packet_limit{-1};//tell the sniffer to run infinetly untill ctrl+c
};
