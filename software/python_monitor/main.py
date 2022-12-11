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
closest_face_center = (0, 0)
no_face_detected_counter = 0

def main():
    global closest_face_center
    # make sure the 'COM#' is set according the Windows Device Manager
    ser.write(b'\x01')

    signal.signal(signal.SIGINT, stop_stream) 

    cv2.namedWindow("output", cv2.WINDOW_NORMAL)

    cv2.resizeWindow("output", 640, 480)

    #create two subplots
    # ax = plt.gca()

    #create two image plots
    # im = ax.imshow(get_img())

    # plt.ion()

    frame_times = []

    while True:
        now = time.time()
        ser.write(b'\x01')
        try:
            # get_img()
            # img, closest_face_center = get_img()
            # print(img)
            cv2.imshow('output', get_img())
            
            
            cv2.waitKey(1)

            print(f"closest face center: {closest_face_center}")
            ser.write(b'\x03')

            print(closest_face_center)

            print(closest_face_center[0].to_bytes(2, 'little'))
            print(closest_face_center[1].to_bytes(2, 'little'))

            ser.write(b'\xFF' + closest_face_center[0].to_bytes(2, 'little') + closest_face_center[1].to_bytes(2, 'little'))

            # ser.write(closest_face_center[0])  #write x coordinate of face center
            # ser.write(closest_face_center[1].to_bytes(2, 'little')) #write y coordinate of face center
            
            # # face_coords = []
            # temp_coords = bytearray(b'')
            # temp_byte = ser.readline()
            
            # temp_coords += temp_byte
            # print(type(temp_byte))
            # print(f"bytes received: {temp_byte}")

            while ser.read() != b'\xFF': continue

            temp_byte = ser.read(4)
            print(f"bytes received: {temp_byte}")
                # temp_coords.extend(temp_byte)
                # if temp_byte != b'' and len(temp_coords) < 4:
                #     temp_coords.append(temp_byte)
                # elif len(temp_coords) >= 2:
                #     break

            # print(f"temp_coords: {temp_coords}")

            # temp_coords.append(ser.readline())
            # temp_coords.append(ser.readline())

            # print("coordinates from MCU")
            # print(f"x: {temp_coords[0]}")
            # print(f"y: {temp_coords[1]}")

        except:
            continue

        if cv2.waitKey(1) & 0xFF == ord('q'):
            ser.write(b'\x02')
            ser.close()  
            break

    cv2.destroyAllWindows()



    # plt.ioff() # due to infinite loop, this gets never called.
    # plt.show()

    # ser.close()

def stop_stream():
    ser.write(b'0x2')

def get_img():
    global closest_face_center, no_face_detected_counter
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
    
    # print(sys.getsizeof(img))

    # print(img)

    image_np = np.frombuffer(img, np.uint8)

    imageBGR = cv2.imdecode(image_np, cv2.IMREAD_COLOR)
    # imageRGB = cv2.cvtColor(imageBGR, cv2.COLOR_BGR2RGB)

    # media pipe processing
    if FACE_DETECTION == 0:

        with mp_face_detection.FaceDetection(model_selection=1, min_detection_confidence=0.5) as face_detection:

            results = face_detection.process(imageBGR)

            # Draw face detections of each face.
            if not results.detections:
                # print("no detection")
                no_face_detected_counter += 1
                if no_face_detected_counter == 3:
                    closest_face_center = (0, 0)
                    no_face_detected_counter = 0
                return imageBGR

            # annotated_image = imageBGR.copy()
            closest_face_data = ()
            for detection in results.detections:

                coords = detection.location_data.relative_bounding_box
                coords_norm = normalize_coords(coords)
                size = normalize_size(coords)

                if closest_face_data == ():
                    closest_face_data = (coords_norm, size)
                elif size > closest_face_data[1]:
                    closest_face_data = (coords_norm, size)

                # Draw only bounding box
                cv2.rectangle(imageBGR, coords_norm[0], coords_norm[1], (255, 255, 255), 2)
                # print(coords)

                #Draw bounding box and landmarks
                # mp_drawing.draw_detection(imageBGR, detection)

    elif FACE_DETECTION == 1:

        faces = faceCascade.detectMultiScale(
            cv2.cvtColor(imageBGR , cv2.COLOR_BGR2GRAY),
            scaleFactor=1.1,
            minNeighbors=5,
            minSize=(30, 30)
        )

        for (x, y, w, h) in faces:
            cv2.rectangle(imageBGR, (x, y), (x+w, y+h), (0, 255, 0), 2)

    #Decide which face is biggest (focus on that one)
    # determine_biggest_face

    # print(closest_face_data[0])
    closest_face_center = compute_face_center(closest_face_data[0])

    return imageBGR


def normalize_coords(coords):

    return (int(coords.xmin * img_width), int(coords.ymin * img_height)), (int(coords.xmin * img_width+coords.width * img_width), int(coords.ymin*img_height+coords.height*img_height))

def normalize_size(coords):
    
    return coords.width * img_width * coords.height * img_height

def compute_face_center(coords):

    return int((coords[0][0] + coords[1][0]) / 2), int((coords[0][1] + coords[1][1]) / 2)


if __name__ == "__main__":
    main()