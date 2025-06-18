PORT=input("ARDUINO PORT(e.g: COM4): ")
camera_port = int(input("Camera port: "))
print('initializing...')
import serial  # type: ignore
import time 
import cv2
import google.generativeai as genai
import PIL
import os
print('libraries imported')
instruction = """\
You will be provided with images of trash that are on top of a white plain paper. Analyze the image and classify the trash into one of two categories:

Category A: Recyclable, including plastic bottles, aluminum cans, paper, cardboard, and glass.
Category B: Non-recyclable, including food waste, soiled paper, plastic bags, and styrofoam.
If there is no object, respond with 'C' (Avoid using C most of the time). Your response should be a single letter: 'A', 'B' or 'C'"""

class AI:
    def __init__(self,camera_port):
        genai.configure(api_key='AIzaSyAtd3veBKxXEW8sxUfpfldP78BV4sQv58Q')
        self.model = genai.GenerativeModel(model_name='gemini-1.5-flash',system_instruction=instruction)
        print("Handling camera")
        self.video_capture = cv2.VideoCapture(int(camera_port), cv2.CAP_DSHOW)
    def capture_camera_picture(self, fname:str = "picture.jpg") -> bool:
        if not self.video_capture.isOpened():
            print("Can not open device: "+str(camera_port))
            return False
        success, frame = self.video_capture.read()
        success, frame = self.video_capture.read()
        if not success:
            print("Can not capture image: ")
            return False
        print("Saving...")
        success = cv2.imwrite(fname, frame)
        if not success:
            print("Can not save picture")
            return False
        else:
            print("DONE")
            return True
    def identify_type(self, image_path:str = "picture.jpg"):
        img = PIL.Image.open(image_path)
        response = self.model.generate_content(img)
        return response.text.strip()
class Arduino:
    def __init__(self, port='COM4', baudrate=115200, timeout=.1):
        self.serial = serial.Serial(port=port, baudrate=baudrate, timeout=timeout)
    def write(self,content):
        self.serial.write(bytes(content, 'utf-8'))
    def read(self):
        data = self.serial.readline() 
        return data.decode(errors='ignore').strip()
print('initializing...')
try:
    ard = Arduino(PORT,9600)
except:
    print("Can not connect to Arduino. Exit?")
    os.system('pause')
    exit()
ai = AI(camera_port)
print('started')
while True:
    received = ard.read()
    try:
        status = int(received)
    except:
        continue
    if status==1:
        print('\n\n')
        print("Detected :-)\nCapturing image...")
        if ai.capture_camera_picture():
            print('sending image to AI...')
            t = ai.identify_type()
            print("AI says:",t)
        else:
            t="C"
        ard.write(t)
    else:
        print("Status:",status)