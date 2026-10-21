#include "headers.hpp"

//defining the color of the terminal
//033 is the escape code fot terminal
#define RESET "\033[0m"//default terminal color
#define CYAN "\033[36m"//for tcp
#define RED "\033[91m"//for udp
#define YELLOW "\033[93m"//for icmp

int main(){
    //ETH_P_ALL captures all the ipv6 ipv4 etc.
    //hton is used for the host to network short convert the integer to network byte order 
    //say like port  = 80 your host is like [0x080 , 0x00] but intenet wnat oposite so it like some
    // kind of correction  machine which handels the order of the bytes 
    //Sock-raw ensure the clean unfiltered packets of data
    //af_packet is like for bypasing the kerneal and directly talking to the nic for the clean data
    int raw_socks = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
    if(raw_socks<0){//well raw_socks can never be -ve so it only handels like the sudo error 
        cerr << "[perms error] run with sudo!!\n";
        return 1;
    }
    uint8_t buffer[65536];
    snifferconfig config;
    printf(RESET " TERMINAL OUTPUT --> \n" RESET);
    while(true){
        struct ethhdr* eth = (struct ethhdr*)buffer;
        ssize_t data_size = recvfrom(raw_socks,buffer,sizeof(buffer),0,nullptr,nullptr);
        if(data_size<0){
            printf("[error] failed recvfrom");
            continue;
        }
        if(data_size <= ssize_t(sizeof(struct ethhdr)+ sizeof(struct iphdr))){
            continue;
        }
        if(ntohs(eth->h_proto)!=ETH_P_IP){
            continue;
        }
        packet_info pkt;
        pkt.length =data_size;
        char src[32];
        char des[32];
        struct iphdr* iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
        unsigned short iphdr_len = iph->ihl*4; //reperesent the bytes of iphdr length in byte like if 20 bytes then ihl will be 5 
        if(inet_ntop(AF_INET,&iph->saddr,src,32)!=nullptr && inet_ntop(AF_INET,&iph->daddr,des,32)!=nullptr){//ipnet_ntop for the conversion of binary data of hte address into human sting liek ad:45:67:89 like this
            pkt.src_ip = src;
            pkt.des_ip = des;
        }else{
            printf("[failed conversion]");
        }
        if(iph->protocol == IPPROTO_TCP){//this IPPROTO JUST GIVE VARIOUS PROTOCALS 
            if(data_size < (ssize_t)(sizeof(struct ethhdr)+iphdr_len+sizeof(struct tcphdr))){
                continue;
            }
            pkt.protocal = "TCP";
            struct tcphdr* tcp =(struct tcphdr*)(buffer + sizeof(struct ethhdr)+iphdr_len);
            pkt.src_port = ntohs(tcp->source);
            pkt.des_port = ntohs(tcp->dest);
            string total_flag = "";
            if(tcp->syn){
                total_flag.append("SYN ");
            }
            if(tcp->ack){
                total_flag.append("ACK ");
            }
            if(tcp->fin){
                total_flag.append("FIN ");
            }
            if(tcp->psh){
                total_flag.append("PSH ");
            }
            if(tcp->urg){total_flag.append("URG ");
            }
            if(tcp->rst){
                total_flag.append("RST ");
            }
            pkt.flags = total_flag;
        }else if(iph->protocol == IPPROTO_UDP){
            if(data_size < (ssize_t)(sizeof(struct ethhdr)+iphdr_len+sizeof(struct udphdr))){
                continue;
            }
            pkt.protocal = "UDP";
            struct udphdr* udp = (struct udphdr*)(buffer + sizeof(struct ethhdr)+iphdr_len);
            pkt.src_port = ntohs(udp->source);
            pkt.des_port = ntohs(udp->dest);
            pkt.flags = "None";
        }else if(iph->protocol == IPPROTO_ICMP){
            if(data_size < (ssize_t)(sizeof(struct ethhdr)+iphdr_len+sizeof(struct icmphdr))){
                continue;
            }
            pkt.protocal = "ICMP";
            struct icmphdr* icmp = (struct icmphdr*)(buffer + sizeof(struct ethhdr) + iphdr_len);
            pkt.icmp_type = icmp->type;
            pkt.icmp_code = icmp->code;//reading directly as they are 8bit so the byte oder doesnt apply here as it only apply to 16 or 32 as it only uses one byte of memory so no swapping concept so no little endian or big endian no swaping of bytes nedded acress devices 
            pkt.des_port = false;
            pkt.src_port = false;
            pkt.flags = "None";

        }else{
            continue;
        }const char* color = RESET;
        if(pkt.protocal == "TCP"){
            color = CYAN;

        }else if(pkt.protocal == "ICMP"){
            color = YELLOW;

        }else if(pkt.protocal == "UDP"){
            color = RED;
        }else{
            color = RESET;
        }
        //printing the terminal output

        printf("%s[%s] || [%s] [%u] > [%s] [%u]  ||  [%zu]|[%s] %s \n",color,pkt.protocal.c_str(),pkt.src_ip.c_str(),pkt.src_port,pkt.des_ip.c_str(),pkt.des_port,pkt.length,pkt.flags.c_str(),RESET);        


    } 
    


}
