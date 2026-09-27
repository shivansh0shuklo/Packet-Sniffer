#include "headers.hpp"

//defining the color of the terminal
//033 is the escape code fot terminal
#define RESET "\033[0m"//default terminal color
#define CYAN "\033[36m]"//for tcp
#define RED "\033[91m"//for udp
#define YELLOW "\033[93m"//for icmp

int main(){
    //ETH_P_ALL captures all the ipv6 ipv4 etc.
    //hton is used for the host to network short convert the integer to network byte order 
    //say like port  = 80 your host is like [0x080 , 0x00] but intenet wnat oposite so it like some
    // kind of correction  mashine which handels the order of the bytes 
    //Sock-raw ensure the clean unfiltered packets of data
    //af_packet is like for bypasing the kerneal and directly talking to the nic for the clean data
    int raw_socks = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
    if(raw_socks<0){//well raw_socks can never be -ve so it only handels like the sudo error 
        cerr << "[perms error] run with sudo!!\n";
        return 1;
    }
    uint8_t buffer[65536];
    snifferconfig config;

    while(true){
        ssize_t data_size = recvfrom(raw_socks,buffer,sizeof(buffer),0,nullptr,nullptr);
        if(data_size<0){
            continue;
        }
        packet_info pkt;
        pkt.length =data_size;
        struct iphdr* iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));


    } 


}