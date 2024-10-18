import cv2

# Function to capture the pixel value on mouse click
def get_pixel_value(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:  # Left mouse button click
        pixel_value = param[0, 8]  # Get pixel value at (x, y)
        print(f"Pixel at ({x}, {y}): {pixel_value}")

# Load your binary or color image
image_path = 'photos/MaskImage0.png'  # Replace with the path to your image
image = cv2.imread(image_path)

# Create a window and set the mouse callback function
cv2.namedWindow('Image')
cv2.setMouseCallback('Image', get_pixel_value, param=image)

# Display the image
while True:
    cv2.imshow('Image', image)
    if cv2.waitKey(1) & 0xFF == 27:  # Press 'ESC' to exit the window
        break

# Cleanup when done
cv2.destroyAllWindows()
