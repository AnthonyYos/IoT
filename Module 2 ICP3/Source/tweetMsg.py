#!/usr/bin/env python
import sys
from twython import Twython
from picamera import PiCamera
from time import sleep
#Ali69777725

apiKey = 'YlAEbs0q2JpxPrEcIBDgDSlTA'
apiSecret = 'OY8rlFOy5a5D52Ectwj346O8GWnSLxVSY5I5Iak8RTgW6DCyFN'
accessToken = '2525841722-0XQ8m3WpSyXeCHsxtMKYNgmMkyfNWY2OpGpaPoD'
accessTokenSecret = 'bHUsD99zlmV7pxqqoPjH3cm9ZqPywWfggUdnPtLZaoVlT'
twitter = Twython(apiKey,apiSecret,accessToken,accessTokenSecret)

camera = PiCamera()
camera.start_preview(fullscreen=False,window=(500,200,1024,768))
sleep(5)
camera.capture('image.jpg')
msg = 'Please work'
with open('image.jpg', 'rb') as photo:
    twitter.update_status_with_media(status=msg, media = photo)
exit()