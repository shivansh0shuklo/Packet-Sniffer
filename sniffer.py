import socket

PORT  = 5555
FORMAT = "utf-8"
IP_ADDR = "0.0.0.0"
ADDR = (IP_ADDR,PORT)

def main():
    try:
        server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        server.bind(ADDR) 
    except Exception as e:
        print(f"[ERROR!] {e} occured!")
def handel_client(conn,addr):
    print(f"connection establised! [connected] to {addr}")
    