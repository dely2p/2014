import socket
import threading
HOST = ''
PORT = 50000       
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
while True:
        s.listen(1)
        conn, addr = s.accept()
        print('Connected by', addr)
        def sendingMsg():
                while True:
                        data = input()
                        data = data.encode("euc-kr")
                        conn.send(data)
                conn.close()
        def gettingMsg():
                while True:
                        data = conn.recv(1024)
                        if not data: 
                                break
                        else:
                                data = data.decode("euc-kr")
                                print(data)
                conn.close()
        threading._start_new_thread(sendingMsg,())
        threading._start_new_thread(gettingMsg,())
