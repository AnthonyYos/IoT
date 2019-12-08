import cv2
import numpy as np
import os
from twilio.rest import Client

account_sid = "AC0cf5ead72403b20c132fb03a5dd2f487"
auth_token = "ca06ec736e5347d7f37e99acdbb37216"
client = Client(account_sid, auth_token)

os.chdir("/home/pi/opencv/data/haarcascades")
recognizer = cv2.face.LBPHFaceRecognizer_create()
recognizer.read('/home/pi/Face Recognition/trainer/training.yml')
cascadePath = "haarcascade_frontalface_default.xml"
faceCascade = cv2.CascadeClassifier(cascadePath);

font = cv2.FONT_HERSHEY_SIMPLEX

id = 0

names = ['None', 'Amanda', 'Anthony']

account_sid = "AC0cf5ead72403b20c132fb03a5dd2f487"
auth_token = "ca06ec736e5347d7f37e99acdbb37216"
client = Client(account_sid, auth_token)

def sendText(name):
    client.messages.create(
        to="+1phonenumber",
        from_="+19782976741",
        body="I just saw " + name
        )

cam = cv2.VideoCapture(0)
cam.set(3, 640)
cam.set(4, 480)

minW = 0.1*cam.get(3)
minH = 0.1*cam.get(4)

while True:
    ret, img =cam.read()
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor = 1.2,
        minNeighbors = 5,
        minSize = (int(minW), int(minH)),
       )

    for(x,y,w,h) in faces:
        cv2.rectangle(img, (x,y), (x+w,y+h), (0,255,0), 2)
        id, confidence = recognizer.predict(gray[y:y+h,x:x+w])

        if (confidence < 100):
            id = names[id]
            confidence = "  {0}%".format(round(confidence))
            identification = int(confidence.strip('%'))
            if(identification > 30):
                name = id
                sendText(name)
            
        else:
            id = "Stranger Detected"
            confidence = "  {0}%".format(0)

        cv2.putText(img, str(id), (x+5,y-5), font, 1,
                    (255,255,255), 2)
        cv2.putText(img, str(confidence), (x+5,y+h-5), font, 1, (255,255,0), 1)
        

    cv2.imshow('camera',img)
    # 'Esc' key to exit
    k = cv2.waitKey(10) & 0xff
    if k == 27:
        break
    
cam.release()
cv2.destroyAllWindows()
