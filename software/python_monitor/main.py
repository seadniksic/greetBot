import serial
import time
import PIL.Image
import io
import time
import cv2
import matplotlib.pyplot as plt
from PIL import ImageFile
import numpy as np
ImageFile.LOAD_TRUNCATED_IMAGES = True

# make sure the 'COM#' is set according the Windows Device Manager
ser = serial.Serial('COM3', 3000000, timeout=1)
img = bytearray(b'')
ser.write(b'\x01')
faceCascade = cv2.CascadeClassifier("assets\\haarcascade.xml")

def get_img():
    #send start condition for photo

    # scan for initial byte
    while True:
        byte = ser.read()
        # print(byte)
        if byte == b'\xFF':
            break

    now = time.time()
    time.sleep(.08) #wait for image to be fully sent over serial bus
                    #avg image ~3500 bytes which can be sent at 4.6ms (apparently take more time, idk)
                    #using 10 here to be safe
    img = ser.read_all()
    # while byte != b'':
    #     byte = ser.read()
    #     # print(byte)
    #     img.extend(byte)
    # print(ser.readline())
        # print(img)
    print(len(img))
    print(f"One img trans: {time.time() - now}")
    image_np = np.frombuffer(img, np.uint8)
    imageBGR = cv2.imdecode(image_np, cv2.IMREAD_COLOR)

    faces = faceCascade.detectMultiScale(
        cv2.cvtColor(imageBGR , cv2.COLOR_BGR2GRAY),
        scaleFactor=1.1,
        minNeighbors=5,
        minSize=(30, 30)
    )

    imageRGB = cv2.cvtColor(imageBGR , cv2.COLOR_BGR2RGB)

    for (x, y, w, h) in faces:
        cv2.rectangle(imageRGB, (x, y), (x+w, y+h), (0, 255, 0), 2)

    return imageRGB

#create two subplots
ax = plt.gca()

#create two image plots
im = ax.imshow(get_img())

print('hello')
plt.ion()

i = 0
for i in range(200):
    ser.write(b'\x01')
    print('hello')
    imag = get_img()
    print('hello')
    im.set_data(imag)
    plt.pause(0.05)
    i+= 1


plt.ioff() # due to infinite loop, this gets never called.
plt.show()

ser.close()

# print()




# print()


# for i in range(4104):
#     line = ser.readline()   # read a byte
#     if line:
#         string = line.decode()  # convert the byte string to a unicode string
#         num = int(string) # convert the unicode string to an int
#         print(num)

# ser.close()