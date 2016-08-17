import socket
import threading

HOST = 'delyh.com'
PORT = 50000
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

print('enter your name:')
name = input()
enter = name + ' is entered.'
name = bytes(name, "euc-kr")
enter = bytes(enter, "euc-kr")
s.send(enter)
print('--- Chatting is start ---')

def sendingMsg():
	while True:
		data = input()
		data = bytes(data, "euc-kr")
		s.send(data)
	s.close()
	
def gettingMsg():
	while True:
		data = s.recv(1024)
		data = data.decode("euc-kr")
		print(data)
	s.close()

threading._start_new_thread(sendingMsg,())
threading._start_new_thread(gettingMsg,())

while True:
	pass

    
