#!/usr/bin/env python3

from socket import *

sock = socket()
dest = ('127.0.0.1', 8080)
sock.connect(dest)

print("Client connected to {}\n".format(dest))

stay = 1
while stay:
    msg = input()
    if msg.upper() in ["HALT", "EXIT"]:
        msg = msg.upper()
        stay = 0
    msg += "\n\0"
    sock.sendall(msg.encode("utf-8"))
    if not stay:
        print("Leaving...\n")
        sock.close()