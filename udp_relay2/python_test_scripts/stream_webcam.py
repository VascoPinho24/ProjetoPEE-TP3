import cv2, imutils, socket
import numpy as np
import time
import base64

hubID = 453

hubIDlist = [(hubID & 0xFF),((hubID >> 8)  & 0xFF),((hubID >> 16) & 0xFF),((hubID >> 24) & 0xFF)]
hubIDencoded = bytearray(hubIDlist)


BUFF_SIZE = 65536
WIDTH = 300
client_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
client_socket.setsockopt(socket.SOL_SOCKET,socket.SO_RCVBUF,BUFF_SIZE)

vid = cv2.VideoCapture(0)
fps,st,frames_to_count,cnt = (0,0,20,0)

while True:
    
    while(vid.isOpened()):
        _,frame = vid.read()
        frame = imutils.resize(frame,width=WIDTH)
        encoded,buffer = cv2.imencode('.jpg',frame,[cv2.IMWRITE_JPEG_QUALITY,40])
        message = hubIDencoded + base64.b64encode(buffer)
        client_socket.sendto(message,("20.68.171.3",8080))
        frame = cv2.putText(frame,'FPS: '+str(fps),(10,40),cv2.FONT_HERSHEY_SIMPLEX,0.7,(0,0,255),2)
        cv2.imshow('TRANSMITTING VIDEO',frame)
        key = cv2.waitKey(1) & 0xFF
        if key == ord('q'):
            server_socket.close()
            break
        if cnt == frames_to_count:
            try:
                fps = round(frames_to_count/(time.time()-st))
                st=time.time()
                cnt=0
            except:
                pass
        cnt+=1