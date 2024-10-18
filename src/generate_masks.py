import cv2
import numpy as np

for i in range(7):
    image = cv2.imread(f'photos/RenderedImage{i}.png')

    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    lower_red = np.array([2, 85, 100])
    upper_red = np.array([4, 255, 255])


    mask = cv2.inRange(hsv, lower_red, upper_red)

    cv2.imwrite(f'photos/MaskImage{i}.png', mask)