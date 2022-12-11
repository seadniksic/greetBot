import serial

# import the opencv library
import cv2

vid = cv2.VideoCapture(0)
# vid.set(3, 160)
# vid.set(4, 120)


ser = serial.Serial('COM3', 3000000, timeout=None)
img = bytearray(b'')
img_width = 320
img_height = 240

while(True):
      
    # Capture the video frame
    # by frame
    ret, frame = vid.read()
    frame = cv2.resize(cv2.cvtColor(frame , cv2.COLOR_BGR2GRAY), (120, 160))
    print(frame.shape)
    is_success, im_buf_arr = cv2.imencode(".jpg", frame, [int(cv2.IMWRITE_JPEG_QUALITY), 1])
    test = cv2.imdecode(".jpg", im_buf_arr) 
    cv2.imshow('frame', test)
    byte_im = im_buf_arr.tobytes()

    print(len(byte_im))

    for b in byte_im:
        ser.write(b)

    ser.write("\n\n\n")
    # Display the resulting frame
    cv2.imshow('frame', frame)

    # print(type(frame))
      
    # the 'q' button is set as the
    # quitting button you may use any
    # desired button of your choice
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# After the loop release the cap object
vid.release()
# Destroy all the windows
cv2.destroyAllWindows()