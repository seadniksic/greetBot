import serial
import time
import PIL.Image
import io
import time
import cv2
import signal
import matplotlib.pyplot as plt
import mediapipe as mp
from PIL import ImageFile
import numpy as np
import sys
ImageFile.LOAD_TRUNCATED_IMAGES = True
ser = serial.Serial('COM3', 3000000, timeout=None)
img = bytearray(b'')
img_width = 320
img_height = 240
faceCascade = cv2.CascadeClassifier("assets\\haarcascade.xml")
FACE_DETECTION = 0

mp_face_detection = mp.solutions.face_detection
mp_drawing = mp.solutions.drawing_utils

def main():
    # make sure the 'COM#' is set according the Windows Device Manager
    ser.write(b'\x01')

    signal.signal(signal.SIGINT, stop_stream) 

    #create two subplots
    ax = plt.gca()

    #create two image plots
    im = ax.imshow(get_img())

    plt.ion()

    frame_times = []

    while True:
        now = time.time()
        ser.write(b'\x01')
        try:
            # get_img()
            im.set_data(get_img())
        except:
            continue
        plt.pause(0.01)



    plt.ioff() # due to infinite loop, this gets never called.
    plt.show()

    ser.close()

def stop_stream():
    ser.write(b'0x2')

def get_img():
    #send start condition for photo

    # scan for initial byte
    while True:
        byte = ser.read()
        # print(byte)
        if byte == b'\xFF':
            break

    now = time.time()
    
    img = ser.read_until(b"\n\n\n", 8000)
    img = img[:-3]
    
    print(sys.getsizeof(img))

    image_np = np.frombuffer(img, np.uint8)

    imageBGR = cv2.imdecode(image_np, cv2.IMREAD_COLOR)
    imageRGB = cv2.cvtColor(imageBGR, cv2.COLOR_BGR2RGB)

    #media pipe processing
    if FACE_DETECTION == 0:

        with mp_face_detection.FaceDetection(model_selection=1, min_detection_confidence=0.5) as face_detection:

            results = face_detection.process(imageRGB)

            # Draw face detections of each face.
            if not results.detections:
                return imageRGB

            # annotated_image = imageBGR.copy()
            for detection in results.detections:
                # Draw only bounding box
                # coords = detection.location_data.relative_bounding_box
                # coords = normalize_coords(coords)
                # cv2.rectangle(imageRGB, coords[0], coords[1], (255, 255, 255), 2)

                #Draw bounding box and landmarks
                mp_drawing.draw_detection(imageRGB, detection)

    elif FACE_DETECTION == 1:

        faces = faceCascade.detectMultiScale(
            cv2.cvtColor(imageBGR , cv2.COLOR_BGR2GRAY),
            scaleFactor=1.1,
            minNeighbors=5,
            minSize=(30, 30)
        )

        for (x, y, w, h) in faces:
            cv2.rectangle(imageRGB, (x, y), (x+w, y+h), (0, 255, 0), 2)

    return imageRGB

# print()

def normalize_coords(coords):

    return (int(coords.xmin * img_width), int(coords.ymin * img_height)), (int(coords.xmin * img_width+coords.width * img_width), int(coords.ymin*img_height+coords.height*img_height))

if __name__ == "__main__":
    main()


# print()


# for i in range(4104):
#     line = ser.readline()   # read a byte
#     if line:
#         string = line.decode()  # convert the byte string to a unicode string
#         num = int(string) # convert the unicode string to an int
#         print(num)

# ser.close()

# import cv2


# # For static images:
# IMAGE_FILES = []
# with mp_face_detection.FaceDetection(
#     model_selection=1, min_detection_confidence=0.5) as face_detection:
#   for idx, file in enumerate(IMAGE_FILES):
#     image = cv2.imread(file)
#     # Convert the BGR image to RGB and process it with MediaPipe Face Detection.
#     results = face_detection.process(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))

#     # Draw face detections of each face.
#     if not results.detections:
#       continue
#     annotated_image = image.copy()
#     for detection in results.detections:
#       print('Nose tip:')
#       print(mp_face_detection.get_key_point(
#           detection, mp_face_detection.FaceKeyPoint.NOSE_TIP))
#       mp_drawing.draw_detection(annotated_image, detection)
#     cv2.imwrite('/tmp/annotated_image' + str(idx) + '.png', annotated_image)

# # For webcam input:
# cap = cv2.VideoCapture(0)
# with mp_face_detection.FaceDetection(
#     model_selection=0, min_detection_confidence=0.5) as face_detection:
#   while cap.isOpened():
#     success, image = cap.read()
#     if not success:
#       print("Ignoring empty camera frame.")
#       # If loading a video, use 'break' instead of 'continue'.
#       continue

#     # To improve performance, optionally mark the image as not writeable to
#     # pass by reference.
#     image.flags.writeable = False
#     image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
#     results = face_detection.process(image)

#     # Draw the face detection annotations on the image.
#     image.flags.writeable = True
#     image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
#     if results.detections:
#       for detection in results.detections:
#         mp_drawing.draw_detection(image, detection)
#     # Flip the image horizontally for a selfie-view display.
#     cv2.imshow('MediaPipe Face Detection', cv2.flip(image, 1))
#     if cv2.waitKey(5) & 0xFF == 27:
#       break
# cap.release()