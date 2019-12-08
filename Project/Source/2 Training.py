import cv2
import numpy as np
from PIL import Image
import os

path = '/home/pi/Face Recognition/dataset'

os.chdir("/home/pi/opencv/data/haarcascades")
recognizer = cv2.face.LBPHFaceRecognizer_create()
detector = cv2.CascadeClassifier("haarcascade_frontalface_default.xml");

def getImagesAndLabels(path):
    imagePaths = [os.path.join(path,f) for f in os.listdir(path)]
    faceSamples=[]
    ids = []
    for imagePath in imagePaths:
        img_pillow = Image.open(imagePath).convert('L')
        img_numpy = np.array(img_pillow,'uint8')
        id = int(os.path.split(imagePath)[-1].split(".")[1])
        faces = detector.detectMultiScale(img_numpy)
        for (x,y,w,h) in faces:
            faceSamples.append(img_numpy[y:y+h,x:x+w])
            ids.append(id)
    return faceSamples,ids
faces,ids = getImagesAndLabels(path)
recognizer.train(faces, np.array(ids))
recognizer.write('/home/pi/Face Recognition/trainer/training.yml')
print("\n {0} faces were trained on.".format(len(np.unique(ids))))
