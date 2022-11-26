import serial
import time
import PIL.Image
import io
import time
import cv2
import matplotlib.pyplot as plt
from PIL import ImageFile
ImageFile.LOAD_TRUNCATED_IMAGES = True

# make sure the 'COM#' is set according the Windows Device Manager
ser = serial.Serial('COM3', 3000000, timeout=1)
img = bytearray(b'')
ser.write(b'\x01')

def get_img():
    #send start condition for photo

    # scan for initial byte
    while True:
        byte = ser.read()
        if byte == b'\xFF':
            break

    now = time.time()
    time.sleep(.1) #wait for image to be fully sent over serial bus
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
    return PIL.Image.open(io.BytesIO(img)).convert('RGB')

#create two subplots
ax = plt.gca()

#create two image plots
im = ax.imshow(get_img())



plt.ion()
i = 0
for i in range(200):
    imag = get_img()
    print(type(imag))
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