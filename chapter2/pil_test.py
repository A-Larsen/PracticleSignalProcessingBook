from PIL import Image
import os

#installed image magic to get this to work
def displayImage(img):
    os.system(f"imdisplay {img}")

im = Image.open("red.ico")

im = im.convert("RGB")

# get rgba values for each x, y index
impixels = im.load()

for x in range(0, 56):
    for y in range(0, 20):
        impixels[x, y] = (0, 255, 0, 255)

im.save("out.png")

displayImage("out.png")
