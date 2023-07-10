import cv2
from ultralytics import YOLO
from time import time


def main():
    cap = cv2.VideoCapture(0)
    model = YOLO("yolov8n.pt")
    startTime = time()

    while True:
        ret, frame = cap.read()
        results = model.predict(frame, conf=0.6)[0]
        if time() - startTime >= 3:
            resultsOutput = results.boxes.cls.to('cpu').numpy().tolist()
            print(resultsOutput.count(0))
            startTime = time()

        cv2.imshow('YOLOv8', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
